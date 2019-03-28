#pragma once

#include <memory>

#include "NinebotMessageDefinitions.hpp"
#include "NinebotMessageUtils.hpp"

class NinebotMessageStream
{
    public:
        virtual void write(int) = 0;
        virtual int read(void) = 0;
};

class NinebotMessageIntercept
{
    private:
        NinebotMessageStream *_activeStream;
        NinebotDeviceAddr_t _deviceAddr;
        uint8_t readNextByte();
    public:
        NinebotMessageIntercept(NinebotMessageStream *activeStream, NinebotDeviceAddr_t device);
        NinebotMessage awaitMessage(void);
};