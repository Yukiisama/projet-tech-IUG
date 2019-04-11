#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_draw.h"
#include "ei_geometrymanager.h"
#include "ei_eventmanager.h"
#include "ei_application.h"
#include "hw_interface.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define ALPHA_MAX 255
#define ENTRY_NAME "Entry"


namespace ei {
/**
     * \brief   Frame constructor.
     * @param   Parent of the frame widget
**/
Entry::Entry():Entry(Application::getInstance()->root_widget()){}
Entry::Entry(Widget * parent):Frame(parent,"Entry"){}
Entry::~Entry(){}
void Entry::draw (surface_t surface,
                   surface_t pick_surface,
                  Rect*     clipper){

    Frame::draw(surface,pick_surface,clipper);
}
bool_t Entry::get_is_clicked(){
    return is_clicked;
}
void Entry::set_is_clicked(bool_t click){
    is_clicked=click;
}
bool Entry::get_done(){
    return done;
}
void Entry::set_done(bool done){
    done=done;
}
//Defaults color ( if null) is parent color
char * convert_unicode(int x){
    cout<<x<<endl;
    //CHARACTERS
    //********************UPPERCASE****************
    if(x==65) return "A";  if(x==66) return "B"; if(x==67) return "C"; if(x==68) return "D";
    if(x==69) return "E";  if(x==70) return "F"; if(x==71) return "G"; if(x==72) return "H";
    if(x==73) return "I";  if(x==74) return "J"; if(x==75) return "K"; if(x==76) return "L";
    if(x==77) return "M";  if(x==78) return "N"; if(x==79) return "O"; if(x==80) return "P";
    if(x==81) return "Q";  if(x==82) return "R"; if(x==83) return "S"; if(x==84) return "T";
    if(x==85)return "U";   if(x==86) return "V"; if(x==87) return "W"; if(x==88) return "X";
    if(x==89) return "Y";  if(x==90) return "Z";
    //*******************LOWERCASE********************
    if(x==97) return "a";  if(x==98) return "b";  if(x==99) return "c";  if(x==100) return "d";
    if(x==101) return "e"; if(x==102) return "f"; if(x==103) return "g"; if(x==104) return "h";
    if(x==105) return "i"; if(x==106) return "j"; if(x==107) return "k"; if(x==108) return "l";
    if(x==109) return "m"; if(x==110) return "n"; if(x==111) return "o"; if(x==112) return "p";
    if(x==113) return "q"; if(x==114) return "r"; if(x==115) return "s"; if(x==116) return "t";
    if(x==117) return "u"; if(x==118) return "v"; if(x==119) return "w"; if(x==120) return "x";
    if(x==121) return "y"; if(x==122) return "z";
    //Punctuation
    if(x==32) return " ";  if(x==33) return "!";  if(x==34) return "\"";
    if(x==35) return "#";  if(x==36) return "$";  if(x==37) return "%";
    if(x==38) return "&";  if(x==39) return "'";  if(x==40) return "(";
    if(x==41) return ")";  if(x==42) return "*";  if(x==43) return "+";
    if(x==44) return ",";  if(x==45) return "-";  if(x==46) return ".";
    if(x==47) return "/";  if(x==48) return "0";  if(x==49) return "1";
    if(x==50) return "2";  if(x==51) return "3";  if(x==52) return "4";
    if(x==53) return "5";  if(x==54) return "6";  if(x==55) return "7";
    if(x==56) return "8";  if(x==57) return "9";  if(x==58) return ":";
    if(x==59) return ";";  if(x==60) return "<";  if(x==61) return "=";
    if(x==62) return ">";  if(x==63) return "?";  if(x==64) return "@";

    return "";


    }

bool_t move_mouse(Widget* widget, Event* event, void* user_param){
    if(widget->getName()!="Entry"){

        for (list<Widget *>::iterator it = Application::getInstance()->get_widget_root()->getChildren().begin();it!=Application::getInstance()->get_widget_root()->getChildren().end();it++){
            if((*it)->getName()=="Entry"){
                Entry* entry_del = static_cast<Entry*>((*it));
                entry_del->set_is_clicked(EI_FALSE);
                //cout<<"cou"<<endl;


            }

        }
    }
    else if(widget->getPick_id()!=Application::getInstance()->get_who_click()){
        for (list<Widget *>::iterator it = Application::getInstance()->get_widget_root()->getChildren().begin();it!=Application::getInstance()->get_widget_root()->getChildren().end();it++){
            if((*it)->getName()=="Entry"){
                Entry* entry_del = static_cast<Entry*>((*it));
                entry_del->set_is_clicked(EI_FALSE);
                //cout<<"cou"<<endl;


            }
    }
    }
    Entry* entry = static_cast<Entry*>(widget);

    if(event->type==ei_ev_mouse_buttondown){
        MouseEvent* e = static_cast<MouseEvent*>(event);
        if(Application::getInstance()->widget_pick(e->where)->getPick_id()==widget->getPick_id() && !entry->get_is_clicked()){
            entry->set_is_clicked(EI_TRUE);
            cout <<entry->getName()<<endl;
            Application::getInstance()->set_who_click(widget->getPick_id());

            }

        //top->setMouse_pos(e->where);
        return EI_TRUE;
    }
}

bool_t process_key(Widget* widget, Event* event, void* user_param)
{
    if(widget->getName()=="Entry"){
    Entry * e = static_cast<Entry*>(widget);
    if(event->type == ei_ev_keychar && e->get_is_clicked() ){
        Size s = e->getRequested_size();
        color_t c = e->getColor();
        color_t c_txt = e->get_text_color();
        anchor_t anc = e->get_text_anchor();
        relief_t rel = e->get_relief();
        const char * txt = e->get_text();
        anchor_t img_anc = e->get_img_anchor();
        Rect * img_r = e->get_img_rect();
        int bd_width = e->getBorder_width();
        font_t font = e->get_text_font();
        surface_t  img = e->get_img();
        char* concatString = new char[strlen(txt)+2];
        strcpy( concatString, e->get_text() );
        //not Back space
        if(static_cast<KeyEvent*>(event)->unichar!=8){
        char * b;
        b= convert_unicode(static_cast<KeyEvent*>(event)->unichar);
        strcat( concatString,b);
        }
        else{
            int a = strlen(concatString);
            char * c = concatString;
            c[a-1] = '\0';
        }
        Size *size_text = new Size(0,0);
        hw_text_compute_size((const char *)concatString,font,*size_text);
        if(size_text->width()>=widget->getParent()->getContent_rect()->size.width())
            return EI_FALSE;
        e->set_text(concatString);
        if(strlen(concatString)<2) size_text->width() = 20; size_text->height()=25;
        //ADD MARGIN
        size_text->height()+=5;size_text->width()+=20;
        Placer * geo = (Placer *) e->getGeom_manager();
        int x = geo->getX();
        int y = geo->getY();
        geo->configure(e, NULL, &x, &y, NULL, NULL, NULL, NULL, NULL, NULL);
        e->configure(size_text,&c,&bd_width,&rel
                    ,(const char **)&concatString,&font,&c_txt,&anc
                    ,&img,&img_r,&img_anc);


        return EI_TRUE;
    }
    }

    return EI_FALSE;
}

void Entry::configure (Size*           requested_size,
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

    anchor_t anc = ei_anc_northwest;
    color_t colr;
    (!color) ? colr = getParent()->getColor() : colr = *color;
    Frame::configure(requested_size,&colr,border_width,relief,text,text_font,text_color,&anc,img,img_rect,img_anchor);
    if(!done){
        EventManager::getInstance().bind(ei_ev_keychar,NULL, "Entry", process_key, NULL);
        EventManager::getInstance().bind(ei_ev_mouse_buttondown,NULL, "all", move_mouse, NULL);

    }
    done = true;

}


//todo event to get keyboard --> configure if this widget has been pressed earlier
//END GETTER & SETTER
}
