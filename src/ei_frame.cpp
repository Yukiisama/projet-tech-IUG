#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_eventmanager.h"
#include "ei_application.h"
#include "hw_interface.h"
#include <iostream>
#define ALPHA_MAX 255
#define FRAME_NAME "Frame"

namespace ei {
/**
     * \brief   Frame constructor.
     * @param   Parent of the frame widget
**/
Frame::Frame(Widget *parent):Widget(FRAME_NAME,parent){
    //Initialiaze defaults value
    relief=ei_relief_none;
    //Initialiaze defaults text value and text is nullptr by default (no text)
    text=nullptr;
    text_font=hw_text_font_create(default_font_filename, font_default_size);
    text_color=font_default_color;
    text_anchor=ei_anc_center;
    //Initialiaze defaults img value and img is nullptr by default (no img)
    img=nullptr;
    img_rect=nullptr;
    img_anchor = ei_anc_center;
    //Tag for later used if event called with tag Frame
    addTag(FRAME_NAME);
}
Frame::Frame(Widget *parent,const widgetclass_name_t &class_name):Widget(class_name,parent){
    //Initialiaze defaults value
    relief=ei_relief_none;
    //Initialiaze defaults text value and text is nullptr by default (no text)
    text=nullptr;
    text_font=hw_text_font_create(default_font_filename, font_default_size);
    text_color=font_default_color;
    text_anchor=ei_anc_center;
    //Initialiaze defaults img value and img is nullptr by default (no img)
    img=nullptr;
    img_rect=nullptr;
    img_anchor = ei_anc_center;
    //Tag for later used if event called with tag Frame
    addTag(class_name);
}
/**
     * \brief   Frame deconstructor.
**/
Frame::~Frame(){
    if(getName().compare("root")){
        getParent()->removeChildren(this);
    }
    hw_text_font_free(text_font);
}
/**
     * \brief   Method that draws the widget.
     *
     * @param   surface     Where to draw the widget. The actual location of the widget in the
     *                      surface is stored in its "screen_location" field.
     * @param   pick_surface  Offscreen buffer to draw the widget \ref pick_id
     * @param   clipper     If not NULL, the drawing is restricted within this rectangle
     *                      (expressed in the surface reference frame).
     */
void Frame::draw(surface_t surface,
                 surface_t pick_surface,
                 Rect *clipper)
{
    if(!surface){
        fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
        exit(EXIT_FAILURE);
    }
    if(!pick_surface){
        fprintf(stderr,"Error occured for Frame::draw - pick_surface is not vaild\n");
        exit(EXIT_FAILURE);
    }
    //Draw on pick_surface the rectangle  with frame's pick_color.
    pick_color.alpha=ALPHA_MAX;
    draw_rectangle(pick_surface,*content_rect,pick_color,clipper);
    //Finally draw the frame on the main surface
    draw_rectangle(surface,*content_rect,color,clipper);
    if (text)
    {
          Point where = anchor_to_pos(screen_location, text_anchor);
          //Finally draw the text at the where position
          text_container=Application::getInstance()->intersectedRect(*content_rect,*clipper);
          draw_text(surface, &where, text, text_font, &text_color,&text_container);
    }
    if(img){
        //Case where only subpart of img should be display.
        if(img_rect){
            hw_surface_lock(img);
            hw_surface_lock(surface);
            for (int i =img_rect->top_left.x(); i < img_rect->top_left.x() + img_rect->size.width();i++) {
                for (int j = img_rect->top_left.y(); j < img_rect->top_left.y() + img_rect->size.height();j++) {
                    Point pos = Point(i,j);
                    color_t img_c = hw_get_pixel(img,pos);
                    hw_put_pixel(surface,pos,img_c);
                }
            }
            hw_surface_unlock(surface);
            hw_surface_unlock(img);
        //Default case, display img according to img_anchor.
        }else {
            cout<<"img detected"<<endl;
            Point pos = anchor_to_pos(screen_location,img_anchor);
            ei_copy_surface(surface,img,&pos,EI_TRUE);
        }
    }
    //recursive method that draw all the children.
    if(!children.empty()){
        std::list<Widget *>&c_list =children;
        //update clipper for children
        Rect new_clipper=Application::getInstance()->intersectedRect(*getContent_rect(),*clipper);
        for (std::list<Widget *>::iterator it = c_list.begin(); it != c_list.end(); it++)
            //Children should be display inside the content_rect of his parent.
            (*it)->draw(surface, pick_surface, &new_clipper);
    }


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
                       const char**          text,
                       font_t*         text_font,
                       color_t*        text_color,
                       anchor_t*       text_anchor,
                       surface_t*      img,
                       Rect**          img_rect,
                       anchor_t*       img_anchor){
    //Call the configure function of herited class widget
    if (!requested_size){
        if (img && img_rect){
            requested_size = (new Size((**img_rect).top_left.x() + (**img_rect).size.width(), (**img_rect).top_left.y() + (**img_rect).size.height()));
        }
        else if (text && text_font){
            Size* size = new Size(0,0);
            hw_text_compute_size(*text, *text_font, *size);
            requested_size = size;
        }
        else if (!img && !text){
            requested_size = new Size(0,0);
        }
        else{
            fprintf(stderr, "Informations are invalid you either gave frame::configure() an image without img_rect, or a text without a font!\n");
            return;
        }
    }
    if (!color){
        const color_t c = default_background_color;
        color = &c;
    }

    Widget::configure(requested_size, color);
    //Assign values and run the geometry manager
    (border_width) ? this->border_width = *border_width : this->border_width = 0;
    (relief) ? this->relief = *relief : this->relief = ei_relief_none;
    if (text && !img) this->text = *text;
    (text_color) ? this->text_color = *text_color : this->text_color = font_default_color;
    (text_anchor) ? this->text_anchor=*text_anchor : this->text_anchor = ei_anc_center;
    if (text_font) this->text_font = *text_font;
    if (img && !text) this->img = *img;
    if (img_rect) this->img_rect = *img_rect;
    (img_anchor) ? this->img_anchor = *img_anchor : this->img_anchor = ei_anc_center;
    if (geom_manager) geom_manager->run(this);
}
/**
 * @brief Frame::to_string ( string with all informations you need for debugging)
 * @return the stream string
 */
string Frame::to_string()
{
    stringstream stream;
    stream << Widget::to_string();
    stream << "relief_t relief : " << relief << "\n";
    stream << "char** text : " << text << "\n";
    stream << "font_t text_font : " << text_font << "\n";
    stream << "color_t text_color : "
           << "Red : " << int(text_color.red) << " Green : " << int(text_color.green) << " Blue : " << int(text_color.blue) << " Alpha : " << int(text_color.alpha) << "\n";
    stream << "anchor_t text_anchor : " << text_anchor << "\n";
    stream << "surface_t* img : " << img << "\n";
    if (img_rect)
        stream << "Rect* img_rect : "
               << "Width : " << img_rect->size.width() << " Height : " << img_rect->size.height() << " X : " << img_rect->top_left.x() << " Y : " << img_rect->top_left.y() << "\n";
    stream << "anchor_t* img_anchor : " << img_anchor << "\n";
    return stream.str();
}
//GETTER & SETTER

relief_t Frame::get_relief(){
    return this->relief;
}
void Frame::set_relief(relief_t relief){
    this->relief = relief;
}
const char * Frame::get_text(){
    return this->text;
}
void Frame::set_text(const char * text){
    this->text = text;
}
font_t Frame::get_text_font(){
    return this->text_font;
}
void Frame::set_text_font(font_t text_font){
    this->text_font = text_font;
}
color_t Frame::get_text_color(){
    return this->text_color;
}
void Frame::set_text_color(color_t text_color){
    this->text_color = text_color;
}
anchor_t Frame::get_text_anchor(){
    return this->text_anchor;
}
void Frame::set_text_anchor(anchor_t text_anchor){
    this->text_anchor=text_anchor;
}
surface_t Frame::get_img(){
    return this->img;
}
void Frame::get_img(surface_t img){
    this->img=img;
}
Rect * Frame::get_img_rect(){
    return this->img_rect;
}
void Frame::set_img_rect(Rect * img_rect){
    this->img_rect = img_rect;

}
anchor_t Frame::get_img_anchor(){
    return this->img_anchor;
}
void Frame::set_img_anchor(anchor_t img_anchor){
    this->img_anchor=img_anchor;
}

//END GETTER & SETTER
}
