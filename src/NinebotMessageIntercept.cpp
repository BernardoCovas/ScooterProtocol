#include <stdio.h>
#include <memory.h>

#include "NinebotMessageIntercept.hpp"


NinebotMessageIntercept::NinebotMessageIntercept(
    NinebotMessageStream *activeStream, NinebotDeviceAddr_t device)
{
    _deviceAddr = device;
    _activeStream = activeStream;
}

uint8_t NinebotMessageIntercept::readNextByte(void)
{
    int _data;
    while(1){
        _data = _activeStream->read();
        if (_data >= 0) return (uint8_t) _data;
    }
}

NinebotMessage NinebotMessageIntercept::awaitMessage(void)
{
    uint8_t _data;
    uint8_t _NinebotHeader[] = NINEBOT_PACKET_HEADER;

    uint8_t headerBuf[NINEBOT_MESSAGE_HEADER_LEN];
    NinebotMessageHeader_t *msgHeader = NULL;

    while(1){

        while(1)
        {
            _data = readNextByte();
            loopStart:
                if (_data != _NinebotHeader[0])
                    continue;
                _data = readNextByte();
                if (_data != _NinebotHeader[1])
                    goto loopStart;
                break;
        }

        for (int i=0; i<NINEBOT_MESSAGE_HEADER_LEN; i++)
            headerBuf[i] = readNextByte();

        msgHeader = (NinebotMessageHeader_t*) headerBuf;

        if ( _deviceAddr != NINEBOT_ADDR_ANY && msgHeader->recv != _deviceAddr )
            /* We don't care. */
            continue;

        int transferLen = NINEBOT_MESSAGE_HEADER_LEN + msgHeader->payloadLen;
        uint8_t message[transferLen];

        memcpy(message, headerBuf, NINEBOT_MESSAGE_HEADER_LEN);
        for (int i=NINEBOT_MESSAGE_HEADER_LEN; i<transferLen; i++)
            message[i] = readNextByte();

        uint8_t ck0 = readNextByte();
        uint8_t ck1 = readNextByte();

        NinebotMessage NinebotMessage(message);
        NinebotMessageCkSum_t ck = NinebotMessage.getCkSum();

        if (ck.ck0 == ck0 && ck.ck1 == ck1)
        {
            return NinebotMessage;
        }
        else
        {
            /* packet lost */;
        }
    }
}