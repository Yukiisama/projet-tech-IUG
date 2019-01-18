/**
 * @file    ei_widget.h
 *
 * @brief   API for widgets management: creation, configuration, hierarchy, redisplay.
 *
 *  Created by François Bérard on 30.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 */

#ifndef EI_WIDGET_H
#define EI_WIDGET_H

#include "ei_draw.h"

#include <functional>

namespace ei {

struct Event;

/**
 * \brief   A name of a class of widget.
 */
typedef std::string widgetclass_name_t;


class GeometryManager;

/**
 * \brief   Abstract class representing a widget
 *          Every widget class specializes this base class by adding its own attributs.
 */
class Widget
{
public:

    /**
     * @brief   Construct a new instance of a widget of some particular class, as a descendant of an existing widget.
     *
     *      The widget is not displayed on screen until it is managed by a geometry manager.
     *
     * @param   class_name  The name of the class of the widget that is to be created.
     * @param   parent      A pointer to the parent widget. Cannot be NULL except for the root widget.
     */
    Widget(const widgetclass_name_t& class_name, Widget* parent);

    /**
     * @brief   Destroys the widget. Removes it from screen if it is managed by a geometry manager.
     *          Destroys all its descendants.
     */
    virtual ~Widget();

    /**
     * \brief   Method that draws the widget.
     *
     * @param   surface     Where to draw the widget. The actual location of the widget in the
     *                      surface is stored in its "screen_location" field.
     * @param   pick_surface  Offscreen buffer to draw the widget \ref pick_id
     * @param   clipper     If not NULL, the drawing is restricted within this rectangle
     *                      (expressed in the surface reference frame).
     */
    virtual void draw (surface_t surface, surface_t pick_surface, Rect* clipper);

    /**
     * \brief   Method that is called to notify the widget that its geometry has been modified
     *      by its geometry manager.
     *
     * @param   rect        The new rectangular screen location of the widget
     *                      (i.e., = widget->screen_location).
     */
    virtual void geomnotify (Rect rect);

    Widget* pick(uint32_t id);
    uint32_t getPick_id() const;

    Widget *getParent() const;

protected:
    widgetclass_name_t name; ///< The string name of this class of widget.

    static uint32_t s_idGenerator; ///< Static counter to assure the uniqueness of the generated Ids
    uint32_t     pick_id;    ///< Id of this widget in the picking offscreen.
    color_t   pick_color;    ///< pick_id encoded as a color.

    /* Widget Hierachy Management */
    Widget*  parent;             ///< Pointer to the parent of this widget.
    std::list<Widget*> children; ///< List of this widget's children

    /* Geometry Management */
    GeometryManager* geom_manager; ///< Pointer to the geometry management for this widget.
                                   ///  If NULL, the widget is not currently managed and thus, is not mapped on the screen.
    Size  requested_size;  ///< Size requested by the widget (big enough for its label, for example), or by the programmer. This can be different than its screen size defined by the placer.
    Rect  screen_location; ///< Position and size of the widget expressed in the root window reference.
    Rect* content_rect;    ///< Where to place children, when this widget is used as a container. By defaults, points to the screen_location.
};


/**
 * @brief   A function that is called in response to a user event.
 *          Usually passed as a parameter to \ref ei::EventManager::bind.
 *
 * @param   widget      The widget for which the event was generated.
 * @param   event       The event containing all its parameters (type, etc.)
 * @param   user_param  The user parameters that was provided by the caller when registering this callback.
 *
 * @return      A boolean telling if the event was consumed by the callback or not.
 *              If TRUE, the library does not try to call other callbacks for this
 *              event. If FALSE, the library will call the next callback registered
 *              for this event, if any.
 *              Note: The callback may execute many operations and still return
 *              FALSE, or return TRUE without having done anything.
 */
typedef std::function<bool_t(Widget*,Event*,void*)> ei_callback_t;


class Frame : public Widget
{
public:

    Frame(Widget* parent);

    virtual ~Frame();

    virtual void draw (surface_t surface,
                       surface_t pick_surface,
                       Rect*     clipper);

    /**
     * @brief   Configures the attributes of widgets of the class "frame".
     *
     *      Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
     *      been defined before, then a default value should be used (default values are
     *      specified for each parameter). If the parameter is "NULL" but was defined on a
     *      previous call, then its value must not be changed.
     *
     * @param   requested_size  The size requested for this widget. The geometry manager may
     *                  override this size due to other constraints.
     *                  Defaults to the "natural size" of the widget, ie. big enough to
     *                  display the text or the image, or (0, 0) if the widget has no text
     *                  and no image.
     * @param   color       The color of the background of the widget. Defaults to
     *                      \ref ei_default_background_color.
     * @param   border_width    The width in pixel of the border decoration of the widget. The final
     *                          appearance depends on the "relief" parameter. Defaults to 0.
     * @param   relief      Appearance of the border of the widget. Defaults to \ref ei_relief_none.
     * @param   text        The text to display in the widget, or NULL. Only one of the
     *                      parameter "text" and "img" should be used (i.e. non-NULL). Defaults
     *                      to NULL.
     * @param   text_font   The font used to display the text. Defaults to \ref ei_default_font.
     * @param   text_color  The color used to display the text. Defaults to
     *                      \ref ei_font_default_color.
     * @param   text_anchor The anchor of the text, i.e. where it is placed whithin the widget
     *                      when the size of the widget is bigger than the size of the text.
     *                      Defaults to \ref ei_anc_center.
     * @param   img     The image to display in the widget, or NULL. Any surface can be
     *                  used, but usually a surface returned by \ref hw_image_load. Only one
     *                  of the parameter "text" and "img" should be used (i.e. non-NULL).
     *                  Defaults to NULL.
     * @param   img_rect    If not NULL, this rectangle defines a subpart of "img" to use as the
     *                      image displayed in the widget. Defaults to NULL.
     * @param   img_anchor  The anchor of the image, i.e. where it is placed whithin the widget
     *                      when the size of the widget is bigger than the size of the image.
     *                      Defaults to \ref ei_anc_center.
     */
    void configure (Size*           requested_size,
                    const color_t*  color,
                    int*            border_width,
                    relief_t*       relief,
                    char**          text,
                    font_t*         text_font,
                    color_t*        text_color,
                    anchor_t*       text_anchor,
                    surface_t*      img,
                    Rect**          img_rect,
                    anchor_t*       img_anchor);
};


struct MouseEvent;

class Button : public Widget
{
public:

    Button(Widget* parent);

    virtual ~Button();

    virtual void draw (surface_t surface,
                       surface_t pick_surface,
                       Rect*     clipper);

    /**
     * @brief   Configures the attributes of widgets of the class "button".
     *
     * @param   widget, requested_size, color, border_width, relief, text, text_font,
     *          text_color, text_anchor, img, img_rect, img_anchor
     *              See the parameter definition of \ref Frame::configure. The only
     *              difference is that relief defaults to \ref ei_relief_raised
     *              and border_width defaults to \ref k_default_button_border_width.
     * @param   corner_radius   The radius (in pixels) of the rounded corners of the button.
     *                          0 means straight corners. Defaults to k_default_button_corner_radius.
     */
    void configure (Size*            requested_size,
                    const color_t*   color,
                    int*             border_width,
                    int*             corner_radius,
                    relief_t*        relief,
                    const char **    text,
                    font_t*          text_font,
                    color_t*         text_color,
                    anchor_t*        text_anchor,
                    surface_t*       img,
                    Rect**           img_rect,
                    anchor_t*        img_anchor);
};



class Toplevel : public Widget
{
public:
    Toplevel(Widget* parent);

    virtual ~Toplevel();

    virtual void draw (surface_t surface,
                       surface_t pick_surface,
                       Rect*     clipper);

    /**
     * @brief   Configures the attributes of widgets of the class "toplevel".
     *
     * @param   widget      The widget to configure.
     * @param   requested_size  The content size requested for this widget, this does not include
     *              the decorations (border, title bar). The geometry manager may
     *              override this size due to other constraints.
     *              Defaults to (320x240).
     * @param   color       The color of the background of the content of the widget. Defaults
     *                      to \ref ei_default_background_color.
     * @param   border_width    The width in pixel of the border of the widget. Defaults to 4.
     * @param   title       The string title diplayed in the title bar. Defaults to "Toplevel".
     * @param   closable    If true, the toplevel is closable by the user, the toplevel must
     *                      show a close button in its title bar. Defaults to \ref EI_TRUE.
     * @param   resizable   Defines if the widget can be resized horizontally and/or vertically
     *                      by the user. Defaults to \ref ei_axis_both.
     * @param   min_size    For resizable widgets, defines the minimum size. Defaults to (160, 120).
     */
    void configure (Size*           requested_size,
                    color_t*        color,
                    int*            border_width,
                    const char**    title,
                    bool_t*         closable,
                    axis_set_t*     resizable,
                    Size*           min_size);
};

}

#endif
