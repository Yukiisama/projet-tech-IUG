#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include <sstream>
#include <string>

using namespace std;

namespace ei
{

/**
 * @brief GeometryManager Constructor
 */
GeometryManager::GeometryManager()
{
   // No placer and no widget at initialize
   placer = false;
   widget = nullptr;
   //Assign defaults value
   anchor = ei_anc_center;
   x = 0;
   y = 0;
   width = 0.0;
   height = 0.0;
   rel_x = 0.0;
   rel_y = 0.0;
   rel_width = 0.0;
   rel_height = 0.0;
}
/**
 * @brief Destructor of GeometryManager
 */
GeometryManager::~GeometryManager()
{


}
/**
 * @brief GeometryManager::to_string ( string with all informations you need for debugging)
 * @return return the stream string
 */
string GeometryManager::to_string()
{
   stringstream stream;
   stream << "bool placer : " << placer << "\n";
   stream << "Widget* widget : " << widget << "\n";
   stream << "anchor_t* anchor : " << anchor << "\n";
   stream << "int* x : " << x << "\n";
   stream << "int* y : " << y << "\n";
   stream << "float* width : " << width << "\n";
   stream << "float* height : " << height << "\n";
   stream << "float* rel_x : " << rel_x << "\n";
   stream << "float* rel_y : " << rel_y << "\n";
   stream << "float* rel_width : " << rel_width << "\n";
   stream << "float* rel_height : " << rel_height << "\n";
   return stream.str();
}

bool GeometryManager::getPlacer() const
{
    return placer;
}

void GeometryManager::setPlacer(bool value)
{
    placer = value;
}

Widget *GeometryManager::getWidget() const
{
    return widget;
}

void GeometryManager::setWidget(Widget *value)
{
    widget = value;
}

anchor_t GeometryManager::getAnchor() const
{
    return anchor;
}

void GeometryManager::setAnchor(const anchor_t &value)
{
    anchor = value;
}

int GeometryManager::getX() const
{
    return x;
}

void GeometryManager::setX(int value)
{
    x = value;
}

int GeometryManager::getY() const
{
    return y;
}

void GeometryManager::setY(int value)
{
    y = value;
}

float GeometryManager::getWidth() const
{
    return width;
}

void GeometryManager::setWidth(float value)
{
    width = value;
}

float GeometryManager::getHeight() const
{
    return height;
}

void GeometryManager::setHeight(float value)
{
    height = value;
}

float GeometryManager::getRel_x() const
{
    return rel_x;
}

void GeometryManager::setRel_x(float value)
{
    rel_x = value;
}

float GeometryManager::getRel_y() const
{
    return rel_y;
}

void GeometryManager::setRel_y(float value)
{
    rel_y = value;
}

float GeometryManager::getRel_width() const
{
    return rel_width;
}

void GeometryManager::setRel_width(float value)
{
    rel_width = value;
}

float GeometryManager::getRel_height() const
{
    return rel_height;
}

void GeometryManager::setRel_height(float value)
{
    rel_height = value;
}

} // namespace ei
