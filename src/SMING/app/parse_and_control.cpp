#include <app.h>

/**
 * @file
 * @brief All your parsing and control related stuff
 */

void switchcase_set(COMMAND command, int value){
    switch(command){
        case ON:
            PRINTF_INFO("##### ON\n");
            if(value & SWITCH_1)
                digitalWrite(R, true);
            if(value & SWITCH_2)
                digitalWrite(G, true);
            if(value & SWITCH_3)
                digitalWrite(B, true);
            break;
        case OFF:
            PRINTF_INFO("#### OFF\n");
            if(value & SWITCH_1)
                digitalWrite(R, false);
            if(value & SWITCH_2)
                digitalWrite(G, false);
            if(value & SWITCH_3)
                digitalWrite(B, false);
            break;
        default:
            PRINTF_DBG("#### INVALID\n");
    }
}

void json_control(JsonObject& object){
    for(JsonObject::iterator it = object.begin(); it != object.end(); ++it)
    {
        String key = it->key;

        if(key == "ON"){
            PRINTF_INFO("@@@@@ ON\n");
            switchcase_set(ON, it->value);
        }
        else if(key == "OFF"){
            PRINTF_INFO("@@@@@ OFF\n");
            switchcase_set(OFF, it->value);
        }
        else
            PRINTF_INFO("@@@@@ INVALID KEY\n");
    }
}

void json_parse(String message){
    char switch_message[100] = {0};
    sprintf(switch_message, "%s", message.c_str());

    PRINTF_DBG("@@@ Json message -> %s @@@\n", message.c_str());

    StaticJsonBuffer<200> json_buff;
    JsonObject& root = json_buff.parseObject(switch_message);

    if(!root.success()){
        PRINTF_ERR("@@@ Parsing failed @@@\n");
        return;
    }

    //add validation for this when containsKey() method is working
    JsonObject& control = root["control"];
}
