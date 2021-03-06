/**
 *  @file ei_eventmanager.h
 *  @brief  Manages event callbacks.
 *
 *  \author
 *  Created by François Bérard on 18.12.11.
 *  Modified by Pierre Bénard and Aurélie Bugeau
 *
 */

#ifndef EI_EVENTMANAGER_H
#define EI_EVENTMANAGER_H

#include "ei_event.h"
#include <unordered_map>
namespace ei {

/**
 * \brief Class that represents the event manager.
 */
class EventManager
{
public:
    /**
     * @return the singleton instance
     */
    static EventManager& getInstance() {
        static EventManager instance;
        return instance;
    }      
  private:
    EventManager();
    //param_callback : contain essentail datas for callback function.
    typedef struct param_callback
    {
        /* data */
        Widget *widget;
        ei_callback_t callback;
        void *user_param;
        bool_t exc_on_widget=EI_TRUE; //default value is EI_TRUE, callback can excute only on widget.
    } param_callback;

    /**
     * @brief hashMap key will be of ei_eventtype_t type and mapped value will be of std::vector<param_callback> type
     * Used by eventHandler to excute callbacks according to ei_eventtype.
     *
     */
    std::unordered_map<ei_eventtype_t,std::vector<param_callback>, std::hash<int>> hashMap;
   

public:
    EventManager(EventManager const&)    = delete;
    void operator=(EventManager const&)  = delete;
    /**
     * \brief	Binds a callback to an event type and a widget or a tag.
     *
     * @param	eventtype	The type of the event.
     * @param	widget		The callback is only called if the event is related to this widget.
     *                      This parameter must be NULL if the "tag" parameter is not NULL.
     * @param	tag		The callback is only called if the event is related to a widget that
     *                  has this tag. A tag can be a widget class name, or the tag "all".
     *      			This parameter must be NULL is the "widget" parameter is not NULL.
     * @param	callback	The callback (i.e. the function to call).
     * @param	user_param	A user parameter that will be passed to the callback when it is called.
     */
    void bind (ei_eventtype_t eventtype,
               Widget*        widget,
               tag_t          tag,
               ei_callback_t  callback,
               void*          user_param);

    /**
     * \brief	Unbinds a callback from an event type and widget or tag.
     *
     * @param	eventtype, widget, tag, callback, user_param
     *			All parameters must have the same value as when
     *          \ref ei::EventManager::bind was called to create the binding.
     */
    void unbind (ei_eventtype_t eventtype,
                 Widget*        widget,
                 tag_t          tag,
                 ei_callback_t  callback,
                 void*          user_param);
    /**
     * \brief   Excute callbacks according to ei_eventtype.
     * 
     * @param   eventtype   The type of the event.
     * @param   widget      The callback is only called if the event is related to this widget.    
     */
    void eventHandler(Event *event);


    /**
     * @brief setExc_Outside_Widget allow callback to excute out the widget.
     * @param eventtype     The type of the event.
     * @param widget        The callback is only called if the event is related to this widget.
     * @param callback      The callback (i.e. the function to call) to be modify.
     * @param user_param    A user parameter that will be passed to the callback when it is called.
     */
    void setExc_Outside_Widget(ei_eventtype_t eventtype,Widget* widget,ei_callback_t callback,void * user_param);



};

}

#endif // EI_EVENTMANAGER_H
