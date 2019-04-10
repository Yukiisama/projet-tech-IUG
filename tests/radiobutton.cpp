#include <iostream>
#include <stdlib.h>

#include "hw_interface.h"
#include "ei_main.h"
#include "ei_widget.h"
#include "ei_application.h"
#include "ei_eventmanager.h"
#include "ei_geometrymanager.h"

#include <iostream>

using namespace ei;

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

    /* Create the application and change the color of the background. */
    Application* app = new Application(&screen_size);
    app->root_widget()->configure(&screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Create and configure the containers */
    Size containerSize = Size(300, 600);
    Frame* firstContainer = new Frame(app->root_widget());
    firstContainer->configure(&containerSize, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Frame* secondContainer = new Frame(app->root_widget());
    secondContainer->configure(&containerSize, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    int xFirstContainer=0, yFirstContainer=0, xSecondContainer=300, ySecondContainer=0;
    Placer* firstContainerPlacer = new Placer(); firstContainerPlacer->configure(firstContainer, NULL, &xFirstContainer, &yFirstContainer, NULL, NULL, NULL, NULL, NULL, NULL);
    Placer* secondContainerPlacer = new Placer();secondContainerPlacer->configure(secondContainer, NULL, &xSecondContainer, &ySecondContainer, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Create and configure the first radiobutton's group */
    const char *firstText = "first radio button", *secondText = "second radio button", *thirdText = "third radio button";
    RadioButton* first = new RadioButton(firstContainer);
    RadioButton* second = new RadioButton(firstContainer);
    RadioButton* third = new RadioButton(firstContainer);
    Size radioButtonSize(30, 30);


    first->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &firstText, NULL, NULL, NULL, NULL, NULL, NULL);
    second->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &secondText, NULL, NULL, NULL, NULL, NULL, NULL);
    third->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &thirdText, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Place the first radiobutton's group on screen. */
    int firstY=0, secondY=50, thirdY=100;
    float width=30, height=30;
    Placer* firstPlacer = new Placer();firstPlacer->configure(first, NULL, NULL, &firstY, &width, &height, NULL, NULL, NULL, NULL);
    Placer* secondPlacer = new Placer();secondPlacer->configure(second, NULL, NULL, &secondY, &width, &height, NULL, NULL, NULL, NULL);
    Placer* thirdPlacer = new Placer();thirdPlacer->configure(third, NULL, NULL, &thirdY, &width, &height, NULL, NULL, NULL, NULL);

    /* Create and configure the second radiobutton's group */
    const char *first2Text = "fourth button", *second2Text = "fifth radio button";
    RadioButton* first2 = new RadioButton(secondContainer);
    RadioButton* second2 = new RadioButton(secondContainer);
    first2->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &first2Text, NULL, NULL, NULL, NULL, NULL, NULL);
    second2->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &second2Text, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Place the second radiobutton's group on screen. */
    int firstY2=0, secondY2=50, thirdY2=100;
    Placer* firstPlacer2 = new Placer();firstPlacer2->configure(first2, NULL, NULL, &firstY2, &width, &height, NULL, NULL, NULL, NULL);
    Placer* secondPlacer2 = new Placer();secondPlacer2->configure(second2, NULL, NULL, &secondY2, &width, &height, NULL, NULL, NULL, NULL);

    /* Hook the keypress and close button callbacks to the event. */
    EventManager::getInstance().bind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().bind(ei_ev_display, NULL, "all", process_display, app);

    /* Run the application's main loop. */
    app->run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    EventManager::getInstance().unbind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().unbind(ei_ev_display, NULL, "all", process_display, app);

    delete app;

    return (EXIT_SUCCESS);
}
