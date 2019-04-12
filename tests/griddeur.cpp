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
    Size         screen_size(1600, 900);
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
    const char*  window_title    = "Relatif";
    const char*  window_title_    = "Absolu";
    color_t      window_color    = {0xA0,0xA0,0xA0, 0xff};
    int          window_border_width    = 3;
    bool_t       closable        = EI_TRUE;
    axis_set_t   window_resizable = ei_axis_both;
    Point        window_position(30, 10);
    Point        window_position_(530, 10);

    Application* app = new Application(&screen_size);
    app->root_widget()->configure(&screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    //Toplevel 1
    Toplevel* toplevel = new Toplevel(app->root_widget());
    toplevel->configure(&window_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);

    Button* button_ok = new Button(toplevel);
    button_ok->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_ok, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_ok2 = new Button(toplevel);
    button_ok2->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_ok, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cancel = new Button(toplevel);
    button_cancel->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cancel, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cancel2 = new Button(toplevel);
    button_cancel2->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cancel, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cut = new Button(toplevel);
    button_cut->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cut, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cut2 = new Button(toplevel);
    button_cut2->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cut, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cut3 = new Button(toplevel);
    button_cut3->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cut, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Placer* p1 = new Placer();
    p1->configure(toplevel, NULL, &(window_position.x()), &(window_position.y()), NULL, NULL, NULL, NULL, NULL, NULL);
    Griddeur* g1 = new Griddeur(toplevel, new int(18), new int(18), EI_TRUE);
    g1->addWidget(button_cut, new int(0), new int(0), new int(1), new int(1));
    g1->addWidget(button_ok, new int(1), new int(1), new int(3), new int(1));
    g1->addWidget(button_cancel, new int(1), new int(2), new int(3), new int(1));
    g1->addWidget(button_cancel2, new int(1), new int(3), new int(3), new int(1));
    g1->addWidget(button_ok2, new int(4), new int(0), new int(1), new int(3));
    g1->addWidget(button_cut2, new int(0), new int(4), new int(3), new int(1));
    g1->addWidget(button_cut3, new int(4), new int(4), new int(1), new int(1));
    // redef
    g1->addWidget(button_cancel2, new int(0), new int(0), new int(1), new int(1));
    g1->addWidget(button_cut, new int(1), new int(3), new int(3), new int(1));
    g1->run_all();

    //Toplevel2
    Toplevel* toplevel_ = new Toplevel(app->root_widget());
    toplevel_->configure(&window_size, &window_color, &window_border_width, &window_title_, &closable, &window_resizable, NULL);

    Button* button_ok_ = new Button(toplevel_);
    button_ok_->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_ok, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_ok2_ = new Button(toplevel_);
    button_ok2_->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_ok, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cancel_ = new Button(toplevel_);
    button_cancel_->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cancel, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cancel2_ = new Button(toplevel_);
    button_cancel2_->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cancel, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cut_ = new Button(toplevel_);
    button_cut_->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cut, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cut2_ = new Button(toplevel_);
    button_cut2_->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cut, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Button* button_cut3_ = new Button(toplevel_);
    button_cut3_->configure (&button_size, &button_color, &button_border_width, NULL, &relief, &button_title_cut, NULL, &text_color, &text_anchor, NULL, NULL, NULL);

    Placer* p1_ = new Placer();
    p1_->configure(toplevel_, NULL, &(window_position_.x()), &(window_position_.y()), NULL, NULL, NULL, NULL, NULL, NULL);
    Griddeur* g1_ = new Griddeur(toplevel_, new int(50), new int(50), EI_FALSE);
    g1_->addWidget(button_cut_, new int(0), new int(0), new int(1), new int(1));
    g1_->addWidget(button_ok_, new int(1), new int(1), new int(3), new int(1));
    g1_->addWidget(button_cancel_, new int(1), new int(2), new int(3), new int(1));
    g1_->addWidget(button_cancel2_, new int(1), new int(3), new int(3), new int(1));
    g1_->addWidget(button_ok2_, new int(4), new int(0), new int(1), new int(3));
    g1_->addWidget(button_cut2_, new int(0), new int(4), new int(3), new int(1));
    g1_->addWidget(button_cut3_, new int(4), new int(4), new int(1), new int(1));
    // redef
    g1_->addWidget(button_cancel2_, new int(0), new int(0), new int(1), new int(1));
    g1_->addWidget(button_cut_, new int(1), new int(3), new int(3), new int(1));
    g1_->run_all();

    EventManager::getInstance().bind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().bind(ei_ev_display, NULL, "all", process_display, app);

    app->run();

    EventManager::getInstance().unbind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().unbind(ei_ev_display, NULL, "all", process_display, app);

    delete app;
    delete p1;
    delete g1;
    delete g1_;

    return (EXIT_SUCCESS);
}
