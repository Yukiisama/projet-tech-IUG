#include "ei_event.h"
#include "ei_eventmanager.h"
#include "ei_application.h"
#include <iostream>
#include <stdio.h>
namespace ei
{
EventManager::EventManager() {}
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

void EventManager::bind(ei_eventtype_t eventtype,
                        Widget *widget,
                        tag_t tag,
                        ei_callback_t callback,
                        void *user_param)
{

    if ((widget && !tag.empty()) || (!widget && tag.empty()))
    {
        fprintf(stderr, "Please check widget and tag\n");
        return;
    }
    param_callback _callback; //creat param_callback structure to bind.

    //Bind with widget
    if (widget && tag.empty())
    {
        _callback.widget = widget;
    }
    //Bind with tag
    else if (!widget && !tag.empty())
    {
        _callback.tag = tag;
        _callback.widget = NULL;
    }
    _callback.callback = callback;
    _callback.user_param = user_param;

    //Add eventtype as key, if it's not in hashMap.
    if (hashMap.find(eventtype) == hashMap.end())
    {
        hashMap[eventtype] = std::vector<param_callback>();
        hashMap[eventtype].push_back(_callback); //add _callback to the vecot where it's key is eventtype.
    }
    else
    {                                            //evettype already existe as key in hashMap.
        hashMap[eventtype].push_back(_callback); //add _callback to the vecot where it's key is eventtype.
    }
}

/**
     * \brief	Unbinds a callback from an event type and widget or tag.
     *
     * @param	eventtype, widget, tag, callback, user_param
     *			All parameters must have the same value as when
     *          \ref ei::EventManager::bind was called to create the binding.
     */
void EventManager::unbind(ei_eventtype_t eventtype,
                          Widget *widget,
                          tag_t tag,
                          ei_callback_t callback,
                          void *user_param)
{

    //unbind with tags
    if (!widget && !tag.empty())
    {
        if (hashMap.find(eventtype) != hashMap.end())
        {
            for (std::vector<param_callback>::iterator it = hashMap[eventtype].begin(); it != hashMap[eventtype].end();)
            {
                // if exist delete where all the paramaters have the same value
                if (!it->tag.compare(tag) && it->user_param == user_param 
                && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
                {
                    it = hashMap[eventtype].erase(it);
                }
                else
                {
                    ++it;
                }
            }
        }
    }

    //unbind with widget
    if (widget != nullptr && tag.empty())
    {
        for (std::vector<param_callback>::iterator it = hashMap[eventtype].begin(); it != hashMap[eventtype].end();)
        {
            if (it->widget->getPick_id() == widget->getPick_id() && it->user_param == user_param 
            && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
            {
                it = hashMap[eventtype].erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void EventManager::eventHandler(Event *event)
{

    //Event * event = hw_event_wait_next();
    //Handle with mouse type of event.
    if (event->type == ei_ev_mouse_buttondown || event->type == ei_ev_mouse_buttonup || event->type == ei_ev_mouse_move)
    {
        //Casting Event to MouseEvent
        MouseEvent *M = static_cast<MouseEvent *>(event);
        Widget *w = Application::getInstance()->widget_pick(M->where);
        //std::cout<< M->where.x()<<std::endl;
        if (w)
        {
            for (std::vector<param_callback>::iterator it = hashMap[event->type].begin(); it != hashMap[event->type].end(); ++it)
            {
                if (it->widget)
                {
                    if (it->widget->getPick_id() == w->getPick_id())
                    {
                        if (event->type == ei_ev_mouse_buttondown)
                        {
                            static_cast<Button *>(it->widget)->clicked = EI_TRUE;
                            Application::getInstance()->invalidate_rect(it->widget->getContent_rect());
                            it->callback(it->widget, event, it->user_param);
                            break;
                        }
                        if (event->type == ei_ev_mouse_buttonup)
                        {
                            static_cast<Button *>(it->widget)->clicked = EI_FALSE;
                            Application::getInstance()->invalidate_rect(it->widget->getContent_rect());
                            it->callback(it->widget, event, it->user_param);
                            break;
                        }
                    }
                }
                else
                {
                    std::list<Widget *> w_geo = Application::getInstance()->root_widget()->getChildren();
                    for (std::list<Widget *>::iterator it2 = w_geo.begin(); it2 != w_geo.end(); ++it2)
                    {
                        if ((*it2)->getName() == (it->tag) || (it->tag) == "all")
                        {
                            if (event->type == ei_ev_mouse_buttondown)
                            {
                                static_cast<Button *>(*it2)->clicked = EI_TRUE;
                                Application::getInstance()->invalidate_rect((*it2)->getContent_rect());
                                it->callback((*it2), event, it->user_param);
                                break;
                            }
                            if (event->type == ei_ev_mouse_buttonup)
                            {
                                static_cast<Button *>(*it2)->clicked = EI_FALSE;
                                Application::getInstance()->invalidate_rect((*it2)->getContent_rect());
                                it->callback((*it2), event, it->user_param);
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        //other types of events
        for (std::vector<param_callback>::iterator it = hashMap[event->type].begin(); it != hashMap[event->type].end(); ++it)
        {
            if (it->widget)
            {
                Application::getInstance()->invalidate_rect(it->widget->getContent_rect());
                it->callback(it->widget, event, it->user_param);
            }
            else
            {
                std::list<Widget *> w_geo = Application::getInstance()->root_widget()->getChildren();
                for (std::list<Widget *>::iterator it2 = w_geo.begin(); it2 != w_geo.end(); ++it2)
                {
                    if ((*it2)->getName() == (it->tag) || (it->tag) == "all")
                    {
                        Application::getInstance()->invalidate_rect((*it2)->getContent_rect());
                        it->callback((*it2), event, it->user_param);
                    }
                }
            }
        }
    }
}

} // namespace ei
