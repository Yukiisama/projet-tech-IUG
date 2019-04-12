/**
 *  @file application.h
 *  @brief  Manages the main steps of a graphical application: initialization, main window,
 *    main loop, quitting, resource freeing.
 *
 *  \author
 *  Created by François Bérard on 30.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 *
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "hw_interface.h"
namespace ei {

class Application
{
public:
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
    Application(Size* main_window_size);

    /**
     * \brief Releases all the resources of the application, and releases the hardware
     *    (eventually calls \ref hw_quit).
     */
    ~Application();

    /**
     * \brief Returns a pointer to the application's instance. If no instance has been allocated, NULL is returned.
     * @return the instance
     */
    static Application* getInstance() {
        return instance;
    }

    /**
     * \brief Runs the application: enters the main event loop. Exits when
     *    \ref app_quit_request is called.
     */
    void run();

    /**
     * \brief Adds a rectangle to the list of rectangles that must be updated on screen. The real
     *    update on the screen will be done at the right moment in the main loop.
     *
     * @param rect    The rectangle to add, expressed in the root window coordinates.
     *        A copy is made, so it is safe to release the rectangle on return.
     */
    void invalidate_rect(const Rect &rect);

    /**
     * \brief Tells the application to quite. Is usually called by an event handler (for example
     *    when pressing the "Escape" key).
     */
    void quit_request();

    /**
     * \brief Returns the "root widget" of the application: a "frame" widget that encapsulate the
     *    root window.
     *
     * @return      The root widget.
     */
    Frame* root_widget();

    /**
     * \brief Returns the surface of the root window. Used to create surfaces with similar r, g, b
     *    channels.
     *
     * @return      The surface of the root window.
     */
    surface_t root_surface();

    /**
     * @brief   Returns the widget that is at a given location on screen.
     *
     * @param   where       The location on screen, expressed in the root window coordinates.
     *
     * @return      The top-most widget at this location, or NULL if there is no widget
     *              at this location (except for the root widget).
     */
    Widget* widget_pick (const Point& where);

    /**
     * @brief inside_root
     *
     * @param where     The point to check.
     *
     * @return      true if the point is inside the root window, else false.
     */
    bool_t inside_root (const Point& where);

    /**
     * @brief rectFusion    check if the intersection between two rectangles is big enough
     *                      to merge them. The fusion is done on rect1.
     *
     * @param rect1         The first rectangle
     *
     * @param rect2         The second rectangle
     *
     * @return              True if there were a fusion, else False.
     */
    bool_t rectFusion(Rect *rect1, Rect *rect2);

    /**
     * @brief optimizedRect
     */
    void optimizedRect();
    Rect intersectedRect(Rect r1, Rect r2);
    bool isIntersect(Rect rect1, Rect rect2);

    Application(Application const&)     = delete; // prevents  this  method  to be used
    void operator=(Application const&)  = delete; // prevents  this  method  to be used


    // *** Getters & Setter ***
    surface_t get_root_window();
    void set_root_window(surface_t root_window);
    surface_t get_offscreen();
    void set_offscreen(surface_t offscreen);
    Frame * get_widget_root();
    void set_widget_root(Frame * root_widget);
    linked_rect_t get_to_clear_rectangle_list();
    void set_to_clear_rectangle_list(linked_rect_t to_clear_rectangle_list);
    bool get_running();
    void set_running(bool running);
    double get_update_time();
    void set_update_time(double update_time);
    void set_who_click(uint32_t x);
    uint32_t get_who_click();
    // *** End Getter & Setter ***
private:
    static Application *instance;
    surface_t root_window;
    surface_t offscreen;
    Frame* widget_root;
    linked_rect_t   to_clear_rectangle_list;
    bool running = false;
    double update_time = 0;
    int who_click = 0;
};

}

#endif
