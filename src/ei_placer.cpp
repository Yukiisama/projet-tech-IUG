#include "ei_geometrymanager.h"
#include <string>
#include <iostream>
#include "ei_types.h"
#include "ei_widget.h"
using namespace std;
namespace ei {

///modif///
Placer::Placer():GeometryManager(){

    setWidget(nullptr);
    setPlacer(true);
    setAnchor(ei_anc_northwest);
    setX(0);
    setY(0);
    setWidth((0.0));
    setHeight((0.0));
    setRel_x(0.0);
    setRel_y(0.0);
    setRel_width(0.0);
    setRel_height(0.0);

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
                        setWidget(widget);
                        widget->setGeom_manager(this);
                        if (anchor != nullptr) setAnchor(*anchor);


                        if (x != nullptr)setX(*x);


                        if (y != nullptr) setY(*y);


                        if (width != nullptr) setWidth(*width);
                        else setWidth(widget->getRequested_size().width());

                        if (height != nullptr) setHeight(*height);
                        else setHeight(widget->getRequested_size().height());

                        if (rel_x != nullptr) setRel_x(*rel_x);
                        else setRel_x(0.0);

                        if (rel_y != nullptr) setRel_y(*rel_y);
                        else setRel_y(0.0);

                        if (rel_width != nullptr) setRel_width(*rel_width);
                        else setRel_width(0.0);

                        if (rel_height != nullptr) setRel_height(*rel_height);
                        else setRel_height(0.0);
                        run(widget);
                    }

    void Placer::run (Widget* widget){
        if (!widget ) return;
        /*if (widget){
            if(!widget->getParent())
                return;
        }
        */

        // Creating the new rectangle and setting the right values into it
        Rect new_rect = Rect();
        Rect contect_rect = *(widget->getParent()->getContent_rect());
        // Positioning
        int temp_x = contect_rect.top_left.x(), temp_y = contect_rect.top_left.y();

        temp_x += getX()+ contect_rect.size.width() * getRel_x();
        temp_y += getY()+ contect_rect.size.height() * getRel_y();

        switch (getAnchor()){
            case ei_anc_center:
                temp_x -= getWidth()/2;
                temp_y -= getHeight()/2;
                break;
            case ei_anc_north:
                temp_x -= getWidth()/2;
                break;
            case ei_anc_northeast:
                temp_x -= getWidth();
                break;
            case ei_anc_east:
                temp_x -= getWidth();
                temp_y -= getHeight()/2;
                break;
            case ei_anc_southeast:
                temp_x -= getWidth();
                temp_y -= getHeight();
                break;
            case ei_anc_south:
                temp_x -= getWidth()/2;
                temp_y -= getHeight();
                break;
            case ei_anc_southwest:
                temp_y -= getHeight();
                break;
            case ei_anc_west:
                temp_y -= getHeight()/2;
                break;
            case ei_anc_northwest:
                break;
            case ei_anc_none:
                break;
        }

        new_rect.top_left.x() = temp_x;
        new_rect.top_left.y() = temp_y;
        //update container of Toplevel
        if(!widget->getName().compare("Toplevel")){
            Toplevel* top = static_cast<Toplevel*>(widget);
            Point topsl(temp_x+top->getBorder_width(),temp_y+top->getTop_bar_height());
            //cout<<top->getContainer().size.width()<<";"<<top->getContainer().size.height()<<endl;
            top->setContainer_topleft(topsl);
        }
        // Sizing
        int temp_width = 0, temp_height = 0;

        temp_width += contect_rect.size.width() * getRel_width();
        temp_height += contect_rect.size.height() * getRel_height();
        temp_width += getWidth();
        temp_height += getHeight();

        if (temp_width < 0 || temp_height < 0){
            fprintf(stderr, "We won't be able to draw something with a negative width/height!\n");
            return;
        }

        new_rect.size.width() = temp_width;
        new_rect.size.height() = temp_height;
        // Setting new positioning and sizing rectangle to the widget
        widget->geomnotify(new_rect);

        // Calling run for the widget's children
        if (!widget->getChildren().empty())
        {
            list<Widget *> w_child = widget->getChildren();
            for (list<Widget *>::iterator it = w_child.begin();it!=w_child.end();it++){
                if ((*it)->getGeom_manager()) (*it)->getGeom_manager()->run(*it);
            }

        }

        return;
    }

    void Placer::release (Widget* widget){
        if (getWidget() != widget) {
            return;
        }
        else {
            setWidget(nullptr);
            setX(0);
            setY(0);
        }
    }

}
