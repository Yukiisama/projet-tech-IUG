#include "ei_types.h"
#include "ei_widget.h"
#include "ei_draw.h"

using namespace std;

#include <string>
#include <iostream>

namespace ei{
/**
 * @brief   Construct a new instance of a widget of some particular class, as a descendant of an existing widget.
 *
 *      The widget is not displayed on screen until it is managed by a geometry manager.
 *
 * @param   class_name  The name of the class of the widget that is to be created.
 * @param   parent      A pointer to the parent widget. Cannot be NULL except for the root widget.
 */

 uint32_t Widget::s_idGenerator = 0;

Widget::Widget(){
  return;
}

Widget::Widget(const widgetclass_name_t& class_name, Widget* parent){
  this->name = class_name;
  if(parent){
    this->parent=parent;
    this->parent->children.push_back(this); //add this widget as children to the parent in parameter.
  }else{
    this->parent = NULL;
    this->name = "root";
  }
  this->pick_id=s_idGenerator++; //increase by 1 to assure the uniqueness of the generated Ids
  this->pick_color=ConvertIdToColor(this->pick_id);
  this->geom_manager = NULL;
  this->requested_size.width()=100; //set a default size
  this->requested_size.height()=100;
  this->screen_location = Rect(Point(0,0),requested_size); //set up default screen location
  this->content_rect = &screen_location;
  this->color=ei::default_background_color; //use default background color from ei_types
  this->border_width=0; //default value = 0.
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
  std::cout << getName() << std::endl;
  if(surface == NULL){
    fprintf(stderr,"Error occured for Widget::draw - surface is NULL\n");
    exit(EXIT_FAILURE);
  }
  if(pick_surface == NULL){
    fprintf(stderr,"Error occured for Widget::draw - pick_surface is NULL\n");
    exit(EXIT_FAILURE);
  }
  if(clipper==NULL){

  }else{

  }
}

Point Widget::getAnchorPosition(Rect rect, anchor_t anchor) const{
    int x, y;
    if(anchor == ei_anc_center){
        x = rect.top_left.x()+rect.size.width()/2;
        y = rect.top_left.y()+requested_size.height()/2;
    }
    if(anchor == ei_anc_north){
        x = rect.top_left.x()+requested_size.width()/2;
        y = rect.top_left.y();
    }
    if(anchor == ei_anc_northeast){
        x = rect.top_left.x()+requested_size.width();
        y = rect.top_left.y();
    }
    if(anchor == ei_anc_east){
        x = rect.top_left.x()+requested_size.width();
        y = rect.top_left.y()+requested_size.height()/2;
    }
    if(anchor == ei_anc_southeast){
        x = rect.top_left.x()+requested_size.width();
        y = rect.top_left.y()+requested_size.height();
    }
    if(anchor == ei_anc_south){
        x = rect.top_left.x()+requested_size.width()/2;
        y = rect.top_left.y()+requested_size.height();
    }
    if(anchor == ei_anc_southwest){
        x = rect.top_left.x();
        y = rect.top_left.y()+requested_size.height();
    }
    if(anchor == ei_anc_west){
        x = rect.top_left.x();
        y = rect.top_left.y()+requested_size.height()/2;
    }
    if(anchor == ei_anc_northwest){
        x = rect.top_left.x();
        y = rect.top_left.y();
    }
    return Point(x,y);
}

/**
 * \brief   Method that is called to notify the widget that its geometry has been modified
 *      by its geometry manager.
 *
 * @param   rect        The new rectangular screen location of the widget
 *                      (i.e., = widget->screen_location).
 */
void Widget::geomnotify (Rect rect){
  this->screen_location.size.width() = rect.size.width();
  this->screen_location.size.height() = rect.size.height();
  this->screen_location.top_left.x() = rect.top_left.x();
  this->screen_location.top_left.y() = rect.top_left.y();
}

Widget* Widget::pick(uint32_t id){
  //nullptr is returned if id is not belong to the existing widget id.
  if(id <0 || id>this->s_idGenerator) return nullptr;
  //case where id is equals to current widget's id.
  if(id==this->pick_id) return this;  //return current widget;
  for (std::list<Widget *>::iterator it = children.begin(); it != children.end(); ++it)
    {
      if ((*it)->getPick_id() == id) return *(it);
      Widget *res = (*it)->pick(id); //recursive
      if(res) return res;
    }
    return nullptr;
  }

uint32_t Widget::getPick_id() const{
  return this->pick_id;
}

Widget* Widget::getParent() const{
  return this->parent;
}

GeometryManager* Widget::getGeom_manager() const{
  return this->geom_manager;
}

std::list<Widget*> Widget::getChildren(){
    return children;
}

Rect* Widget::getScreenLocation(){
  return &screen_location;
}

color_t Widget::ConvertIdToColor(uint32_t id){

  color_t color;
  color.red = (unsigned char)(id >> 16);
  color.green = (unsigned char)(id >> 8);
  color.blue = (unsigned char)(id >> 0);
  return color;

}
uint32_t Widget::ConvertColorToId(color_t color){

  return (uint32_t)((color.red << 16) |
                    (color.green << 8)  | (color.blue << 0));

}
widgetclass_name_t Widget::getName(){
    if(!this->name.empty()) return this->name ;
    return "name is null" ;
}

Rect Widget::getRect(){
  return *content_rect;
}

Size Widget::getRequested_size(){
  return requested_size;
}

void Widget::setGeom_manager(GeometryManager *geom_manager){
  this->geom_manager=geom_manager;
}

void Widget::configure(Size * requested_size, const color_t * color){
  //assign requested_size if it's not nullptr else this->requested_size stay unchange.

  this->requested_size = (requested_size) ? *requested_size : this->requested_size;

  if(color){
    this->color.red = color->red;
    this->color.green = color->green;
    this->color.blue = color->blue;
    this->color.alpha = color->alpha;
  }
}

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
}
