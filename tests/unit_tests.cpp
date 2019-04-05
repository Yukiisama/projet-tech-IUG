#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

#include "ei_application.h"
#include "ei_eventmanager.h"
#include "hw_interface.h"
#include "ei_widget.h"
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
    Button* button2 = new Button(app->root_widget());
    button2->configure (&button_size, &button_color,
                       &button_border_width, &button_corner_radius, &button_relief, &button_title, NULL, &button_text_color, NULL,
                       NULL, NULL, NULL);

    Placer* p1 = new Placer();
    Placer* p2 = new Placer();

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
        float   button_rel_x    = 1.0;
        float   button_rel_y    = 1.0;
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

        p2->configure(button2, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, NULL);

        REQUIRE(p2->getWidget() == button2);
        REQUIRE(p2->getAnchor() == button_anchor);
        REQUIRE(p2->getX() == button_x);
        REQUIRE(p2->getY() == button_y);
        REQUIRE(p2->getWidth() == button2->getRequested_size().width());
        REQUIRE(p2->getHeight() == button2->getRequested_size().height());
        REQUIRE(p2->getRel_x() == button_rel_x);
        REQUIRE(p2->getRel_y() == button_rel_y);
        REQUIRE(p2->getRel_width() == button_rel_size_x);
        REQUIRE(p2->getRel_height() == 0.0);

        p2->configure(button2, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

        REQUIRE(p2->getWidget() == button2);
        REQUIRE(p2->getAnchor() == ei_anc_northwest);
        REQUIRE(p2->getX() == 0);
        REQUIRE(p2->getY() == 0);
        REQUIRE(p2->getWidth() == button2->getRequested_size().width());
        REQUIRE(p2->getHeight() == button2->getRequested_size().height());
        REQUIRE(p2->getRel_x() == 0.0);
        REQUIRE(p2->getRel_y() == 0.0);
        REQUIRE(p2->getRel_width() == 0.0);
        REQUIRE(p2->getRel_height() == 0.0);
    }

    delete p1;
    delete p2;
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
