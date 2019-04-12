/**
 * @file    ei_widget.h
 *
 * @brief   API for widgets management: creation, configuration, hierarchy, redisplay.
 *
 *  Created by François Bérard on 30.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 */

#ifndef EI_WIDGET_H
#define EI_WIDGET_H

#include "ei_draw.h"
#include <functional>
#include <sstream>
#include <string>

using namespace std;
namespace ei {

    struct Event;

    /**
     * \brief   A name of a class of widget.
     */
    typedef std::string widgetclass_name_t;


    class GeometryManager;
    class Placer;
    /**
     * \brief   Abstract class representing a widget
     *          Every widget class specializes this base class by adding its own attributs.
     */
    class Widget
    {
    public:
        Widget();
        /**
         * @brief   Construct a new instance of a widget of some particular class, as a descendant of an existing widget.
         *
         *      The widget is not displayed on screen until it is managed by a geometry manager.
         *
         * @param   class_name  The name of the class of the widget that is to be created.
         * @param   parent      A pointer to the parent widget. Cannot be NULL except for the root widget.
         */
        Widget(const widgetclass_name_t& class_name, Widget* parent);

        /**
         * @brief   Destroys the widget. Removes it from screen if it is managed by a geometry manager.
         *          Destroys all its descendants.
         */
        virtual ~Widget();

        /**
         * \brief   Method that draws the widget.
         *
         * @param   surface     Where to draw the widget. The actual location of the widget in the
         *                      surface is stored in its "screen_location" field.
         * @param   pick_surface  Offscreen buffer to draw the widget \ref pick_id
         * @param   clipper     If not NULL, the drawing is restricted within this rectangle
         *                      (expressed in the surface reference frame).
         */
        virtual void draw (surface_t surface, surface_t pick_surface, Rect* clipper);


        //Getter
        widgetclass_name_t getName(); //used to test in eventmanager
        uint32_t getPick_id() const;
        color_t getPick_color()const;
        Widget *getParent() ;
        std::list<Widget*>& getChildren();
        GeometryManager *getGeom_manager() const;
        Size getRequested_size();     //used to initialiser default value of requested width and height in geomanager
        Rect getScreen_location();
        Rect *getContent_rect() const;       //used to add to invalidate_rec in Application to update
        linked_tag_t getTag_list()const;
        color_t getColor()const;
        int getBorder_width()const;

        //Setter
        void setGeom_manager(GeometryManager *geom_manager);
        void setRequested_size(Size  requested_size);
        void setScreen_location(Rect screen_location);
        void setContent_rect(Rect * content_rect);
        void setColor(color_t color);
        void setBorder_width(int border_width);

        //Methods
        virtual void updateContent_rect();
        Point anchor_to_pos(Rect rect, anchor_t anchor) const;
        Point text_anchor_to_pos(Rect rect, anchor_t anchor,Size text_size,int border_width)const;
        color_t convert_id_color(uint32_t id);
        uint32_t conver_color_id(color_t color);
        void addTag(string newTag);
        void removeChildren(Widget * widget);
        /**
         * \brief   Method that is called to notify the widget that its geometry has been modified
         *      by its geometry manager.
         *
         * @param   rect        The new rectangular screen location of the widget
         *                      (i.e., = widget->screen_location).
         */
        virtual void geomnotify (Rect rect);
        //From the root find the widget that pick_id is equal to id.
        Widget* pick(uint32_t id);
        void configure(Size *requested_size, const color_t *color);
        virtual string to_string();


    protected:
        widgetclass_name_t name; ///< The string name of this class of widget.

        static uint32_t s_idGenerator; ///< Static counter to assure the uniqueness of the generated Ids
        uint32_t     pick_id;    ///< Id of this widget in the picking offscreen.
        color_t   pick_color;    ///< pick_id encoded as a color.

        /* Widget Hierachy Management */
        Widget*  parent;             ///< Pointer to the parent of this widget.
        std::list<Widget*> children; ///< List of this widget's children

        /* Geometry Management */
        GeometryManager* geom_manager; ///< Pointer to the geometry management for this widget.
                                       ///  If NULL, the widget is not currently managed and thus, is not mapped on the screen.
        Size  requested_size;  ///< Size requested by the widget (big enough for its label, for example), or by the programmer. This can be different than its screen size defined by the placer.
        Rect  screen_location; ///< Position and size of the widget expressed in the root window reference.
        Rect *content_rect;    ///< Where to place children, when this widget is used as a container. By defaults, points to the screen_location.
        linked_tag_t tag_list;  ///<list of tags that belongs to this widget
        // common variables for sub class
        color_t color;
        int border_width;
    };


    /**
     * @brief   A function that is called in response to a user event.
     *          Usually passed as a parameter to \ref ei::EventManager::bind.
     *
     * @param   widget      The widget for which the event was generated.
     * @param   event       The event containing all its parameters (type, etc.)
     * @param   user_param  The user parameters that was provided by the caller when registering this callback.
     *
     * @return      A boolean telling if the event was consumed by the callback or not.
     *              If TRUE, the library does not try to call other callbacks for this
     *              event. If FALSE, the library will call the next callback registered
     *              for this event, if any.
     *              Note: The callback may execute many operations and still return
     *              FALSE, or return TRUE without having done anything.
     */
    typedef std::function<bool_t(Widget*,Event*,void*)> ei_callback_t;


    class Frame : public Widget
    {
    public:
        Frame();
        Frame(Widget* parent);
        Frame(Widget *parent,const widgetclass_name_t &class_name);
        virtual ~Frame();
        virtual string to_string();
        virtual void draw (surface_t surface,
                           surface_t pick_surface,
                           Rect*     clipper);

        /**
         * @brief   Configures the attributes of widgets of the class "frame".
         *
         *      Parameters obey the "default" pr
            color = default_background_color;otocol: if a parameter is "NULL" and it has never
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
        void configure (Size*           requested_size,
                        const color_t*  color,
                        int*            border_width,
                        relief_t*       relief,
                        const char**          text,
                        font_t*         text_font,
                        color_t*        text_color,
                        anchor_t*       text_anchor,
                        surface_t*      img,
                        Rect**          img_rect,
                        anchor_t*       img_anchor);
        //private variables that belongs to frame class
        //GETTER & SETTER
        
        relief_t get_relief();
        void set_relief(relief_t relief);
        const char * get_text();
        void set_text(const char * text);
        font_t get_text_font();
        void set_text_font(font_t text_font);
        color_t get_text_color();
        void set_text_color(color_t text_color);
        anchor_t get_text_anchor();
        void set_text_anchor(anchor_t text_anchor);
        surface_t get_img();
        void get_img(surface_t img);
        Rect * get_img_rect();
        void set_img_rect(Rect * img_rect);
        anchor_t get_img_anchor();
        void set_img_anchor(anchor_t img_anchor);
        //END GETTER & SETTER
        private:
            relief_t    relief;
            const char*      text;
            font_t      text_font;
            color_t     text_color;
            anchor_t    text_anchor;
            surface_t  img;
            Rect*       img_rect;
            anchor_t  img_anchor;
    };


    struct MouseEvent;

    class Button : public Widget
    {
    public:
        Button(Widget* parent);
        Button(Widget *parent,const widgetclass_name_t& class_name);

        virtual ~Button();

        virtual void draw (surface_t surface,
                           surface_t pick_surface,
                           Rect*     clipper);

        /**
         * @brief   Configures the attributes of widgets of the class "button".
         *
         * @param   widget, requested_size, color, border_width, relief, text, text_font,
         *          text_color, text_anchor, img, img_rect, img_anchor
         *              See the parameter definition of \ref Frame::configure. The only
         *              difference is that relief defaults to \ref ei_relief_raised
         *              and border_width defaults to \ref k_default_button_border_width.
         * @param   corner_radius   The radius (in pixels) of the rounded corners of the button.
         *                          0 means straight corners. Defaults to k_default_button_corner_radius.
         */
        void configure (Size*            requested_size,
                        const color_t*   color,
                        int*             border_width,
                        int*             corner_radius,
                        relief_t*        relief,
                        const char       **text,
                        font_t*          text_font,
                        color_t*         text_color,
                        anchor_t*        text_anchor,
                        surface_t*       img,
                        Rect**           img_rect,
                        anchor_t*        img_anchor);
    //GETTER & SETTER
    int get_corner_radius();
    void set_corner_radius(int corner_radius);
    relief_t get_relief();
    void set_relief(relief_t relief);
    const char * get_text();
    void set_text(const char * text);
    font_t get_text_font();
    void set_text_font(font_t text_font);
    color_t get_text_color();
    void set_text_color(color_t text_color);
    anchor_t get_text_anchor();
    void set_text_anchor(anchor_t text_anchor);
    surface_t get_img();
    void get_img(surface_t img);
    Rect * get_img_rect();
    void set_img_rect(Rect * img_rect);
    anchor_t get_img_anchor();
    void set_img_anchor(anchor_t img_anchor);

    //Methode
    void updateText_Container(Rect clipper);


    private:
        int        corner_radius;
        relief_t    relief;
        const char*      text;
        font_t      text_font;
        color_t     text_color;
        anchor_t    text_anchor;
        surface_t  img;
        Rect*       img_rect;
        anchor_t   img_anchor;
        Rect       text_container;
    };



    class Toplevel : public Widget
    {
    public:
        Toplevel(Widget* parent);

        virtual ~Toplevel();

        bool_t inside_top_bar(Point where) const;

        bool_t inside_right_bottom_corner(Point where) const;
        
        virtual void draw (surface_t surface,
                           surface_t pick_surface,
                           Rect*     clipper);
        virtual void updateContent_rect();
        void drawBasic_toplevel(surface_t surface,
                                surface_t pick_surface,
                                Rect*     clipper);
        /**
         * @brief   Configures the attributes of widgets of the class "toplevel".
         *
         * @param   widget      The widget to configure.
         * @param   requested_size  The content size requested for this widget, this does not include
         *              the decorations (border, title bar). The geometry manager may
         *              override this size due to other constraints.
         *              Defaults to (320x240).
         * @param   color       The color of the background of the content of the widget. Defaults
         *                      to \ref ei_default_background_color.
         * @param   border_width    The width in pixel of the border of the widget. Defaults to 4.
         * @param   title       The string title diplayed in the title bar. Defaults to "Toplevel".
         * @param   closable    If true, the toplevel is closable by the user, the toplevel must
         *                      show a close button in its title bar. Defaults to \ref EI_TRUE.
         * @param   resizable   Defines if the widget can be resized horizontally and/or vertically
         *                      by the user. Defaults to \ref ei_axis_both.
         * @param   min_size    For resizable widgets, defines the minimum size. Defaults to (160, 120).
         */
        void configure (Size*           requested_size,
                        color_t*        color,
                        int*            border_width,
                        const char**    title,
                        bool_t*         closable,
                        axis_set_t*     resizable,
                        Size*           min_size);
    //Getter & Setter

    int getBorder_width() const;
    void setBorder_width(int value);

    double getTop_bar_height() const;

    void setTop_bar_height(double value);

    const char *getTitle() const;

    void setTitle(const char *value);

    bool_t getClosable() const;

    void setClosable(const bool_t &value);

    axis_set_t getResizable() const;

    void setResizable(const axis_set_t &value);

    Size getMin_size() const;

    void setMin_size(const Size &value);

    Size getButton_size() const;

    void setButton_size(const Size &value);

    Button* getButton_close() const;

    void setButton_close(Button *value);

    Size getResize_button_window_size() const;

    void setResize_button_window_size(const Size &value);

    Placer *getP_button_close() const;

    void setP_button_close(Placer *value);

    Rect getContainer() const;

    void setContainer(const Rect &value);

    Placer *getP_in_window() const;

    void setP_in_window(Placer *value);

    bool_t getTop_bar_clicked() const;

    void setTop_bar_clicked(const bool_t &value);

    bool_t getResize_button_pressed() const;

    void setResize_button_pressed( bool_t value);

    bool_t getButton_close_pressed() const;

    void setButton_close_pressed(const bool_t &value);

    Point getMouse_pos() const;

    void setMouse_pos(Point point);

    bool_t getClosable_done() const;

    void setClosable_done(const bool_t &value);

    bool_t getTo_forget() const;

    void setTo_forget(const bool_t &value);

    void setContainer_topleft(Point contopleft);

    Point getPre_pos() const;

    void setPre_pos(const Point &value);

    Size getPre_size() const;

    void setPre_size(const Size &value);

    bool_t getFixScreen() const;

    void setFixScreen(const bool_t &value);

    bool_t getFix_screen_released() const;

    void setFix_screen_released(const bool_t &value);

    bool_t get_show_arrow() const;

    void set_show_arrow(bool_t show);

    //End Getter & Setter




    private:
    int             border_width;
    double          top_bar_height;//The width of the top bar
    const char*     title;
    bool_t          closable;
    axis_set_t      resizable;
    Size            min_size;
    Size            button_size;
    Size            resize_button_window_size;
    Button*         button_close;//The button that close the window
    Placer*         p_button_close;
    Rect            container;      //use to store toplevel's own content rec.
    Placer*         p_in_window;
    bool_t          top_bar_clicked=EI_FALSE;
    bool_t          resize_button_pressed=EI_FALSE;
    bool_t          button_close_pressed=EI_FALSE;
    Point           mouse_pos;
    bool_t          closable_done = EI_FALSE; //to prevent recreate button close
    bool_t          to_forget =EI_FALSE;
    //Bonus resize to half screen
    Point           pre_pos;
    Size            pre_size;
    bool_t          fixScreen=EI_FALSE;
    bool_t          fix_screen_released=EI_FALSE;
    bool_t          show_arrow = EI_FALSE;
    };



    class RadioButton : public Button
    {
    public:
        RadioButton();
        RadioButton(Widget * parent);
        virtual ~RadioButton();
        virtual void draw (surface_t surface,
                           surface_t pick_surface,
                           Rect*     clipper);
        void configure (Size*            requested_size,
                        const color_t*   color,
                        int*             border_width,
                        int*             corner_radius,
                        relief_t*        relief,
                        const char       **text,
                        font_t*          text_font,
                        color_t*         text_color,
                        anchor_t*        text_anchor,
                        surface_t*       img,
                        Rect**           img_rect,
                        anchor_t*        img_anchor);




    };
    class Entry : public Frame
    {
    public:
        Entry();
        Entry(Widget * parent);
        virtual ~Entry();
        virtual void draw (surface_t surface,
                           surface_t pick_surface,
                           Rect*     clipper);
        void configure (Size*   requested_size,
                        const color_t*  color,
                        int*            border_width,
                        relief_t*       relief,
                        const char**          text,
                        font_t*         text_font,
                        color_t*        text_color,
                        anchor_t*       text_anchor,
                        surface_t*      img,
                        Rect**          img_rect,
                        anchor_t*       img_anchor);
        bool_t get_is_clicked();
        void set_is_clicked(bool_t click);
        bool get_done();
        void set_done(bool done);
    private:
        bool_t is_clicked = EI_FALSE;
        bool done = false;




    };

}

#endif
