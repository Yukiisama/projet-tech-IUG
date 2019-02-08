#include "ei_widget.h"

namespace ei
{

    Button::Button(Widget *parent) : Widget("Button", parent){

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
    void configure(Size *requested_size,
                   const color_t *color,
                   int *border_width,
                   int *corner_radius,
                   relief_t *relief,
                   const char **text,
                   font_t *text_font,
                   color_t *text_color,
                   anchor_t *text_anchor,
                   surface_t *img,
                   Rect **img_rect,
                   anchor_t *img_anchor);


}
