#pragma once

#include "NinebotMessageDefinitions.hpp"
#include "NinebotMessageIntercept.hpp"


class NinebotControllerEmulator
{
    private:
        NinebotMessageStream *_activeStream;
        void _sendBuff(std::vector<uint8_t> buf);
    public:
        NinebotControllerEmulator(NinebotMessageStream *stream);

        NinebotMessageIntercept msgIntercept;
        void broadcastStatusCmd(NinebotBleBroadcastCommand_t *cmd);
        void broadcastStatusCmdAns(NinebotBleBroadcastCommandAnsw_t *cmd);
};

class ScooterInterface
{
    protected:
        ScooterInterface(NinebotMessageStream *activeStream);

        NinebotMessageStream *_activeStream;
        NinebotMessageIntercept _msgIntercept;
        void sendMsg(NinebotMessageHeader_t *header, uint8_t *payload);

};

class ScooterBmsInterface : public ScooterInterface
{
    private:
        NinebotMessageHeader_t _headerNew(uint8_t regAddr);
        int _bmsGetReg(uint8_t reg);
    public:
        ScooterBmsInterface(NinebotMessageStream* activeStream);

        int getBatteryCurrent(void);
        int getBatteryLevel(void);
        int getCellBatteryLevel(uint8_t idx);
};
