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
    Toplevel* top = static_cast<Toplevel*>(user_param);
    if(event->type==ei_ev_mouse_buttonup){
        if(top->resizing()) {
            top->set_resize_button_pressed(EI_FALSE);
            return EI_TRUE;
        }
    }else if(event->type==ei_ev_mouse_buttondown){
        top->set_resize_button_pressed(EI_TRUE);
        return EI_TRUE;
    }else if(event->type==ei_ev_mouse_move){
        if(top->resizing()){
            MouseEvent* e = static_cast<MouseEvent*>(event);
            if(Application::getInstance()->inside_root(e->where)){
                int new_width = (e->where.x())-(top->getScreen_location().top_left.x());
                int new_height = (e->where.y())-(top->getScreen_location().top_left.y());
                float deltaX = top->getContent_rect()->size.width()-new_width;
                float deltaY = top->getContent_rect()->size.height()-new_height;
                //we return if the delta is 0 or just incredibly insignifiant
                if((deltaX * deltaX)/2 <2.0f || (deltaY * deltaY)/2 <2.0f) return EI_FALSE;
                //Limit the top level to a minimal size
                if(new_width < top->getMin_size().width())new_width = top->getMin_size().width();
                if(new_height < top->getMin_size().height())new_height = top->getMin_size().height();
                //finally update the new size of the top level
                Size *new_size = new Size(new_width,new_height);
                top->configure(new_size,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
                delete(new_size);
                return EI_TRUE;
            }
        }
    }
    return EI_FALSE;
}

bool_t button_close_callback(Widget* widget, Event* event, void* user_param){
    Toplevel* top = static_cast<Toplevel*>(user_param);
    MouseEvent* e = static_cast<MouseEvent*>(event);
    if(top->closing()){
        if(event->type==ei_ev_mouse_buttonup &&
                Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getButton_close()->getPick_id()){
            top->getGeom_manager()->release(top);
            delete top;
            return EI_TRUE;
        }
        top->set_button_close_pressed(EI_FALSE);//optimization
    }else if(event->type==ei_ev_mouse_buttondown &&
             Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getButton_close()->getPick_id()){
        top->set_button_close_pressed(EI_TRUE);
        return EI_TRUE;
    }
    return EI_FALSE;
}

bool_t topbar_move_callback(Widget* widget, Event* event, void* user_param){
    Toplevel* top = static_cast<Toplevel*>(user_param);
    MouseEvent* e = static_cast<MouseEvent*>(event);
    if(top->moving()&& event->type==ei_ev_mouse_buttonup){
        top->set_top_bar_clicked(EI_FALSE);
        return EI_FALSE;
    }else if(event->type==ei_ev_mouse_buttondown &&
             Application::getInstance()->widget_pick(e->where)->getPick_id()==top->getPick_id()){
        if(top->inside_top_bar(e->where)){
            //Tells the toplevel that its top_bar is clicked
            if(!top->moving()){
                top->set_top_bar_clicked(EI_TRUE);
                top->setMouse_pos(e->where);
             }
            return EI_FALSE;
        }
    }else if(top->moving() && event->type==ei_ev_mouse_move && Application::getInstance()->inside_root(e->where)){
        int move_x = (e->where.x())-(top->getMouse_pos().x());
        int move_y = (e->where.y())-(top->getMouse_pos().y());
        //we return if the movement result is 0 or just incredibly insignifiant
        if(move_x==0.0 && move_y==0.0)return EI_FALSE;
        if(move_x!= 0.0 && (move_x * move_x)/2<1) return EI_FALSE;
        if(move_y!= 0.0 && (move_y * move_y)/2<1) return EI_FALSE;
        //Update geom_manager x & y to update the position of toplevel
        top->getGeom_manager()->setX(int(top->getScreen_location().top_left.x()+move_x));
        top->getGeom_manager()->setY(int(top->getScreen_location().top_left.y()+move_y));
        //finally run the geom_manager that will result in updating the position
        top->getGeom_manager()->run(top);
        top->setMouse_pos(e->where);
        return EI_TRUE;
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
    resize_button = new Button(this);
    resize_button_window_size = Size(RESIZE_DIM,RESIZE_DIM);
    resize_button->configure(&resize_button_window_size,&color,
                             NULL,&radius,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
    p_resize_button= new Placer();
    addTag(TOPLEVEL_NAME);

    //update Toplevel's own content rect which is now depending on container
    setContent_rect(&container);
    //bind resize button
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, resize_button, "",resize_button_callback , this);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, resize_button, "",resize_button_callback , this);
    EventManager::getInstance().bind(ei_ev_mouse_move, resize_button, "",resize_button_callback , this);
    //bind topbar mov
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, NULL, TOPLEVEL_NAME,topbar_move_callback , this);
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, NULL, TOPLEVEL_NAME,topbar_move_callback , this);
    EventManager::getInstance().bind(ei_ev_mouse_move, NULL, TOPLEVEL_NAME,topbar_move_callback , this);
}

Toplevel::~Toplevel(){
    std::list<Widget*>&c_list =children;
    for(std::list<Widget*>::iterator it = c_list.begin();it!= c_list.end();it++){
        //delete button close only if it exist
        if((*it)->getPick_id()==button_close->getPick_id()){
            if(closable){
                delete (*it);
            }
        }
        else//delete children
            delete (*it);
    }
    //delete toplevel basic placer
    delete p_button_close;
    delete p_resize_button;

    //TODO update pick surface with parant's pick color
    //remove from parent's child list
    if(getParent()){
        //getParent()->removeChildren(this);
//        getParent()->draw(Application::getInstance()->get_root_window(),
//                          Application::getInstance()->get_offscreen(),getParent()->getContent_rect());
        Application::getInstance()->invalidate_rect(*getParent()->getContent_rect());
    }
    EventManager::getInstance().deleteWidget(this);
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
 * @brief Draw the toplevel into the main window
 * @param surface : the main window
 * @param pick_surface : pick surface to identify widgets
 * @param clipper If not NULL, the drawing is restricted within this rectangle
 *                      (expressed in the surface reference frame).
*/
void Toplevel::draw (surface_t surface,
                     surface_t pick_surface,
                     Rect*     clipper){

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
    //Placer config for button resize, positon according to toplevel , then run the placer
    int resize_button_x = int(container.size.width()-resize_button_window_size.width());int resize_button_y = int(container.size.height()-top_bar_height/2);
    p_resize_button->configure(resize_button,nullptr,&resize_button_x,&resize_button_y,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr);
    p_resize_button->run(resize_button);

    //draw outside the basic toplevel
    drawBasic_toplevel(surface,pick_surface,clipper);
    //recursive draw
    for(std::list<Widget*>::iterator it = children.begin();it!= children.end();it++){
        //donc apply content rect to button close because it display on the top bar which is not belong to content rect
        if((*it)->getPick_id()==button_close->getPick_id()
                ||(*it)->getPick_id()==resize_button->getPick_id())
            (*it)->draw(surface,pick_surface,clipper);
        else
            (*it)->draw(surface,pick_surface,content_rect);

    }
    return;
}
void Toplevel::updateContent_rect(){
    //do nothing
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
    linked_point_t list_point;
    //Outside the container
    int border = int(border_width);
    int top_bar = int(top_bar_height);
    int r_height = int(requested_size.height());
    int r_width = int(requested_size.width());
    //Assign to the list of point each point we need in order to draw the basic toplevel
    list_point.push_back(Point(screen_location.top_left.x()+border,
                               screen_location.top_left.y()+top_bar));

    list_point.push_back(Point(screen_location.top_left.x(),
                               screen_location.top_left.y()+top_bar));

    list_point.push_back(Point(screen_location.top_left.x(),
                               screen_location.top_left.y()+r_height));

    list_point.push_back(Point(screen_location.top_left.x()+r_width,
                               screen_location.top_left.y()+r_height));

    list_point.push_back(Point(screen_location.top_left.x()+r_width,
                               screen_location.top_left.y()));

    list_point.push_back(screen_location.top_left);

    list_point.push_back(Point(screen_location.top_left.x(),
                               screen_location.top_left.y()+top_bar));

    list_point.push_back(Point(screen_location.top_left.x()+r_width-border,
                               screen_location.top_left.y()+top_bar));

    list_point.push_back(Point(screen_location.top_left.x()+r_width-border,
                               screen_location.top_left.y()+r_height-border));
    list_point.push_back(Point(screen_location.top_left.x()+border,
                               screen_location.top_left.y()+r_height-border));

    //Finally draw the top_level with the list of points
    draw_polygon(surface,list_point,color,clipper);
    //Draw pick_surface outside the container
    pick_color.alpha=ALPHA_MAX;
    draw_polygon(pick_surface,list_point,pick_color,clipper);
    list_point.clear();

    color_t color_white = {255,255,255,ALPHA_MAX};
    //Draw toplevel background
    linked_point_t list_point_bgr;
    //Assign list of points we used in order to draw the toplevel background
    list_point_bgr.push_back(content_rect->top_left);
    list_point_bgr.push_back(Point(content_rect->top_left.x(),
                                   content_rect->top_left.y()+int(content_rect->size.height())));
    list_point_bgr.push_back(Point(content_rect->top_left.x()+int(content_rect->size.width()),
                                   content_rect->top_left.y()+int(content_rect->size.height())));
    list_point_bgr.push_back(Point(content_rect->top_left.x()+int(content_rect->size.width()),
                                   content_rect->top_left.y()));
    draw_polygon(surface,list_point_bgr,color_white,clipper);
    //Draw pick_surface outside the container
    pick_color.alpha=ALPHA_MAX;
    draw_polygon(pick_surface,list_point_bgr,pick_color,clipper);
    list_point_bgr.clear();


    //Title of the window
    Point where = Point(screen_location.top_left.x()+40,screen_location.top_left.y()+int(top_bar_height*0.05));
    font_t title_font=hw_text_font_create(default_font_filename, int(top_bar_height*0.8));
    draw_text(surface,&where,title,title_font,&color_white);
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
        EventManager::getInstance().bind(ei_ev_mouse_buttonup, button_close, "",button_close_callback , this);
        EventManager::getInstance().bind(ei_ev_mouse_buttondown, button_close, "",button_close_callback , this);
    }
    //Finally run the geometry manager in order to place the toplevel and the buttons
    if(geom_manager)geom_manager->run(this);
}

bool_t Toplevel::moving() const{
    return top_bar_clicked;
}

bool_t Toplevel::resizing() const{
    return resize_button_pressed;
}

bool_t Toplevel::closing() const{
    return button_close_pressed;
}



//Getter & Setter

int  Toplevel::get_border_width(){
    return this->border_width;
}
void Toplevel::set_border_width(int border_width){
    this->border_width = border_width;
}
double Toplevel::getTop_bar_height() const
{
    return top_bar_height;
}
void   Toplevel::set_top_bar_height( double top_bar_height){
    this->top_bar_height = top_bar_height;
}
const char * Toplevel::get_title(){
    return this->title;
}
void Toplevel::set_title(const char * title){
    this->title = title;
}
bool_t Toplevel::get_closable(){
    return this->closable;
}
void Toplevel::set_closable(bool_t closable){
    this->closable = closable;
}
axis_set_t Toplevel::get_resizable(){
    return this->resizable;
}
void Toplevel::set_resizable(axis_set_t resizable){
    this->resizable = resizable;
}
Size Toplevel::get_min_size(){
    return this->min_size;
}
void Toplevel::set_min_size( Size min_size){
    this->min_size = min_size;
}
Button * Toplevel::get_button_close(){
    return this->button_close;
}
void Toplevel::set_button_close(Button * button_close){
    this->button_close = button_close;
}
Placer * Toplevel::get_p_button_close(){
    return this->p_button_close;
}
void Toplevel::set_p_button_close(Placer * p_button_close){
    this->p_button_close = p_button_close;
}
Button * Toplevel::get_resize_button(){
    return this->resize_button;
}
void Toplevel::set_resize_button(Button * resize_button){
    this->resize_button = resize_button;
}
Placer * Toplevel::get_p_resize_button(){
    return this->p_resize_button;
}
void Toplevel::set_p_resize_button(Placer * p_resize_button){
    this->p_resize_button = p_resize_button;
}
Placer * Toplevel::get_p_in_window(){
    return this->p_in_window;
}
void Toplevel::set_p_in_window(Placer * p_in_window){
    this->p_in_window = p_in_window;
}
void Toplevel::set_top_bar_clicked(bool_t clicking){
    top_bar_clicked = clicking;
}

void Toplevel::set_resize_button_pressed(bool_t pressed){
    resize_button_pressed = pressed;
}

void Toplevel::set_button_close_pressed(bool_t pressed){
    button_close_pressed = pressed;
}

Button* Toplevel::getButton_close() const{
    return button_close;
}

Button* Toplevel::getResize_button() const{
    return resize_button;
}



Point Toplevel::getMouse_pos() const{
    return mouse_pos;
}

Size Toplevel::getMin_size() const{
    return min_size;
}

void Toplevel::setMouse_pos(Point point){
    mouse_pos = point;
}

void Toplevel::setContainer_topleft(Point contopleft){
    container.top_left=contopleft;
}
//End Getter & Setter

}
