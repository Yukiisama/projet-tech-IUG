/**
 * @file  hw_interface.h
 *
 * @brief Low level interface with the graphic hadware. This interface is based on the Allegro library.
 *
 *  Created by François Bérard on 30.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 */

#ifndef HW_INTERFACE_H
#define HW_INTERFACE_H

#include <stdint.h>

#include "ei_types.h"

extern const int EI_MOUSEBUTTON_LEFT;
extern const int EI_MOUSEBUTTON_MIDDLE;
extern const int EI_MOUSEBUTTON_RIGHT;

/**
 * @typedef   ei_surface_t
 * @brief   Surface hidden type. A surface represents a 2 dimentional array of pixels
 *          where drawing can be done. The displayed screen itself is represented by a surface,
 *          it is accessed by \ref hw_create_window. Other "offscreen" surfaces can be created
 *          by \ref hw_surface_create.
 */

typedef void* surface_t;

/**
 * @brief Initialises access to the low-level operating system services.
 */
void hw_init();

/**
 * @brief Closes the access to the low-level operating system services.
 */
void hw_quit();

/**
 * @brief   Opens the main graphical window of the application.
 *
 * @param   size        Number of horizontal and vertical pixels.
 * @param   fullScreen  If true, opens the window in full screen.
 *                      Otherwise opens a floating window.
 *
 * @return  The unlocked drawing surface (see \ref hw_surface_lock).
 *          This surface  should not be freed by calling \ref hw_surface_free,
 *          it is freed when releasing access to the low-level services
 *          by calling \ref hw_quit.
 */

surface_t hw_create_window(ei::Size* size, const ei::bool_t fullScreen);

/**
 * @brief   Allocates an off-screen drawing surface.
 *
 * @param   root      The root window which channel indices will be used.
 *                    This insures that the offscreen uses the same channel indices
 *                    (Red, Green, Blue, Alpha) as the root surface.
 * @param   size      Number of horizontal and vertical pixels.
 *
 * @return      The unlocked drawing surface (see \ref hw_surface_lock).
 *              The surface should be freed by calling \ref hw_surface_free.
 */

surface_t hw_surface_create(const surface_t root, const ei::Size* size);

/**
 * @brief   Frees a surface allocated by \ref hw_surface_create.
 *          This must be called on an unlocked surface (see \ref hw_surface_unlock).
 *
 * @param   surface   The surface to be freed.
 */
void hw_surface_free(surface_t surface);

/**
 * @brief Gains exclusive access to a surface to draw a single pixel with \ref hw_put_pixel.
 *        Every call to this function must be matched by a call to \ref hw_surface_unlock.
 *
 * @param   surface   The surface to lock.
 */
void hw_surface_lock(surface_t surface);

/**
 * @brief   Releases the exclusive access to a surface that was locked by \ref hw_surface_lock.
 *
 * @param   surface   The surface to unlock.
 */
void hw_surface_unlock(surface_t surface);

/**
 * @brief Returns the size of a surface.
 *
 * @param   surface   The surface which size is requested.
 *
 * @return  The size of the surface.
 */
ei::Size hw_surface_get_size(const surface_t surface);

/**
 * @brief Requests that a list of rectangular regions of the root surface
 *        be updated on screen.
 *
 * @param rects   The list of rectangle to be updated on screen.
 *                If empty, them the entire surface is updated.
 */
void hw_surface_update_rects(const ei::linked_rect_t &rects);

/**
 * @brief Returns the rectangle of a surface (origin and size).
 *
 * @param   surface   The surface which rectangle is requested.
 *
 * @return  The rectangle of the surface.
 */
ei::Rect hw_surface_get_rect(const surface_t surface);

/**
 * @brief Returns the color of the pixel at pos.
 *
 * @param   surface   The surface on which to draw.
 *
 * @param   pos       The position at which to draw.
 *
 * @param   color     The color to draw.
 *
 * @return  The color of the pixel.
 */
ei::color_t hw_get_pixel(const surface_t surface, const ei::Point pos);

/**
 * @brief Draw the pixel of the surface at pos.
 *
 * @param   surface   The surface on which to draw.
 *
 * @param   pos       The position at which to draw.
 *
 * @param   color     The color to draw.
 *
 */
void hw_put_pixel(const surface_t surface, const ei::Point pos,
                  const ei::color_t color);

/**
 * @brief Creates a surface and loads into it an image read from a file.
 *        The caller is responsible to release this surface (\ref hw_surface_free)
 *        when it is no more needed.
 *
 * @param filename  The name of the file containing the image. The file can be
 *                  .bmp, .png, .jpg, .tga
 *
 * @return  A new unlocked surface containing the image.
 */
surface_t hw_image_load(const char* filename);

/**
 * @brief Creates a font that can be used to render text. The font must be
 *        freed by calling \ref hw_text_font_free.
 *
 * @param   filename  The path to the file containing the ttf font definition.
 *                    Can be relative.
 * @param   style   The style of the font (normal, bold, ...).
 * @param   size    The size of the characters in pixels.
 *
 * @return  The font.
 */
ei::font_t hw_text_font_create(const char* filename, int size);

/**
 * @brief Frees a font created by \ref hw_text_font_create.
 *
 * @param font    The font to be freed.
 */
void hw_text_font_free(ei::font_t font);

/**
 * @brief Computes the size of a text surface givent the font and the
 *text.
 *
 * @param text    The string of the message.
 * @param font    The font used to render the text.
 * @param size      Addresses where to store the computed width
 *                  and height of the text surface.
 */
void hw_text_compute_size(const char* text, const ei::font_t font, ei::Size &size);

/**
 * @brief Creates a surface containing a text. The size of the created surface is just big
 *    enough to contain the text. The caller is responsible to release this surface
 *    (\ref hw_surface_free) when it is no more needed.
 *
 * @param text    The string of the message.
 * @param font    The font used to render the text.
 * @param color   The text color. The alpha parameter is not used.
 *                However, the text is rendered with alpha blending to smooth
 *                the curves of the letters (anti-aliasing).
 *
 * @return    A newly created unlocked surface containing an anti-aliased rendering of the text.
 *            The anti-aliasing is implemented with the alpha channel of the surface:
 *            pixels on the text's boundaries have some transparency.
 */
surface_t hw_text_create_surface(const char* text, const ei::font_t font,
                                    const ei::color_t* color);

/**
 * @brief Waits for the specified number of seconds.
 *        This tells the system to pause the current thread for the given amount of time.
 *
 * @param s_delay The amount of time, in seconds, to wait.
 *
 */
void hw_wait(int s_delay);

namespace ei {
    struct Event;
}

/**
 * @brief Lets this processus sleep until a new event is available.
 *
 * @param event   Where to store the new event. The structure will be allocated in this function.
 *                      On return, the structure is filled with informations about the new event.
 */
ei::Event *hw_event_wait_next();

/**
 * @brief Put an application-generated event on the event queue.
 *        This will cause \ref hw_event_wait_next to wake.
 *
 * @param user_param  The user parameter that will be retrievable in the event.
 */
ei::bool_t hw_event_post_app(void* user_param);

/**
 * @brief Returns  the number of seconds since the library was initialised
 *        by \ref hw_init. Can be used to measure elpased time between to calls.
 *
 * @return  The current time, in seconds.
 */
double hw_now();

#endif
