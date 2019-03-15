#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include <functional>
#include <iostream>
using namespace std;
namespace ei {

Toplevel::Toplevel(Widget *parent) : Widget("Toplevel", parent){
    color = default_background_color;
    border_width =4;
    top_bar_height = 24;
    title = "Toplevel";
    closable = EI_TRUE;
    resizable = ei_axis_both;
    min_size.width() = 160;
    min_size.height()=120;
    container=screen_location;

    button_size = Size(15.0,15.0);
    ///Resize button
    resize_button = new Button(this);
    resize_button_window_size = Size(12.0,12.0);
    int radius =0;
    resize_button->configure(&resize_button_window_size,&color,
                             NULL,&radius,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    p_resize_button=new Placer();

}

    Toplevel::~Toplevel(){
        delete p_button_close;
        delete p_resize_button;
        if(closable)delete button_close;
        delete resize_button;

    }

    Button* Toplevel::getButton_close() const{
        return button_close;
    }

    Button* Toplevel::getResize_button() const{
        return resize_button;
    }



    void Toplevel::draw (surface_t surface,
                        surface_t pick_surface,
                       Rect*     clipper){

        if(surface == nullptr){
            fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
            exit(EXIT_FAILURE);
        }
        //placer config for children,positon according to toplevel
        if(closable){
            int button_close_x = 3;int button_close_y = -top_bar_height+4;
            p_button_close->configure(button_close,NULL,&button_close_x,&button_close_y,NULL,NULL,NULL,NULL,NULL,NULL);
            p_button_close->run(button_close);
        }


        //positon according to toplevel
        int resize_button_x = requested_size.width()-resize_button_window_size.width();int resize_button_y = requested_size.height()-top_bar_height/2;
        p_resize_button->configure(resize_button,NULL,&resize_button_x,&resize_button_y,NULL,NULL,NULL,NULL,NULL,NULL);
        p_resize_button->run(resize_button);

        //draw outside the basic toplevel
        drawBasic_toplevel(surface,pick_surface,clipper);
        //recursive draw
        for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
            (*it)->draw(surface,pick_surface,clipper);
        }
        return;
    }
    void Toplevel::updateContent_rect(){
        container.size=requested_size;
        container.top_left.x()=screen_location.top_left.x()+border_width;
        container.top_left.y()=screen_location.top_left.y()+top_bar_height;
        setContent_rect(&container);
    }

    void Toplevel::drawBasic_toplevel(surface_t surface, surface_t pick_surface, Rect *clipper){
        if(!surface || !pick_surface){
            fprintf(stderr,"Error occured for Frame::draw - surface or pick_surface is not valid\n");
            exit(EXIT_FAILURE);
        }
        linked_point_t list_point;

        //Outside the container
        float border = (float)border_width;
        float top_bar = (float)top_bar_height;

        list_point.push_back(Point(screen_location.top_left.x()+border,
                                   screen_location.top_left.y()+top_bar));
        list_point.push_back(Point(screen_location.top_left.x(),
                                   screen_location.top_left.y()+top_bar));
        list_point.push_back(Point(screen_location.top_left.x(),
                                   screen_location.top_left.y()+requested_size.height()+top_bar+border));
        list_point.push_back(Point(screen_location.top_left.x()+requested_size.width()+2*border,
                                   screen_location.top_left.y()+requested_size.height()+top_bar+border));
        list_point.push_back(Point(screen_location.top_left.x()+requested_size.width()+2*border,
                                   screen_location.top_left.y()));
        list_point.push_back(screen_location.top_left);

        list_point.push_back(Point(screen_location.top_left.x(),
                                   screen_location.top_left.y()+top_bar));
        list_point.push_back(Point(screen_location.top_left.x()+requested_size.width()+border,
                                   screen_location.top_left.y()+top_bar));
        list_point.push_back(Point(screen_location.top_left.x()+requested_size.width()+border,
                                   screen_location.top_left.y()+requested_size.height()+top_bar));
        list_point.push_back(Point(screen_location.top_left.x()+border,
                                   screen_location.top_left.y()+requested_size.height()+top_bar));
        draw_polygon(surface,list_point,color,clipper);
        //draw pick_surface outside the container
        hw_surface_lock(pick_surface);
        pick_color.alpha=255;
        draw_polygon(pick_surface,list_point,pick_color,clipper);
        hw_surface_unlock(pick_surface);

        color_t color_white = {255,255,255,255};
        //draw toplevel background
        linked_point_t list_point_bgr;
        list_point_bgr.push_back(content_rect->top_left);
        list_point_bgr.push_back(Point(content_rect->top_left.x(),
                                       content_rect->top_left.y()+content_rect->size.height()));
        list_point_bgr.push_back(Point(content_rect->top_left.x()+content_rect->size.width(),
                                       content_rect->top_left.y()+content_rect->size.height()));
        list_point_bgr.push_back(Point(content_rect->top_left.x()+content_rect->size.width(),
                                       content_rect->top_left.y()));
        draw_polygon(surface,list_point_bgr,color_white,clipper);
        //draw pick_surface outside the container
        hw_surface_lock(pick_surface);
        pick_color.alpha=255;
        draw_polygon(pick_surface,list_point_bgr,pick_color,clipper);
        hw_surface_unlock(pick_surface);


        ///Title of the window
        Point where = Point(screen_location.top_left.x()+40,screen_location.top_left.y()+top_bar_height*0.05);
        font_t title_font=hw_text_font_create(default_font_filename, top_bar_height*0.8);
        draw_text(surface,&where,title,title_font,&color_white);
        hw_text_font_free(title_font);

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
                              const char**    title,
                              bool_t*         closable,
                              axis_set_t*     resizable,
                              Size*           min_size){
        Widget::configure(requested_size,color);
        if(title) this->title = *title;
        if(border_width) this->border_width = *border_width;
        if(closable) this->closable = *closable;

        if(resizable) this->resizable = *resizable;
        if(min_size) this->min_size = *min_size;
        //Button close
        if(this->closable) {
            cout<<"check"<<endl;
            button_close = new Button(this);
            color_t button_color = {255,0,0,255};
            int button_close_radius =7;
            button_close->configure(&button_size,&button_color,
                                    NULL,&button_close_radius,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
            p_button_close=new Placer();
        }

    }
}
