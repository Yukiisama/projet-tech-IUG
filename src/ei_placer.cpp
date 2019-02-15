#include "ei_geometrymanager.h"
#include <string>
#include <iostream>
#include "ei_types.h"
#include "ei_widget.h"
using namespace std;
namespace ei {

///modif///
	Placer::Placer(){
        return;
		
		set_placer(true);
        set_widget(NULL);
        set_anchor(NULL);
        set_x((int)NULL);
        set_y((int)NULL);
        set_width((float)NULL);
        set_height((float)NULL);
        set_rel_x((float)NULL);
        set_rel_y((float)NULL);
        set_rel_width((float)NULL);
        set_rel_height((float)NULL);
	}

Placer::~Placer(){
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
                        if (widget == nullptr) return;
                        set_widget(widget);

                        if (anchor != nullptr) set_anchor(anchor);
                        else{
                            anchor_t anc = ei_anc_northwest;
                            set_anchor(&anc);
                        }

                        if (x != nullptr) set_x(*x);
                        else set_x(0);

                        if (y != nullptr) set_y(*y);
                        else set_y(0);

                        if (width != nullptr) set_width(*width);
                        else set_width(0);

                        if (height != nullptr) set_height(*height);
                        else set_height(0);

                        if (rel_x != nullptr) set_rel_x(*rel_x);
                        else set_rel_x(0.0);

                        if (rel_y != nullptr) set_rel_y(*rel_y);
                        else set_rel_y(0.0);

                        if (rel_width != nullptr) set_rel_width(*rel_width);
                        else set_rel_width(0.0);

                        if (rel_height != nullptr) set_rel_height(*rel_height);
                        else set_rel_height(0.0);
                    }

    void Placer::run (Widget* widget){
        if (!widget || !widget->getParent()) return;

        // Creating the new rectangle and setting the right values into it
        Rect new_rect = Rect();

        // Positioning
        int temp_x = widget->getParent()->getScreenLocation()->top_left.x(), temp_y = widget->getParent()->getScreenLocation()->top_left.y();

        temp_x += widget->getParent()->getScreenLocation()->size.width() * get_rel_x();
        temp_y += widget->getParent()->getScreenLocation()->size.height() * get_rel_y();
        temp_x += get_x();
        temp_y += get_y();

        switch (*(get_anchor())){
            case ei_anc_center:
                temp_x -= get_width()/2;
                temp_y -= get_height()/2;
                break;
            case ei_anc_north:
                temp_x -= get_width()/2;
                break;
            case ei_anc_northeast:
                temp_x -= get_width();
                break;
            case ei_anc_east:
                temp_x -= get_width();
                temp_y -= get_height()/2;
                break;
            case ei_anc_southeast:
                temp_x -= get_width();
                temp_y -= get_height();
                break;
            case ei_anc_south:
                temp_x -= get_width()/2;
                temp_y -= get_height();
                break;
            case ei_anc_southwest:
                temp_y -= get_height();
                break;
            case ei_anc_west:
                temp_y -= get_height()/2;
                break;
            case ei_anc_northwest:
                break;
        }

        new_rect.top_left.x() = temp_x;
        new_rect.top_left.y() = temp_y;

        // Sizing
        int temp_width = 0, temp_height = 0;

        temp_width += widget->getParent()->getScreenLocation()->size.width() * get_rel_width();
        temp_height += widget->getParent()->getScreenLocation()->size.height() * get_rel_height();
        temp_width += get_width();
        temp_height += get_height();

        if (temp_width < 0 || temp_height < 0){
            fprintf(stderr, "We won't be able to draw something with a negative width/height!\n");
            return;
        }
        
        new_rect.size.width() = temp_width;
        new_rect.size.height() = temp_height;
        
        // Setting new positioning and sizing rectangle to the widget
        widget->geomnotify(new_rect);

        // Calling run for the widget's children
        int children_list_size = widget->getChildren().size();
        if (children_list_size > 0){
            for (std::list<Widget*>::iterator it = widget->getChildren().begin();it!= widget->getChildren().end();it++){
                (*it)->getGeom_manager()->run(*it);
            }
        }

        return;
    }

    void Placer::release (Widget* widget){
        if (get_widget() != widget) {
            return;
        }
        else {
            set_widget(nullptr);
            set_x(0);
            set_y(0);
        }
    }

}
