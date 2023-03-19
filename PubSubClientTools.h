#ifndef __PubSubClientTools_h__
#define __PubSubClientTools_h__


// Platform
#include <Arduino.h>
#include <WString.h>

// std
#include <functional>

// Libs
#include "PubSubClient.h"
#include "MqttWildcard.h"
#include "CallBackTopic.h"


#if !defined(ESP8266) && !defined(ESP32)
#warning This library was developed for ESP8266 and ESP32 microcontrollers
#endif

void dummy_cb(CALLBACK_ARGS);


// External Interfaces use String for onward compatability
// Internally we use std::string as it makes using standard C++ easer

class PubSubClientTools {
    private:
        PubSubClient & pubSub;

        std::list<CallBackTopic> callBackList = {};

        // Multiplexed callback provided to pubSub
        std::function<void(char * topicChar, uint8_t * payload, unsigned int length)> mqtt_callback = std::bind(&PubSubClientTools::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        void callback(char * topicChar, uint8_t * payload, unsigned int length);

        // Internal lookup - returns nullptr if not found
        CallBackTopic * findCallbackTopic(const std::string & topic,
            const callback_pt cb_ptr=nullptr);

    public:
        PubSubClientTools(PubSubClient & pubSub);

        size_t countCallBacks() { return callBackList.size();};

        bool connected();

        bool connect(const String & clientId, const String & user="",
          const String & pass="", const String & willTopic="",
          int willQoS=0, bool willRetain=0, const String & willMessage="");
        
        bool publish(const String & topic, const String & message="", bool retained=false);
        bool publish(const std::string & topic, const std::string & message="", bool retained=false);
        bool publish(const char * topic, const char * message="", bool retained=false);

        bool subscribe(const String & topic, const callback_pt cb_ptr);
        bool subscribe(const std::string & topic, const callback_pt cb_ptr);
        bool subscribe(const char * topic, const callback_pt cb_ptr);

        bool unsubscribe(const std::string & topic);

        bool pause_restore(const std::string & topic="",
            const callback_pt cb_ptr=nullptr, bool pause=true);

        int resubscribe();
};



#endif
