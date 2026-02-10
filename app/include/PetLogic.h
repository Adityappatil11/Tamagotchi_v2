#ifndef PETLOGIC_H
#define PETLOGIC_H

#include <cstdint>

// Define lifecycle stages for the state machine
enum class LifeStage { EGG, BABY, TEEN, ADULT, DEAD };

/**
 * @brief Plain Old Data (POD) structure for easy serialization.
 * This is what will be saved to NVS and the Cloud.
 */
struct PetStats {
    float hunger = 100.0f;      // Range: 0.0 - 100.0
    float happiness = 100.0f;   // Range: 0.0 - 100.0
    float health = 100.0f;      // Range: 0.0 - 100.0
    float cleanliness = 100.0f; // Range: 0.0 - 100.0
    uint32_t ageSeconds = 0;    // Total lifetime in seconds
    LifeStage stage = LifeStage::EGG;
    bool isSleeping = false;
};

namespace PetConfig {
    const float HUNGER_DECAY_PER_HOUR = 8.0f;
    const float HYGIENE_DECAY_PER_HOUR = 2.0f;
    const uint32_t EGG_HATCH_SECONDS = 300;
}

class Tamagotchi {
public:
    Tamagotchi() = default;

    // --- Core Engine ---
    // Advances the pet logic based on delta time
    void OnTick(uint32_t secondsPassed);
    
    // --- Actions ---
    void Feed(float amount);    // Increases hunger
    void Sleep(bool start);     // Toggles sleep metabolic rate
    void Clean();               // Resets cleanliness to 100
    void Heal(float amount);    // Increases health

    // --- Data Access ---
    PetStats GetStats() const { return stats; }

private:
    PetStats stats;

    // --- Internal Logic ---
    void UpdateVitality(uint32_t dt); // Main math for stats decay
    void CheckEvolution();           // Handles life stage transitions
};


#endif // PETLOGIC_H