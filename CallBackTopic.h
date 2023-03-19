#ifndef __CALLBACK_TOPIC_H__
#define __CALLBACK_TOPIC_H__

#include <list>
#include <string>


// The de-multiplexed callbacks this module provides
#define CALLBACK_ARGS const std::string & topic, const std::string & message
#define CALLBACK_SIGNATURE void (callback_t)(CALLBACK_ARGS)
typedef CALLBACK_SIGNATURE;
typedef void (*callback_pt)(CALLBACK_ARGS);


class CallBackTopic {
    public:
        CallBackTopic(const std::string & topic, const callback_pt callback_func_ptr){
            mtopic = std::string(topic);
            mcallback = callback_func_ptr;
        };
        callback_pt getCallBack() {return paused?nullptr:mcallback;};
        const char * getTopic() {return mtopic.c_str();};
        bool paused = false;

    private:
        std::string mtopic;
        callback_pt mcallback;
};


#endif

