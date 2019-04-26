#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_eventmanager.h"
#include "ei_application.h"
#include "hw_interface.h"
#include <iostream>
#define ALPHA_MAX 255
#define CLASS_NAME "ButtonRadio"


using namespace std;
namespace ei {

bool_t button_click_down2(Widget* widget, Event* event, void* user_param)
{
    MouseEvent* e = static_cast<MouseEvent*>(event);
    RadioButton* button = static_cast<RadioButton*>(widget);
    if(Application::getInstance()->widget_pick(e->where)->getPick_id()==button->getPick_id()){

        button->setRadio_button_relief(ei_relief_sunken);
        for(std::list<Widget*>::iterator it =button->getParent()->getChildren().begin();it!= button->getParent()->getChildren().end();it++){
            if(button!=(*it)){
                RadioButton* iterate_button = static_cast<RadioButton*>((*it));
                iterate_button->setRadio_button_relief(ei_relief_raised);

            }
        }
        return EI_TRUE;
    }
    return EI_FALSE;
}
bool_t button_click_up2(Widget* widget, Event* event, void* user_param)
{

    return EI_FALSE;
}

RadioButton::RadioButton():RadioButton(Application::getInstance()->root_widget()){}
RadioButton::RadioButton(Widget * parent):Button(parent,"RadioButton")
{

}
RadioButton::~RadioButton()
{
    EventManager::getInstance().unbind(ei_ev_mouse_buttondown,this, "", button_click_down2, NULL);
    EventManager::getInstance().unbind(ei_ev_mouse_buttondown,this, "", button_click_up2, NULL);
}
void RadioButton::draw (surface_t surface,
                        surface_t pick_surface,
                        Rect*     clipper)
{
    if(!surface){
        fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
        exit(EXIT_FAILURE);
    }
    if(!pick_surface){
        fprintf(stderr,"Error occured for Frame::draw - pick_surface is not vaild\n");
        exit(EXIT_FAILURE);
    }

    //The Rect of the button.
    Rect button_rect = Rect(content_rect->top_left,content_rect->size);
    //Draw on pick_surface the forme of button with button's pick_color.
    //The list of points to draw the button
    pick_color.alpha=ALPHA_MAX;
    //Draw button polygon on pick_surface with color pick_color
    int vertex_count =0;
    Rect new_rec = Application::getInstance()->intersectedRect(button_rect,*clipper);
    float *vertices  = convert_linked_point_to_vertices(&vertex_count,rounded_frame(new_rec,get_corner_radius(), BT_FULL),clipper);
    draw_polygon_pick_surface(pick_surface,vertices,vertex_count,pick_color);
    //Draw button on the main surface
    draw_button(surface,&button_rect,color,get_corner_radius(),border_width,clipper,getRadio_button_relief());
    // Text
    color_t t = get_text_color();
    surface_t textSurface = hw_text_create_surface(get_text(), this->get_text_font(), &t);
    Size sizeText = hw_surface_get_size(textSurface);
    Point posTexte;
    int32_t margin = 5;
    posTexte.x() = margin + screen_location.top_left.x()+int(screen_location.size.width());
    posTexte.y() = screen_location.top_left.y();
    ei_copy_surface(surface, textSurface, &posTexte, EI_TRUE);



    //Recursive method that draw all children of current button.
    for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
        //Children should be display inside the content_rect of his parent.
        std::cout<<(*it)->getPick_id()<<std::endl;
        (*it)->draw(surface,pick_surface,content_rect);
    }
}


void RadioButton::configure (Size*            requested_size,
                             const color_t*   color,
                             int*             border_width,
                             int*             corner_radius,
                             relief_t*        relief,
                             const char       **text,
                             font_t*          text_font,
                             color_t*         text_color,
                             anchor_t*        text_anchor,
                             surface_t*       img,
                             Rect**           img_rect,
                             anchor_t*        img_anchor)
{
    Button::configure(requested_size, color, border_width, corner_radius, relief, text, text_font, text_color, text_anchor, img, img_rect, img_anchor);
    (relief) ? this->radio_button_relief = *relief : this->radio_button_relief = ei_relief_none;
    EventManager::getInstance().bind(ei_ev_mouse_buttondown,this, "", button_click_down2, NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown,this, "", button_click_up2, NULL);
}

//getter and setter
relief_t RadioButton::getRadio_button_relief()
{
    return radio_button_relief;
}

void RadioButton::setRadio_button_relief(relief_t relief)
{
    this->radio_button_relief = relief;
}


}

