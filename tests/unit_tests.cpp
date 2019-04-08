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
    SECTION("ID <=> COLOR"){
        uint id = 30; // aka uint32
        Application* app = new Application(&main_window_size);
        REQUIRE(app->root_widget()->conver_color_id(app->root_widget()->convert_id_color(id)) == id);
        }
    SECTION("ADD TAG"){
            string c = "coucou";
            Application* app = new Application(&main_window_size);
            app->get_widget_root()->addTag(c);
            REQUIRE(app->get_widget_root()->getTag_list().back()==c);
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
    anchor_t button_anchor   = ei_anc_southeast;
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
        float   button_rel_x    = 1.5;
        float   button_rel_y    = 1.5;
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
    SECTION("Run on button son"){
        float   button_rel_x    = 1.5;
        float   button_rel_y    = 1.5;
        float   button_rel_size_x = 0.45;
        p1->configure(button, &button_anchor, &button_x, &button_y, NULL, NULL, &button_rel_x, &button_rel_y, &button_rel_size_x, NULL);
        REQUIRE(button->getContent_rect()->size.height() == 200);
        REQUIRE(button->getContent_rect()->size.width() == 570);
        REQUIRE(button->getContent_rect()->top_left.x() == 750);
        REQUIRE(button->getContent_rect()->top_left.y() == 900);
        Size* new_screen_size = new Size(2000, 1000);
        app->root_widget()->configure(new_screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        p1->run(button);
        REQUIRE(button->getContent_rect()->size.height() == 200);
        REQUIRE(button->getContent_rect()->size.width() == 1200);
        REQUIRE(button->getContent_rect()->top_left.x() == 2850);
        REQUIRE(button->getContent_rect()->top_left.y() == 1500);
    }
    SECTION("Run on frame son"){
        Size frame_size = Size(300, 200);
        int frame_x = 150;
        int frame_y = 200;
        float rel_width = 0.75;
        float rel_height = 0.1;
        float rel_x = 0.15;
        float rel_y = 0.8;
        color_t frame_color = { 0x88, 0x88, 0x88, 0xff };
        const char* button_title        = "Test text on frame";
        color_t  button_text_color   = {0x00, 0x00, 0x00, 0xff};
        relief_t frame_relief = ei_relief_raised;
        int frame_border_width = 6;
        Frame* frame = new Frame(app->root_widget());
        frame->configure(&frame_size, &frame_color, &frame_border_width,
                         &frame_relief, &button_title, NULL, &button_text_color, NULL, NULL, NULL, NULL);
        p1->configure(frame, NULL, &frame_x, &frame_y, NULL, NULL, &rel_x, &rel_y, &rel_width, &rel_height);
        REQUIRE(frame->getContent_rect()->size.height() == 260);
        REQUIRE(frame->getContent_rect()->size.width() == 750);
        REQUIRE(frame->getContent_rect()->top_left.x() == 240);
        REQUIRE(frame->getContent_rect()->top_left.y() == 680);
        Size* new_screen_size = new Size(2000, 1000);
        app->root_widget()->configure(new_screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        p1->run(frame);
        REQUIRE(frame->getContent_rect()->size.height() == 300);
        REQUIRE(frame->getContent_rect()->size.width() == 1800);
        REQUIRE(frame->getContent_rect()->top_left.x() == 450);
        REQUIRE(frame->getContent_rect()->top_left.y() == 1000);
    }
    SECTION("Release"){
        p1->release(button);
        REQUIRE(p1->getWidget() == nullptr);
        REQUIRE(button->getGeom_manager() == nullptr);
        REQUIRE(button->getScreen_location().size.width() == -1);
        REQUIRE(button->getScreen_location().size.height() == -1);
        REQUIRE(button->getScreen_location().top_left.x() == -1);
        REQUIRE(button->getScreen_location().top_left.y() == -1);
    }

    delete p1;
}

TEST_CASE("Frame class"){

    Size       screen_size = Size(600, 600);
    Application* app = new Application(&screen_size);

    Frame* frame;


    SECTION("Constructor"){
        frame = new Frame(app->root_widget());

        REQUIRE(frame->get_relief() == ei_relief_none);
        REQUIRE(frame->get_text() == nullptr);
        REQUIRE(frame->get_text_color().alpha == font_default_color.alpha);
        REQUIRE(frame->get_text_color().red == font_default_color.red);
        REQUIRE(frame->get_text_color().green == font_default_color.green);
        REQUIRE(frame->get_text_color().blue == font_default_color.blue);
        REQUIRE(frame->get_text_anchor() == ei_anc_center);
        REQUIRE(frame->get_img() == nullptr);
        REQUIRE(frame->get_img_rect() == nullptr);
        REQUIRE(frame->get_img_anchor() == ei_anc_center);
    }

    SECTION("Configure"){
        /*
         * J'ai capté des cas non traités par configure (requested size et color à NULL font un segfault ->
         *                                               faut pas se contenter de mettre un if (requested_size && color),
         *                                               il faut calculer une requested size par défaut qui correspond à la taille de l'img ou du texte passé un paramètre, ou (0,0) sinon
         *                                               et il faut mettre un couleur par défaut)
         */
        frame->configure(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

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
