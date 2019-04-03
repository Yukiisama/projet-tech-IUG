#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_eventmanager.h"
#include "hw_interface.h"
#include <iostream>
#define FPS_MAX (1.0/60.0)
namespace ei {
Application *Application::instance = nullptr;

/**
     * \brief Creates an application.
     *    <ul>
     *      <li> initializes the hardware (calls \ref hw_init), </li>
     *      <li> creates the root window, </li>
     *      <li> creates the root widget to access the root window. </li>
     *    </ul>
     *
     * @param main_window_size  the size of the root window of the application.
     */
Application::Application(Size* main_window_size){
    if(!main_window_size){
        fprintf(stderr,"Error occured for Application::Application - main_window_size is nullptr\n");
        exit(EXIT_FAILURE);
    }
    //Initialises access to the low-level operating system services.
    hw_init();
    //create root and offscreen surface , and the root widget being a frame
    this->root_window = hw_create_window(main_window_size, EI_FALSE);
    this->offscreen = hw_surface_create(this->root_window, main_window_size);
    this->widget_root = new Frame(nullptr);
    //singleton application
    this->instance = this;
}

/**
     * \brief Releases all the resources of the application, and releases the hardware
     *    (eventually calls \ref hw_quit).
     */
Application::~Application(){
    hw_surface_free(this->offscreen);
    delete widget_root;
    hw_quit();
}

/**
     * @brief toplevel_click_up is called when a click occurs and check if top is moving , resizing or closing
     * @param widget must be a Toplevel
     * @param event the mouse event
     * @param user_param A user parameter that will be passed to the callback when it is called.
     * @return EI_TRUE if top moving , resizing or closing , else EI_FALSE
     */
bool_t toplevel_click_up(Widget* widget, Event* event, void* user_param)
{

    MouseEvent* e = static_cast<MouseEvent*>(event);
    Toplevel* top = static_cast<Toplevel*>(widget);
    //Clicked up , update if moving , resizing or closing
    if(top->moving()|| top->resizing()|| top->closing()){
        if(top->moving())
            top->set_top_bar_clicked(EI_FALSE);
        if(top->resizing())
            top->set_resize_button_pressed(EI_FALSE);
        if(top->closing()){
            if(Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getButton_close()->getPick_id()){
                top->getGeom_manager()->release(top);
                delete top;
                cout<<"deleting top"<<endl;
                return EI_TRUE;
            }

            top->set_button_close_pressed(EI_FALSE);
        }
        return EI_TRUE;
    }
    return EI_FALSE;
}

/**
     * @brief toplevel_click_down for resize and move and close
     * @param widget must be a Toplevel
     * @param event the mouse event
     * @param user_param A user parameter that will be passed to the callback when it is called.
     * @return EI_TRUE if resize , move or close successfull else EI_FALSE
     */
bool_t toplevel_click_down(Widget* widget, Event* event, void* user_param)
{
    MouseEvent* e = static_cast<MouseEvent*>(event);
    Toplevel* top = static_cast<Toplevel*>(widget);

    //The case when the user is clicking on the toplevel
    if(Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getPick_id()){
        //The case when the user is clicking on the top_bar
        if(top->inside_top_bar(e->where)){
            //Tells the toplevel that its top_bar is clicked
            top->set_top_bar_clicked(EI_TRUE);
            top->setMouse_pos(e->where);

            return EI_TRUE;
        }
    }
    //The case when clicking on the resize button
    else if(Application::getInstance()->widget_pick(e->where)->getPick_id()
            ==top->getResize_button()->getPick_id()){
        top->set_resize_button_pressed(EI_TRUE);
        return EI_TRUE;
    }
    //The case when clicking on the button close
    else if(Application::getInstance()->widget_pick(e->where)->getPick_id()
            ==top->getButton_close()->getPick_id()){
        top->set_button_close_pressed(EI_TRUE);
        return EI_TRUE;
    }
    return EI_FALSE;
}

/**
     * @brief The function that manages mouse event movement on toplevel
     * @param widget must be Toplevel
     * @param event the mouse event
     * @param user_param A user parameter that will be passed to the callback when it is called.
     * @return EI_TRUE if an action is done as moving or resizing else EI_FALSE
     */
bool_t default_toplevel(Widget* widget, Event* event, void* user_param)
{
    MouseEvent* e = static_cast<MouseEvent*>(event);
    Toplevel* top = static_cast<Toplevel*>(widget);

    //The case when the top_bar is still pressed and the mouse is moving
    if(top->moving()){
        if(Application::getInstance()->inside_root(e->where)){//check if the mouse position is valid
            double move_x = (e->where.x())-(top->getMouse_pos().x());
            double move_y = (e->where.y())-(top->getMouse_pos().y());
            //Update geom_manager x & y to update the position of toplevel
            top->getGeom_manager()->setX(int(top->getScreen_location().top_left.x()+move_x));
            top->getGeom_manager()->setY(int(top->getScreen_location().top_left.y()+move_y));
            //finally run the geom_manager that will result in updating the position
            top->getGeom_manager()->run(top);
            top->setMouse_pos(e->where);
        }
        return EI_TRUE;
    }
    //The case when the user is clicking on the resize button and moving the mouse
    if(top->resizing()){
        if(Application::getInstance()->inside_root(e->where)){
            float new_width = (e->where.x())-(top->getScreen_location().top_left.x());
            float new_height = (e->where.y())-(top->getScreen_location().top_left.y());
            //Limit the top level to a minimal size
            if(new_width < top->getMin_size().width()){
                new_width = top->getMin_size().width();
            }
            if(new_height < top->getMin_size().height()){
                new_height = top->getMin_size().height();
            }
            //finally update the new size of the top level
            Size *new_size = new Size(new_width,new_height);
            top->configure(new_size,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
            delete(new_size);
            return EI_TRUE;

        }

    }
    return EI_FALSE;
}

/**
     * \brief Runs the application: enters the main event loop. Exits when
     *    \ref app_quit_request is called.
     */
void Application::run(){
    //Binding the default comportments of TopLevel class of widget
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, nullptr, "Toplevel", toplevel_click_up, nullptr);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, nullptr, "Toplevel", toplevel_click_down, nullptr);
    EventManager::getInstance().bind(ei_ev_mouse_move, nullptr, "Toplevel", default_toplevel, nullptr);
    running = true;
    //used to limit fps of the application
    double current_time ;
    //We first need to add the window rect to invalid rect list in order to update it at launch
    Rect window_rect = hw_surface_get_rect(root_window);
    invalidate_rect(window_rect);
    /*This loop waits an event , then treat it with the event manager
     *Then update the screen and limit it to 60 fps
     */
    while(running){
        Event *ev = hw_event_wait_next();
        if( ev->type == ei_ev_keydown ){
            KeyEvent * ev_key = static_cast<KeyEvent*>  (ev);
            if(ev_key->key_sym == ALLEGRO_KEY_Q) quit_request();
        }else{
            EventManager::getInstance().eventHandler(ev);
        }
        current_time = hw_now();
        if(update_time<=current_time){
            //Screen need to be update , draw the widgets then update rects
            if(!to_clear_rectangle_list.empty()){
                widget_root->draw(root_window,offscreen,widget_root->getContent_rect());
                hw_surface_update_rects(to_clear_rectangle_list);
            }
            //next step is to clear the rectangle list.
            to_clear_rectangle_list.clear();
            update_time  = current_time + FPS_MAX;
        }
    }

    return;
}

/**
     * \brief Adds a rectangle to the list of rectangles that must be updated on screen. The real
     *    update on the screen will be done at the right moment in the main loop.
     *
     * @param rect    The rectangle to add, expressed in the root window coordinates.
     *        A copy is made, so it is safe to release the rectangle on return.
     */
void Application::invalidate_rect(const Rect &rect){
    to_clear_rectangle_list.push_back(rect);
}

/**
     * \brief Tells the application to quite. Is usually called by an event handler (for example
     *    when pressing the "Escape" key).
     */
void Application::quit_request(){
    running = false;
}

/**
     * \brief Returns the "root widget" of the application: a "frame" widget that encapsulate the
     *    root window.
     *
     * @return      The root widget.
     */
Frame* Application::root_widget(){
    return widget_root;
}

/**
     * \brief Returns the surface of the root window. Used to create surfaces with similar r, g, b
     *    channels.
     *
     * @return      The surface of the root window.
     */
surface_t Application::root_surface(){
    return this->root_window;
}

/**
     * @brief   Returns the widget that is at a given location on screen.
     *
     * @param   where       The location on screen, expressed in the root window coordinates.
     *
     * @return      The top-most widget at this location, or nullptr if there is no widget
     *              at this location (except for the root widget).
     */
Widget* Application::widget_pick (const Point& where){
    if(!inside_root(where)){
        fprintf(stderr,"Error occured for Application::widget_pick - param where is out the root_window\n");
        exit(EXIT_FAILURE);
    }
    //Get the pixel at the where pos then convert it to get the unique id of the widget being clicked
    color_t color = hw_get_pixel(this->offscreen, where);
    uint32_t ID = widget_root->conver_color_id(color);
    return widget_root->pick(ID);
}

/**
     * @brief   Returns if the point where is inside the root
     *
     * @param   where       The location on screen, expressed in the root window coordinates.
     *
     * @return      True if inside_root else False
     */
bool_t Application::inside_root (const Point& where){
    if((where.x() < 0 || where.x() > hw_surface_get_size(this->root_window).width())
            || (where.y() < 0 || where.y() > hw_surface_get_size(this->root_window).height())){
        return EI_FALSE;
    }
    return EI_TRUE;
}
// *** Getters & Setter ***
surface_t Application::get_root_window(){
    return this->root_window;
}
void Application::set_root_window(surface_t root_window){
    this->root_window = root_window;
}
surface_t Application::get_offscreen(){
    return this->offscreen;
}
void Application::set_offscreen(surface_t offscreen){
    this->offscreen = offscreen;
}
Frame * Application::get_widget_root(){
    return this->widget_root;
}
void Application::set_widget_root(Frame * root_widget){
    this->widget_root = root_widget;
}
linked_rect_t Application::get_to_clear_rectangle_list(){
    return this->to_clear_rectangle_list;
}
void Application::set_to_clear_rectangle_list(linked_rect_t to_clear_rectangle_list){
    this->to_clear_rectangle_list = to_clear_rectangle_list;
}
bool Application::get_running(){
    return this->running;
}
void Application::set_running(bool running){
    this->running = running;
}
double Application::get_update_time(){
    return this->update_time;
}
void Application::set_update_time(double update_time){
    this->update_time =update_time;
}
// *** End Getter & Setter ***


}
