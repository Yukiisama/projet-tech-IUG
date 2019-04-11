#include "ei_geometrymanager.h"
#include <string>
#include <iostream>
#include <vector>
#include "ei_types.h"
#include "ei_widget.h"
#include "ei_application.h"
using namespace std;
namespace ei {


Griddeur::Griddeur(Widget *parent, int *cell_width, int *cell_height):GeometryManager(){
    (parent) ? setWidget(parent) : setWidget(Application::getInstance()->get_widget_root());
    (cell_width) ? setCell_width(*cell_width) : setCell_width(10);
    (cell_height) ? setCell_height(*cell_height) : setCell_height(10);
}

Griddeur::~Griddeur(){ }

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
 * @param widget    The widget on which the grid applies (default to the root widget).
 * @param cell_width   The width of each cell (default to 10)
 * @param cell_height   The height of each cell (default to 10)
 */
void Griddeur::configure (Widget*    widget,
                int*     cell_width,
                int*     cell_height){
    (widget) ? setWidget(widget): setWidget(Application::getInstance()->get_widget_root());
    (cell_width) ? setCell_width(*cell_width) : setCell_width(10);
    (cell_height) ? setCell_height(*cell_height): setCell_height(10);
}

void Griddeur::run (Widget* widget){
    for (list <widget_in_grid>::iterator i = this->Widgets.begin(); i != this->Widgets.end(); ++i){
        if (i->widget == widget){
            //Positioning
            Point starting_point = Point(this->getWidget()->getContent_rect()->top_left.x(), this->getWidget()->getContent_rect()->top_left.y());
            int temp_x = starting_point.x() + this->cell_width * i->x;
            int temp_y = starting_point.y() + this->cell_height * i->y;
            //Sizing
            int temp_width = this->cell_width * i->width;
            int temp_height = this->cell_height * i->height;

            Rect new_rect = Rect(Point(temp_x, temp_y), Size(temp_width, temp_height));
            widget->geomnotify(new_rect);
        }
    }
}

void Griddeur::run_all(){
    for (auto i = this->Widgets.begin(); i != this->Widgets.end(); ++i){
        run(i->widget);
    }
}

void Griddeur::release (Widget* widget){
    if (!(widget->getGeom_manager())){ return; }
    for (list <widget_in_grid>::iterator i = this->Widgets.begin(); i != this->Widgets.end(); ++i) {
        if (i->widget == widget){
            this->Widgets.pop_front();
            Application::getInstance()->invalidate_rect(*(widget->getContent_rect()));
            Rect r = Rect(Point(-1,-1),Size(-1,-1));
            widget->setGeom_manager(nullptr);
            widget->setScreen_location(r);
            // Calling release for the widget's children
            if (widget->getChildren().empty())
            {
                list<Widget *> w_child = widget->getChildren();
                for (list<Widget *>::iterator it = w_child.begin();it!=w_child.end();it++){
                    if ((*it)->getGeom_manager()) (*it)->getGeom_manager()->release(*it);
                }

            }
        }
    }
}

/**
 * @brief Adds a widget on the grid of his father. Simply returns if the given widget is not a son of the widget this grid applies on.
 * @param widget The widget we wanna place of the grid
 * @param x      The x position we want for the widget (in cells)
 * @param y      The y position we want for the widget (in cells)
 * @param width  The width we want for the widget (in cells)
 * @param height The height we want for the widget (in cells)
 */
void Griddeur::addWidget (Widget * widget,
                int* x,
                int* y,
                int* width,
                int* height){
    if (!widget || !x || !y || !width || !height){
        cout << "Missing argument in a call for Griddeur::addWidget()!" << endl;
        return;
    }
    if (widget->getName() == "Toplevel"){
        cout << "You can't bind a toplevel to a griddeur!" << endl;
        return;
    }
    if (*x < 0 || *y < 0 || *width < 0 || *height < 0){
        cout << "Griddeur::addWidget() was given a negative arguments as pos or size!" << endl;
        return;
    }
    int max_x = this->getWidget()->getContent_rect()->size.width();
    int max_y = this->getWidget()->getContent_rect()->size.height();
    if (*x * this->cell_width > max_x || *y * this->cell_height > max_y){
        cout << "Griddeur::addWidget() was given a too large argument as pos!" << endl;
        return;
    }
    struct widget_in_grid new_push = { .widget = widget, .x = *x, .y = *y, .width = *width,  .height = *height };
    this->Widgets.push_back(new_push);
    widget->setGeom_manager(this);
}

string Griddeur::getName(){ return "griddeur"; }

int Griddeur::getCell_width(){ return cell_width; }

void Griddeur::setCell_width(int cell_width){ this->cell_width = cell_width; }

int Griddeur::getCell_height(){ return cell_height; }

void Griddeur::setCell_height(int cell_height){ this->cell_height = cell_height; }

list <struct widget_in_grid> Griddeur::getWidgets(){ return Widgets; }

void Griddeur::setWidgets(list <struct widget_in_grid>Widgets){ this->Widgets = Widgets; }

}
