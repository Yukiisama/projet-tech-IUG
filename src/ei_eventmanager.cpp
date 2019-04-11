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
     * Bind tags :
     *  if tag = all, then push root and all it's children to hashmap[eventtype]
     *  if tag != all then checkthe tag_list of all the widget with the param tag
     *  if tag belong to tag_list then bind that widget to hashmap.
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
    //create param_callback structure to bind.
    param_callback _callback;

    //Bind with widget (tag must be empty)
    if (widget && tag.empty())
    {
        _callback.widget = widget;
        _callback.callback = callback;
        _callback.user_param = user_param;
        //Add eventtype as key, if it's not in hashMap.
        if (hashMap.find(eventtype) == hashMap.end()) hashMap[eventtype] = std::vector<param_callback>();
        //Add _callback to the vec where it's key is eventtype
        hashMap[eventtype].push_back(_callback);

    }

    //Bind with tag (widget must be NULL)
    else if (!widget && !tag.empty())
    {
        //Use to store all widgets
        std::stack<Widget *> w_stack;
        //Push root to the head of the stack
        w_stack.push(Application::getInstance()->root_widget());
        while (!w_stack.empty()) {
            //Store the head of the stack to widget
            Widget* widget1=w_stack.top();
            //Remove the head of the stack
            w_stack.pop();
            //Case all
            if(!tag.compare("all")){
                //Add eventtype as key, if it's not in hashMap.
                if(hashMap.find(eventtype)==hashMap.end()) hashMap[eventtype]=std::vector<param_callback>();
                _callback.widget=widget1;
                _callback.callback = callback;
                _callback.user_param = user_param;
                 //Add _callback to the vec where it's key is eventtype
                hashMap[eventtype].push_back(_callback);
            }else{
                //Bind the tag that is type of class(Frame,Button and Toplevel) or custom tags.
                std::list<tag_t>tag_list = widget1->getTag_list();
                for(std::list<tag_t>::iterator it = tag_list.begin(); it!=tag_list.end();++it){
                    if(!tag.compare(*it)){
                        if(hashMap.find(eventtype)==hashMap.end()) hashMap[eventtype]=std::vector<param_callback>();
                        _callback.widget=widget1;
                        _callback.callback = callback;
                        _callback.user_param = user_param;
                         //Add _callback to the vec where it's key is eventtype
                        hashMap[eventtype].push_back(_callback);
                    }
                }
            }
            //Add children to stack
            list<Widget *> w_child = widget1->getChildren();
            for (list<Widget *>::iterator it = w_child.begin();it!=w_child.end();++it)
                w_stack.push(*it);


        }

    }

}

/**
     * \brief	Unbinds a callback from an event type and widget or tag.
     * Unbind tag :
     *   if tag = all then unbind the whole hashmap
     *   else compare tag with tag_list of the the widget in the hashmap and compare other params,
     *   if equal then unbind
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
    //Unbind with tags
    if (!widget && !tag.empty())
    {
        //Unbind only if eventtype is in the hashmap
        if (hashMap.find(eventtype) != hashMap.end())
        {
            //Case tag equals to all, then unbind all widget for this particulier eventtype
            if(!tag.compare("all")){
                for (std::vector<param_callback>::iterator it = hashMap[eventtype].begin(); it != hashMap[eventtype].end();)
                {
                    if ( it->user_param == user_param
                         && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
                        it = hashMap[eventtype].erase(it);
                    else ++it;
                }
            }else{
                //Unbind the param_callback that has the parameters as in unbind fonction.
                for (std::vector<param_callback>::iterator it = hashMap[eventtype].begin(); it != hashMap[eventtype].end();)
                {
                    if ( it->user_param == user_param
                         && it->callback.target<bool_t(Widget *, Event *, void *)>() == callback.target<bool_t(Widget *, Event *, void *)>())
                    {
                        //Get tag_list and delete tag that differs with it_tag
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
/**
     * \brief	Handle the events and add invalid rects into the singleton Application
     * @param   event the event that we need to handle : call the callback associated to the event
     *
**/
void EventManager::eventHandler(Event *event)
{

    if( event->type == ei_ev_keydown ){
        KeyEvent * ev_key= static_cast<KeyEvent*>  (event);
        if(ev_key->key_sym == ALLEGRO_KEY_Q){
            Application::getInstance()->quit_request();
        }

    }

    //Iterate and callback the associated function to the event
    for (std::vector<param_callback>::iterator it = hashMap[event->type].begin(); it != hashMap[event->type].end(); ++it)
    {
        if (it->widget)
        {
            if(event->type==ei_ev_mouse_buttondown){
                MouseEvent * me= static_cast<MouseEvent*>(event);
                uint32_t mouse_id =  Application::getInstance()->widget_pick(me->where)->getPick_id();
                if(!it->widget->getName().compare("Toplevel")){
                    Toplevel * top= static_cast<Toplevel*>(it->widget);
                    if(mouse_id == top->getButton_close()->getPick_id()
                            || mouse_id == top->getResize_button()->getPick_id()
                            || mouse_id == top->getPick_id()){
                        if(it->callback(it->widget, event, it->user_param)){
                            Application::getInstance()->invalidate_rect((it->widget->getScreen_location()));
                            break;
                        }
                    }
                }
                else if(mouse_id==it->widget->getPick_id()){
                    if(it->callback(it->widget, event, it->user_param)){
                        Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                        break;
                    }
                }
                //use for update button sunken, because button click down callback return false.
                if(Application::getInstance()->widget_pick(me->where)->getPick_id()==it->widget->getPick_id()
                        && !it->widget->getName().compare("Button")){
                    Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                }
            }else if(event->type==ei_ev_mouse_buttonup && !it->widget->getName().compare("Button") ){
                Button* button = static_cast<Button*>(it->widget);
                //use for update button raised, because button click up callback return false.
                if(button->get_relief()==ei_relief_sunken){
                    button->set_relief(ei_relief_raised);
                    Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                }
                if(!button->getParent()->getName().compare("Toplevel")){
                    Toplevel* toplevel = static_cast<Toplevel*>(button->getParent());
                    toplevel->setResize_button_pressed(EI_FALSE);
                }
                MouseEvent * me= static_cast<MouseEvent*>(event);

                if(Application::getInstance()->inside_root(me->where) && Application::getInstance()->widget_pick(me->where)->getPick_id()==it->widget->getPick_id()){
                    if(it->callback(it->widget, event, it->user_param)){
                        Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                        break;
                    }
                }

            }else{
                if(it->callback(it->widget, event, it->user_param)){
                    if(!it->widget->getName().compare("Toplevel"))
                        Application::getInstance()->invalidate_rect((it->widget->getScreen_location()));
                    else
                        Application::getInstance()->invalidate_rect((*it->widget->getContent_rect()));
                    break;
                }
            }
        }
    }
}

void EventManager::deleteWidget(Widget* widget){
    if(widget){
        int i;
        for( i= 0;i<=ei_ev_last;++i){
            ei_eventtype_t in= (ei_eventtype_t)i;
            std::vector<param_callback> &p_list = hashMap[in];
            for(std::vector<param_callback>::iterator itt = p_list.begin(); itt != p_list.end();){
                if(itt->widget->getPick_id() == widget->getPick_id()){
                    itt = p_list.erase(itt);
                }
                else ++itt;
            }
        }
    }

}

void EventManager::totalCallback()
{
    int count =0;
    for(std::unordered_map<ei_eventtype_t,std::vector<param_callback>>::iterator it = hashMap.begin(); it!= hashMap.end(); ++it){

        std::vector<param_callback> p_list = it->second;
        for(std::vector<param_callback>::iterator itt = p_list.begin(); itt != p_list.end();++itt){
            count++;
        }
    }
    cout<<count<<endl;
}

} // namespace ei
