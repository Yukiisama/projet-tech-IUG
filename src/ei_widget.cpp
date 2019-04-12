#include "ei_types.h"
#include "ei_widget.h"
#include "ei_draw.h"
#include <string>
#include <iostream>
#define ROOT_NAME "root"
#define DEFAULT_DIM 100
#define BORDER_WIDTH 0
using namespace std;
namespace ei{


uint32_t Widget::s_idGenerator = 0;

Widget::Widget(){
    return;
}
/**
     * @brief   Construct a new instance of a widget of some particular class, as a descendant of an existing widget.
     *
     *      The widget is not displayed on screen until it is managed by a geometry manager.
     *
     * @param   class_name  The name of the class of the widget that is to be created.
     * @param   parent      A pointer to the parent widget. Cannot be NULL except for the root widget.
     */
Widget::Widget(const widgetclass_name_t& class_name, Widget* parent){
    if(parent){
        name = class_name;
        this->parent=parent;
        //Add this widget as children to the parent in parameter.
        parent->children.push_back(this);
    }else{
        name=ROOT_NAME;
        parent = nullptr;
    }
    //Increase by 1 to assure the uniqueness of the generated Ids
    pick_id=s_idGenerator++;
    pick_color=convert_id_color(this->pick_id);
    geom_manager = nullptr;

    //Set a default size
    requested_size.width()=DEFAULT_DIM;
    requested_size.height()=DEFAULT_DIM;

    //Set up default screen location
    screen_location = Rect(Point(0,0),requested_size);
    content_rect = &screen_location;

    //Use default background color from ei_types
    color=default_background_color;
    border_width=BORDER_WIDTH;
}


/**
     * @brief   Destroys the widget. Removes it from screen if it is managed by a geometry manager.
     *          Destroys all its descendants.
     */
Widget::~Widget(){

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
void Widget::draw (surface_t surface, surface_t pick_surface, Rect* clipper){

}

//Deprecated
void Widget::updateContent_rect(){
    return;
}
/**
 * @brief Widget::anchor_to_pos : Compute the position according to the rect and the anchor given
 * @param rect
 * @param anchor
 * @return the position according to anchor
 */
Point Widget::anchor_to_pos(Rect rect, anchor_t anchor) const{
    int x = 0, y =0;
    int r_height = int(requested_size.height());
    int r_width = int(requested_size.width());
    if(anchor == ei_anc_center){
        x = rect.top_left.x()+r_width/2;
        y = rect.top_left.y()+r_height/2;
    }
    if(anchor == ei_anc_north){
        x = rect.top_left.x()+r_width/2;
        y = rect.top_left.y();
    }
    if(anchor == ei_anc_northeast){
        x = rect.top_left.x()+r_width;
        y = rect.top_left.y();
    }
    if(anchor == ei_anc_east){
        x = rect.top_left.x()+r_width;
        y = rect.top_left.y()+r_height/2;
    }
    if(anchor == ei_anc_southeast){
        x = rect.top_left.x()+r_width;
        y = rect.top_left.y()+r_height;
    }
    if(anchor == ei_anc_south){
        x = rect.top_left.x()+r_width/2;
        y = rect.top_left.y()+r_height;
    }
    if(anchor == ei_anc_southwest){
        x = rect.top_left.x();
        y = rect.top_left.y()+r_height;
    }
    if(anchor == ei_anc_west){
        x = rect.top_left.x();
        y = rect.top_left.y()+r_height/2;
    }
    if(anchor == ei_anc_northwest){
        x = rect.top_left.x();
        y = rect.top_left.y();
    }
    return Point(x,y);
}
/**
 * @brief Widget::text_anchor_to_pos Compute the position according to the rect and the anchor given
 * @param rect
 * @param anchor
 * @param text_size
 * @param border_width
 * @return the position according to the text_anchor
 */
Point Widget::text_anchor_to_pos(Rect rect, anchor_t anchor,Size text_size,int border_width) const{
    int x = 0, y =0;
    int r_height = int(requested_size.height());
    int r_width = int(requested_size.width());
    if(anchor == ei_anc_center){
        x = int(rect.top_left.x()+(r_width-text_size.width())/2);
        y = int(rect.top_left.y()+(r_height-text_size.height())/2);
    }
    if(anchor == ei_anc_north){
        x = int(rect.top_left.x()+(r_width-text_size.width())/2);
        y = int(rect.top_left.y()+border_width);
    }
    if(anchor == ei_anc_northeast){
        x = int(rect.top_left.x()+(r_width-border_width-text_size.width()));
        y = int(rect.top_left.y()+border_width);
    }
    if(anchor == ei_anc_east){
        x = int(rect.top_left.x()+(r_width-border_width-text_size.width()));
        y = int(rect.top_left.y()+(r_height-text_size.height())/2);
    }
    if(anchor == ei_anc_southeast){
        x = int(rect.top_left.x()+(r_width-border_width-text_size.width()));
        y = int(rect.top_left.y()+(r_height-border_width-text_size.height()));
    }
    if(anchor == ei_anc_south){
        x = int(rect.top_left.x()+(r_width-text_size.width())/2);
        y = int(rect.top_left.y()+(r_height-border_width-text_size.height()));
    }
    if(anchor == ei_anc_southwest){
        x = int(rect.top_left.x()+border_width);
        y = int(rect.top_left.y()+(r_height-border_width-text_size.height()));
    }
    if(anchor == ei_anc_west){
        x = int(rect.top_left.x()+border_width);
        y = int(rect.top_left.y()+(r_height-text_size.height())/2);
    }
    if(anchor == ei_anc_northwest){
        x = rect.top_left.x()+border_width;
        y = rect.top_left.y()+border_width;
    }
    return Point(x,y);
}
/**
 * @brief Widget::convert_id_color : Convert the id of the widget into the corresponding color
 * @param id to convert to the color
 * @return the color corresponding to the id
 */
color_t Widget::convert_id_color(uint32_t id){

    color_t color;
    color.red = (unsigned char)(id >> 16);
    color.green = (unsigned char)(id >> 8);
    color.blue = (unsigned char)(id >> 0);
    return color;

}
/**
 * @brief Widget::conver_color_id : Convert the color given into the corresponding widget
 * @param color The color of the content
 * @return the id given by the color
 */
uint32_t Widget::conver_color_id(color_t color){

    return (uint32_t)((color.red << 16) |
                      (color.green << 8)  | (color.blue << 0));

}
/**
 * @brief Widget::addTag
 * @param newTag to add to the tag_list
 */
void Widget::addTag(string newTag){
    tag_list.push_back(newTag);
}

void Widget::removeChildren(Widget *widget)
{
    cout<<widget->getParent()->getName()<<endl;
   std::list<Widget*>& c_list =widget->getParent()->getChildren();
   for(std::list<Widget*>::iterator it = c_list.begin(); it!=c_list.end();){
       if((*it)->getPick_id()== widget->getPick_id()){
           it=c_list.erase(it);
       }else{
           ++it;
       }
   }
}

/**
     * \brief   Method that is called to notify the widget that its geometry has been modified
     *      by its geometry manager.
     *
     * @param   rect        The new rectangular screen location of the widget
     *                      (i.e., = widget->screen_location).
     */
void Widget::geomnotify (Rect rect){
    if(rect.size.width() < 0 || rect.size.height() <0) {
        cout << "Negatives inside rect (geomnotify) nothing to do " << endl;
        return;
    }
    this->screen_location.size.width() = rect.size.width();
    this->screen_location.size.height() = rect.size.height();
    this->screen_location.top_left.x() = rect.top_left.x();
    this->screen_location.top_left.y() = rect.top_left.y();
}
/**
 * @brief Widget::pick : Return the widget given by the id
 * @param id
 * @return
 */
Widget* Widget::pick(uint32_t id){
    //Nullptr is returned if id is not belong to the existing widget id.
    if(id>this->s_idGenerator) return nullptr;

    //Case where id is equals to current widget's id.
    //Return current widget;
    if(id==this->pick_id) return this;

    for (std::list<Widget *>::iterator it = children.begin(); it != children.end(); ++it)
    {
        if ((*it)->getPick_id() == id) return *(it);
        //recursive
        Widget *res = (*it)->pick(id);
        if(res) return res;
    }
    return nullptr;
}
/**
 * @brief Widget::configure Configure the widget , overide into top_level, button , frame class
 * @param requested_size The content size requested for this widget
 * @param color       The color of the background of the content of the widget.
 */
void Widget::configure(Size * requested_size, const color_t * color){
    //Assign requested_size if it's not nullptr else this->requested_size stay unchange.
     if(requested_size->width() < 0 || requested_size->height() <0 ){
        cout << "Negatives width or height (widget::configure) nothing to do " << endl;
        return;
    }
    setRequested_size(*requested_size);
    if(color){
        this->color.red = color->red;
        this->color.green = color->green;
        this->color.blue = color->blue;
        this->color.alpha = color->alpha;
    }
}
/**
 * @brief Widget::to_string  ( string with all informations you need for debugging)
 * @return the stream string
 */
string Widget::to_string()
{
    stringstream stream;
    stream << "widgetclass_name_t name : " << name << "\n";
    stream << "uint32_t s_idGenerator : " << s_idGenerator << "\n";
    stream << "uint32_t pick_id : " << pick_id << "\n";
    stream << "color_t pick_color : "
           << "Red : " << (int)pick_color.red << " Green : " << (int)pick_color.green << " Blue : " << (int)pick_color.blue << " Alpha : " << (int)pick_color.alpha << "\n";
    stream << "Widget* parent : " << parent << "\n";
    stream << "GeometryManager* geom_manager : " << geom_manager << "\n";
    stream << "screen location :" <<screen_location.top_left.x()<<" ; "<<screen_location.top_left.y()<< "\n";
    stream << "Size requested_size : "
           << "Width : " << requested_size.width() << " Height : " << requested_size.height() << "\n";
    stream << "Rect screen_location : "
           << "Width : " << screen_location.size.width() << " Height : " << screen_location.size.height() << " X : " << screen_location.top_left.x() << " Y : " << screen_location.top_left.y() << "\n";
    stream << "Rect* content_rect : "
           << "Width : " << content_rect->size.width() << " Height : " << content_rect->size.height() << " X : " << content_rect->top_left.x() << " Y : " << content_rect->top_left.y() << "\n";
    stream << "color_t color : "
           << "Red : " << (int)color.red << " Green : " << (int)color.green << " Blue : " << (int)color.blue << " Alpha : " << (int)color.alpha << "\n";
    stream << "int border_width : " << border_width << "\n";


   return stream.str();
}


//Getter & Setter


widgetclass_name_t Widget::getName(){
    if(!name.empty()) return name ;
    return "name is null" ;
}
uint32_t Widget::getPick_id() const{
    return pick_id;
}
color_t Widget::getPick_color()const{
    return pick_color;
}
Widget* Widget::getParent() {
    return parent;
}

std::list<Widget*>& Widget::getChildren(){
    return children;
}

GeometryManager* Widget::getGeom_manager() const{
    return geom_manager;
}

Size Widget::getRequested_size(){
    return requested_size;
}

Rect Widget::getScreen_location(){
    return screen_location;
}

Rect *Widget::getContent_rect() const
{
    return content_rect;
}

linked_tag_t Widget::getTag_list()const{
    return tag_list;
}
color_t Widget::getColor()const{
    return color;
}

int Widget::getBorder_width()const{
    return border_width;
}
//Setter
void Widget::setGeom_manager(GeometryManager *geom_manager){
    if(geom_manager)this->geom_manager=geom_manager;
}

void Widget::setRequested_size(Size  requested_size){
    std::cout<<"To update Top SIZE : "<<" and size : "<<requested_size.width()
            <<","<<requested_size.height()<<endl;
    this->requested_size=requested_size;
    screen_location.size=requested_size;
    std::cout<<"updated Top SIZE : "<<" and size : "<<screen_location.size.width()
            <<","<<screen_location.size.height()<<endl;
}

void Widget::setScreen_location(Rect screen_location){
    this->screen_location=screen_location;
}

void Widget::setContent_rect(Rect * content_rect){
    if(content_rect)this->content_rect=content_rect;
}

void Widget::setColor(color_t color){
    this->color=color;
}

void Widget::setBorder_width(int border_width){
    this->border_width=border_width;
}










//End Getter Setter

}
