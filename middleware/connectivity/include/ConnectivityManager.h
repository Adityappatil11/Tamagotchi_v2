#ifndef CONNECTIVITY_MANAGER_H
#define CONNECTIVITY_MANAGER_H

#include "PacketTypes.h"
#include "PetLogic.h"
#include "CommandParser.h"

class ConnectivityManager {
public:
    ConnectivityManager(Tamagotchi* pet, CommandParser* parser) 
        : _pet(pet), _parser(parser) {}

    /**
     * @brief Processes a raw buffer received from SPI/UART.
     */
    void HandleIncomingPacket(const Packet& packet) {
        switch (packet.type) {
            case PacketType::TIME_SYNC:
                HandleTimeSync(packet);
                break;
            case PacketType::AI_COMMAND:
                HandleAIInteraction(packet);
                break;
        }
    }

private:
    Tamagotchi* _pet;
    CommandParser* _parser;

    void HandleTimeSync(const Packet& p);
    void HandleAIInteraction(const Packet& p);
};

#endif