#include "config.h"

#include "NinebotMessageDefinitions.hpp"
#include "NinebotMessageInterface.hpp"
#include "NinebotMessageUtils.hpp"


/* NinebotControllerEmulator */

NinebotControllerEmulator::NinebotControllerEmulator(
    NinebotMessageStream *stream):
        msgIntercept(stream, NINEBOT_ADDR_BLE)
{
    _activeStream = stream;
};

void NinebotControllerEmulator::_sendBuff(std::vector<uint8_t> buf)
{
    for (int i=0; i<buf.size(); i++)
    {
        _activeStream->write(buf[i]);
    }
}

void NinebotControllerEmulator::broadcastStatusCmd(
    NinebotBleBroadcastCommand_t *cmd)
{
    NinebotMessageHeader_t header = {
        .payloadLen = 5,
        .sndr = NINEBOT_ADDR_BLE,
        .recv = NINEBOT_ADDR_ESC,
        .cmd = NINEBOT_ESC_COMMAND_UPDATE_HEAD_INPUTS_NOANS,
        .cmdArg = 0
    };

    _sendBuff(NinebotMessage(&header, (uint8_t *) cmd).serialize());

}

void NinebotControllerEmulator::broadcastStatusCmdAns(
    NinebotBleBroadcastCommandAnsw_t *cmd)
{
    NinebotMessageHeader_t header = {
        .payloadLen = 7,
        .sndr = NINEBOT_ADDR_BLE,
        .recv = NINEBOT_ADDR_ESC,
        .cmd = NINEBOT_ESC_COMMAND_UPDATE_HEAD_INPUTS,
        .cmdArg = 0
    };

    _sendBuff(NinebotMessage(&header, (uint8_t *) cmd).serialize());
}


/* ScooterInterface */

ScooterInterface::ScooterInterface(NinebotMessageStream *activeStream)
    : _msgIntercept(activeStream, NINEBOT_ADDR_ESC)
{
    _activeStream = activeStream;
}

void ScooterInterface::sendMsg(NinebotMessageHeader_t *hdr, uint8_t *payload)
{
    std::vector<uint8_t> buf = NinebotMessage(hdr, payload).serialize();
    for (int i=0; i<buf.size(); i++)
        _activeStream->write((char) buf[i]);
}


/* ScooterBmsInterface */

ScooterBmsInterface::ScooterBmsInterface(NinebotMessageStream* _activeStream)
    : ScooterInterface(_activeStream) {}

NinebotMessageHeader_t ScooterBmsInterface::_headerNew(uint8_t regAddr)
{
    return (NinebotMessageHeader_t) {
        .payloadLen = 1,
        .sndr = NINEBOT_ADDR_ESC,
        .recv = NINEBOT_ADDR_BMS1,
        .cmd = NINEBOT_ESC_COMMAND_REGREAD,
        .cmdArg = regAddr, /* registery address */
    };
}

int ScooterBmsInterface::_bmsGetReg(uint8_t reg)
{
    NinebotMessageHeader_t header = _headerNew(reg);

    uint8_t payload[] = {0x01}; /* Length of the read */
    sendMsg(&header, payload);

    return _msgIntercept.awaitMessage().getPayload()[0];
}

int ScooterBmsInterface::getBatteryCurrent()
{
    return _bmsGetReg(NINEBOT_BMS_REG_CURRENT);
}

int ScooterBmsInterface::getBatteryLevel()
{
    return _bmsGetReg(NINEBOT_BMS_REG_VOLTAGE);
}

int ScooterBmsInterface::getCellBatteryLevel(uint8_t idx)
{
    return _bmsGetReg(NINEBOT_BMS_REG_VOLTAGE_CELL + idx);
}