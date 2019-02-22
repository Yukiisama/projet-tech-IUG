#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include <sstream>
#include <string>

using namespace std;

namespace ei
{

///modif///
GeometryManager::GeometryManager()
{
   placer = false;
   widget = new Widget;
   anchor = new anchor_t;
   x = new int;
   y = new int;
   width = new float;
   height = new float;
   rel_x = new float;
   rel_y = new float;
   rel_width = new float;
   rel_height = new float;
}
///modif///
GeometryManager::~GeometryManager()
{
   delete widget;
   delete anchor;
   delete x;
   delete y;
   delete width;
   delete height;
   delete rel_x;
   delete rel_y;
   delete rel_width;
   delete rel_height;
}

string GeometryManager::to_string()
{
   stringstream stream;
   stream << "bool placer : " << placer << "\n";
   stream << "Widget* widget : " << widget << "\n";
   stream << "anchor_t* anchor : " << *(anchor) << "\n";
   stream << "int* x : " << *(x) << "\n";
   stream << "int* y : " << *(y) << "\n";
   stream << "float* width : " << *(width) << "\n";
   stream << "float* height : " << *(height) << "\n";
   stream << "float* rel_x : " << *(rel_x) << "\n";
   stream << "float* rel_y : " << *(rel_y) << "\n";
   stream << "float* rel_width : " << *(rel_width) << "\n";
   stream << "float* rel_height : " << *(rel_height) << "\n";
   return stream.str();
}

bool GeometryManager::is_placer()
{
   return placer;
}

void GeometryManager::set_placer(bool state)
{
   placer = state;
}

Widget *GeometryManager::get_widget()
{
   return widget;
}
void GeometryManager::set_widget(Widget *new_widget)
{
   widget = new_widget;
}

anchor_t GeometryManager::get_anchor()
{
   return *anchor;
}
void GeometryManager::set_anchor(anchor_t new_anchor)
{
   *anchor = new_anchor;
}

int GeometryManager::get_x()
{
   return *x;
}
void GeometryManager::set_x(int new_x)
{
   *x = new_x;
}

int GeometryManager::get_y()
{
   return *y;
}
void GeometryManager::set_y(int new_y)
{
   *y = new_y;
}

float GeometryManager::get_width()
{
   return *width;
}
void GeometryManager::set_width(float new_width)
{
   *width = new_width;
}

float GeometryManager::get_height()
{
   return *height;
}
void GeometryManager::set_height(float new_height)
{
   *height = new_height;
}

float GeometryManager::get_rel_x()
{
   return *rel_x;
}
void GeometryManager::set_rel_x(float new_rel_x)
{
   *rel_x = new_rel_x;
}

float GeometryManager::get_rel_y()
{
   return *rel_y;
}
void GeometryManager::set_rel_y(float new_rel_y)
{
   *rel_y = new_rel_y;
}

float GeometryManager::get_rel_width()
{
   return *rel_width;
}
void GeometryManager::set_rel_width(float new_rel_width)
{
   *rel_width = new_rel_width;
}

float GeometryManager::get_rel_height()
{
   return *rel_height;
}
void GeometryManager::set_rel_height(float new_rel_height)
{
   *rel_height = new_rel_height;
}
} // namespace ei
