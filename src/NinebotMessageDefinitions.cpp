#include <memory.h>
#include <stdio.h>

#include "NinebotMessageDefinitions.hpp"
#include "NinebotMessageUtils.hpp"

NinebotMessage::NinebotMessage(uint8_t *buf)
{
    if (memcmp(buf, (const uint8_t []) NINEBOT_PACKET_HEADER, 2) == 0)
        buf += 2; /* drop packet header */

    uint8_t *payloadStart = buf + NINEBOT_MESSAGE_HEADER_LEN;
    memcpy(&_msgHeader, buf, NINEBOT_MESSAGE_HEADER_LEN);
    uint8_t *ckStart = payloadStart + _msgHeader.payloadLen;
    _payload.assign(payloadStart, ckStart);

}

NinebotMessage::NinebotMessage(NinebotMessageHeader_t *hdr, uint8_t *payload)
{
    _msgHeader = *hdr;
    _payload.assign(payload, payload + hdr->payloadLen);
};

NinebotMessageHeader_t NinebotMessage::getHeader(void)
{
    return _msgHeader;
}

std::vector<uint8_t> NinebotMessage::getPayload(void)
{
    return _payload;
}

NinebotMessageCkSum_t NinebotMessage::getCkSum(void)
{
    int bufLen = NINEBOT_MESSAGE_HEADER_LEN + _payload.size();
    uint8_t buf[bufLen];

    /* useful pointers */
    uint8_t *headerStart = buf;
    uint8_t *payloadStart = headerStart + NINEBOT_MESSAGE_HEADER_LEN;

    memcpy(headerStart, &_msgHeader, sizeof(_msgHeader));
    memcpy(payloadStart, _payload.data(), _payload.size());

    return NinebotMessageCheckSum(buf, bufLen);
}

std::vector<uint8_t> NinebotMessage::serialize(void)
{
    NinebotMessageCkSum_t cks = getCkSum();

    int bufSize = 2 + NINEBOT_MESSAGE_HEADER_LEN + _payload.size() + 2;
    uint8_t buf[bufSize] = NINEBOT_PACKET_HEADER;

    /* useful pointers */
    uint8_t *headerStart = buf + 2;
    uint8_t *payloadStart = headerStart + NINEBOT_MESSAGE_HEADER_LEN;
    uint8_t *ckStart = payloadStart + _payload.size();

    memcpy(headerStart, &_msgHeader, sizeof(_msgHeader));
    memcpy(payloadStart, _payload.data(), _payload.size());
    memcpy(ckStart, &cks, 2);

    std::vector<uint8_t> _buf;
    _buf.assign(buf, buf + bufSize);

    return _buf;
}