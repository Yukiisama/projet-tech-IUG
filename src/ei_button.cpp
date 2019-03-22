#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include <iostream>
#include "ei_application.h"
namespace ei
{

    Button::Button(Widget *parent) : Widget("Button", parent){
        border_width = default_button_border_width;
        corner_radius = default_button_corner_radius;
        relief=ei_relief_raised;
        text=nullptr;
        text_font= hw_text_font_create(default_font_filename, font_default_size);
        text_color=font_default_color;
        text_anchor=ei_anc_center;
        img=nullptr;
        img_rect=nullptr;
        img_anchor =ei_anc_center;
        clicked=EI_FALSE;
    }


    Button::~Button()
    {
        hw_text_font_free(text_font);

    }

    void Button::draw(surface_t surface,
                      surface_t pick_surface,
                      Rect *clipper)
    {
        if(surface == nullptr){
          fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
          exit(EXIT_FAILURE);
        }
        if(!pick_surface){
          fprintf(stderr,"Error occured for Frame::draw - pick_surface is not vaild\n");
          exit(EXIT_FAILURE);
        }

        //The Rect of the button.
        Rect button_rect = Rect(screen_location.top_left,requested_size);
        //Draw on pick_surface the forme of button with button's pick_color.
        hw_surface_lock(pick_surface);
        //The list of points to draw the button
        linked_point_t list_frame = rounded_frame(button_rect, corner_radius, BT_FULL);
        pick_color.alpha=255;
        draw_polygon(pick_surface, list_frame, pick_color, clipper);
        hw_surface_unlock(pick_surface);

        //Draw button.
        if(clicked){//Case button is clicked by a mouse, draw clicked version of button.
            Application::getInstance()->invalidate_rect(*content_rect);
            draw_button(surface,&button_rect,color,corner_radius,border_width,clipper,clicked);
            clicked=EI_FALSE;
        }else{      //Draw normale static button.
            draw_button(surface,&button_rect,color,corner_radius,border_width,clipper,clicked);
        }

        if (text)
        {
            Size text_size=Size(0,0);
            hw_text_compute_size(text,text_font,text_size);
            Point where = text_anchor_to_pos(screen_location, text_anchor,text_size,border_width);
            draw_text(surface, &where, text, text_font, &text_color);
        }
        if(img){


            if(img_rect){ //case where only subpart of img should be display.
                hw_surface_lock(surface);
                hw_surface_lock(img);
                for (int i =img_rect->top_left.x();i<img_rect->top_left.x()+img_rect->size.width();i++) {
                    for (int j = img_rect->top_left.y();j<img_rect->top_left.y()+img_rect->size.height();j++) {
                        Point pos = Point(i,j);
                        color_t img_c = hw_get_pixel(img,pos);
                        hw_put_pixel(surface,pos,img_c);
                    }
                }
                hw_surface_unlock(surface);
                hw_surface_unlock(img);
            }else {       //default case, display img according to img_anchor.
                cout<<"img detected"<<endl;
                Point pos = anchor_to_pos(screen_location,img_anchor);
                ei_copy_surface(surface,img,&pos,EI_TRUE);
            }
        }

        //Recursive method that draw all children of current button.
        for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
            //Children should be display inside the content_rect of his parent.
            (*it)->draw(surface,pick_surface,content_rect);
        }
    }

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
    void Button::configure(Size *requested_size,
                           const color_t    *color,
                           int              *border_width,
                           int              *corner_radius,
                           relief_t         *relief,
                           const char       **text,
                           font_t           *text_font,
                           color_t          *text_color,
                           anchor_t         *text_anchor,
                           surface_t        *img,
                           Rect             **img_rect,
                           anchor_t         *img_anchor){
        if(img && text) fprintf(stderr,"Only one of the parameter \"text\" and \"img\" should be used (i.e. non-NULL).");
        Widget::configure(requested_size,color);
        if(border_width) this->border_width = *border_width;
        if(corner_radius) this->corner_radius = *corner_radius;
        if(relief) this->relief = *relief;
        if(text && !img) this->text = *text;
        if(text_font) this->text_font = *text_font;
        if(text_color) this->text_color = *text_color;
        if(text_anchor) this->text_anchor = *text_anchor;
        if(img && !text) this->img = img;
        if(img_rect) this->img_rect = *img_rect;
        if(img_anchor)this->img_anchor = *img_anchor;
    }
    //GETTER & SETTER
        int Button::get_corner_radius(){
            return this->corner_radius;
        }
        void Button::set_corner_radius(int corner_radius){
            this->corner_radius = corner_radius;
        }
        relief_t Button::get_relief(){
            return this->relief;
        }
        void Button::set_relief(relief_t relief){
            this->relief = relief;
        }
        const char * Button::get_text(){
            return this->text;
        }
        void Button::set_text(const char * text){
            this->text = text;
        }
        font_t Button::get_text_font(){
            return this->text_font;
        }
        void Button::set_text_font(font_t text_font){
            this->text_font = text_font;
        }
        color_t Button::get_text_color(){
            return this->text_color;
        }
        void Button::set_text_color(color_t text_color){
            this->text_color = text_color;
        }
        anchor_t Button::get_text_anchor(){
            return this->text_anchor;
        }
        void Button::set_text_anchor(anchor_t text_anchor){
            this->text_anchor=text_anchor;
        }
        surface_t Button::get_img(){
            return this->img;
        }
        void Button::get_img(surface_t img){
            this->img=img;
        }
        Rect * Button::get_img_rect(){
            return this->img_rect;
        }
        void Button::set_img_rect(Rect * img_rect){
            this->img_rect = img_rect;
        
        }
        anchor_t Button::get_img_anchor(){
            return this->img_anchor;
        }
        void Button::set_img_anchor(anchor_t img_anchor){
            this->img_anchor=img_anchor;
        }
    //END GETTER & SETTER
}
