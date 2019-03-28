#include <string>
#include <stdio.h>

#include "NinebotMessageUtils.hpp"

typedef union
{
    uint16_t sum;
    struct
    {
        /*
            NOTE (bcovas): The checksums after
            the calculation are in reverse order.
            Since we are using direct memory
            casting, the order of these fields
            depends on the host's endianness.
            In the esp32, this reverses the data again.

            TODO (bcovas): Detect endianness, if
            it is worth the trouble.
        */
        uint8_t ck0;
        uint8_t ck1;
    } cks;
}
NinebotMessageCkSumCalc_t;

NinebotMessageCkSum_t NinebotMessageCheckSum(uint8_t *buf, int len)
{
    NinebotMessageCkSumCalc_t cksum{.sum = 0};

    for (int i=0; i<len; i++)
        cksum.sum += buf[i];
    cksum.sum ^= 0xFFFF;

    return {.ck0 = cksum.cks.ck0, .ck1 = cksum.cks.ck1 };
}

std::string buffToHexString(uint8_t *buf, int len)
{
    std::string _msg;
    char _data[6];

    for (int i=0; i<len; i++)
    { 
        // sprintf(_data, "0x%02x ", buf[i]);
        sprintf(_data, "%d ", buf[i]);
        _msg += _data;
    }

    return _msg;
}