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
    Size top_size = Size(300,300);
    const char*  window_title    = "Window";
    color_t      window_color    = {0xA0,0xA0,0xA0, 0xff};
    int          window_border_width    = 3;
    bool_t       closable        = EI_TRUE;
    axis_set_t   window_resizable = ei_axis_both;
    Point        window_position(30, 250);
    /* Create the application and change the color of the background. */
    Application* app = new Application(&screen_size);
    app->root_widget()->configure(&screen_size, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Create and configure the containers */
    Size containerSize = Size(300, 600);
    Frame* first_Frame_Container = new Frame(app->root_widget());
    first_Frame_Container->configure(&containerSize, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    //Frame* second_Frame_Container = new Frame(app->root_widget());
    //second_Frame_Container->configure(&containerSize, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    Toplevel* toplevel = new Toplevel(app->root_widget());
    toplevel->configure(&top_size, &window_color, &window_border_width, &window_title, &closable, &window_resizable, NULL);
    int xFirstContainer=0, yFirstContainer=0;
    int xSecondContainer=300, ySecondContainer=0;

    Placer* first_Frame_ContainerPlacer = new Placer();
    first_Frame_ContainerPlacer->configure(first_Frame_Container, NULL, &xFirstContainer, &yFirstContainer, NULL, NULL, NULL, NULL, NULL, NULL);
    //Placer* second_Frame_ContainerPlacer = new Placer();
    //second_Frame_ContainerPlacer->configure(second_Frame_Container, NULL, &xSecondContainer, &ySecondContainer, NULL, NULL, NULL, NULL, NULL, NULL);
    Placer* p5 = new Placer();
    p5->configure(toplevel, NULL, &(window_position.x()), &(window_position.y()), NULL, NULL, NULL, NULL, NULL, NULL);
    /* Create and configure the first radiobutton's group */
    const char *firstText = "1st radio button (group 1)", *secondText = "2nd radio button (group 1)", *thirdText = "3rd radio button (group 1)";
    RadioButton* first = new RadioButton(first_Frame_Container);
    RadioButton* second = new RadioButton(first_Frame_Container);
    RadioButton* third = new RadioButton(first_Frame_Container);
    Size radioButtonSize(30, 30);


    first->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &firstText, NULL, NULL, NULL, NULL, NULL, NULL);
    second->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &secondText, NULL, NULL, NULL, NULL, NULL, NULL);
    third->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &thirdText, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Place the first radiobutton's group on screen. */
    int firstY=50, secondY=100, thirdY=150;
    float width=30, height=30;
    Placer* p_first = new Placer();
    p_first->configure(first, NULL, NULL, &firstY, &width, &height, NULL, NULL, NULL, NULL);
    Placer* p_second = new Placer();
    p_second->configure(second, NULL, NULL, &secondY, &width, &height, NULL, NULL, NULL, NULL);
    Placer* p_third = new Placer();
    p_third->configure(third, NULL, NULL, &thirdY, &width, &height, NULL, NULL, NULL, NULL);

    /* Create and configure the second radiobutton's group */
    const char *first2Text = "1st radio button (group 2)", *second2Text = "2nd radio button (group 2)";
    RadioButton* first_group2 = new RadioButton(toplevel);
    RadioButton* second_group2 = new RadioButton(toplevel);
    first_group2->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &first2Text, NULL, NULL, NULL, NULL, NULL, NULL);
    second_group2->configure(&radioButtonSize, NULL, NULL,NULL,NULL, &second2Text, NULL, NULL, NULL, NULL, NULL, NULL);

    /* Place the second radiobutton's group on screen. */
    int firstY2=50, secondY2=100, thirdY2=150;
    Placer* p_first2 = new Placer();
    p_first2->configure(first_group2, NULL, NULL, &firstY2, &width, &height, NULL, NULL, NULL, NULL);
    Placer* p_second2 = new Placer();
    p_second2->configure(second_group2, NULL, NULL, &secondY2, &width, &height, NULL, NULL, NULL, NULL);

    /* Hook the keypress and close button callbacks to the event. */
    EventManager::getInstance().bind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().bind(ei_ev_display, NULL, "all", process_display, app);

    /* Run the application's main loop. */
    app->run();

    /* We just exited from the main loop. Terminate the application (cleanup). */
    EventManager::getInstance().unbind(ei_ev_keydown, NULL, "all", process_key, app);
    EventManager::getInstance().unbind(ei_ev_display, NULL, "all", process_display, app);

    delete app;
    delete p_first2;
    delete p_second2;
    delete p_first;
    delete p_second;
    delete p_third;
    return (EXIT_SUCCESS);
}
