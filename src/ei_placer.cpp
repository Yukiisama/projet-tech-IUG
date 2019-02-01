#include "ei_geometrymanager.h"
#include <string>
#include <iostream>
#include "ei_types.h"
#include "ei_widget.h"
using namespace std;
namespace ei {
	
	Placer::Placer(){
		set_placer(true);
        set_widget(NULL);
        set_anchor(NULL);
        set_x(NULL);
        set_y(NULL);
        set_width(NULL);
        set_height(NULL);
        set_rel_x(NULL);
        set_rel_y(NULL);
        set_rel_width(NULL);
        set_rel_height(NULL);
	}

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
                        if (widget == NULL) return;
                        set_widget(widget);

                        if (anchor != NULL) set_anchor(anchor);
                        else{
                            anchor_t anc = ei_anc_northwest;
                            set_anchor(&anc);
                        } 

                        if (x != NULL) set_x(*x);
                        else set_x(0);

                        if (y != NULL) set_y(y);
                        else set_y(0);

                        if (width != NULL) set_width(*width);
                        else set_width(0);

                        if (height != NULL) set_height(*height);
                        else set_height(0);

                        if (rel_x != NULL) set_rel_x(*rel_x);
                        else set_rel_x(0.0);

                        if (rel_y != NULL) set_rel_y(*rel_y);
                        else set_rel_y(0.0);

                        if (rel_width != NULL) set_rel_width(*rel_width);
                        else set_rel_width(0.0);

                        if (rel_height != NULL) set_rel_height(*rel_height);
                        else set_rel_height(0.0);
                    }

    void Placer::run (Widget* widget){
        return;
    }

    void Placer::release (Widget* widget){
        return;
    }

}


