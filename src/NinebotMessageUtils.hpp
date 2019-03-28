#pragma once

#include <string>
#include <stdint.h>

typedef struct
{
    uint8_t ck0;
    uint8_t ck1;
}
NinebotMessageCkSum_t;

NinebotMessageCkSum_t NinebotMessageCheckSum(uint8_t *buf, int len);
std::string buffToHexString(uint8_t *buf, int len);