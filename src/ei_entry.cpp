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


//Defaults color ( if null) is parent color
char * convert_unicode(int x){
    cout<<x<<endl;
    char * result = const_cast<char *>("");
    //CHARACTERS
    //********************UPPERCASE****************
    if(x==65)  result  = const_cast<char*>( "A");  if(x==66)  result  = const_cast<char*>( "B"); if(x==67)  result  = const_cast<char*>( "C"); if(x==68) result  = const_cast<char*>( "D");
    if(x==69)  result  = const_cast<char*>( "E");  if(x==70)  result  = const_cast<char*>( "F"); if(x==71)  result  = const_cast<char*>( "G"); if(x==72) result  = const_cast<char*>( "H");
    if(x==73)  result  = const_cast<char*>( "I");  if(x==74)  result  = const_cast<char*>( "J"); if(x==75)  result  = const_cast<char*>( "K"); if(x==76) result  = const_cast<char*>( "L");
    if(x==77)  result  = const_cast<char*>( "M");  if(x==78)  result  = const_cast<char*>( "N"); if(x==79)  result  = const_cast<char*>( "O"); if(x==80) result  = const_cast<char*>( "P");
    if(x==81)  result  = const_cast<char*>( "Q");  if(x==82)  result  = const_cast<char*>( "R"); if(x==83)  result  = const_cast<char*>( "S"); if(x==84) result  = const_cast<char*>( "T");
    if(x==85)  result  = const_cast<char*>( "U");  if(x==86)  result  = const_cast<char*>( "V"); if(x==87)  result  = const_cast<char*>( "W"); if(x==88) result  = const_cast<char*>( "X");
    if(x==89)  result  = const_cast<char*>( "Y");  if(x==90)  result  = const_cast<char*>( "Z");
    //*******************LOWERCASE********************
    if(x==97)  result  = const_cast<char*>( "a");  if(x==98)  result  = const_cast<char*>( "b"); if(x==99)  result  = const_cast<char*>( "c"); if(x==100) result  = const_cast<char*>( "d");
    if(x==101) result  = const_cast<char*>( "e");  if(x==102) result  = const_cast<char*>( "f"); if(x==103) result  = const_cast<char*>( "g"); if(x==104) result  = const_cast<char*>( "h");
    if(x==105) result  = const_cast<char*>( "i");  if(x==106) result  = const_cast<char*>( "j"); if(x==107) result  = const_cast<char*>( "k"); if(x==108) result  = const_cast<char*>( "l");
    if(x==109) result  = const_cast<char*>( "m");  if(x==110) result  = const_cast<char*>( "n"); if(x==111) result  = const_cast<char*>( "o"); if(x==112) result  = const_cast<char*>( "p");
    if(x==113) result  = const_cast<char*>( "q");  if(x==114) result  = const_cast<char*>( "r"); if(x==115) result  = const_cast<char*>( "s"); if(x==116) result  = const_cast<char*>( "t");
    if(x==117) result  = const_cast<char*>( "u");  if(x==118) result  = const_cast<char*>( "v"); if(x==119) result  = const_cast<char*>( "w"); if(x==120) result  = const_cast<char*>( "x");
    if(x==121) result  = const_cast<char*>( "y");  if(x==122) result  = const_cast<char*>( "z");
    //Punctuation
    if(x==32)  result  = const_cast<char*>( " ");  if(x==33)  result  = const_cast<char*>( "!");  if(x==34) result  = const_cast<char*>( "\"");
    if(x==35)  result  = const_cast<char*>( "#");  if(x==36)  result  = const_cast<char*>( "$");  if(x==37) result  = const_cast<char*>( "%");
    if(x==38)  result  = const_cast<char*>( "&");  if(x==39)  result  = const_cast<char*>( "'");  if(x==40) result  = const_cast<char*>( "(");
    if(x==41)  result  = const_cast<char*>( ")");  if(x==42)  result  = const_cast<char*>( "*");  if(x==43) result  = const_cast<char*>( "+");
    if(x==44)  result  = const_cast<char*>( ",");  if(x==45)  result  = const_cast<char*>( "-");  if(x==46) result  = const_cast<char*>( ".");
    if(x==47)  result  = const_cast<char*>( "/");  if(x==48)  result  = const_cast<char*>( "0");  if(x==49) result  = const_cast<char*>( "1");
    if(x==50)  result  = const_cast<char*>( "2");  if(x==51)  result  = const_cast<char*>( "3");  if(x==52) result  = const_cast<char*>( "4");
    if(x==53)  result  = const_cast<char*>( "5");  if(x==54)  result  = const_cast<char*>( "6");  if(x==55) result  = const_cast<char*>( "7");
    if(x==56)  result  = const_cast<char*>( "8");  if(x==57)  result  = const_cast<char*>( "9");  if(x==58) result  = const_cast<char*>( ":");
    if(x==59)  result  = const_cast<char*>( ";");  if(x==60)  result  = const_cast<char*>( "<");  if(x==61) result  = const_cast<char*>( "=");
    if(x==62)  result  = const_cast<char*>( ">");  if(x==63)  result  = const_cast<char*>( "?");  if(x==64) result  = const_cast<char*>( "@");

    return result;


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
    return EI_FALSE;
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
        geo->configure(e, nullptr, &x, &y, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
        e->configure(size_text,&c,&bd_width,&rel
                    ,const_cast<const char **>(&concatString),&font,&c_txt,&anc
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
    (!color) ? colr = (color_t){0,0,0,0} : colr = *color;
    Frame::configure(requested_size,&colr,border_width,relief,text,text_font,text_color,&anc,img,img_rect,img_anchor);
    if(!done){
        EventManager::getInstance().bind(ei_ev_keychar,NULL, "Entry", process_key, NULL);
        EventManager::getInstance().bind(ei_ev_mouse_buttondown,NULL, "all", move_mouse, NULL);

    }
    done = true;

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
    this->done=done;
}

//todo event to get keyboard --> configure if this widget has been pressed earlier
//END GETTER & SETTER
}
