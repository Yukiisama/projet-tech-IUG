#include "ei_event.h"
#include "ei_eventmanager.h"
#include "ei_application.h"
#include <iostream>
namespace ei {
    EventManager::EventManager(){}
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
               Widget*        widget,
               tag_t          tag,
               ei_callback_t  callback,
               void*          user_param){

        if ((widget != nullptr && !tag.empty()) || (widget == nullptr && tag.empty()))
            return;

        param_callback _callback; //creat param_callback structure to bind.
        _callback.eventtype=eventtype;
        
        //Bind with widget
        if (widget != nullptr && tag.empty())
        {
            _callback.widget = widget;
            _callback.tag = ""; //can't assign null to std::string type
        }
        //Bind with tag
        else if(widget ==nullptr && !tag.empty())
        {
            _callback.tag=tag;
            _callback.widget= widget;
        }
        _callback.callback= callback;
        _callback.user_param = user_param;
        //add to the current vector the new callback
        vec_callback.push_back(_callback);
    }

    /**
     * \brief	Unbinds a callback from an event type and widget or tag.
     *
     * @param	eventtype, widget, tag, callback, user_param
     *			All parameters must have the same value as when
     *          \ref ei::EventManager::bind was called to create the binding.
     */
    void EventManager::unbind(ei_eventtype_t eventtype,
                 Widget*        widget,
                 tag_t          tag,
                 ei_callback_t  callback,
                 void*          user_param){
        //run through vector of  callback event and seek if call back exists
        //unbind with tags
        if(widget ==nullptr && !tag.empty()){
        for (std::vector<param_callback>::iterator it = vec_callback.begin(); it != vec_callback.end();)
        {
            // if exist delete where all the paramaters have the same value
            if (!it->tag.compare(tag) 
            && it->user_param == user_param 
            && it->eventtype == eventtype 
            && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
            {
             it=vec_callback.erase(it);   
            }else{
                ++it;
            }
        }
        }

        //unbind with widget
        if(widget!= nullptr && tag.empty()){
            for (std::vector<param_callback>::iterator it = vec_callback.begin(); it != vec_callback.end();)
            {
                if(it->widget->getPick_id()==widget->getPick_id()
                && it->user_param==user_param
                && it->eventtype== eventtype
                && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>()){
                    it = vec_callback.erase(it);
                }else{
                ++it;
                }
            }
        }
    }




    void EventManager::eventHandler(Event *event)
    {
        //Event * event = hw_event_wait_next();
        //Handle with mouse type of event.
        if(event->type==ei_ev_mouse_buttondown || event->type==ei_ev_mouse_buttonup 
            ||event->type==ei_ev_mouse_move ){
            //Casting Event to MouseEvent
            MouseEvent* M = static_cast<MouseEvent*>(event);
            Widget * w = Application::getInstance()->widget_pick(M->where);
            //std::cout<< M->where.x()<<std::endl;
            if(w){
                for(std::vector<param_callback>::iterator it =vec_callback.begin(); it !=vec_callback.end();++it){
                    if(it->widget){
                        if (it->widget->getPick_id() == w->getPick_id())
                        {
                            it->callback(it->widget,event,it->user_param);
                        }
                    }
                   
                }
                //std::cout << "w ok" << std::endl;
                //w->getPick_id();
                
            }
    
    
    
    
    
    
    
    }
}


}