#include <app.h>

#define R 12
#define G 13
#define B 14

void switchcase_set(COMMAND command, int value){
    switch(command){
        case ON:
            Serial.println("##### ON");
            if(value & SWITCH_1)
                digitalWrite(R, true);
            if(value & SWITCH_2)
                digitalWrite(G, true);
            if(value & SWITCH_3)
                digitalWrite(B, true);
            break;
        case OFF:
            Serial.println("#### OFF");
            if(value & SWITCH_1)
                digitalWrite(R, false);
            if(value & SWITCH_2)
                digitalWrite(G, false);
            if(value & SWITCH_3)
                digitalWrite(B, false);
            break;
        default:
            Serial.println("#### INVALID");
    }
}

void json_parseAndExecute(String message){
    char switch_message[100] = {0};
    sprintf(switch_message, "%s", message.c_str());

    Serial.printf("Json message -> %s\n", message.c_str());

    StaticJsonBuffer<200> json_buff;
    JsonObject& root = json_buff.parseObject(switch_message);

    if(!root.success()){
        Serial.println("Parsing failed");
        return;
    }

    //add validation for this when containsKey() method is working
    JsonObject& switchboard = root["switchboard"];

    for(JsonObject::iterator it = switchboard.begin(); it != switchboard.end(); ++it)
    {
        String key = it->key;

        if(key == "ON"){
            Serial.println("@@@@@ ON");
            switchcase_set(ON, it->value);
        }
        else if(key == "OFF"){
            Serial.println("@@@@@ OFF");
            switchcase_set(OFF, it->value);
        }
        else
            Serial.println("@@@@@ INVALID KEY");
    }
}
