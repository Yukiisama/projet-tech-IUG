#include "ei_geometrymanager.h"
#include <string>
#include <iostream>
#include "ei_types.h"
#include "ei_widget.h"
using namespace std;
namespace ei {
/**
 * @brief The Placer class
 */
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
     *        side of the master, 1.0 to the bottom side (defaults to 0.0).
     * @param rel_width The relative width of the widget: 0.0 corresponds to a width of 0,
     *        1.0 to the width of the master (defaults to 0.0).
     * @param rel_height  The relative height of the widget: 0.0 corresponds to a height of 0,
     *        1.0 to the height of the master (defaults to 0.0).
     */
    void Placer::configure (Widget*    widget,
                    anchor_t*  anchor,
                    int*       x,
                    int*       y,
                    float*     width,
                    float*     height,
                    float*     rel_x,
                    float*     rel_y,
                    float*     rel_width,
                    float*     rel_height)
                    {
                        return;
                    }

    void Placer::run (Widget* widget){
        return;
    }

    void Placer::release (Widget* widget){
        return;
    }

}


