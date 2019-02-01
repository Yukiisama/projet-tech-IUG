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
  this->parent = parent;
  this->children = NULL;
  this->geom_manager = NULL;
  this->requested_size = NULL;
  this->screen_location = NULL;
  this->content_rect = NULL;

  this->s_idGenerator = 0;

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
  if(clipper==NULL){

  }else{

  }
  //ei_copy_surface(surface, this->screen_location, , EI_FALSE);
}

/**
 * \brief   Method that is called to notify the widget that its geometry has been modified
 *      by its geometry manager.
 *
 * @param   rect        The new rectangular screen location of the widget
 *                      (i.e., = widget->screen_location).
 */
virtual void Widget::geomnotify (Rect rect){
  this->screen_location.top_left.x = rect.top_left.x;
  this->screen_location.top_left.y = rect.top_left.y;
  this->screen_location.size.width = rect.size.width;
  this->screen_location.size.height = rect.size.height;
}

Widget* Widget::pick(uint32_t id){
  this->pick_id = id;
  return this->parent;
}
uint32_t Widget::getPick_id() const{
  return this->pick_id;}

Widget* Widget::getParent() const{
  return this->parent;
}

}
