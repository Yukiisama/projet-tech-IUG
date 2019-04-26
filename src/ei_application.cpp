#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_eventmanager.h"
#include "hw_interface.h"
#include <iostream>
#include <algorithm>
#include <cctype>
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
    hw_quit();
    delete (widget_root);
}
/**
 * @brief Application::isIntersect check if two rectangles are overlapping.
 * @param rect1 The rectangle to check.
 * @param rect2 The rectangle to check.
 * @return true if rectangles are overlapping, false else.
 */
bool Application::isIntersect(Rect rect1, Rect rect2){
    // If one rectangle is on left side of other
    if (rect1.top_left.x() > rect2.top_left.x()+rect2.size.width()
            || rect2.top_left.x() > rect1.top_left.x()+rect1.size.width())
        return false;

    // If one rectangle is above other
    if (rect1.top_left.y() > rect2.top_left.y()+rect2.size.height()
            || rect2.top_left.y() > rect1.top_left.y()+rect1.size.height())
        return false;

    return true;
}

/**
     * \brief Runs the application: enters the main event loop. Exits when
     *    \ref app_quit_request is called.
     */
void Application::run(){
    running = true;
    update_time  = hw_now();
    //We first need to add the window rect to invalid rect list in order to update it at launch
    Rect window_rect = hw_surface_get_rect(root_window);
    invalidate_rect(window_rect);
    /*This loop wait for an event , then treat it with the event manager
     *Then update the screen and limit it to 60 fps
     */
    while(running){
        Event *ev=hw_event_wait_next();
        EventManager::getInstance().eventHandler(ev);
        if(hw_now()-update_time>FPS_MAX){
            //if there is aleast an rectangle zone to update, then draw the whole screen.
            if(!to_clear_rectangle_list.empty()){
                widget_root->draw(root_window,offscreen,widget_root->getContent_rect());
                hw_surface_update_rects(to_clear_rectangle_list);
            }
            //clear list after updated.
            to_clear_rectangle_list.clear();
            update_time  = hw_now();
        }
        if(ev)delete ev;
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
    to_clear_rectangle_list.push_front(rect);
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
     * @param   where The location on screen, expressed in the root window coordinates.
     *
     * @return  EI_TRUE if inside_root, else EI_FALSE
     */
bool_t Application::inside_root (const Point& where){

    if((where.x() < 0 || where.x() > hw_surface_get_size(this->root_window).width())
            || (where.y() < 0 || where.y() > hw_surface_get_size(this->root_window).height())){
        return EI_FALSE;
    }
    return EI_TRUE;
}

/**
 * @brief Application::intersectedRect
 * @param rect1 The rectangle to use for computing
 * @param rect2 The rectangle to use for computing
 * @return The rectangle that intersect with rect1 and rect2
 */
Rect Application::intersectedRect(Rect rect1, Rect rect2)
{
    Rect newR;
    if(!Application::getInstance()->isIntersect(rect1,rect2)){
        newR.size.width()=-1;
        newR.size.height()=-1;
        return newR;
    }

    int x,y,width,height;
    int x1=rect1.top_left.x(),y1 = rect1.top_left.y(),w1=rect1.size.width(),h1=rect1.size.height();
    int x2=rect2.top_left.x(),y2 = rect2.top_left.y(),w2=rect2.size.width(),h2=rect2.size.height();

    x=max(x1,x2);
    y=max(y1,y2);
    width=min(x1+w1,x2+w2)-x;
    height=min(y1+h1,y2+h2)-y;
    newR.top_left.x()=x;newR.top_left.y()=y;
    newR.size.width()=width;newR.size.height()=height;
    return newR;

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
void Application::set_who_click(uint32_t x){
    this->who_click = x;
}
uint32_t Application::get_who_click(){
    return this->who_click;
}

// *** End Getter & Setter ***


}
