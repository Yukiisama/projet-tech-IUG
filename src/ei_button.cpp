#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include <iostream>
#include "ei_application.h"
#include "ei_eventmanager.h"

#define ALPHA_MAX 255
#define BUTTON_NAME "Button"
namespace ei
{


/**
     * @brief   When widget clicked , activate relief_sunken on it .
     *
     * @param   widget the button widget given
     * @param   event the mouse click event
     * @param   user_param to bind event
     */
bool_t button_click_down(Widget* widget, Event* event, void* user_param)
{
    MouseEvent* e = static_cast<MouseEvent*>(event);
    Button* button = static_cast<Button*>(widget);
    if(Application::getInstance()->widget_pick(e->where)->getPick_id()==button->getPick_id()){
        button->set_relief(ei_relief_sunken);
        Application::getInstance()->invalidate_rect(*widget->getContent_rect());
    }
    return EI_FALSE;
}

/**
     * @brief   When widget clicked , activate relief_raised on it .
     *
     * @param   widget the button widget given
     * @param   event the mouse click event
     * @param   user_param to bind event
     */
bool_t button_click_up(Widget* widget, Event* event, void* user_param)
{
    Button* button = static_cast<Button*>(widget);
    if(button->get_relief()==ei_relief_sunken){
        button->set_relief(ei_relief_raised);
        Application::getInstance()->invalidate_rect(*widget->getContent_rect());
    }
    return EI_FALSE;
}
/**
     * @brief   Constructor of Button widget
     *
     * @param   parent of this widget.
     */
Button::Button(Widget *parent) : Widget(BUTTON_NAME, parent){
    //Initialiaze defaults value
    border_width = default_button_border_width;
    corner_radius = default_button_corner_radius;
    relief=ei_relief_raised;
    //Initialiaze defaults text value and text is nullptr by default (no text)
    text=nullptr;
    text_font= hw_text_font_create(default_font_filename, font_default_size);
    text_color=font_default_color;
    text_anchor=ei_anc_center;
    //Initialiaze defaults img value and img is nullptr by default (no img)
    img=nullptr;
    img_rect=nullptr;
    img_anchor =ei_anc_center;
    //Tag for later used if event called with tag Button
    addTag(BUTTON_NAME);
    //Bind relief button function
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, this, "", button_click_down, NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, this, "", button_click_up, NULL);
    EventManager::getInstance().setExc_Outside_Widget(ei_ev_mouse_buttonup,this,button_click_up,NULL);

}

Button::Button(Widget *parent, const widgetclass_name_t &class_name) : Widget(class_name, parent){
    //Initialiaze defaults value
    border_width = default_button_border_width;
    corner_radius = default_button_corner_radius;
    relief=ei_relief_raised;
    //Initialiaze defaults text value and text is nullptr by default (no text)
    text=nullptr;
    text_font= hw_text_font_create(default_font_filename, font_default_size);
    text_color=font_default_color;
    text_anchor=ei_anc_center;
    //Initialiaze defaults img value and img is nullptr by default (no img)
    img=nullptr;
    img_rect=nullptr;
    img_anchor =ei_anc_center;
    //Tag for later used if event called with tag Button
    addTag(class_name);
    //Bind relief button function
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, this, "", button_click_down, NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, this, "", button_click_up, NULL);
}
/**
    * @brief   Destructor of Button widget
 */
Button::~Button()
{
    EventManager::getInstance().deleteWidget(this);
    if(getParent()){
        getParent()->removeChildren(this);
        Application::getInstance()->invalidate_rect(*getParent()->getContent_rect());
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
void Button::draw(surface_t surface,
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
    Rect new_clipper = *clipper;
    if(clipper!=NULL){
       new_clipper=Application::getInstance()->intersectedRect(*content_rect,*clipper);
        if(new_clipper.size.width()==-1)return;
     }

    //The Rect of the button.
    Rect button_rect = Rect(content_rect->top_left,content_rect->size);
    //The list of points to draw the button
    linked_point_t list_frame = rounded_frame(button_rect, corner_radius, BT_FULL);
    pick_color.alpha=ALPHA_MAX;
    //Draw button polygon on pick_surface with color pick_color
    draw_polygon(pick_surface, list_frame, pick_color, &new_clipper);
    list_frame.clear();
    //Draw button on the main surface
    draw_button(surface,&button_rect,color,corner_radius,border_width,&new_clipper,relief);

    if (text)
    {
        Size text_size=Size(0,0);
        //Register the size that the text will take in text_size
        hw_text_compute_size(text,text_font,text_size);
        Point where = text_anchor_to_pos(*content_rect, text_anchor,text_size,border_width);
        //Finally draw the text at the where position
        updateText_Container(*clipper);
        draw_text(surface, &where, text, text_font, &text_color,&text_container);
    }
    if(img){
        //Case where only subpart of img should be display.
        if(img_rect){
            hw_surface_lock(surface);
            hw_surface_lock(img);
            for (int i =img_rect->top_left.x(); i < img_rect->top_left.x() + img_rect->size.width(); i++) {
                for (int j = img_rect->top_left.y(); j < img_rect->top_left.y() + img_rect->size.height() ;j++) {
                    Point pos = Point(i,j);
                    color_t img_c = hw_get_pixel(img,pos);
                    hw_put_pixel(surface,pos,img_c);
                }
            }
            hw_surface_unlock(surface);
            hw_surface_unlock(img);
        //default case, display img according to img_anchor.
        }else {
            cout<<"img detected"<<endl;
            Point pos = anchor_to_pos(screen_location,img_anchor);
            ei_copy_surface(surface,img,&pos,EI_TRUE);
        }
    }

    //Recursive method that draw all children of current button.
    for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){


        //Children should be display inside the content_rect of his parent.
        std::cout<<(*it)->getPick_id()<<std::endl;
        (*it)->draw(surface,pick_surface,&new_clipper);
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

    if(img && text) fprintf(stderr,"Only one of the parameter \"text\" and \"img\" should be used (i.e. non-NULL).");
    //Call the configure function of herited class widget
    Widget::configure(requested_size,color);
    //Assign values and run the geometry manager
    (corner_radius) ? this->corner_radius = *corner_radius : this->corner_radius = default_button_corner_radius;
    (border_width) ? this->border_width = *border_width : this->border_width = default_button_border_width;
    (relief) ? this->relief = *relief : this->relief = ei_relief_none;
    if(text && !img) this->text = *text;
    (text_color) ? this->text_color = *text_color : this->text_color = font_default_color;
    (text_anchor) ? this->text_anchor=*text_anchor : this->text_anchor = ei_anc_center;
    if (text_font) this->text_font = *text_font;
    if (img && !text) this->img = *img;
    if (img_rect) this->img_rect = *img_rect;
    (img_anchor) ? this->img_anchor = *img_anchor : this->img_anchor = ei_anc_center;
    if(geom_manager)geom_manager->run(this);
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

//Methode
void Button::updateText_Container(Rect clipper){
    text_container=*content_rect;
    if(text_container.top_left.x()>=clipper.top_left.x()&& text_container.top_left.y()>=clipper.top_left.y()
            && text_container.top_left.x()+text_container.size.width()<=clipper.top_left.x()+clipper.size.width()
            && text_container.top_left.y()+text_container.size.height()<=clipper.top_left.y()+clipper.size.height())
        return;

    text_container.top_left.x()=max(text_container.top_left.x(),clipper.top_left.x());
    text_container.top_left.y()=max(text_container.top_left.y(),clipper.top_left.y());
    text_container.size.width()=min(text_container.top_left.x()+text_container.size.width(),
                                    clipper.top_left.x()+clipper.size.width())-text_container.top_left.x();
    text_container.size.height()=min(text_container.top_left.y()+text_container.size.height(),
                                    clipper.top_left.y()+clipper.size.height())-text_container.top_left.y();
}

}
