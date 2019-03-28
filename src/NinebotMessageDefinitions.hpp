#pragma once

#include <vector>

#include "NinebotMessageUtils.hpp"

#define NINEBOT_SERIAL_DEFAULT_BAUD 115200
#define NINEBOT_PACKET_HEADER {0x5A, 0xA5} // instead of 0x55 0xAA
#define NINEBOT_MESSAGE_HEADER_LEN 5

/* TODO: (bcovas) Unknown values */
typedef enum
{
    NINEBOT_ADDR_ANY,
    NINEBOT_ADDR_ESC = 0x20,
    NINEBOT_ADDR_BLE = 0x21,
    NINEBOT_ADDR_BMS1 = 0x22,
    NINEBOT_ADDR_BMS2 = 0x23,
    NINEBOT_ADDR_APP1 = 0x3D,
    NINEBOT_ADDR_APP2 = 0x3E,
    NINEBOT_ADDR_APP3 = 0x3F
}
NinebotDeviceAddr_t;

typedef enum
{
    NINEBOT_ESC_COMMAND_REGREAD = 0x01,
    NINEBOT_ESC_COMMAND_REGWRITE = 0x02,
    NINEBOT_ESC_COMMAND_REGWRITE_NO_RESP = 0x03,
    NINEBOT_ESC_COMMAND_START_FIRMWARE_UPDATE = 0x07,
    NINEBOT_ESC_COMMAND_WRITE_FIRMWARE_UPDATE = 0x08,
    NINEBOT_ESC_COMMAND_FINISH_FIRMWARE_UPDATE = 0x09,
    NINEBOT_ESC_COMMAND_USER_RESET = 0x5A,
    NINEBOT_ESC_COMMAND_SET_ODOMETER = 0x5C,
    NINEBOT_ESC_COMMAND_REGREAD_UPDATE_HEAD_INPUTS = 0x61,
    NINEBOT_ESC_COMMAND_UPDATE_HEAD_INPUTS = 0x64,
    NINEBOT_ESC_COMMAND_UPDATE_HEAD_INPUTS_NOANS = 0x65,
}
NinebotEscCommand_t;

typedef enum
{
    NINEBOT_ESC_REG_MAGIC = 0x00, /* 0x515C ? */
    NINEBOT_ESC_REG_MOTOR_PHASE_A_CURRENT = 0x0D,
    NINEBOT_ESC_REG_MOTOR_PHASE_B_CURRENT = 0x0E,
    NINEBOT_ESC_REG_MOTOR_PHASE_C_CURRENT = 0x0F,
    NINEBOT_ESC_REG_ESC_SERIAL = 0x10,
    NINEBOT_ESC_REG_SCOOTER_PIN = 0x17,
    NINEBOT_ESC_REG_ESC_FIRMWARE_V = 0x1A,
    NINEBOT_ESC_REG_ERROR_CODE = 0x1B,
    NINEBOT_ESC_REG_WARNING_CODE = 0x1C,
    NINEBOT_ESC_REG_ESC_STATUS = 0x1C,
    NINEBOT_ESC_REG_BATTERY_LEVEL = 0x22,
    NINEBOT_ESC_REG_REMAINING_MILAGE__ = 0x24, /* ? */
    NINEBOT_ESC_REG_REMAINING_MILAGE = 0x25, /* km ? */
    NINEBOT_ESC_REG_SPEED = 0x26,
    /* Not important, skipped */
    NINEBOT_ESC_REG_BATTERY_VOLTAGE = 0x48,
    NINEBOT_ESC_REG_BATTERY_CURRENT = 0x48,
    NINEBOT_ESC_REG_LOCK_COMMAND = 0x70,
    NINEBOT_ESC_REG_UNLOCK_COMMAND = 0x71,
    NINEBOT_ESC_REG_ECO_MODE = 0x75,
    NINEBOT_ESC_REG_REBOOT_COMMAND = 0x78,
    NINEBOT_ESC_REG_POWER_DOWN_COMMAND = 0x79,
    NINEBOT_ESC_REG_KERS_LEVEL = 0x7B,
    NINEBOT_ESC_REG_CRUISE_CONTROL_ENABLE = 0x7C,
    NINEBOT_ESC_REG_TAIL_LIGHT_ON = 0x7D,
    /* Not important, skipped */
    /* ... */
}
NinebotEscRegister_t;

typedef enum {
    NINEBOT_BMS_REG_REMAINING_CAPACITY_mAh = 0x31,
    NINEBOT_BMS_REG_REMAINING_CAPACITY = 0x32,
    NINEBOT_BMS_REG_CURRENT = 0x33,
    NINEBOT_BMS_REG_VOLTAGE = 0x34,
    NINEBOT_BMS_REG_VOLTAGE_CELL = 0x40, /* Add 1 byte for each cell. (Ex: Cell 3 = 0x42, up to cell 10 at 0x49) */
}
NinebotBmsRegister_t;

typedef struct
{
    unsigned payloadLen: 8;
    unsigned sndr: 8;
    unsigned recv: 8;
    unsigned cmd: 8;
    unsigned cmdArg: 8;
} 
NinebotMessageHeader_t;

struct NinebotBleBroadcastCommand_t
{
    NinebotBleBroadcastCommand_t() : len(4) {}
    const unsigned len:8;
    unsigned thr:8;
    unsigned brk:8;
    unsigned unk:8;
    unsigned nbp:8;
};

struct NinebotBleBroadcastCommandAnsw_t
{
    NinebotBleBroadcastCommandAnsw_t() : len(6) {}
    const unsigned len:8;
    unsigned thr:8;
    unsigned brk:8;
    unsigned unk:8;
    unsigned nbp:8;
    unsigned unk0:8;
    unsigned unk1:8;
};

class NinebotMessage
{
    private:
        NinebotMessageHeader_t _msgHeader;
        std::vector<uint8_t> _payload;
    public:
        NinebotMessage(uint8_t *buf);
        NinebotMessage(NinebotMessageHeader_t *hdr, uint8_t *payload);

        NinebotMessageHeader_t getHeader(void);
        std::vector<uint8_t> getPayload(void);
        NinebotMessageCkSum_t getCkSum(void);

        std::vector<uint8_t> serialize(void);
};


