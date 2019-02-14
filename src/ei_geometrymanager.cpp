#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"


namespace ei {

///modif///
  GeometryManager::GeometryManager(){
    return;
  }
 ///modif///
 GeometryManager::~GeometryManager(){
 }

    bool  GeometryManager::is_placer(){
		return placer;
	 }

	 void GeometryManager::set_placer(bool state){
		placer = state;
	 }

     Widget * GeometryManager::get_widget(){
         return widget;
     }
	 void GeometryManager::set_widget(Widget * new_widget){
         widget = new_widget;
     }

     anchor_t * GeometryManager::get_anchor(){
         return anchor;
     }
	 void GeometryManager::set_anchor(anchor_t * new_anchor){
         anchor = new_anchor;
     }

     int GeometryManager::get_x(){
         return x;
     }
	 void GeometryManager::set_x(int new_x){
         widget->screen_location.top_left.x()=new_x;
     }

     int GeometryManager::get_y(){
         return y;
     }
	 void GeometryManager::set_y(int new_y){

         widget->screen_location.top_left.y()=new_y;
     }

     float GeometryManager::get_width(){
         return width;
     }
	 void GeometryManager::set_width(float new_width){
         width = new_width;
     }

     float GeometryManager::get_height(){
         return height;
     }
	 void GeometryManager::set_height(float new_height){
         height = new_height;
     }

     float GeometryManager::get_rel_x(){
         return rel_x;
     }
	 void GeometryManager::set_rel_x(float new_rel_x){
         rel_x = new_rel_x;
     }

     float GeometryManager::get_rel_y(){
         return rel_y;
     }
	 void GeometryManager::set_rel_y(float new_rel_y){
         rel_y = new_rel_y;
     }

     float GeometryManager::get_rel_width(){
         return rel_width;
     }
	 void GeometryManager::set_rel_width(float new_rel_width){
         rel_width = new_rel_width;
     }

     float GeometryManager::get_rel_height(){
         return rel_height;
     }
	 void GeometryManager::set_rel_height(float new_rel_height){
         rel_height = new_rel_height;
     }
}
