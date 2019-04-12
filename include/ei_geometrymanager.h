/**
 *  @file ei_geometrymanager.h
 *  @brief  Manages the positionning and sizing of widgets on the screen.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 *
 */

#ifndef EI_GEOMETRYMANAGER_H
#define EI_GEOMETRYMANAGER_H

#include <string>
#include <vector>

#include "ei_types.h"
#include "ei_widget.h"

using namespace std;

namespace ei {

/**
 * \brief Abstract class that represent a geometry manager.
 */
class GeometryManager
{
public:
    GeometryManager();
    virtual ~GeometryManager();

    /**
     * \brief Method that runs the geometry computation for this widget. This may trigger
     *    geometry computation for this widget's parent and the other children of the parent.
     *
     * @param widget    The widget instance for which to compute geometry.
     */
    virtual void run (Widget* widget) = 0;

    /**
     * \brief Tells the geometry manager in charge of a widget to forget it. This removes the
     *    widget from the screen. If the widget is not currently managed, this function
     *    returns silently.
     *    Side effects:
     *    <ul>
     *      <li> the current screen_location of the widget is invalided (which will trigger a redraw), </li>
     *      <li> the screen_location of the widget is reset to 0. </li>
     *    </ul>
     *
     * @param widget    The widget instance that must be forgotten by the geometry manager.
     */
    virtual void release (Widget* widget) = 0;

    /**
     * \brief Returns the name of the Geometry Manager type.
     */
    virtual string getName() = 0;

    Widget *getWidget() const;

    void setWidget(Widget *value);

private:
    Widget* widget;
};


/**
 * @brief The Placer class
 */
class Placer : public GeometryManager
{
public:
    Placer();
    virtual ~Placer();
    /**
     * \brief Configures the geometry of a widget using the "placer" geometry manager.
     *    If the widget was already managed by another geometry manager, then it is first
     *    removed from the previous geometry manager.
     *    If the widget was already managed by the "placer", then this calls simply updates
     *    the placer parameters: arguments that are not NULL replace previous values.
     *    When the arguments are passed as NULL, the placer uses default values (detailed in
     *    the argument descriptions below). If no size is provided (either absolute or
     *    relative), then the requested size of the widget is used, i.e., the minimal size
     *    required to display its content.
     *
     * @param widget    The widget to place.
     * @param anchor    How to anchor the widget to the position defined by the placer
     *        (defaults to ei_anc_northwest).
     * @param x   The absolute x position of the widget (defaults to 0).
     * @param y   The absolute y position of the widget (defaults to 0).
     * @param width   The absolute width for the widget (defaults to the requested width
     *        of the widget).
     * @param height    The absolute height for the widget (defaults to the requested height
     *        of the widget).
     * @param rel_x   The relative x position of the widget: 0.0 corresponds to the left
     *        side of the master, 1.0 to the right side (defaults to 0.0).
     * @param rel_y   The relative y position of the widget: 0.0 corresponds to the top
     *        side of the master, 1.har 0 to the bottom side (defaults to 0.0).
     * @param rel_width The relative width of the widget: 0.0 corresponds to a width of 0,
     *        1.0 to the width of the master (defaults to 0.0).
     * @param rel_height  The relative height of the widget: 0.0 corresponds to a height of 0,
     *        1.0 to the height of the master (defaults to 0.0).
     */
    void configure (Widget*    widget,
                    anchor_t*  anchor,
                    int*       x,
                    int*       y,
                    float*     width,
                    float*     height,
                    float*     rel_x,
                    float*     rel_y,
                    float*     rel_width,
                    float*     rel_height);

    virtual void run (Widget* widget);

    virtual void release (Widget* widget);

    virtual string getName();

    anchor_t getAnchor() const;

    void setAnchor(const anchor_t &value);

    float getWidth() const;

    void setWidth(float value);

    float getHeight() const;

    void setHeight(float value);

    float getRel_x() const;

    void setRel_x(float value);

    float getRel_y() const;

    void setRel_y(float value);

    float getRel_width() const;

    void setRel_width(float value);

    float getRel_height() const;

    void setRel_height(float value);

    int getX() const;

    void setX(int value);

    int getY() const;

    void setY(int value);
    
private:
    anchor_t  anchor;
    int       x;
    int       y;
    float     rel_x;
    float     rel_y;
    float     rel_width;
    float     rel_height;
    float     width;
    float     height;
    
};

struct widget_in_grid{
    Widget* widget;
    int x;
    int y;
    int width;
    int height;
};

/**
 * @brief The Placer class
 */
class Griddeur : public GeometryManager
{
public:
    Griddeur(Widget* parent, int* cell_width, int* cell_height, bool pourcentage);
    virtual ~Griddeur();
    /**
     * \brief Configures the geometry of a widget using the "griddeur" geometry manager.
     *    If the widget was already managed by another geometry manager, then it is first
     *    removed from the previous geometry manager.
     *    If the widget was already managed by the "griddeur", then this calls simply updates
     *    the placer parameters: arguments that are not NULL replace previous values.
     *    When the arguments are passed as NULL, the placer uses default values (detailed in
     *    the argument descriptions below). If no size is provided (either absolute or
     *    relative), then the requested size of the widget is used, i.e., the minimal size
     *    required to display its content.
     *
     * @param widget    The widget on which the grid applies.
     * @param cell_width   The width of each cell (default to 10)
     * @param cell_height   The height of each cell (default to 10)
     */
    void configure (Widget*    widget,
                    int*       cell_width,
                    int*       cell_height,
                    bool       pourcentage);

    virtual void run (Widget* widget);

    virtual void release (Widget* widget);

    virtual string getName();

    void run_all();

    /**
     * @brief Adds a widget on the grid of his father. Simply returns if the given widget is not a son of the widget this grid applies on.
     * @param widget The widget we wanna place of the grid
     * @param x      The x position we want for the widget (in cells)
     * @param y      The y position we want for the widget (in cells)
     * @param width  The width we want for the widget (in cells)
     * @param height The height we want for the widget (in cells)
     */
    void addWidget (Widget * widget,
                    int* x,
                    int* y,
                    int* width,
                    int* height);

    bool isPourcentage();

    void setPourcentage(bool pourcentage);

    int getCell_width();

    void setCell_width(int cell_width);

    int getCell_height();

    void setCell_height(int cell_height);

    list <struct widget_in_grid> getWidgets();

    void setWidgets(list <struct widget_in_grid>Widgets);

private:
    bool pourcentage;
    int cell_width;
    int cell_height;
    list <struct widget_in_grid>Widgets;
};

}

#endif
