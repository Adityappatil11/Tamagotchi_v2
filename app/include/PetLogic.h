#ifndef PETLOGIC_H
#define PETLOGIC_H

#include <cstdint>

// Define lifecycle stages for the state machine
enum class LifeStage { EGG, BABY, TEEN, ADULT, DEAD };
// Defining the mood and personality
enum class Mood { HAPPY, NEUTRAL, SAD, ANGRY, BORED, STRESSED };
enum class Personality { NEUTRAL, LAZY, ENERGETIC, FRIENDLY };

/**
 * @brief Plain Old Data (POD) structure for easy serialization.
 * This is what will be saved to NVS and the Cloud.
 */
struct PetStats {
    float hunger = 100.0f;      // Range: 0.0 - 100.0
    float happiness = 100.0f;   // Range: 0.0 - 100.0
    float health = 100.0f;      // Range: 0.0 - 100.0
    float cleanliness = 100.0f; // Range: 0.0 - 100.0
    float energy = 100.0f;      // Range: 0.0 - 100.0
    float boredom = 0.0f;       // Range: 0.0 - 100.0
    uint32_t ageSeconds = 0;    // Total lifetime in seconds
    
    Personality personality = Personality::NEUTRAL;
    LifeStage stage = LifeStage::EGG;
    bool isSleeping = false;
};

namespace PetConfig {
    const float HUNGER_DECAY_PER_HOUR = 8.0f;
    const float HYGIENE_DECAY_PER_HOUR = 2.0f;
    const uint32_t EGG_HATCH_SECONDS = 300;
    const float PLAY_COST_ENERGY  = 5.0f;
    const float BOREDOM_INCREASE_RATE = 4.0f; // % per hour
    const float ENERGY_DECAY_BASE = 6.0f;     // % per hour
    const float HAPPINESS_DECAY_BASE = 5.0f;  // % per hour
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
    void Play(float funValue); 
    Mood GetCurrentMood() const; // Dynamic mood calculation

    // --- Data Access ---
    PetStats GetStats() const { return stats; }

    // Data modification
    void SetPersonality(Personality p) { stats.personality = p; }

private:
    PetStats stats;

    // --- Internal Logic ---
    void UpdateVitality(uint32_t dt); // Main math for stats decay
    void CheckEvolution();           // Handles life stage transitions
    float CalculateMoodScore() const;
};


#endif // PETLOGIC_H