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

    bool_t click_up(Widget* widget, Event* event, void* user_param)
    {
        MouseEvent* e = static_cast<MouseEvent*>(event);
        Toplevel* top = static_cast<Toplevel*>(widget);

        if(top->moving()==EI_TRUE || top->resizing()==EI_TRUE || top->closing()==EI_TRUE){
            if(top->moving()==EI_TRUE)top->set_top_bar_clicked(EI_FALSE);
            if(top->resizing()==EI_TRUE)top->set_resize_button_pressed(EI_FALSE);
            if(top->closing()==EI_TRUE){
                if(Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getButton_close()->getPick_id()){
                    delete top;
                }
                top->set_button_close_pressed(EI_FALSE);
            }
            return EI_TRUE;
        }
        return EI_FALSE;
    }

    bool_t click_down(Widget* widget, Event* event, void* user_param)
    {
        MouseEvent* e = static_cast<MouseEvent*>(event);
        Toplevel* top = static_cast<Toplevel*>(widget);

        if(Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getPick_id()){
            if(top->inside_top_bar(e->where)==EI_TRUE){
                top->set_top_bar_clicked(EI_TRUE);
                top->setMouse_pos(e->where);
                return EI_TRUE;
            }
        }
        else if(Application::getInstance()->widget_pick(e->where)->getPick_id()
                ==top->getResize_button()->getPick_id()){
            top->set_resize_button_pressed(EI_TRUE);
            return EI_TRUE;
        }
        else if(Application::getInstance()->widget_pick(e->where)->getPick_id()
                ==top->getButton_close()->getPick_id()){
            top->set_button_close_pressed(EI_TRUE);
            return EI_TRUE;
        }
        return EI_FALSE;
    }

    bool_t move_toplevel(Widget* widget, Event* event, void* user_param)
    {
        MouseEvent* e = static_cast<MouseEvent*>(event);
        Toplevel* top = static_cast<Toplevel*>(widget);

        if(top->moving()==EI_TRUE){
            if(Application::inside_root(e->where)){
                float move_x = (e->where.x())-(top->getMouse_pos().x());
                float move_y = (e->where.y())-(top->getMouse_pos().y());

                top->getGeom_manager()->set_x(top->getScreenLocation()->top_left.x()+move_x);
                top->getGeom_manager()->set_y(top->getScreenLocation()->top_left.y()+move_y);

                top->setMouse_pos(e->where);
            }
            return EI_TRUE;
        }
        if(top->resizing()==EI_TRUE){
            float new_width = (e->where.x())-(top->getScreenLocation()->top_left.x());
            float new_height = (e->where.y())-(top->getScreenLocation()->top_left.y());

            if(new_width < top->getMin_size().width()){
                new_width = top->getRequested_size().width();
            }
            if(new_height < top->getMin_size().height()){
                new_height = top->getRequested_size().height();
            }

            top->configure(new Size(new_width,new_height),NULL,NULL,NULL,NULL,NULL,NULL);

            return EI_TRUE;
        }
        return EI_FALSE;
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

    //Binding the default comportments of widgets
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, NULL, "Toplevel", click_up, NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, NULL, "Toplevel", click_down, NULL);
    EventManager::getInstance().bind(ei_ev_mouse_move, NULL, "Toplevel", move_toplevel, NULL);
    EventManager::getInstance().bind(ei_ev_mouse_move, NULL, "Toplevel", resize_toplevel, NULL);
	
    while(running){
      std::list<Widget *> w_geo = widget_root->getChildren();
      for(std::list<Widget *>::iterator it =w_geo.begin() ;it!=w_geo.end();++it){
        if((*it)->getGeom_manager()){
          (*it)->getGeom_manager()->run((*it));
          //std::cout<<"after run \n"<<std::endl;
          //std::cout << (*it)->to_string()<<std::endl;
        }
      }
      Event *ev = hw_event_wait_next();
      EventManager::getInstance().eventHandler(ev);
      KeyEvent * ev_key = static_cast<KeyEvent*>  (ev);
      if( ev->type == ei_ev_keydown && ev_key->key_sym == ALLEGRO_KEY_ESCAPE)  // 59 == escape
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
      if(inside_root(where)==EI_FALSE){
          fprintf(stderr,"Error occured for Application::widget_pick - param where is out the root_window\n");
          exit(EXIT_FAILURE);
      }
      color_t color = hw_get_pixel(this->offscreen, where);
      uint32_t ID = widget_root->ConvertColorToId(color);
      return widget_root->pick(ID);
    }

    bool_t Application::inside_root (const Point& where){
        if((where.x() < 0 || where.x() > hw_surface_get_size(this->root_window).width())
        || (where.y() < 0 || where.y() > hw_surface_get_size(this->root_window).height())){
            return EI_FALSE;
        }
        return EI_TRUE;
    }

}
