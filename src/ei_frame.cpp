#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"

namespace ei {

    Frame::Frame(Widget *parent){
        if(parent)
            Widget("frame",parent);
        else
            return;
        relief=ei_relief_none;
        text=nullptr;
        text_font=default_font;
        text_color=font_default_color;
        text_anchor=ei_anc_center;
        img=nullptr;
        img_rect=nullptr;
        img_anchor=ei_anc_center;

    }

    Frame::~Frame(){

    }
    void Frame::draw(surface_t surface,
                    surface_t pick_surface,
                    Rect *clipper)
    {
     // SEE YA LATER
    }

    /**
     * @brief   Configures the attributes of widgets of the class "frame".
     *
     *      Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
     *      been defined before, then a default value should be used (default values are
     *      specified for each parameter). If the parameter is "NULL" but was defined on a
     *      previous call, then its value must not be changed.
     *
     * @param   requested_size  The size requested for this widget. The geometry manager may
     *                  override this size due to other constraints.
     *                  Defaults to the "natural size" of the widget, ie. big enough to
     *                  display the text or the image, or (0, 0) if the widget has no text
     *                  and no image.
     * @param   color       The color of the background of the widget. Defaults to
     *                      \ref ei_default_background_color.
     * @param   border_width    The width in pixel of the border decoration of the widget. The final
     *                          appearance depends on the "relief" parameter. Defaults to 0.
     * @param   relief      Appearance of the border of the widget. Defaults to \ref ei_relief_none.
     * @param   text        The text to display in the widget, or NULL. Only one of the
     *                      parameter "text" and "img" should be used (i.e. non-NULL). Defaults
     *                      to NULL.
     * @param   text_font   The font used to display the text. Defaults to \ref ei_default_font.
     * @param   text_color  The color used to display the text. Defaults to
     *                      \ref ei_font_default_color.
     * @param   text_anchor The anchor of the text, i.e. where it is placed whithin the widget
     *                      when the size of the widget is bigger than the size of the text.
     *                      Defaults to \ref ei_anc_center.
     * @param   img     The image to display in the widget, or NULL. Any surface can be
     *                  used, but usually a surface returned by \ref hw_image_load. Only one
     *                  of the parameter "text" and "img" should be used (i.e. non-NULL).
     *                  Defaults to NULL.
     * @param   img_rect    If not NULL, this rectangle defines a subpart of "img" to use as the
     *                      image displayed in the widget. Defaults to NULL.
     * @param   img_anchor  The anchor of the image, i.e. where it is placed whithin the widget
     *                      when the size of the widget is bigger than the size of the image.
     *                      Defaults to \ref ei_anc_center.
     */
    void Frame::configure (Size*           requested_size,
                    const color_t*  color,
                    int*            border_width,
                    relief_t*       relief,
                    char**          text,
                    font_t*         text_font,
                    color_t*        text_color,
                    anchor_t*       text_anchor,
                    surface_t*      img,
                    Rect**          img_rect,
                    anchor_t*       img_anchor){

        //Error managment
        if(img && (!img_rect || !img_anchor)) return;
        if (text && (!text_font || !text_color || !text_anchor)) return;

        //Size
        if(!img && !text)
            this->requested_size = Size(0,0); //if doesnt compile, chances are that u need to come here
        else if (requested_size && !text && img)
            this->requested_size = Size((*img_rect)->size);
        else if (requested_size && text && !img)
            hw_text_compute_size(*text, *text_font, this->requested_size);
        else
            this->requested_size = *requested_size;

        //The others (arg exists) ? assign arg : assign default;
        (relief) ? this->relief = *relief : this->relief = ei_relief_none;
        (text) ? this->text = *text : this->text = nullptr;
        (text_font) ? this->text_font = *text_font : this->text_font = default_font;
        (text_color) ? this->text_color = *text_color : this->text_color = font_default_color;
        (img) ? this->img = img : this->img = nullptr;
        (img_rect) ? this->img_rect = *img_rect : this->img_rect = nullptr;
        (img_anchor) ? this->img_anchor = *img_anchor : this->img_anchor = ei_anc_center;
    }
}
