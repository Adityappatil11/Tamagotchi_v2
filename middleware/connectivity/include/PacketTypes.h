#ifndef PACKET_TYPES_H
#define PACKET_TYPES_H

#include <cstdint>

enum class PacketType : uint8_t {
    TIME_SYNC = 0x01,   // NTP Time from ESP32
    AI_COMMAND = 0x02,  // Text from OpenAI
    WIFI_STATUS = 0x03  // Connection strength
};

struct Packet {
    PacketType type;
    uint8_t length;
    uint8_t data[64]; // Fixed buffer for embedded safety
};

#endif