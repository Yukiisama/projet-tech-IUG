#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_geometrymanager.h"
#include "ei_eventmanager.h"
#include "hw_interface.h"
#include <iostream>
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
      if(main_window_size == nullptr){
        fprintf(stderr,"Error occured for Application::Application - main_window_size is nullptr\n");
        exit(EXIT_FAILURE);
      }
      hw_init();
      this->root_window = hw_create_window(main_window_size, EI_FALSE);
      this->offscreen = hw_surface_create(this->root_window, main_window_size);
      this->widget_root = new Frame(NULL);
      this->instance = this;
      //color_t white = {225, 225, 225, 225};
      //this->widget_root->configure(main_window_size,&white,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
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
     * \brief Runs the application: enters the main event loop. Exits when
     *    \ref app_quit_request is called.
     */
    void Application::run(){
        running = true;
        double current_time ;
        Rect window_rect = hw_surface_get_rect(root_window);
        invalidate_rect(window_rect);
        bool geo_done=true;
        while(running){
            std::list<Widget *> w_geo = widget_root->getChildren();
            for(std::list<Widget *>::iterator it =w_geo.begin() ;it!=w_geo.end();++it){
                if((*it)->getGeom_manager()){
                    if(geo_done)(*it)->getGeom_manager()->run((*it));
                    if(!(*it)->getName().compare("Toplevel")){
                        //update content_rect
                        (*it)->updateContent_rect();
                    }
                    //std::cout<<"after run \n"<<std::endl;
                    //std::cout << (*it)->to_string()<<std::endl;
                }
            }
            geo_done = false;
            Event *ev = hw_event_wait_next();
            EventManager::getInstance().eventHandler(ev);
            //Peut être probleme plus tard car cast KEYEVENT sur un event dont on ne connait pas le type
            KeyEvent * ev_key = static_cast<KeyEvent*>  (ev);
            if( ev->type == ei_ev_keydown && ev_key->key_sym == ALLEGRO_KEY_Q)  
                quit_request();
            current_time = hw_now();
            if(update_time<=current_time){

                if(!to_clear_rectangle_list.empty()){
                    widget_root->draw(root_window,offscreen,widget_root->getContent_rect());
                    hw_surface_update_rects(to_clear_rectangle_list);
                }
                //next step is to clear the rectangle list.
                to_clear_rectangle_list.clear();
                update_time  = current_time + (1/60);
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
        /*
        if(&rect == NULL){
          fprintf(stderr,"Error occured for Application::invalide_rect - rect is nullptr\n");
          exit(EXIT_FAILURE);
        }
        */
        to_clear_rectangle_list.push_back(rect);
    }

    /**
     * \brief Tells the application to quite. Is usually called by an event handler (for example
     *    when pressing the "Escape" key).
     */
    void Application::quit_request(){
        running = false;
        //delete this;
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
      if((where.x() < 0 || where.x() > hw_surface_get_size(this->root_window).width())
      || (where.y() < 0 || where.y() > hw_surface_get_size(this->root_window).height())){
          fprintf(stderr,"Error occured for Application::widget_pick - param where is out the root_window\n");
          exit(EXIT_FAILURE);
      }
      color_t color = hw_get_pixel(this->offscreen, where);
      uint32_t ID = widget_root->conver_color_id(color);
      return widget_root->pick(ID);
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
