
// Local
#include "debug.h"
#include "defaults.h"

// Published
#include "PubSubClientMulti.h"

PubSubClientMulti::PubSubClientMulti(PubSubClient & _pubSub) : pubSub(_pubSub) {
    DBGln(3, "+ini");
    pubSub.setCallback(mqtt_callback);
    DBGln(3, "-ini");
};


bool PubSubClientMulti::connected() {
    DBGln(3, "connected");
    return pubSub.connected();
}


bool PubSubClientMulti::connect(const String & clientId,
        const String & user, const String & pass,
        const String & willTopic, int willQoS, bool willRetain,
        const String & willMessage) {

    const char * p_user = user.length()?user.c_str():NULL;
    const char * p_pass = pass.length()?pass.c_str():NULL;
    const char * p_willTopic = willTopic.length()?willTopic.c_str():NULL;
    const char * p_willMessage = willMessage.length()?willMessage.c_str():NULL;

    return pubSub.connect(clientId.c_str(), p_user, p_pass, p_willTopic,
        willQoS, willRetain, p_willMessage);
}


// Example or dummy callback
void dummy_cb(CALLBACK_ARGS){
    DBGln(3, "!!Dummy Callback!!");
}

// std::string & -> char * -> pubSub
bool PubSubClientMulti::publish(const std::string & topic, const std::string & message, bool retained) {
    if (! pubSub.connected())return false;
    Serial_println("pub2");
    return pubSub.publish(topic.c_str(), message.c_str(), retained);
}

// String & -> char *
bool PubSubClientMulti::publish(const String & topic, const String & message, bool retained) {
    if (! pubSub.connected())return false;
    Serial_println("pub2");
    return pubSub.publish(topic.c_str(), message.c_str(), retained);
}

// char * -> PubSub
bool PubSubClientMulti::publish(const char * topic, const char * message, bool retained) {
    if (! pubSub.connected())return false;
    Serial_println("pub2");
    return pubSub.publish(topic, message, retained);
}

// String & -> std::string
bool PubSubClientMulti::subscribe(const String & topic, const callback_pt cb_fn) {
    std::string s(topic.c_str());
    Serial_println("sub1");
    return subscribe(s, cb_fn);
}

// char * -> std::string
bool PubSubClientMulti::subscribe(const char * topic, const callback_pt cb_fn) {
    std::string s(topic);
    Serial_println("sub2");
    return subscribe(s, cb_fn);
}

// Check if at least one callback exists for the given topic
// and optionally if that matches an exsting callback
CallBackTopic * PubSubClientMulti::findCallbackTopic(const std::string & topic,
    const callback_pt cb_ptr) {
    
    CallBackTopic * p_cb;

    for (CallBackTopic & cbt : callBackList) {
        if (cbt.getTopic() != topic) continue;
        if (cb_ptr != nullptr && cbt.getCallBack() != cb_ptr) continue;
        return & cbt;
    }
    return nullptr;
}

// std::string & -> char * -> pubSub 
bool PubSubClientMulti::subscribe(const std::string & topic, const callback_pt cb_ptr) {
    DBGln(3, "sub4");
    if (countCallBacks() >= MAX_CALLBACK_LIST_SIZE) return false;
    if (cb_ptr == nullptr) return false;

    //de-duplicate
    //Existing topics with existing callbacks do not 
    //add another subscription to the same topic
    //New callbacks with existing topics are permitted
    //Existing callbacks with new topics are permitted
    CallBackTopic * p_cbt = findCallbackTopic(topic, cb_ptr);
    if (p_cbt == nullptr) {
        DBGf(1,"\nSubscribing to '%s'", topic.c_str());
        CallBackTopic cbt(topic, cb_ptr);
        callBackList.push_back(cbt);
        DBGln(3, "sub+");
        p_cbt = &cbt;
    } else {
        DBGf(1,"\nnot Subscribing to '%s', already subscribed", topic.c_str());
    }
    DBGln(3, "sub-");
    return pubSub.subscribe(p_cbt->getTopic());
}



/*
Unsubscribe from *ALL* subscriptions to a given topic
return:
 false if the unsubscribe failed
 true if you were unsubscribed or you were never subscribed
*/
bool PubSubClientMulti::unsubscribe(const std::string & topic) {
    CallBackTopic * p_cbt = findCallbackTopic(topic);
    
    if (p_cbt == nullptr) {
        DBGf(1,"\nnot un-subscribing from '%s', not subscribed", topic.c_str());
    }

    // Ask the Broker to unsubscribe
    if(!pubSub.unsubscribe(topic.c_str())) return false;

    DBGf(1,"\nun-subscribed from '%s'", topic.c_str());

    // If that worked, delete the records
    callBackList.remove_if(
        [topic](CallBackTopic & cbt) {
        return cbt.getTopic()==topic;
    });

    return true;
}


// Stop responding to messages but don't unsubscribe
// You can suspend a given callback or a given topic or both
// Returns true if anything changed else false
bool PubSubClientMulti::pause_restore(const std::string & topic, const callback_pt callback, bool pause) {
    CallBackTopic * p_cbt;
    bool ret = false;

    if (topic.empty() && callback == nullptr)return ret;

    for (CallBackTopic & cbt : callBackList) {
        p_cbt=&cbt;
        if (callback != nullptr && callback != cbt.getCallBack())continue;
        if (topic != "" && topic != cbt.getTopic())continue;
        ret = true;
    }

    if(ret) {
        if(p_cbt->paused != pause)DBGf(1,"\n%sPausing '%s'", pause?"":"un-", topic.c_str());
        else DBGf(1,"\n'%s', already %sPaused", pause?"":"un-", topic.c_str());
        p_cbt->paused = pause;
    } else {
        DBGf(1,"\nnot Pausing '%s', topic not found", topic.c_str());
    }
    return ret;
}

// call if you think the broker has forgotten you
int PubSubClientMulti::resubscribe() {
    int count = 0;
    DBGln(3, "+res");
    pubSub.setCallback(mqtt_callback);

    for (CallBackTopic & cbt : callBackList) {
        if (pubSub.subscribe(cbt.getTopic())) count++;
    }
    DBGln(3, "-res");
    return count;
}

// Private
void PubSubClientMulti::callback(char * topicChar, uint8_t * payload, unsigned int length) {
    unsigned long len = length;
    const std::string topic_cpy(topicChar);
    DBGln(3, "+cb");
    //Using std::string as it has an assign method and String does not
    //payload is not char so can't use constructors
    std::string message;   // New string with zero allocation;
    message.reserve(len);  // Allocate <length>
    message.assign((const char *)payload, 0, len);  // Fill message
    DBGln(3, "cb0");

    unsigned cnt=1;
    for (CallBackTopic & cbt : callBackList) {
        callback_pt p_func;

        if (MqttWildcard::wildcardMatch(String(topicChar), String(cbt.getTopic()))) {
            Serial_printf("+cb%d\n", cnt);
            p_func = cbt.getCallBack();
            if(p_func == nullptr)return;

            (*p_func)(topic_cpy, message);
            Serial_printf("-cb%d\n", cnt);
            cnt++;
        }
    }
    DBGln(3, "-cb");
}
