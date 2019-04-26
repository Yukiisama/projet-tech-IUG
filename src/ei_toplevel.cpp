#include "ei_draw.h"
#include "ei_widget.h"
#include "ei_types.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "ei_eventmanager.h"
#include "ei_application.h"
#include "hw_interface.h"
#include <functional>
#include <iostream>
#define BD_WIDTH 4
#define TB_HEIGHT 24
#define MIN_WIDTH 160
#define MIN_HEIGHT 120
#define CLOSE_DIM 15.0
#define RESIZE_DIM 12.0
#define BTCLOSE_X 3.0
#define ALPHA_MAX 255
#define BUTTON_RADIUS 7
#define TOPLEVEL_NAME "Toplevel"
using namespace std;
namespace ei {

/**
 * @brief resize_button_callback callback called when a user clicks on the resize button.
 * @param widget
 * @param event
 * @param user_param
 * @return
 */
bool_t resize_button_callback(Widget* widget, Event* event, void* user_param){
    Toplevel* top = static_cast<Toplevel*>(widget);
    MouseEvent* e = static_cast<MouseEvent*>(event);
    if(event->type==ei_ev_mouse_buttonup){
        if(top->getResize_button_pressed()) {
            top->setResize_button_pressed(EI_FALSE);
            if(top->get_show_arrow()){
                top->set_show_arrow(EI_FALSE);
            }
            Application::getInstance()->invalidate_rect(top->getScreen_location());
            return EI_FALSE;
        }


    }else if(event->type==ei_ev_mouse_buttondown && top->inside_right_bottom_corner(e->where)){
        top->setResize_button_pressed(EI_TRUE);
        top->setMouse_pos(e->where);
        top->setFixScreen(EI_FALSE);
        top->setFix_screen_released(EI_FALSE);
        Application::getInstance()->invalidate_rect(top->getScreen_location());
        return EI_FALSE;
    }else if(event->type==ei_ev_mouse_move){
        if(top->getResize_button_pressed()){
            if(Application::getInstance()->inside_root(e->where)){
                int dx = e->where.x()-top->getMouse_pos().x();
                int dy = e->where.y()-top->getMouse_pos().y();
                int new_width = top->getRequested_size().width()+dx-top->getBorder_width()*2;
                int new_height = top->getRequested_size().height()+dy-top->getBorder_width()-top->getTop_bar_height();
                float deltaX = top->getContent_rect()->size.width()-new_width;
                float deltaY = top->getContent_rect()->size.height()-new_height;
                //we return if the delta is 0 or just incredibly insignifiant
                if((deltaX * deltaX)/2 <2.0f || (deltaY * deltaY)/2 <2.0f) return EI_FALSE;
                Application::getInstance()->invalidate_rect((top->getScreen_location()));
                //Limit the top level to a minimal size
                if(new_width < top->getMin_size().width())new_width = top->getMin_size().width();
                if(new_height < top->getMin_size().height())new_height = top->getMin_size().height();
                //finally update the new size of the top level
                Size *new_size = new Size(new_width,new_height);
                Placer * geo = (Placer *)top->getGeom_manager();
                geo->setWidth(new_width+top->getBorder_width()*2);
                geo->setHeight(new_height+top->getTop_bar_height()+top->getBorder_width());
                top->configure(new_size,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
                delete(new_size);
                //update the last position of the mouse
                top->setMouse_pos(e->where);
                return EI_TRUE;
            }
        }
    }
    return EI_FALSE;
}

bool_t arrow_callback(Widget* widget, Event* event, void* user_param){
    Toplevel* top = static_cast<Toplevel*>(widget);
    MouseEvent* e = static_cast<MouseEvent*>(event);

    if(top->inside_right_bottom_corner(e->where)){
        if(!top->get_show_arrow()){
            top->set_show_arrow(EI_TRUE);
            Application::getInstance()->invalidate_rect(top->getScreen_location());
        }
    }
    else{
        if(top->get_show_arrow()){
            top->set_show_arrow(EI_FALSE);
            Application::getInstance()->invalidate_rect(top->getScreen_location());
        }
    }
    return EI_FALSE;
}



bool_t button_close_callback(Widget* widget, Event* event, void* user_param){
    Toplevel* top = static_cast<Toplevel*>(widget);
    MouseEvent* e = static_cast<MouseEvent*>(event);
    if(top->getButton_close_pressed()){
        if(event->type==ei_ev_mouse_buttonup &&
                Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getButton_close()->getPick_id()){
            top->getGeom_manager()->release(top);
            top->setTo_forget(EI_TRUE);
            return EI_FALSE;
        }
        top->setButton_close_pressed(EI_FALSE);//optimization
    }else if(event->type==ei_ev_mouse_buttondown &&
             Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getButton_close()->getPick_id()){
        top->setButton_close_pressed(EI_TRUE);
        top->setTo_forget(EI_TRUE);
        return EI_TRUE;
    }
    return EI_FALSE;
}

bool_t topbar_move_callback(Widget* widget, Event* event, void* user_param){
    Toplevel* top = static_cast<Toplevel*>(widget);
    MouseEvent* e = static_cast<MouseEvent*>(event);

    if(top->getTop_bar_clicked()&& event->type==ei_ev_mouse_buttonup){
        top->setTop_bar_clicked(EI_FALSE);
        Application::getInstance()->invalidate_rect((top->getScreen_location()));
        if(top->getFixScreen()) top->setFix_screen_released(EI_TRUE);
        return EI_FALSE;
    }else if(event->type==ei_ev_mouse_buttondown &&
             Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getPick_id()){
        if(top->inside_top_bar(e->where)){
            //Top goes upside the other top if an other one
            for (list<Widget *>::iterator it = widget->getParent()->getChildren().begin();it!=widget->getParent()->getChildren().end();it++){
                         if (*it == top){
                             widget->getParent()->getChildren().push_back(*it);
                             it = widget->getParent()->getChildren().erase(it);
                         }
                     }
            //Tells the toplevel that its top_bar is clicked
            if(!top->getTop_bar_clicked()){
                top->setTop_bar_clicked(EI_TRUE);
                top->setMouse_pos(e->where);
             }
            return EI_FALSE;
        }
    }else if(top->getTop_bar_clicked() && event->type==ei_ev_mouse_move ){
        //left side halfscreen
        if(e->where.x()<=top->getParent()->getContent_rect()->top_left.x() &&!top->getFixScreen()){
            top->setFixScreen(EI_TRUE);
            top->setPre_pos(top->getScreen_location().top_left);
            top->setPre_size(top->getContent_rect()->size);
            Size *new_size = new Size(top->getParent()->getContent_rect()->size.width()/2-top->getBorder_width()*2,top->getParent()->getContent_rect()->size.height()-top->getTop_bar_height()-top->getBorder_width());
            if (top->getGeom_manager()->getName() == "placer"){
                Placer * geo = (Placer *)top->getGeom_manager();
                geo->setX(top->getParent()->getContent_rect()->top_left.x());
                geo->setY(top->getParent()->getContent_rect()->top_left.y());
                geo->setWidth(new_size->width()+top->getBorder_width()*2);
                geo->setHeight(new_size->height()+top->getTop_bar_height()+top->getBorder_width());
            }
            top->configure(new_size,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
            delete(new_size);
            return EI_TRUE;
        }
        //right side halfscreen
        else if(e->where.x()>=top->getParent()->getContent_rect()->top_left.x()+top->getParent()->getContent_rect()->size.width()
                 && !top->getFixScreen()){
            top->setFixScreen(EI_TRUE);
            top->setPre_pos(top->getScreen_location().top_left);
            top->setPre_size(top->getContent_rect()->size);
            Size *new_size = new Size(top->getParent()->getContent_rect()->size.width()/2-top->getBorder_width()*2,top->getParent()->getContent_rect()->size.height()-top->getTop_bar_height()-top->getBorder_width());
            if (top->getGeom_manager()->getName() == "placer"){
                Placer * geo = (Placer *)top->getGeom_manager();
                geo->setX(top->getParent()->getContent_rect()->top_left.x()+top->getParent()->getContent_rect()->size.width()/2);
                geo->setY(top->getParent()->getContent_rect()->top_left.y());
                geo->setWidth(new_size->width()+top->getBorder_width()*2);
                geo->setHeight(new_size->height()+top->getTop_bar_height()+top->getBorder_width());
            }
            top->configure(new_size,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
            delete(new_size);
            return EI_TRUE;
        }
        //full screen
        else if(e->where.y()<=top->getParent()->getContent_rect()->top_left.y() && !top->getFixScreen()){
            top->setFixScreen(EI_TRUE);
            top->setPre_pos(top->getScreen_location().top_left);
            top->setPre_size(top->getContent_rect()->size);
            Size *new_size = new Size(top->getParent()->getContent_rect()->size.width()-top->getBorder_width(),top->getParent()->getContent_rect()->size.height()-top->getTop_bar_height()-top->getBorder_width());
            if (top->getGeom_manager()->getName() == "placer"){
                Placer * geo = (Placer *)top->getGeom_manager();
                geo->setX(top->getParent()->getContent_rect()->top_left.x());
                geo->setY(top->getParent()->getContent_rect()->top_left.y());
                geo->setWidth(new_size->width()+top->getBorder_width()*2);
                geo->setHeight(new_size->height()+top->getTop_bar_height()+top->getBorder_width());
            }
            top->configure(new_size,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
            delete(new_size);
            return EI_TRUE;
        }
        //remove halfscreen and become previous screen
        else if(top->getFixScreen() && e->where.x()>top->getParent()->getContent_rect()->top_left.x()
                 && e->where.x()<top->getParent()->getContent_rect()->top_left.x()+top->getParent()->getContent_rect()->size.width()
                && e->where.y()>top->getParent()->getContent_rect()->top_left.y()){
            top->setFixScreen(EI_FALSE);
            Application::getInstance()->invalidate_rect((top->getScreen_location()));
            //case where halfscreen is display after mouse up
            if(top->getFix_screen_released()&& Application::getInstance()->inside_root(e->where)){
                int move_x = e->where.x()-top->getScreen_location().top_left.x()-top->getPre_size().width()/2;
                int move_y = (e->where.y())-(top->getMouse_pos().y());
                top->setFix_screen_released(EI_FALSE);
                if (top->getGeom_manager()->getName() == "placer"){
                    Placer * geo = (Placer *)top->getGeom_manager();
                    geo->setX(int(top->getScreen_location().top_left.x()+move_x));
                    geo->setY(int(top->getScreen_location().top_left.y()+move_y));
                }
            }
            //case halfscreen display while mouse is moving
            else{
                if (top->getGeom_manager()->getName() == "placer"){
                    Placer * geo = (Placer *)top->getGeom_manager();
                    geo->setX(top->getPre_pos().x());
                    geo->setY(top->getPre_pos().y());
                }
            }

            Size *new_size = new Size(top->getPre_size().width(),top->getPre_size().height());
            top->configure(new_size,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
            delete(new_size);
            top->setPre_pos(Point(0,0));
            top->setPre_size(Size(0,0));
            return EI_TRUE;
        }
        //normale moving
        else{
            if(Application::getInstance()->inside_root(e->where)){
                Application::getInstance()->invalidate_rect((top->getScreen_location()));
                int move_x = (e->where.x())-(top->getMouse_pos().x());
                int move_y = (e->where.y())-(top->getMouse_pos().y());
                //we return if the movement result is 0 or just incredibly insignifiant
                if(move_x==0.0 && move_y==0.0)return EI_FALSE;
                if(move_x!= 0.0 && (move_x * move_x)/2<1) return EI_FALSE;
                if(move_y!= 0.0 && (move_y * move_y)/2<1) return EI_FALSE;
                //Update geom_manager x & y to update the position of toplevel
                if (top->getGeom_manager()->getName() == "placer"){
                    Placer * geo = (Placer *)top->getGeom_manager();
                    geo->setX(int(top->getScreen_location().top_left.x()+move_x));
                    if(top->getScreen_location().top_left.y()+move_y<=top->getParent()->getContent_rect()->top_left.y()){
                        geo->setY(int(top->getParent()->getContent_rect()->top_left.y()));
                    }else{
                        geo->setY(int(top->getScreen_location().top_left.y()+move_y));
                    }
                }
                //finally run the geom_manager that will result in updating the position
                top->getGeom_manager()->run(top);
                top->setMouse_pos(e->where);
                return EI_TRUE;
            }

        }

    }
    return EI_FALSE;
}

/**
 * @brief Toplevel Constructor
 * @param parent of the toplevel widget
 */
Toplevel::Toplevel(Widget *parent) : Widget(TOPLEVEL_NAME, parent){
    //Initialize defaults values
    color = default_background_color;
    border_width = BD_WIDTH;
    top_bar_height = TB_HEIGHT;
    title = TOPLEVEL_NAME;
    closable = EI_TRUE;
    resizable = ei_axis_both;
    min_size.width() = MIN_WIDTH;
    min_size.height()= MIN_HEIGHT;

    //Initialize Button close size
    button_size = Size(CLOSE_DIM,CLOSE_DIM);

    //Initialize Resize button
    int radius = 0;
    resize_button_window_size = Size(RESIZE_DIM,RESIZE_DIM);
    addTag(TOPLEVEL_NAME);

    //update Toplevel's own content rect which is now depending on container
    setContent_rect(&container);
    //bind resize button
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, this, "",resize_button_callback , NULL);
    EventManager::getInstance().setExc_Outside_Widget(ei_ev_mouse_buttonup,this,resize_button_callback,NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, this, "",resize_button_callback , NULL);
    EventManager::getInstance().bind(ei_ev_mouse_move, this, "",resize_button_callback , NULL);
    //bind topbar mov
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, this, "",topbar_move_callback , NULL);
    EventManager::getInstance().setExc_Outside_Widget(ei_ev_mouse_buttonup,this,topbar_move_callback,NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, this, "",topbar_move_callback , NULL);
    EventManager::getInstance().bind(ei_ev_mouse_move, this, "",topbar_move_callback , NULL);
    //bind show arrow
    EventManager::getInstance().bind(ei_ev_mouse_move, this, "", arrow_callback, NULL);
}

Toplevel::~Toplevel(){
    //unbind resize button
    EventManager::getInstance().unbind(ei_ev_mouse_buttonup, this, "",resize_button_callback , NULL);
    EventManager::getInstance().unbind(ei_ev_mouse_buttondown, this, "",resize_button_callback , NULL);
    EventManager::getInstance().unbind(ei_ev_mouse_move, this, "",resize_button_callback , NULL);
    //unbind topbar mov
    EventManager::getInstance().unbind(ei_ev_mouse_buttonup, this, "",topbar_move_callback , NULL);
    EventManager::getInstance().unbind(ei_ev_mouse_buttondown, this, "",topbar_move_callback , NULL);
    EventManager::getInstance().unbind(ei_ev_mouse_move, this, "",topbar_move_callback , NULL);
    EventManager::getInstance().deleteWidget(this);

    if(closable_done)delete button_close;
    //delete toplevel basic placer
    delete p_button_close;

    //TODO update pick surface with parant's pick color
    //remove from parent's child list
    if(getParent()){
        getParent()->removeChildren(this);
        getParent()->draw(Application::getInstance()->get_root_window(),
                          Application::getInstance()->get_offscreen(),getParent()->getContent_rect());
        Application::getInstance()->invalidate_rect(*getParent()->getContent_rect());
    }
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, this, "",topbar_move_callback , NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, this, "",topbar_move_callback , NULL);
    EventManager::getInstance().bind(ei_ev_mouse_move, this, "",topbar_move_callback , NULL);

}


/**
 * @brief Toplevel::inside_top_bar : Check if inside the top_bar of the top level
 * @param where the position given
 * @return boolean state ( true if inside else false)
 */
bool_t Toplevel::inside_top_bar(Point where) const{
    if(where.x()>=screen_location.top_left.x()
            && where.x()<=screen_location.top_left.x()+requested_size.width()
            && where.y()>=screen_location.top_left.y()
            && where.y()<=screen_location.top_left.y()+top_bar_height){
        return EI_TRUE;
    }
    else{
        return EI_FALSE;
    }
}

/**
 * @brief Toplevel::inside_right_bottom_corner : Check if inside the right bottom corner of the top level
 * @param where the given position
 * @return boolean state ( true if inside else false)
 */
bool_t Toplevel::inside_right_bottom_corner(Point where) const{
    if(where.x()>=content_rect->top_left.x()+content_rect->size.width()-RESIZE_DIM
            && where.x()<=content_rect->top_left.x()+content_rect->size.width()+border_width
            && where.y()>=content_rect->top_left.y()+content_rect->size.height()-RESIZE_DIM
            && where.y()<=content_rect->top_left.y()+content_rect->size.height()+border_width){
        return EI_TRUE;
    }
    else{
        return EI_FALSE;
    }
}

/**
 * @brief Draw the toplevel into the main window
 * @param surface : the main window
 * @param pick_surface : pick surface to identify widgets
 * @param clipper If not NULL, the drawing is restricted within this rectangle
 *                      (expressed in the surface reference frame).
*/
void Toplevel::draw (surface_t surface,
                     surface_t pick_surface,
                     Rect*     clipper){
    //case when button close has been trigged
    if(to_forget) return;

    Rect new_clipper = *clipper;
    Rect new_container =container;
    if(clipper!=NULL){
       new_clipper=Application::getInstance()->intersectedRect(screen_location,*clipper);
        if(new_clipper.size.width()==-1)return;

        new_container=Application::getInstance()->intersectedRect(container,*clipper);
        if(new_container.size.width()==-1)return;
     }

    if(!surface){
        fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
        exit(EXIT_FAILURE);
    }
    //Placer config for button close,positon according to toplevel content rect , then run the placer
    if(closable){
        int button_close_x = BTCLOSE_X;
        int button_close_y =-int(top_bar_height)+4;
        p_button_close->configure(button_close,nullptr,&button_close_x,&button_close_y,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
        p_button_close->run(button_close);
    }

    //draw outside the basic toplevel
    drawBasic_toplevel(surface,pick_surface,&new_clipper);

    if(show_arrow || resize_button_pressed)
        draw_arrow(surface,
                   Point(content_rect->top_left.x()+content_rect->size.width()-RESIZE_DIM+border_width,
                         content_rect->top_left.y()+content_rect->size.height()-RESIZE_DIM+border_width),
                   RESIZE_DIM,
                   NULL);


    //recursive draw
    for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
        //donc apply content rect to button close because it display on the top bar which is not belong to content rect
        if((*it)->getPick_id()==button_close->getPick_id())
            (*it)->draw(surface,pick_surface,&new_clipper);
        else
            (*it)->draw(surface,pick_surface,&new_container);
    }
    return;
}

/**
 * @brief Draw the basic top_level without buttons
 * @param surface : the main window
 * @param pick_surface : pick surface to identify widgets
 * @param clipper If not NULL, the drawing is restricted within this rectangle
 *                      (expressed in the surface reference frame).
 */
void Toplevel::drawBasic_toplevel(surface_t surface, surface_t pick_surface, Rect *clipper){
    if(!surface || !pick_surface){
        fprintf(stderr,"Error occured for Frame::draw - surface or pick_surface is not valid\n");
        exit(EXIT_FAILURE);
    }

    int LR_height = requested_size.height()-top_bar_height-border_width;
    pick_color.alpha=ALPHA_MAX;
    color.alpha=ALPHA_MAX;
    //Top bar rectangle
    Rect topbar_rec= Rect(screen_location.top_left,Size(requested_size.width(),top_bar_height));
    draw_rectangle(pick_surface,topbar_rec,pick_color,clipper);
    draw_rectangle(surface,topbar_rec,color,clipper);

    //left border rec
    Rect left_border_rec = Rect(Point(screen_location.top_left.x(),screen_location.top_left.y()+top_bar_height),Size(border_width,LR_height));
    draw_rectangle(pick_surface,left_border_rec,pick_color,clipper);
    draw_rectangle(surface,left_border_rec,color,clipper);

    //right border rec
    Rect right_border_rec = Rect(Point(screen_location.top_left.x()+requested_size.width()-border_width,screen_location.top_left.y()+top_bar_height),
                                 Size(border_width,LR_height));
    draw_rectangle(pick_surface,right_border_rec,pick_color,clipper);
    draw_rectangle(surface,right_border_rec,color,clipper);

    //buttom border rec
    Rect buttom_border_rec = Rect(Point(screen_location.top_left.x(),screen_location.top_left.y()+requested_size.height()-border_width),
                                 Size(requested_size.width(),border_width));
    draw_rectangle(pick_surface,buttom_border_rec,pick_color,clipper);
    draw_rectangle(surface,buttom_border_rec,color,clipper);

    //Draw toplevel background that means the container zone
    color_t color_white = {255,255,255,ALPHA_MAX};
    draw_rectangle(pick_surface,*content_rect,pick_color,clipper);
    draw_rectangle(surface,*content_rect,color_white,clipper);


    //Title of the window
    Point where = Point(screen_location.top_left.x()+40,screen_location.top_left.y()+int(top_bar_height*0.05));
    font_t title_font=hw_text_font_create(default_font_filename, int(top_bar_height*0.8));
    draw_text(surface,&where,title,title_font,&color_white,NULL);
    hw_text_font_free(title_font);

}

/**
     * @brief   Configures the attributes of widgets of the class "toplevel".
     *
     * @param   widget      The widget to configure.
     * @param   requested_size  The content size requested for this widget, this does not include
     *              the decorations (border, title bar). The geometry manager may
     *              override this size due to other constraints.
     *              Defaults to (320x240).
     * @param   color       The color of the background of the content of the widget. Defaults
     *                      to \ref ei_default_background_color.
     * @param   border_width    The width in pixel of the border of the widget. Defaults to 4.
     * @param   title       The string title diplayed in the title bar. Defaults to "Toplevel".
     * @param   closable    If true, the toplevel is closable by the user, the toplevel must
     *                      show a close button in its title bar. Defaults to \ref EI_TRUE.
     * @param   resizable   Defines if the widget can be resized horizontally and/or vertically
     *                      by the user. Defaults to \ref ei_axis_both.
     * @param   min_size    For resizable widgets, defines the minimum size. Defaults to (160, 120).
     */
void Toplevel::configure (Size*           requested_size,
                          color_t*        color,
                          int*            border_width,
                          const char**    title,
                          bool_t*         closable,
                          axis_set_t*     resizable,
                          Size*           min_size){

    if(border_width) this->border_width = *border_width;
    if(requested_size ){
        if(requested_size->width()<this->min_size.width() || requested_size->height()<this->min_size.height())
            cout<<"New requested size"<<endl;
        //Assign requested size to container
        container.size=*requested_size;
        //Create and update requested size for the widget Toplevel including border and top bar height.
        Size TopSL;
        TopSL.width()=requested_size->width()+this->border_width*2;
        TopSL.height()=requested_size->height()+this->border_width+top_bar_height;
        setRequested_size(TopSL);
    }
    if(color) setColor(*color);
    if(title) this->title = *title;
    if(closable)  this->closable = *closable;
    if(resizable) this->resizable = *resizable;
    if(min_size)  this->min_size = *min_size;

    //Button close (closable done == true if it has already be done)
    if(this->closable && !closable_done) {
        button_close = new Button(this);
        color_t button_color = {255,0,0,ALPHA_MAX};
        int button_close_radius =BUTTON_RADIUS;
        button_close->configure(&button_size,&button_color,
                                nullptr,&button_close_radius,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
        //Assign placer to the new button created
        p_button_close=new Placer();
        closable_done=EI_TRUE;
        //bind button close
        EventManager::getInstance().bind(ei_ev_mouse_buttonup, this, "",button_close_callback , NULL);
        EventManager::getInstance().bind(ei_ev_mouse_buttondown, this, "",button_close_callback , NULL);
    }
    //Finally run the geometry manager in order to place the toplevel and the buttons
    if(geom_manager)geom_manager->run(this);
}





//Getter & Setter
int Toplevel::getBorder_width() const
{
    return border_width;
}

void Toplevel::setBorder_width(int value)
{
    border_width = value;
}

double Toplevel::getTop_bar_height() const
{
    return top_bar_height;
}

void Toplevel::setTop_bar_height(double value)
{
    top_bar_height = value;
}

const char *Toplevel::getTitle() const
{
    return title;
}

void Toplevel::setTitle(const char *value)
{
    title = value;
}

bool_t Toplevel::getClosable() const
{
    return closable;
}

void Toplevel::setClosable(const bool_t &value)
{
    closable = value;
}

axis_set_t Toplevel::getResizable() const
{
    return resizable;
}

void Toplevel::setResizable(const axis_set_t &value)
{
    resizable = value;
}

Size Toplevel::getMin_size() const{
    return min_size;
}

void Toplevel::setMin_size(const Size &value)
{
    min_size = value;
}

Size Toplevel::getButton_size() const
{
    return button_size;
}

void Toplevel::setButton_size(const Size &value)
{
    button_size = value;
}

Button* Toplevel::getButton_close() const{
    return button_close;
}

void Toplevel::setButton_close(Button *value)
{
    button_close = value;
}

Size Toplevel::getResize_button_window_size() const
{
    return resize_button_window_size;
}

void Toplevel::setResize_button_window_size(const Size &value)
{
    resize_button_window_size = value;
}

Placer *Toplevel::getP_button_close() const
{
    return p_button_close;
}

void Toplevel::setP_button_close(Placer *value)
{
    p_button_close = value;
}

Rect Toplevel::getContainer() const
{
    return container;
}

void Toplevel::setContainer(const Rect &value)
{
    container = value;
}

Placer *Toplevel::getP_in_window() const
{
    return p_in_window;
}

void Toplevel::setP_in_window(Placer *value)
{
    p_in_window = value;
}

bool_t Toplevel::getTop_bar_clicked() const
{
    return top_bar_clicked;
}

void Toplevel::setTop_bar_clicked(const bool_t &value)
{
    top_bar_clicked = value;
}

bool_t Toplevel::getResize_button_pressed() const
{
    return resize_button_pressed;
}

void Toplevel::setResize_button_pressed( bool_t value)
{
    resize_button_pressed = value;
}

bool_t Toplevel::getButton_close_pressed() const
{
    return button_close_pressed;
}

void Toplevel::setButton_close_pressed(const bool_t &value)
{
    button_close_pressed = value;
}

Point Toplevel::getMouse_pos() const{
    return mouse_pos;
}

void Toplevel::setMouse_pos(Point point){
    mouse_pos = point;
}

void Toplevel::setContainer_topleft(Point contopleft){
    container.top_left=contopleft;
}

bool_t Toplevel::getClosable_done() const
{
    return closable_done;
}

void Toplevel::setClosable_done(const bool_t &value)
{
    closable_done = value;
}


bool_t Toplevel::getTo_forget() const
{
    return to_forget;
}

void Toplevel::setTo_forget(const bool_t &value)
{
    to_forget = value;
}

Point Toplevel::getPre_pos() const
{
    return pre_pos;
}

void Toplevel::setPre_pos(const Point &value)
{
    pre_pos = value;
}

Size Toplevel::getPre_size() const
{
    return pre_size;
}

void Toplevel::setPre_size(const Size &value)
{
    pre_size = value;
}

bool_t Toplevel::getFixScreen() const
{
    return fixScreen;
}

void Toplevel::setFixScreen(const bool_t &value)
{
    fixScreen = value;
}


bool_t Toplevel::getFix_screen_released() const
{
    return fix_screen_released;
}

void Toplevel::setFix_screen_released(const bool_t &value)
{
    fix_screen_released = value;
}

bool_t Toplevel::get_show_arrow() const
{
    return show_arrow;
}

void Toplevel::set_show_arrow(bool_t show)
{
    show_arrow = show;
}


//End Getter & Setter

}
