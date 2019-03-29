#include "ei_event.h"
#include "ei_eventmanager.h"
#include "ei_application.h"
#include <iostream>
#include <stdio.h>
#include <stack>
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
/*
 *
 * Bind tags :
 * if tag = all, then push root and all it's children to hashmap[eventtype]
 * if tag != all then checkthe tag_list of all the widget with the param tag
 *  if tag belong to tag_list then bind that widget to hashmap.
 *
 *
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
        _callback.callback = callback;
        _callback.user_param = user_param;
        //Add eventtype as key, if it's not in hashMap.
        if (hashMap.find(eventtype) == hashMap.end()) hashMap[eventtype] = std::vector<param_callback>();
        hashMap[eventtype].push_back(_callback); //add _callback to the vecot where it's key is eventtype.

    }

    //Bind with tag
    else if (!widget && !tag.empty())
    {
       std::stack<Widget *> w_stack;//use to store all widgets
       w_stack.push(Application::getInstance()->root_widget()); //push root to the head of the stack
       while (!w_stack.empty()) {
           Widget* widget1=w_stack.top();//store the head of the stack to widget
           w_stack.pop();//remove the head of the stack
           if(!tag.compare("all")){//case all
               //Add eventtype as key, if it's not in hashMap.
               if(hashMap.find(eventtype)==hashMap.end()) hashMap[eventtype]=std::vector<param_callback>();
               _callback.widget=widget1;
               _callback.callback = callback;
               _callback.user_param = user_param;
               hashMap[eventtype].push_back(_callback);
           }else{//Bind the tag that is type of class(Frame,Button and Toplevel) or custom tags.
                for(std::list<tag_t>::iterator it = widget1->getTag_list().begin(); it!=widget1->getTag_list().end();++it){
                    if(!tag.compare(*it)){
                        if(hashMap.find(eventtype)==hashMap.end()) hashMap[eventtype]=std::vector<param_callback>();
                        _callback.widget=widget1;
                        _callback.callback = callback;
                        _callback.user_param = user_param;
                        hashMap[eventtype].push_back(_callback);
                    }
                }
           }
           //add children to stack
           list<Widget *> w_child = widget1->getChildren();
           for (list<Widget *>::iterator it = w_child.begin();it!=w_child.end();++it){
               w_stack.push(*it);
           }

       }

    }

}

/**
     * \brief	Unbinds a callback from an event type and widget or tag.
     *
     * @param	eventtype, widget, tag, callback, user_param
     *			All parameters must have the same value as when
     *          \ref ei::EventManager::bind was called to create the binding.
     */

/*
 * Unbind tag :
 * if tag = all then unbind the whole hashmap
 * else compare tag with tag_list of the the widget in the hashmap and compare other params,
 *       if equal then unbind
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
        if (hashMap.find(eventtype) != hashMap.end())//unbind only if eventtype is in the hashmap
        {
            //case tag equals to all, then unbind all widget for this particulier eventtype
            if(!tag.compare("all")){
                for (std::vector<param_callback>::iterator it = hashMap[eventtype].begin(); it != hashMap[eventtype].end();)
                {
                    if ( it->user_param == user_param
                    && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
                        it = hashMap[eventtype].erase(it);
                    else ++it;
                }
            }else{//unbind the param_callback that has the parameters as in unbind fonction.
                for (std::vector<param_callback>::iterator it = hashMap[eventtype].begin(); it != hashMap[eventtype].end();)
                {
                    if ( it->user_param == user_param
                    && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
                    {
                        linked_tag_t tag_list = it->widget->getTag_list();
                        for(linked_tag_t::iterator it_tag = tag_list.begin(); it_tag!=tag_list.end();++it_tag){
                            if(!tag.compare(*it_tag))
                                it = hashMap[eventtype].erase(it);
                            else ++it;
                        }

                    }

                }

            }
        }
    }

    //unbind with widget
    if (widget && tag.empty())
    {
        for (std::vector<param_callback>::iterator it = hashMap[eventtype].begin(); it != hashMap[eventtype].end();++it)
        {
            if (it->widget->getPick_id() == widget->getPick_id() && it->user_param == user_param 
            && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
                it = hashMap[eventtype].erase(it);
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
                            if(!(it->widget)->getName().compare("Button")){
                                static_cast<Button *>(it->widget)->set_relief(ei_relief_sunken);
                            }

                            Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                            it->callback(it->widget, event, it->user_param);
                            break;
                        }
                        if (event->type == ei_ev_mouse_buttonup)
                        {
                            if(!(it->widget)->getName().compare("Button")){
                                static_cast<Button *>(it->widget)->set_relief(ei_relief_raised);
                            }

                            Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                            it->callback(it->widget, event, it->user_param);
                            break;
                        }
                    }
                }
            }
        }
    }//other types of events
    else
    {

        for (std::vector<param_callback>::iterator it = hashMap[event->type].begin(); it != hashMap[event->type].end(); ++it)
        {
            if (it->widget)
            {
                Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                it->callback(it->widget, event, it->user_param);
            }
        }
    }
}

} // namespace ei
