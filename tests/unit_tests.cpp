#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "ei_application.h"
#include "ei_eventmanager.h"
#include <iostream>
#include <stdlib.h>

#include "hw_interface.h"
#include "ei_main.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_eventmanager.h"
#include "ei_geometrymanager.h"
using namespace ei;

TEST_CASE("create_window", "[unit]")
{
    surface_t main_window = NULL;
    Size main_window_size(640,480), query_size;

    main_window = hw_create_window(&main_window_size, EI_FALSE);
    REQUIRE( main_window != NULL );

    query_size = hw_surface_get_size(main_window);
    REQUIRE( main_window_size.width()  == query_size.width() );
    REQUIRE( main_window_size.height() == query_size.height() );

}

TEST_CASE("fill_window", "[unit]")
{
    surface_t main_window = NULL;
    Size main_window_size(640,480), query_size;
    color_t red = {0xff, 0x00, 0x00, 0xff}, query_color;
    Point origin;

    main_window = hw_create_window(&main_window_size, EI_FALSE);

    SECTION( "fill_red" ) {
        fill(main_window, &red, EI_FALSE);
        hw_surface_update_rects(linked_rect_t());

        query_color = hw_get_pixel(main_window, origin);
        REQUIRE( query_color.red == red.red );
        REQUIRE( query_color.green == red.green );
        REQUIRE( query_color.blue == red.blue );
    }

    SECTION( "fill_default" ) {
        fill(main_window, NULL, EI_FALSE);
        hw_surface_update_rects(linked_rect_t());

        query_color = hw_get_pixel(main_window, origin);
        REQUIRE( query_color.red == font_default_color.red );
        REQUIRE( query_color.green == font_default_color.green );
        REQUIRE( query_color.blue == font_default_color.blue );
    }

    SECTION ("conver_id_color"){
        Widget w;
        color_t c = w.convert_id_color(7631988);
        uint32_t id = w.conver_color_id(c);
        REQUIRE(id == 7631988);
    }
}
TEST_CASE("widget class","[unit]"){
    surface_t main_window = NULL;
    Size main_window_size(640,480), query_size;
    color_t red = {0xff, 0x00, 0x00, 0xff}, query_color;

    Point origin;
    main_window = hw_create_window(&main_window_size, EI_FALSE);
    SECTION ("Constructor"){
        /* Create the application and change the color of the background. */
        bool state = true;
        Application* app = new Application(&main_window_size);
        app->root_widget()->configure(&main_window_size, &red, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        Widget * widget = new Widget("test_widget",app->root_widget());

        REQUIRE(widget->getName() == "test_widget");
        REQUIRE(widget->getParent()==app->root_widget());
        REQUIRE(app->root_widget()->getChildren().front()==widget);
        REQUIRE(widget->getPick_id()==1);
        REQUIRE(widget->getPick_color().blue==widget->convert_id_color(widget->getPick_id()).blue);
        REQUIRE(widget->getPick_color().red==widget->convert_id_color(widget->getPick_id()).red);
        REQUIRE(widget->getPick_color().green==widget->convert_id_color(widget->getPick_id()).red);
        REQUIRE(widget->getPick_color().alpha==widget->convert_id_color(widget->getPick_id()).alpha);
        REQUIRE(!widget->getGeom_manager());
        REQUIRE(widget->getRequested_size().width()==100.f);
        REQUIRE(widget->getRequested_size().height()==100.f);
        REQUIRE(widget->getScreen_location().top_left.x()==0);
        REQUIRE(widget->getScreen_location().top_left.y()==0);
        REQUIRE(widget->getScreen_location().size.width()==100.f);
        REQUIRE(widget->getScreen_location().size.height()==100.f);
        REQUIRE(widget->getColor().red==0xA0);
        REQUIRE(widget->getColor().green==0xA0);
        REQUIRE(widget->getColor().blue==0xA0);
        REQUIRE(widget->getColor().alpha==0xff);
        delete widget;

    }
    SECTION ("Configure"){
        Application* app = new Application(&main_window_size);
        app->root_widget()->configure(&main_window_size, &red, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        Size s = Size(200,200);
        Widget * widget = new Widget("test_widget",app->root_widget());
        widget->configure(&s,&red);
        REQUIRE(widget->getRequested_size().width()==200.f);
        REQUIRE(widget->getRequested_size().height()==200.f);

        REQUIRE(widget->getColor().red==0xff);
        REQUIRE(widget->getColor().green==0x00);
        REQUIRE(widget->getColor().blue==0x00);
        REQUIRE(widget->getColor().alpha==0xff);
        s = Size(400,300);
        widget->configure(&s,&red);
        REQUIRE(widget->getRequested_size().width()==400.f);
        REQUIRE(widget->getRequested_size().height()==300.f);
        delete widget;
        }
    }


TEST_CASE("Placer class", "[unit]"){
    Size       screen_size = Size(600, 600);
    color_t root_bgcol  = {0x52, 0x7f, 0xb4, 0xff};
    Size    button_size = Size(300,200);
    int     button_x    = 150;
    int     button_y    = 200;
    color_t  button_color        = {0x88, 0x88, 0x88, 0xff};
    const char* button_title        = "Mon premier Bouton !";
    color_t  button_text_color   = {0x00, 0x00, 0x00, 0xff};
    int button_corner_radius        = 20;
    relief_t button_relief       = ei_relief_raised;
    int button_border_width      = 6;
    /* Create the application and change the color of the background. */
    Application* app = new Application(&screen_size);
    app->root_widget()->configure(&screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    /* Create and configure the button */
    Button* button = new Button(app->root_widget());
    button->configure (&button_size, &button_color,
                       &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
                       NULL, NULL, NULL);
    Placer* p1 = new Placer();
    SECTION("Constructor"){
        REQUIRE(p1->getWidget() == nullptr);
        REQUIRE(p1->getPlacer() == true);
        REQUIRE(p1->getAnchor() == ei_anc_northwest);
        REQUIRE(p1->getX() == 0);
        REQUIRE(p1->getY() == 0);
        REQUIRE(p1->getHeight() == 0.0);
        REQUIRE(p1->getWidth() == 0.0);
        REQUIRE(p1->getRel_x() == 0.0);
        REQUIRE(p1->getRel_y() == 0.0);
        REQUIRE(p1->getRel_width() == 0.0);
        REQUIRE(p1->getRel_height() == 0.0);
    }
    SECTION("Configure"){
        anchor_t button_anchor   = ei_anc_southeast;
        float   button_rel_x    = 1.5;
        float   button_rel_y    = 1.5;
        int     button_x    = -10;
        int     button_y    = -10;
        float   button_rel_size_x = 0.45;
        p1->configure(button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, NULL);
        REQUIRE(p1->getWidget() == button);
        REQUIRE(p1->getAnchor() == button_anchor);
        REQUIRE(p1->getX() == button_x);
        REQUIRE(p1->getY() == button_y);
        REQUIRE(p1->getWidth() == button->getRequested_size().width());
        REQUIRE(p1->getHeight() == button->getRequested_size().height());
        REQUIRE(p1->getRel_x() == button_rel_x);
        REQUIRE(p1->getRel_y() == button_rel_y);
        REQUIRE(p1->getRel_width() == button_rel_size_x);
        REQUIRE(p1->getRel_height() == 0.0);
        p1->configure(button, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        REQUIRE(p1->getWidget() == button);
        REQUIRE(p1->getAnchor() == ei_anc_northwest);
        REQUIRE(p1->getX() == 0);
        REQUIRE(p1->getY() == 0);
        REQUIRE(p1->getWidth() == button->getRequested_size().width());
        REQUIRE(p1->getHeight() == button->getRequested_size().height());
        REQUIRE(p1->getRel_x() == 0.0);
        REQUIRE(p1->getRel_y() == 0.0);
        REQUIRE(p1->getRel_width() == 0.0);
        REQUIRE(p1->getRel_height() == 0.0);
        p1->configure(button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, NULL);
    }
    SECTION("Run"){
        cout << "button->getContent_rect()->size.height() : " << button->getContent_rect()->size.height() << endl;
        cout << "button->getContent_rect()->size.width() : " << button->getContent_rect()->size.width() << endl;
        cout << "button->getContent_rect()->top_left.x() : " << button->getContent_rect()->top_left.x() << endl;
        cout << "button->getContent_rect()->top_left.y() : " << button->getContent_rect()->top_left.y() << endl;
        Size      * new_screen_size = new Size(2000, 1000);
        cout << "app->root_widget()->configure(&new_screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL) :D" << endl;
        app->root_widget()->configure(new_screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        cout << "p1->run(button) :D" << endl;
        p1->run(button);
        cout<<app->root_widget()->getContent_rect()->size.height()<<endl;
        cout << "button->getContent_rect()->size.height() : " << button->getContent_rect()->size.height() << endl;
        cout << "button->getContent_rect()->size.width() : " << button->getContent_rect()->size.width() << endl;
        cout << "button->getContent_rect()->top_left.x() : " << button->getContent_rect()->top_left.x() << endl;
        cout << "button->getContent_rect()->top_left.y() : " << button->getContent_rect()->top_left.y() << endl;
    }
    delete p1;
}

int ei_main(int argc, char* argv[])
{
    // Init acces to hardware.
    hw_init();

    int result = Catch::Session().run( argc, argv );

    // Free hardware resources.
    hw_quit();

    return result;
}
