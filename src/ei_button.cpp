#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"

namespace ei
{

    Button::Button(Widget *parent) : Widget("Button", parent){
        border_width = default_button_border_width;
        *corner_radius = default_button_corner_radius;
        relief=ei_relief_raised;
        text=nullptr;
        text_font=default_font;
        text_color=font_default_color;
        text_anchor=ei_anc_center;
        img=nullptr;
        img_rect=nullptr;
        img_anchor= nullptr;
    }


    Button::~Button()
    {

    }

    void Button::draw(surface_t surface,
                      surface_t pick_surface,
                      Rect *clipper)
    {

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
        Widget::configure(requested_size,color);
        //The others (arg exists) ? assign arg : assign default;

        if(border_width == nullptr){
            *border_width = default_button_border_width;
        }
        if(corner_radius == nullptr){
            *corner_radius = default_button_corner_radius;
        }
        (relief) ? this->relief = *relief : this->relief = ei_relief_none;
        (text) ? this->text = text : this->text = nullptr;
        (text_font) ? this->text_font = *text_font : this->text_font = default_font;
        (text_color) ? this->text_color = *text_color : this->text_color = font_default_color;
        (img) ? this->img = img : this->img = nullptr;
        (img_rect) ? this->img_rect = *img_rect : this->img_rect = nullptr;
        if(img_anchor)this->img_anchor = img_anchor;
        //else *img_anchor = ei_anc_center;[TO DEBUG]
    }


}
