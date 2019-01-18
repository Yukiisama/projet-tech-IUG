/**
 *  @file ei_event.h
 *  @brief  Allows the binding and unbinding of callbacks to events.
 *
 *  \author
 *  Created by François Bérard on 30.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 *
 */
#ifndef EI_EVENT_H
#define EI_EVENT_H

#include "ei_types.h"
#include "ei_widget.h"

#include <allegro5/keycodes.h>

namespace ei {

/**
 * @brief The types of events.
 */
typedef enum {
    ei_ev_none = 0,  ///< No event, used on an un-initialized structure.
    ei_ev_app,       ///< An application event, created by \ref hw_event_post_app.
    ei_ev_display,   ///< A display / window event.
    ei_ev_keydown,   ///< A keyboard key has been pressed.
    ei_ev_keyup,     ///< A keyboard key has been released.
    ei_ev_keychar,   ///< A character was typed on the keyboard.

    /* event types at or below "ei_ev_mouse_buttondown" are mouse events, need picking. */
    ei_ev_mouse_buttondown,  ///< A mouse button has been pressed.
    ei_ev_mouse_buttonup,    ///< A mouse button has been released.
    ei_ev_mouse_move,        ///< The mouse has moved.

    /* event types at or below "ei_ev_touch_begin" are touch events, need picking. */
    ei_ev_touch_begin,  ///< The touch input device registered a new touch.
    ei_ev_touch_end,    ///< A touch ended.
    ei_ev_touch_move,   ///< The position of a touch changed.

    ei_ev_last  ///< Last event type, its value is the number of event types.
} ei_eventtype_t;

/**
 * @brief Describes an event.
 */
struct Event {
    ei_eventtype_t type;       ///< The type of the event.
};

/**
 * @brief The event parameter for display-related event types.
 */
struct DisplayEvent : public Event {
    bool_t resized;      ///< The window has been resized.
    bool_t closed;       ///< The close button of the window has been pressed.
    bool_t switched_out; ///< The window is no longer active.
    bool_t switched_in;  ///< The window is active once again.
};

/**
 * @brief The modifier keys (shift, alt, etc.)
 */
typedef enum {
    ei_mod_shift = 0x00001,       ///< The "shift" key.
    ei_mod_ctrl = 0x00002,        ///< The "control" key.
    ei_mod_alt = 0x00004,         ///< The "alternate" key at the left of the space bar.
    ei_mod_meta_left = 0x00008,   ///< The "meta" (command) key at the left of the space bar.
    ei_mod_meta_right = 0x00010,  ///< The "meta" (command) key at the right of the space bar.
    ei_mod_alt_grad = 0x00040     ///< The "alternate" key at the right of the space bar.
} ei_modifier_key_t;

/**
 * @brief A bitfield indicating which of the modifier keys are currently pressed.
 */
typedef uint32_t ei_modifier_mask_t;

/**
 * @brief The event parameter for keyboard-related event types.
 */
struct KeyEvent : public Event {
    int key_sym;  ///< The keyboard key symbol (see \ref allegro5/keycodes.h).
    int unichar;  ///< For \ref ei_ev_keychar, a Unicode code point (character).
    ei_modifier_mask_t modifier_mask;  ///< The state of the modifier keys at the time of the event.

    /**
     * @brief Tests is a modifier key is currently pressed, according to a bitfield.
     *
     * @param mask      The bitfield.
     * @param modifier  The modifier key that is tested.
     *
     * @return      EI_TRUE is this modifier key is currently pressed, EI_FALSE otherwise.
     */
    static inline bool_t ei_has_modifier(ei_modifier_mask_t mask,
                                            ei_modifier_key_t modifier)
    {
        return (bool_t)(mask == modifier);
    }
};

/**
 * @brief The event parameter for mouse-related event types.
 */
struct MouseEvent : public Event {
    Point where;                 ///< Where the mouse pointer was at the time of the event.
    unsigned int button_number;  ///< The number of the button that was pressed or released.
                                 ///  Only valid for \ref ei_ev_mouse_buttondown or \ref ei_ev_mouse_buttonup event types.
};

/**
 * @brief The event parameter for mouse-related event types.
 */
struct TouchEvent : public Event {
    Point where;    ///< Where the touch was at the time of the event.
    Point speed;    ///< Movement speed in pixels.
    int touch_id;   ///< An identifier for this touch. If supported by the device
                    ///  it will stay the same for events from the same finger until the touch ends.
    bool_t primary; ///< Whether this is the only/first touch or an additional touch.
};

/**
 * @brief The event parameter for application defined event types.
 */
struct AppEvent : public Event {
    void* user_param;
};

}

#endif
