#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_application.h"
namespace ei {


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
    }

    /**
     * \brief Releases all the resources of the application, and releases the hardware
     *    (eventually calls \ref hw_quit).
     */
    Application::~Application(){
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
        return;
    }

    /**
     * \brief Returns the "root widget" of the application: a "frame" widget that encapsulate the
     *    root window.
     *
     * @return      The root widget.
     */
    Frame* Application::root_widget(){
        return NULL;
    }

    /**
     * \brief Returns the surface of the root window. Used to create surfaces with similar r, g, b
     *    channels.
     *
     * @return      The surface of the root window.
     */
    surface_t Application::root_surface(){
        return;
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
        return NULL;
    }

}
