#include "ConnectivityManager.h"

void ConnectivityManager::HandleTimeSync(const Packet& p) {
    // Convert bytes to epoch time
    uint32_t networkTime = *(uint32_t*)p.data;
    
    // Logic for Week 5: Global Time Sync
    // Calculate difference between networkTime and internal RTC
    // pet->OnTick(difference); 
}

void ConnectivityManager::HandleAIInteraction(const Packet& p) {
    // Convert data to string (e.g., "The user says give him a treat")
    std::string aiText((char*)p.data, p.length);
    
    // Feed directly into your existing CommandParser!
    _parser->ParseAndExecute(aiText);
}