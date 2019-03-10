#include "ei_types.h"
#include "ei_event.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "hw_interface.h"
#include <iostream>


namespace ei {

    Frame::Frame(Widget *parent):Widget("frame",parent){
        relief=ei_relief_none;
        text=nullptr;
        text_font=hw_text_font_create(default_font_filename, font_default_size);
        text_color=font_default_color;
        text_anchor=ei_anc_center;
        img=nullptr;
        img_rect=nullptr;
        img_anchor = new anchor_t{ei_anc_center};
    }

    Frame::~Frame(){
        delete img_anchor;
        if(img)delete img;
    }
    /**
     * \brief   Method that draws the widget.
     *
     * @param   surface     Where to draw the widget. The actual location of the widget in the
     *                      surface is stored in its "screen_location" field.
     * @param   pick_surface  Offscreen buffer to draw the widget \ref pick_id
     * @param   clipper     If not NULL, the drawing is restricted within this rectangle
     *                      (expressed in the surface reference frame).
     */
    void Frame::draw(surface_t surface,
                    surface_t pick_surface,
                    Rect *clipper)
    {

        
        if(!surface){
          fprintf(stderr,"Error occured for Frame::draw - surface is not valid\n");
          exit(EXIT_FAILURE);
        }

        if(!pick_surface){
          fprintf(stderr,"Error occured for Frame::draw - pick_surface is not vaild\n");
          exit(EXIT_FAILURE);
        }
        linked_point_t list_frame;
        list_frame.push_back(screen_location.top_left);
        list_frame.push_back(Point(screen_location.top_left.x()+requested_size.width(),
                                   screen_location.top_left.y()));
        list_frame.push_back(Point(screen_location.top_left.x()+requested_size.width(),
                                   screen_location.top_left.y()+requested_size.height()));
        list_frame.push_back(Point(screen_location.top_left.x(),
                                   screen_location.top_left.y()+requested_size.height()));
        
        hw_surface_lock(pick_surface);
        pick_color.alpha=255;
        draw_polygon(pick_surface,list_frame,pick_color,clipper);
        draw_polygon(surface,list_frame,color,clipper);
        hw_surface_unlock(pick_surface);
        if (text)
        {
            Point where = Widget::getAnchorPosition(screen_location, text_anchor);
            draw_text(surface, &where, *text, text_font, &text_color);
        }
        else if(img){
            cout<<"img detected"<<endl;
            hw_surface_lock(surface);
            
            Point frameImgRectTopLeft = Point(200, 200);
            //surface_t a = hw_image_load(DATA_DIR"img.jpg");
            /*ei::Size frameImgRectSize(30, 40);
            Rect* frameImgRect = new Rect(frameImgRectTopLeft, frameImgRectSize);*/
            Point * where = new Point;
            //cout<<a<<endl;
            if(img_anchor)    
                (*where) = Widget::getAnchorPosition(screen_location, *img_anchor);
            else
                where=NULL;
            
            ei_copy_surface(surface,(*img),where,EI_TRUE);
            
            hw_surface_unlock(surface);           
            
            //cas IMG RECT a faire 
        }
        if(!children.empty()){
            for (std::list<Widget *>::iterator it = children.begin(); it != children.end(); it++)
            {
                (*it)->draw(surface, pick_surface, clipper);
            }
        }
        

    }

    /**
     * @brief   Configures the attributes of widgets of the class "frame".
     *
     *      Parameters obey the "default" protocol: if a parameter is "NULL" and it has never
     *      been defined before, then a default value should be used (default values are
     *      specified for each parameter). If the parameter is "NULL" but was defined on a
     *      previous call, then its value must not be changed.
     *
     * @param   requested_size  The size requested for this widget. The geometry manager may
     *                  override this size due to other constraints.
     *                  Defaults to the "natural size" of the widget, ie. big enough to
     *                  display the text or the image, or (0, 0) if the widget has no text
     *                  and no image.
     * @param   color       The color of the background of the widget. Defaults to
     *                      \ref ei_default_background_color.
     * @param   border_width    The width in pixel of the border decoration of the widget. The final
     *                          appearance depends on the "relief" parameter. Defaults to 0.
     * @param   relief      Appearance of the border of the widget. Defaults to \ref ei_relief_none.
     * @param   text        The text to display in the widget, or NULL. Only one of the
     *                      parameter "text" and "img" should be used (i.e. non-NULL). Defaults
     *                      to NULL.
     * @param   text_font   The font used to display the text. Defaults to \ref ei_default_font.
     * @param   text_color  The color used to display the text. Defaults to
     *                      \ref ei_font_default_color.
     * @param   text_anchor The anchor of the text, i.e. where it is placed whithin the widget
     *                      when the size of the widget is bigger than the size of the text.
     *                      Defaults to \ref ei_anc_center.
     * @param   img     The image to display in the widget, or NULL. Any surface can be
     *                  used, but usually a surface returned by \ref hw_image_load. Only one
     *                  of the parameter "text" and "img" should be used (i.e. non-NULL).
     *                  Defaults to NULL.
     * @param   img_rect    If not NULL, this rectangle defines a subpart of "img" to use as the
     *                      image displayed in the widget. Defaults to NULL.
     * @param   img_anchor  The anchor of the image, i.e. where it is placed whithin the widget
     *                      when the size of the widget is bigger than the size of the image.
     *                      Defaults to \ref ei_anc_center.
     */

    void Frame::configure (Size*           requested_size,
                           const color_t*  color,
                           int*            border_width,
                           relief_t*       relief,
                           const char**          text,
                           font_t*         text_font,
                           color_t*        text_color,
                           anchor_t*       text_anchor,
                           surface_t*      img,
                           Rect**          img_rect,
                           anchor_t*       img_anchor){
        if(img && text) fprintf(stderr,"Only one of the parameter \"text\" and \"img\" should be used (i.e. non-NULL).");
        Widget::configure(requested_size,color);
        if(relief) this->relief = *relief;
        if(text) this->text = text;
        if(text_font) this->text_font = *text_font;
        if(text_color) this->text_color = *text_color;
        
        if(img) {this->img = new surface_t;(*this->img) = *img;}
        //if(img) {this->img = new surface_t; surface_t a = hw_image_load(DATA_DIR"img.jpg"); (*this->img) = a;}
        if(img_rect) this->img_rect = *img_rect;
        if(img_anchor)(*this->img_anchor) = *img_anchor;
    }

    string Frame::to_string()
    {
        stringstream stream;
        stream << Widget::to_string();
        stream << "relief_t relief : " << relief << "\n";
        stream << "char** text : " << text << "\n";
        stream << "font_t text_font : " << text_font << "\n";
        stream << "color_t text_color : "
               << "Red : " << (int)text_color.red << " Green : " << (int)text_color.green << " Blue : " << (int)text_color.blue << " Alpha : " << (int)text_color.alpha << "\n";
        stream << "anchor_t text_anchor : " << text_anchor << "\n";
        stream << "surface_t* img : " << img << "\n";
        if (img_rect)
            stream << "Rect* img_rect : "
                   << "Width : " << img_rect->size.width() << " Height : " << img_rect->size.height() << " X : " << img_rect->top_left.x() << " Y : " << img_rect->top_left.y() << "\n";
        stream << "anchor_t* img_anchor : " << img_anchor << "\n";
        return stream.str();
    }
}
