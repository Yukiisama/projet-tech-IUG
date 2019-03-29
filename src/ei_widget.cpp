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
      if(parent){
          name = class_name;
          this->parent=parent;
          parent->children.push_back(this); //add this widget as children to the parent in parameter.
          /*
          if(parent->getName() == "Toplevel"){
              Toplevel* top = dynamic_cast<Toplevel*>(parent);
              if(top!=NULL){
                  if(top->getButton_close() && top->getResize_button() && top->getIn_window()){
                      this->parent=top->getIn_window();
                      top->getIn_window()->children.push_back(this); //add this widget as children to the parent in parameter.
                  }
                  else{
                      this->parent=top;
                      top->children.push_back(this); //add this widget as children to the parent in parameter.
                  }
              }
          }
          else{
              this->parent=parent;
              parent->children.push_back(this); //add this widget as children to the parent in parameter.
          }*/

      }else{
          name="root";
          parent = NULL;
      }
      pick_id=s_idGenerator++; //increase by 1 to assure the uniqueness of the generated Ids
      pick_color=convert_id_color(this->pick_id);
      geom_manager = NULL;
      requested_size.width()=100; //set a default size
      requested_size.height()=100;
      screen_location = Rect(Point(0,0),requested_size); //set up default screen location
      content_rect = &screen_location;
      color=default_background_color; //use default background color from ei_types
      border_width=0; //default value = 0.
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
    //Getter
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
    Widget* Widget::getParent() const{
      return parent;
    }

    std::list<Widget*> Widget::getChildren(){
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

    Rect* Widget::getContent_rect(){
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
        this->requested_size=requested_size;
        screen_location.size=requested_size;
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

    //Methods
    void Widget::updateContent_rect(){
        return;
    }

    Point Widget::anchor_to_pos(Rect rect, anchor_t anchor) const{
        int x, y;
        if(anchor == ei_anc_center){
            x = rect.top_left.x()+requested_size.width()/2;
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

    Point Widget::text_anchor_to_pos(Rect rect, anchor_t anchor,Size text_size,int border_width) const{
        int x, y;
        if(anchor == ei_anc_center){
            x = rect.top_left.x()+(requested_size.width()-text_size.width())/2;
            y = rect.top_left.y()+(requested_size.height()-text_size.height())/2;
        }
        if(anchor == ei_anc_north){
            x = rect.top_left.x()+(requested_size.width()-text_size.width())/2;
            y = rect.top_left.y()+border_width;
        }
        if(anchor == ei_anc_northeast){
            x = rect.top_left.x()+(requested_size.width()-border_width-text_size.width());
            y = rect.top_left.y()+border_width;
        }
        if(anchor == ei_anc_east){
            x = rect.top_left.x()+(requested_size.width()-border_width-text_size.width());
            y = rect.top_left.y()+(requested_size.height()-text_size.height())/2;
        }
        if(anchor == ei_anc_southeast){
            x = rect.top_left.x()+(requested_size.width()-border_width-text_size.width());
            y = rect.top_left.y()+(requested_size.height()-border_width-text_size.height());
        }
        if(anchor == ei_anc_south){
            x = rect.top_left.x()+(requested_size.width()-text_size.width())/2;
            y = rect.top_left.y()+(requested_size.height()-border_width-text_size.height());
        }
        if(anchor == ei_anc_southwest){
            x = rect.top_left.x()+border_width;
            y = rect.top_left.y()+(requested_size.height()-border_width-text_size.height());
        }
        if(anchor == ei_anc_west){
            x = rect.top_left.x()+border_width;
            y = rect.top_left.y()+(requested_size.height()-text_size.height())/2;
        }
        if(anchor == ei_anc_northwest){
            x = rect.top_left.x()+border_width;
            y = rect.top_left.y()+border_width;
        }
        return Point(x,y);
    }

    color_t Widget::convert_id_color(uint32_t id){

      color_t color;
      color.red = (unsigned char)(id >> 16);
      color.green = (unsigned char)(id >> 8);
      color.blue = (unsigned char)(id >> 0);
      return color;

    }
    uint32_t Widget::conver_color_id(color_t color){

      return (uint32_t)((color.red << 16) |
                        (color.green << 8)  | (color.blue << 0));

    }

    void Widget::addTag(string newTag){
        tag_list.push_back(newTag);
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

    void Widget::configure(Size * requested_size, const color_t * color){
      //assign requested_size if it's not nullptr else this->requested_size stay unchange.

      setRequested_size(*requested_size);
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
