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
    top_bar_height = 20;
    title = "Toplevel";
    closable = EI_TRUE;
    resizable = ei_axis_both;
    min_size.width() = 160;
    min_size.height()=120;

    ///Button close
    button_close = new Button(this);
    Size button_size = Size(24.0,24.0);
    color_t button_color = {255,0,0,255};
    button_close->configure(&button_size,&button_color,
                            NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    //TODO button_close placer according to toplevel requested size and toplevel bar
    p_button_close=new Placer();

    ///Resize button
    resize_button = new Button(this);
    Size resize_button_window_size = Size(24.0,24.0);
    resize_button->configure(&resize_button_window_size,&color,
                             NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    //TODO resize_button placer according to toplevel requested
    p_resize_button=new Placer();

    ///Frame zone
    //in_window = new Frame(this);
    //TODO button_close placer according to toplevel requested size
    //p_in_window=new Placer();
}

    Toplevel::~Toplevel(){
        delete p_button_close;
        delete p_resize_button;
        delete p_in_window;
        delete button_close;
        delete resize_button;
        //delete in_window;

    }

    Button* Toplevel::getButton_close() const{
        return button_close;
    }

    Button* Toplevel::getResize_button() const{
        return resize_button;
    }

    Frame* Toplevel::getIn_window() const{
        //return in_window;
        return NULL;
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

        ///Title of the window

        color_t color_white = {255,255,255,255};

        Point where = Point(screen_location.top_left.x()+40,screen_location.top_left.y()+top_bar_height*0.05);
        draw_text(surface,&where,title,hw_text_font_create(default_font_filename, top_bar_height*0.8),&color_white);

        for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
            cout<< (*it)->getName()<<endl;
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
        if(title) this->title = *title;
        if(border_width) this->border_width = *border_width;
        if(closable) this->closable = *closable;
        if(resizable) this->resizable = *resizable;
        if(min_size) this->min_size = *min_size;

        //in_window->configure(&this->requested_size,NULL,
        //                     NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    }
    //Getter & Setter
    int  Toplevel::get_border_width(){
        return this->border_width;
    }
    void Toplevel::set_border_width(int border_width){
        this->border_width = border_width;
    }
    double Toplevel::get_top_bar_height(){
        return this->top_bar_height;
    }
    void   Toplevel::set_top_bar_height( double top_bar_height){
        this->top_bar_height = top_bar_height;
    }
    const char * Toplevel::get_title(){
        return this->title;
    }
    void Toplevel::set_title(const char * title){
        this->title = title;
    }
    bool_t Toplevel::get_closable(){
        return this->closable;
    }
    void Toplevel::set_closable(bool_t closable){
        this->closable = closable;
    }
    axis_set_t Toplevel::get_resizable(){
        return this->resizable;
    }
    void Toplevel::set_resizable(axis_set_t resizable){
        this->resizable = resizable;
    }
    Size Toplevel::get_min_size(){
        return this->min_size;
    }
    void Toplevel::set_min_size( Size min_size){
        this->min_size = min_size;
    }
    Button * Toplevel::get_button_close(){
        return this->button_close;
    }
    void Toplevel::set_button_close(Button * button_close){
        this->button_close = button_close;
    }
    Placer * Toplevel::get_p_button_close(){
        return this->p_button_close;
    }
    void Toplevel::set_p_button_close(Placer * p_button_close){
        this->p_button_close = p_button_close;
    }
    Button * Toplevel::get_resize_button(){
        return this->resize_button;
    }
    void Toplevel::set_resize_button(Button * resize_button){
        this->resize_button = resize_button;
    }
    Placer * Toplevel::get_p_resize_button(){
        return this->p_resize_button;
    }
    void Toplevel::set_p_resize_button(Placer * p_resize_button){
        this->p_resize_button = p_resize_button;
    }
    Placer * Toplevel::get_p_in_window(){
        return this->p_in_window;
    }
    void Toplevel::set_p_in_window(Placer * p_in_window){
        this->p_in_window = p_in_window;
    }
    //End Getter & Setter
}
