#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_eventmanager.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"

using namespace ei;

bool_t process_key(Widget* widget, Event* event, void* user_param)
{
    if (static_cast<KeyEvent*>(event)->key_sym == ALLEGRO_KEY_ESCAPE) {
        static_cast<Application*>(user_param)->quit_request();
        return EI_TRUE;
    }

    return EI_FALSE;
}

bool_t process_display(Widget* widget, Event* event, void* user_param)
{
    if (static_cast<DisplayEvent*>(event)->closed == EI_TRUE) {
        static_cast<Application*>(user_param)->quit_request();
        return EI_TRUE;
    }

    return EI_FALSE;
}

int ei_main(int argc, char* argv[])
{
    Size         screen_size(800, 800);
    color_t      root_bgcol      = {0x52, 0x7f, 0xb4, 0xff};

    color_t      button_color    = {0x88, 0x88, 0x88, 0xff};
    color_t      text_color      = {0x00, 0x00, 0x00, 0xff};
    anchor_t     text_anchor     = ei_anc_northwest;
    Size         button_size     = Size(100,40);
    relief_t     relief          = ei_relief_raised;
    int          button_border_width = 3;
    const char*  button_title_ok     = "Ok mon ami";
    const char*  button_title_cancel = "Cancel tout stp";
    const char*  button_title_cut = "Cut";

    Size         window_size(400,400);
    const char*  window_title    = "Window";
    color_t      window_color    = {0xA0,0xA0,0xA0, 0xff};
    int          window_border_width    = 3;
    bool_t       closable        = EI_TRUE;
    axis_set_t   window_resizable = ei_axis_both;
    Point        window_position(30, 10);

    Application* app = new Application(&screen_size);
    app->root_widget()->configure(&screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    Toplevel* toplevel = new Toplevel(app->root_widget());
    toplevel->configure(&window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);

    Button* button_ok = new Button(toplevel);
    button_ok->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_ok, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cancel = new Button(toplevel);
    button_cancel->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cancel, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cancel2 = new Button(toplevel);
    button_cancel2->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cancel, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cut = new Button(toplevel);
    button_cut->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cut, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Placer* p1 = new Placer();
    p1->configure(toplevel, NULL, &(window_position.x()), &(window_position.y()), NULL, NULL, NULL, NULL, NULL, NULL);
    Griddeur* g1 = new Griddeur(toplevel, new int(50), new int(35));
    g1->addWidget(button_cut, new int(0), new int(0), new int(1), new int(1));
    g1->addWidget(button_ok, new int(1), new int(1), new int(5), new int(1));
    g1->addWidget(button_cancel, new int(1), new int(2), new int(6), new int(1));
    g1->addWidget(button_cancel2, new int(1), new int(3), new int(7), new int(1));
    g1->run_all();

    EventManager::getInstance().bind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().bind(ei_ev_display, NULL, "all", process_display, app);

    app->run();

    EventManager::getInstance().unbind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().unbind(ei_ev_display, NULL, "all", process_display, app);

    delete app;
    delete p1;
    delete g1;

    return (EXIT_SUCCESS);
}
