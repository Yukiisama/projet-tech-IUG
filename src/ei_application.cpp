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

  uint32_t ColorToUInt(color_t color){
    return (uint32_t)((color.alpha << 24) | (color.red << 16) |
                  (color.green << 8)  | (color.blue << 0));
  }
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
      color_t white = {225, 225, 225, 225};
      this->widget_root->configure(main_window_size,&white,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
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
	
    while(running){
      //widget_root->getGeom_manager()->run(widget_root);
			Event* ev = hw_event_wait_next();
			if(ev->type == ei_ev_keydown)
				quit_request();
      current_time = hw_now();
      
      if(update_time<=current_time){
        
        if(!to_clear_rectangle_list.empty()){
          widget_root->draw(root_window,offscreen,NULL);
			    hw_surface_update_rects(to_clear_rectangle_list);
        }
			  //next step is to clear the rectangle list.
				to_clear_rectangle_list.clear();
        update_time  = current_time + (1/60);
      }
      EventManager::getInstance().eventHandler();
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
        if(&rect == nullptr){
          fprintf(stderr,"Error occured for Application::invalide_rect - rect is nullptr\n");
          exit(EXIT_FAILURE);
        }
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
      if(color.red == 0 && color.green == 0 && color.blue == 0 && color.alpha == 0){
        return nullptr;
      }
      uint32_t ID = ColorToUInt(color);
      //printf("%lu\n",(unsigned long) ID);
      return this->widget_root->pick(ID);
    }

}
