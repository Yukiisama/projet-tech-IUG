#include <stdio.h>
#include <stdlib.h>
#include "ei_types.h"
#include "ei_main.h"
#include "ei_application.h"
#include "ei_event.h"
#include "ei_geometrymanager.h"
#include "ei_widget.h"
#include "hw_interface.h"
#include <iostream>
#include <memory>
using namespace ei;

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
    Size screen_size = Size(600, 600);
    Size top_size = Size(300,300);
    color_t root_bgcol = { 0x52, 0x7f, 0xb4, 0xff };
    hw_init();
    Size frame_size = Size(250, 30);
    int frame_x = 150;
    int frame_y = 100;
    color_t frame_color = { 0xff, 0x80,0x40,200};
    color_t frame_color2 = { 0xff, 0xff,0xff,255};
    const char* button_title        = "Test text on frame";

    color_t  button_text_color   = {0x00, 0x00, 0x00, 0xff};
    surface_t img_a = hw_image_load(DATA_DIR"img.jpg");
    
    
    relief_t frame_relief = ei_relief_raised;
    int frame_border_width = 6;

    const char*  window_title    = "Window";
    color_t      window_color    = {0xA0,0xA0,0xA0, 0xff};
    int          window_border_width    = 3;
    bool_t       closable        = EI_TRUE;
    axis_set_t   window_resizable = ei_axis_both;
    Point        window_position(30, 250);
    /* Create the application and change the color of the background. */
    Application* app = new Application(&screen_size);
    app->root_widget()->configure(&screen_size, &root_bgcol, NULL, NULL, NULL, NULL,
                                NULL, NULL, NULL, NULL, NULL);

    /* Create, configure and place the frame on screen. */
    Entry* frame = new Entry(app->root_widget());
    frame->configure(&frame_size,&root_bgcol, &frame_border_width,
                     &frame_relief, &button_title, NULL, &button_text_color, NULL, NULL, NULL, NULL);
    frame->configure(&frame_size,&frame_color, &frame_border_width,
                     &frame_relief, &button_title, NULL, &root_bgcol, NULL, NULL, NULL, NULL);
    //frame->configure(&frame_size, &frame_color, &frame_border_width,
    //                 &frame_relief, NULL, NULL, &button_text_color, NULL, &img_a, NULL, NULL);
    Placer* p = new Placer();
    p->configure(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL);
    int x2 = 200; int y2 = 200;
    /* Create, configure and place the frame on screen. */
    Entry* frame2 = new Entry(app->get_widget_root());
    frame2->configure(&frame_size,&root_bgcol, &frame_border_width,
                     &frame_relief, &button_title, NULL, &button_text_color, NULL, NULL, NULL, NULL);
    cout<<frame2->getName()<<endl;
    frame2->configure(&frame_size,NULL, &frame_border_width,
                     &frame_relief, &button_title, NULL, &button_text_color, NULL, NULL, NULL, NULL);
    //frame->configure(&frame_size, &frame_color, &frame_border_width,
    //                 &frame_relief, NULL, NULL, &button_text_color, NULL, &img_a, NULL, NULL);
    Placer* p2 = new Placer();
    p2->configure(frame2, NULL, &x2, &y2, NULL, NULL, NULL, NULL, NULL, NULL);



    //frame->configure(&frame_size, &frame_color, &frame_border_width,
    //                 &frame_relief, NULL, NULL, &button_text_color, NULL, &img_a, NULL, NULL);
    Toplevel* toplevel = new Toplevel(app->root_widget());
    toplevel->configure(&top_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
    Placer* p4 = new Placer();
    p4->configure(toplevel, NULL, &(window_position.x()), &(window_position.y()), NULL, NULL, NULL, NULL, NULL, NULL);


    int x3 = 50; int y3 = 50;

    /* Create, configure and place the frame on screen. */
    Entry* frame3 = new Entry(toplevel);
    frame3->configure(&frame_size,NULL, &frame_border_width,
                     &frame_relief, &button_title, NULL, &button_text_color, NULL, NULL, NULL, NULL);
    cout<<frame2->getName()<<endl;
    frame3->configure(&frame_size,NULL, &frame_border_width,
                     &frame_relief, &button_title, NULL, &button_text_color, NULL, NULL, NULL, NULL);
    Placer* p3 = new Placer();
    p3->configure(frame3, NULL, &x3, &y3, NULL, NULL, NULL, NULL, NULL, NULL);
    /* Run the application's main loop. */
    app->run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    delete app;
    delete p;

    return (EXIT_SUCCESS);
}
