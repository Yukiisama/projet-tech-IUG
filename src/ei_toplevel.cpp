#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include <functional>

namespace ei {

    Toplevel::Toplevel(Widget *parent) : Widget("Toplevel", parent){
      requested_size->width() = 320;
      requested_size->height() = 240;
      *border_width = 4;
      *title = "Toplevel";
      *closable = EI_TRUE;
      *resizable = ei_axis_both;
      min_size->width() = 160;
      min_size->height() = 120;
    }

    Toplevel::~Toplevel(){

    }

    void Widget::draw (surface_t surface,
                       surface_t pick_surface,
                       Rect*     clipper){
        return;
    }

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
    void Toplevel::configure (Size*           requested_size,
                              color_t*        color,
                              int*            border_width,
                              const char**          title,
                              bool_t*         closable,
                              axis_set_t*     resizable,
                              Size*           min_size){
        if(img && text) fprintf(stderr,"Only one of the parameter \"text\" and \"img\" should be used (i.e. non-NULL).");
        Widget::configure(requested_size,color);
        if(title) this->title = title;
        if(border_width) this->border_width = border_width;
        if(requested_size) this->requested_size = requested_size;
        if(closable) this->closable = closable;
        if(resizable) this->resizable = resizable;
        if(min_size) this->min_size = min_size;
    }
}
