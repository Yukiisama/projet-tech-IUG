#include <iostream>
#include <stdlib.h>

#include "hw_interface.h"
#include "ei_main.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_eventmanager.h"
#include "ei_geometrymanager.h"

using namespace ei;

/*
 * button_press --
 *
 *  Callback called when a user clicks on the button.
 */
bool_t button_press(Widget* widget, Event* event, void* user_param)
{
    MouseEvent* e = static_cast<MouseEvent*>(event);
    std::cout << "Click " << e->button_number << std::endl;
    return EI_TRUE;
}

/*
 * process_key --
 *
 *  Callback called when any key is pressed by the user.
 *  Simply looks for the "Escape" key to request the application to quit.
 */
bool_t process_key(Widget* widget, Event* event, void* user_param)
{
    if (static_cast<KeyEvent*>(event)->key_sym == ALLEGRO_KEY_ESCAPE) {
        static_cast<Application*>(user_param)->quit_request();
        return EI_TRUE;
    }

    return EI_FALSE;
}

/*
 * process_display --
 *
 *  Callback called when a display event occur.
 */
bool_t process_display(Widget* widget, Event* event, void* user_param)
{
    if (static_cast<DisplayEvent*>(event)->closed == EI_TRUE) {
        static_cast<Application*>(user_param)->quit_request();
        return EI_TRUE;
    }

    return EI_FALSE;
}

/*
 * ei_main --
 *
 *  Main function of the application.
 */
int ei_main(int argc, char** argv)
{
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
    /* Place the button on screen. */
    Placer* p = new Placer();
    p->configure(button, NULL, &button_x, &button_y, NULL, NULL, NULL, NULL, NULL, NULL );

    /* Bind the button callbacks to the mouse events */
    EventManager::getInstance().bind(ei_ev_mouse_buttondown, button, "", button_press, NULL);
    EventManager::getInstance().bind(ei_ev_mouse_buttonup, button, "", button_press, NULL);

    /* Hook the keypress and close button callbacks to the event. */
    EventManager::getInstance().bind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().bind(ei_ev_display, NULL, "all", process_display, app);

    /* Run the application's main loop. */
    app->run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    EventManager::getInstance().unbind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().unbind(ei_ev_display, NULL, "all", process_display, app);

    delete app;
    delete p;

    return (EXIT_SUCCESS);
}
