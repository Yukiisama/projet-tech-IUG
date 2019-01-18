/**
 *  @file ei_draw.h
 *  @brief  Graphical primitives to draw lines, polygons, text, and operation of drawing
 *    surfaces.
 *
 *  \author
 *  Created by François Bérard on 30.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 *
 */

#ifndef EI_DRAW_H
#define EI_DRAW_H

#include <stdint.h>
#include "ei_types.h"
#include "hw_interface.h"

namespace ei {

/**
 * \brief   Allocate and set the list of points defining an arc
 *
 * @param   center      Center of the arc
 * @param   radius      Radius of the arc
 * @param   start_angle Beginning angle
 * @param   end_angle   Ending angle
 * @return  Returns the list of points defining the arc.
 */
linked_point_t arc(const Point& center, float radius, int start_angle, int end_angle);

typedef enum {BT_TOP, BT_BOTTOM, BT_FULL} bt_part;

/**
 * \brief   Defines the list of points to draw a rounded frame
 *
 * @param   rect      Rectangle
 * @param   radius    Radius of the arcs
 * @param   part      Only set the pints for the TOP, BOTTOM or ALL part of the rounded frame
 * @return  Returns the list of points defining the rounded frame
 */
linked_point_t rounded_frame(const Rect &rect, float radius, bt_part part);

/**
 * \brief Draws a single line using Bresenham algorithm
 *
 * @param surface Where to draw the line. The surface must be *locked* by \ref hw_surface_lock.
 * @param start   Start position of the line.
 * @param end     End position of the line.
 * @param color   The color used to draw the line, alpha channel is managed.
 * @param clipper If not NULL, the drawing is restricted within this rectangle.
 */
void draw_line(surface_t surface, const Point &start,
               const Point &end, const color_t &color,
               const Rect* clipper);

/**
 * \brief Draws a line made of many line segments.
 *
 * @param surface     Where to draw the line. The surface must be *locked* by \ref hw_surface_lock.
 * @param point_list  Linked list of the points of the line. It can can be empty, have a single point, or more.
 *                    If the last point is the same as the first point, then this pixel is drawn only once.
 * @param color       The color used to draw the line, alpha channel is managed.
 * @param clipper     If not NULL, the drawing is restricted within this rectangle.
 */
void draw_polyline(surface_t surface,
                   const linked_point_t &point_list,
                   const color_t color, const Rect* clipper);

/**
 * \brief Draws a filled polygon.
 *
 * @param surface     Where to draw the polygon. The surface must be locked* by \ref hw_surface_lock.
 * @param point_list  A linked list of the points of the line.
 *                    It is either empty (i.e. draws nothing), or has more than 2 points.
 * @param color       The color used to draw the polygon, alpha channel is managed.
 * @param clipper     If not NULL, the drawing is restricted within this rectangle.
 */
void draw_polygon(surface_t surface, const linked_point_t& point_list,
                  const color_t &color, const Rect* clipper);

/**
 * \brief Draws text by calling \ref hw_text_create_surface.
 *
 * @param surface   Where to draw the text. The surface must be *locked* by \ref hw_surface_lock.
 * @param where     Coordinates, in the surface, where to anchor the *top-left corner of the rendered text.
 * @param text      The string of the text. Can't be NULL.
 * @param font      The font used to render the text. If NULL, the \ref ei_default_font is used.
 * @param color     The text color. Can't be NULL. The alpha parameter is not used.
 */
void draw_text(surface_t surface, const Point* where,
               const char* text, const font_t font,
               const color_t* color);

/**
 * \brief Fills the surface with the specified color.
 *
 * @param surface   The surface to be filled. The surface must be *locked* by \ref hw_surface_lock.
 * @param color     The color used to fill the surface.
 *                  If NULL, it means that the caller want it painted black (opaque, \ref ei_font_default_color).
 */
void fill(surface_t surface, const color_t* color, const bool_t use_alpha);


/**
 * \brief Copies a surface source onto another one destination.
 *        Both the source and destination surfaces must be *locked* by \ref hw_surface_lock.
 *
 * @param destination The surface on which to copy pixels from the source surface.
 * @param source      The surface from which to copy pixels.
 * @param where    Coordinates, in the destination surface, where to anchor the *top-left corner of the  source
 *                    surface. If NULL, the entire source surface is used.
 *                    If not NULL, defines the rectangle on the source surface from which to copy the pixels.
 * @param use_alpha   If true, the final pixels are a combination of source and destination pixels
 *                    weighted by the source alpha channel. The transparency of the final pixels is set to opaque.
 *                    If false, the final pixels are an exact copy of the source pixels, including the alpha channel.
 */
void ei_copy_surface(surface_t destination, const surface_t source,
                     const Point* where, const bool_t use_alpha);

}
#endif
