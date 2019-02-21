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

        if(surface == nullptr){
            fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
            exit(EXIT_FAILURE);
        }
        //White frame

        color_t color_white;
        color_white.red = 255;
        color_white.green = 255;
        color_white.blue = 255;
        color_white.alpha = 255;

        color_t color_red;
        color_red.red = 255;
        color_red.green = 0;
        color_red.blue = 0;
        color_red.alpha = 255;

        linked_point_t list_frame;
        list_frame.push_back(screen_location.top_left);

        list_frame.push_back(Point(screen_location.top_left.x()+(requested_size)->width(),
                                   screen_location.top_left.y()));

        list_frame.push_back(Point(screen_location.top_left.x()+(requested_size)->width(),
                                   screen_location.top_left.y()+(requested_size)->height()));

        list_frame.push_back(Point(screen_location.top_left.x(),
                                   screen_location.top_left.y()+(requested_size)->height()));

        draw_polygon(surface,list_frame,default_background_color,clipper);
        draw_polygon(pick_surface,list_frame,pick_color,clipper);

        float border = (float)*border_width;
        linked_point_t list_white_frame;
        list_white_frame.push_back(Point(screen_location.top_left.x()+border,
                                   screen_location.top_left.y()*4));

        list_white_frame.push_back(Point(screen_location.top_left.x()+(requested_size)->width()- border,
                                   screen_location.top_left.y()*4));

        list_white_frame.push_back(Point(screen_location.top_left.x()+(requested_size)->width()- border,
                                   screen_location.top_left.y()+(requested_size)->height()- border));

        list_white_frame.push_back(Point(screen_location.top_left.x()+border,
                                   screen_location.top_left.y()+(requested_size)->height()- border));

        draw_polygon(surface,list_white_frame,color_white,clipper);

        linked_point_t list_bottom_right_button;
        list_bottom_right_button.push_back(Point(screen_location.top_left.x()+(requested_size)->width(),
                                   screen_location.top_left.y()+(requested_size)->height()));
        list_bottom_right_button.push_back(Point((screen_location.top_left.x()+(requested_size)->width())*0.90,
                                   screen_location.top_left.y()+(requested_size)->height()));
        list_bottom_right_button.push_back(Point((screen_location.top_left.x()+(requested_size)->width())*0.90,
                                   (screen_location.top_left.y()+(requested_size)->height())*0.90));
        list_bottom_right_button.push_back(Point((screen_location.top_left.x()+(requested_size)->width()),
                                   (screen_location.top_left.y()+(requested_size)->height())*0.90 ));

        draw_polygon(surface,list_bottom_right_button,default_background_color,clipper);

        if(title){
            Point where = {(int) (screen_location.top_left.x()*1.50), (int) (screen_location.top_left.y()*1.10)};
            draw_text(surface,&where,*title,hw_text_font_create(default_font_filename, font_default_size),&color_white);
        }

        Size button_top_left_size = {(float)(((requested_size)->width())*0.05),(float)(((requested_size)->height())*0.05)};
        Rect button_rect = Rect(screen_location.top_left,button_top_left_size);
        //draw_button(surface,&button_rect,color_red,default_button_corner_radius,clipper);

        for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
            (*it)->draw(surface,pick_surface,clipper);
        }

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
