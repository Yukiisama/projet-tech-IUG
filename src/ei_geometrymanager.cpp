#include "ei_types.h"
#include "ei_geometrymanager.h"

using namespace ei;

/**
 * @brief GeometryManager Constructor
 */
GeometryManager::GeometryManager(){ }

/**
 * @brief Destructor of GeometryManager
 */
GeometryManager::~GeometryManager(){ }

Widget *GeometryManager::getWidget() const
{
    return widget;
}

void GeometryManager::setWidget(Widget *value)
{
    widget = value;
}

