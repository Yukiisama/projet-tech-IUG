#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"

namespace ei
{

    Button::Button(Widget *parent) : Widget("Button", parent){
        border_width = default_button_border_width;
        corner_radius = new int(default_button_corner_radius);// check this variable is free in destructor
        relief=ei_relief_raised;
        text=nullptr;
        text_font=default_font;
        text_color=font_default_color;
        text_anchor=ei_anc_center;
        img=nullptr;
        img_rect=nullptr;
        img_anchor = new anchor_t{ei_anc_center};
    }


    Button::~Button()
    {

    }

    void Button::draw(surface_t surface,
                      surface_t pick_surface,
                      Rect *clipper)
    {
        if(surface == nullptr){
          fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
          exit(EXIT_FAILURE);
        }

        //The Rect of the button
        Rect button_rect = Rect(screen_location.top_left,requested_size);
        draw_button(surface,&button_rect,color,*corner_radius,clipper);

        for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
            (*it)->draw(surface,pick_surface,clipper);
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
        if(corner_radius) this->corner_radius = corner_radius;
        if(relief) this->relief = *relief;
        if(text) this->text = text;
        if(text_font) this->text_font = *text_font;
        if(text_color) this->text_color = *text_color;
        if(img) this->img = img;
        if(img_rect) this->img_rect = *img_rect;
        if(img_anchor)this->img_anchor = img_anchor;
    }


}
