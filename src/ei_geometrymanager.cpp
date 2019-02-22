#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


namespace ei {

//Constructor : make sure that attributs are null at the begin.
GeometryManager::GeometryManager(){
      set_placer(false);
      set_widget(NULL);
      set_anchor(NULL);
      set_x(NULL);
      set_y(NULL);
      //set_width(NULL);
      //set_height(NULL);
      width = new float(0.0);
      height = new float(0.0);
      set_rel_x(NULL);
      set_rel_y(NULL);
      set_rel_width(NULL);
      set_rel_height(NULL);
}
//Destructor
GeometryManager::~GeometryManager()
{
}

bool GeometryManager::is_placer()
{
    return placer;
}

void GeometryManager::set_placer(bool state)
{
    placer = state;
}
//Getter and setter
Widget *GeometryManager::get_widget()
{
    return widget;
}
void GeometryManager::set_widget(Widget *new_widget)
{
    widget = new_widget;
}

anchor_t *GeometryManager::get_anchor()
{
    return anchor;
}
void GeometryManager::set_anchor(anchor_t *new_anchor)
{
    anchor = new_anchor;
}

int *GeometryManager::get_x()
{
    return x;
}
void GeometryManager::set_x(int *new_x)
{
    x=new_x;
    //if(new_x) widget->getScreenLocation()->top_left.x() = *new_x;
}

int *GeometryManager::get_y()
{
    return y;
}
void GeometryManager::set_y(int *new_y)
{

    y = new_y;
}

float *GeometryManager::get_width()
{
    return width;
}
void GeometryManager::set_width(float *new_width)
{
    width = new_width;
}

float *GeometryManager::get_height()
{
    return height;
}
void GeometryManager::set_height(float *new_height)
{
    height = new_height;
}

float *GeometryManager::get_rel_x()
{
    return rel_x;
}
void GeometryManager::set_rel_x(float *new_rel_x)
{
    rel_x = new_rel_x;
}

float *GeometryManager::get_rel_y()
{
    return rel_y;
}
void GeometryManager::set_rel_y(float *new_rel_y)
{
    rel_y = new_rel_y;
}

float *GeometryManager::get_rel_width()
{
    return rel_width;
}
void GeometryManager::set_rel_width(float *new_rel_width)
{
    rel_width = new_rel_width;
}

float *GeometryManager::get_rel_height()
{
    return rel_height;
}
void GeometryManager::set_rel_height(float *new_rel_height)
{
    rel_height = new_rel_height;
}

string GeometryManager::to_string()
{
    stringstream stream;
    if(placer)stream << "bool placer : " << placer << "\n";
    if(widget) stream << "Widget* widget : " << widget << "\n";
    if(anchor) stream << "anchor_t* anchor : " << anchor << "\n";
    if(x) stream << "int* x : " << *(x) << "\n";
    if(y) stream << "int* y : " << *(y) << "\n";
    if(width) stream << "float* width : " << *(width) << "\n";
    if(height) stream << "float* height : " << *(height) << "\n";
    if(rel_x) stream << "float* rel_x : " << *(rel_x) << "\n";
    if(rel_y) stream << "float* rel_y : " << *(rel_y) << "\n";
    if(rel_width) stream << "float* rel_width : " << *(rel_width) << "\n";
    if(rel_height) stream << "float* rel_height : " << *(rel_height) << "\n";
    return stream.str();
}
}
