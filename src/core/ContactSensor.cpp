#include "ContactSensor.h"
#include "InputBool.h"

#include <string.h>
#include "frozen.h"
#include "OmniThing.h"

#include "Logger.h"

namespace omni
{
//private
    void ContactSensor::sendJsonPacket()
    {
        char buffer[100] = "";
        struct json_out out = JSON_OUT_BUF(buffer, sizeof(buffer));

        json_printf(&out, "{uid: %u, type: \"%s\", state: \"%s\"}", getUid(), getType(), (read()?"closed":"open"));

        OmniThing::getInstance().sendJson(buffer);
    }

//protected
//public
    ContactSensor::ContactSensor(InputBool& input, bool invert, bool constantPoll):
        Device(constantPoll),
        m_rInput(input),
        m_bInvert(invert)
    {
         
    }

    ContactSensor::~ContactSensor()
    {

    }

    void ContactSensor::recvJson(const char* cmd, const char* json)
    {
        if(!strcmp(cmd, Cmd_Poll))
        {
            LOG << F("Poll triggered for ") << getType() << F(" ") << getUid() << Logger::endl;
            read();
            sendJsonPacket();
        }

    }

    void ContactSensor::run()
    {
        bool val = read(); 
        if(val != m_bLastVal)
        {
            sendJsonPacket();
        }
        m_bLastVal = val; 
    }

    void ContactSensor::init()
    {
        m_bLastVal = read(); 
        sendJsonPacket();
    }

    bool ContactSensor::read()
    {
        bool val = m_rInput.readBool();

        val = isInverted() ? !val : val;
        return val;
    }

    Device* ContactSensor::createFromJson(const char* json)
    {
        return nullptr;
    }


    const char* ContactSensor::Cmd_Poll = "poll";
    const char* ContactSensor::Type     = "ContactSensor";

    DeviceConfig ContactSensor::DevConf(Type, createFromJson); 
}
