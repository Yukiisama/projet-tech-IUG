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

Widget::Widget(const widgetclass_name_t& class_name, Widget* parent){
  this->name = class_name;
  if(parent){
    this->parent=parent;
    this->parent->children.push_back(this); //add this widget as children to the parent in parameter.
  }else{
    this->parent = nullptr;
  }
  this->pick_id= s_idGenerator++; //increase by 1 to assure the uniqueness of the generated Ids
  this->pick_color=ConvertIdToColor(this->pick_id);
  this->geom_manager = NULL;
  this->requested_size=(100,100); //set a default size
  this->screen_location = Rect(Point(0,0),requested_size); //set up default screen location
  this->content_rect = &screen_location;
  this->color=ei::default_background_color; //use default background color from ei_types
  this->border_width=0; //default value = 0.
}
/*
  static uint32_t s_idGenerator; ///< Static counter to assure the uniqueness of the generated Ids
  uint32_t     pick_id;    ///< Id of this widget in the picking offscreen.
  color_t   pick_color;    ///< pick_id encoded as a color.
*/

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

/**
 * \brief   Method that is called to notify the widget that its geometry has been modified
 *      by its geometry manager.
 *
 * @param   rect        The new rectangular screen location of the widget
 *                      (i.e., = widget->screen_location).
 */
virtual void Widget::geomnotify (Rect rect){
  if(rect == NULL){
    fprintf(stderr,"Error occured for Widget::geomnotify - rect is NULL\n");
    exit(EXIT_FAILURE);
  }
  this->screen_location.size=rect.size;
  this->screen_location.top_left=rect.top_left;
}

Widget* Widget::pick(uint32_t id){
  //nullptr is returned if id is not belong to the existing widget id.
  if(id <0 || id>this->s_idGenerator){
    return nullptr; 
  }
  //case where id is equals to current widget's id.
  if(id==this->pick_id){
    return this;  //return current widget;
  }else{
    //if there are no children, go back to parent and pick id.
    if(this->children.empty){
      return this->parent->pick(id);
    }else{
      for(std::list<Widget *>::iterator it = children.begin();it != children.end(); ++it){
        if(*it->getPick_id()==id){

        }
      }
    }

  }
  
  this->pick_id = id;
  return this->parent;
}
uint32_t Widget::getPick_id() const{
  return this->pick_id;
}

Widget* Widget::getParent() const{
  return this->parent;
}

color_t Widget::ConvertIdToColor(uint32_t id){
  //TODO
};
uint32_t Widget::ConverColorToId(color_t color){
  //TODO
};
}
