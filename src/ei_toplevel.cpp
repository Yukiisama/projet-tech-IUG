#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include <functional>

namespace ei {

Toplevel::Toplevel(Widget *parent) : Widget("Toplevel", parent){
    color = default_background_color;
    border_width = new int(4);
    top_bar_height = new double(20);
    title = new const char*("Toplevel");
    closable = EI_TRUE;
    resizable = new axis_set_t (ei_axis_both);
    min_size = new Size(160,120);

    ///Button close
    button_close = new Button(this);
    Size button_size = Size(24.0,24.0);
    color_t button_color = {255,0,0,255};
    button_close->configure(&button_size,&button_color,
                            NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

    ///Resize button
    resize_button = new Button(this);
    Size resize_button_window_size = Size(24.0,24.0);
    resize_button->configure(&resize_button_window_size,&color,
                             NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);

    ///Frame zone
    in_window = new Frame(this);
    color_t in_window_color = {255,255,255,255};
    in_window->configure(&requested_size,&in_window_color,
                         NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
}

    Toplevel::~Toplevel(){
    }

    Button* Toplevel::getButton_close() const{
        return button_close;
    }

    Button* Toplevel::getResize_button() const{
        return resize_button;
    }

    Frame* Toplevel::getIn_window() const{
        return in_window;
    }

    Point Toplevel::getMouse_pos() const{
        return mouse_pos;
    }

    Size Toplevel::getMin_size() const{
        return *min_size;
    }

    void Toplevel::setMouse_pos(Point point){
        mouse_pos = point;
    }

    bool_t Toplevel::inside_top_bar(Point where) const{
        if(where.x()>=screen_location.top_left.x()
                && where.x()<=screen_location.top_left.x()+requested_size.width()
                && where.y()>=screen_location.top_left.y()
                /*&& where.y()<=screen_location.top_left.y()+top_bar_height*/){
            return EI_TRUE;
        }
        else{
            return EI_FALSE;
        }
    }

    bool_t Toplevel::moving() const{
        return top_bar_clicked;
    }

    bool_t Toplevel::resizing() const{
        return resize_button_pressed;
    }

    bool_t Toplevel::closing() const{
        return button_close_pressed;
    }

    void Toplevel::set_top_bar_clicked(bool_t clicking){
        top_bar_clicked = clicking;
    }

    void Toplevel::set_resize_button_pressed(bool_t pressed){
        resize_button_pressed = pressed;
    }

    void Toplevel::set_button_close_pressed(bool_t pressed){
        button_close_pressed = pressed;
    }

    void Toplevel::draw (surface_t surface,
                       surface_t pick_surface,
                       Rect*     clipper){

        if(surface == nullptr){
            fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
            exit(EXIT_FAILURE);
        }

        linked_point_t list_point;

        ///Outside the window

        float border = (float)*border_width;
        float top_bar = (float)*top_bar_height;

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

        ///Title of the window

        color_t color_white = {255,255,255,255};

        Point where = Point(screen_location.top_left.x()+40,screen_location.top_left.y()+*top_bar_height*0.05);
        draw_text(surface,&where,*title,hw_text_font_create(default_font_filename, *top_bar_height*0.8),&color_white);

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
                              const char**    title,
                              bool_t*         closable,
                              axis_set_t*     resizable,
                              Size*           min_size){
        Widget::configure(requested_size,color);
        if(title) this->title = title;
        if(border_width) this->border_width = border_width;
        if(closable) this->closable = *closable;
        if(resizable) this->resizable = resizable;
        if(min_size) this->min_size = min_size;

        in_window->configure(&this->requested_size,NULL,
                             NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
}
