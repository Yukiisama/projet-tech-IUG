#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "hw_interface.h"
namespace ei {

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
      if(main_window_size == NULL){
        fprintf(stderr,"Error occured for Application::Application - main_window_size is NULL\n");
        exit(EXIT_FAILURE);
      }
      hw_init();
      this->root_window = hw_create_window(main_window_size, EI_FALSE);
      this->root_widget = Widget("Frame", NULL);
    }

    /**
     * \brief Releases all the resources of the application, and releases the hardware
     *    (eventually calls \ref hw_quit).
     */
    Application::~Application(){
      hw_surface_unlock(this->root_window);
      hw_surface_free(this->root_window);
      hw_quit();
    }

    /**
     * \brief Runs the application: enters the main event loop. Exits when
     *    \ref app_quit_request is called.
     */
    void Application::run(){
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
        return;
    }

    /**
     * \brief Tells the application to quite. Is usually called by an event handler (for example
     *    when pressing the "Escape" key).
     */
    void Application::quit_request(){
        if(event.param.display.closed == EI_TRUE || event.param.key.key_sym == 59){
          Application::~Application();
        }
    }

    /**
     * \brief Returns the "root widget" of the application: a "frame" widget that encapsulate the
     *    root window.
     *
     * @return      The root widget.
     */
    Frame* Application::root_widget(){
        return this->root_widget;
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
     * @return      The top-most widget at this location, or NULL if there is no widget
     *              at this location (except for the root widget).
     */
    Widget* Application::widget_pick (const Point& where){
      if((where.x < 0 || where.x > this->root_window.size.width)
      || (where.y < 0 || where.y > this->root_window.size.height)){
          fprintf(stderr,"Error occured for Application::widget_pick - param where is out the root_window\n");
          exit(EXIT_FAILURE);
      }
      color_t color = hw_get_pixel(this->root_window, where);
      if(color.red == 0 && color.green == 0 && color.blue == 0 && color.alpha == 0){
        return NULL;
      }
      uint32_t ID = ColorToUInt(color);
      return Widget::pick(ID);
    }

}
