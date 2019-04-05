#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

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
        REQUIRE(widget->getRequested_size().width()==100);
        REQUIRE(widget->getRequested_size().height()==100);
        REQUIRE(widget->getScreen_location().top_left.x()==0);
        REQUIRE(widget->getScreen_location().top_left.y()==0);
        REQUIRE(widget->getScreen_location().size.width()==100);
        REQUIRE(widget->getScreen_location().size.height()==100);
        REQUIRE(widget->getColor().red==0xA0);
        REQUIRE(widget->getColor().green==0xA0);
        REQUIRE(widget->getColor().blue==0xA0);
        REQUIRE(widget->getColor().alpha==0xff);
        REQUIRE(widget->getBorder_width() == 0);

    }
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
