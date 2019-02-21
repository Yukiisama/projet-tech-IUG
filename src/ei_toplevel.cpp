#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include <functional>

namespace ei {

Toplevel::Toplevel(Widget *parent) : Widget("Toplevel", parent){
requested_size = new Size(320,240);
    border_width = new int(4);
    title = new const char*("Toplevel");
    closable = new bool_t(EI_TRUE);
    resizable = new axis_set_t (ei_axis_both);
    min_size = new Size(160,120);
}

    Toplevel::~Toplevel(){

    }

    void Toplevel::draw (surface_t surface,
                       surface_t pick_surface,
                       Rect*     clipper){
                         /*
        if(surface == nullptr){
            fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
            exit(EXIT_FAILURE);
        }
        /*
        Size top_size = {requested_size.width(),requested_size.height()*0.10};
        Rect button_rect = Rect(screen_location.top_left,requested_size);
        //color_t color_top
        draw_button(surface,&button_rect,color,*corner_radius,clipper);

        //White frame
        linked_point_t list_frame;
        Point white_rect;
        white_rect.x() = screen_location.top_left.x();
        white_rect.y() = screen_location.top_left.y()*0.90;

        list_frame.push_back(white_rect);

        list_frame.push_back(Point(screen_location.top_left.x()+requested_size.width(),
                                   white_rect.y()));

        list_frame.push_back(Point(screen_location.top_left.x()+requested_size.width(),
                                   screen_location.top_left.y()+requested_size.height()));

        list_frame.push_back(Point(screen_location.top_left.x(),
                                   screen_location.top_left.y()+requested_size.height()));

        draw_polygon(surface,list_frame,color,clipper);
        */
        /*
        for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
            (*it)->draw(surface,pick_surface,clipper);
        }
        */
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
        Widget::configure(requested_size,color);
        if(title) this->title = title;
        if(border_width) this->border_width = border_width;
        if(requested_size) this->requested_size = requested_size;
        if(closable) this->closable = closable;
        if(resizable) this->resizable = resizable;
        if(min_size) this->min_size = min_size;
    }
}
