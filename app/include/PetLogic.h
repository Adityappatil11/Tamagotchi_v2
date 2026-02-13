#ifndef PETLOGIC_H
#define PETLOGIC_H

#include <cstdint>
#include <string>
#include <vector>

// Define lifecycle stages for the state machine
enum class LifeStage { EGG, BABY, TEEN, ADULT, DEAD };
// Defining the mood and personality
enum class Mood { HAPPY, NEUTRAL, SAD, ANGRY, BORED, STRESSED };
enum class Personality { NEUTRAL, LAZY, ENERGETIC, FRIENDLY };
enum class ItemType { FOOD, TOY, MEDICINE };

struct Item {
    std::string name;
    ItemType type;
    float effectValue; // e.g., hunger restored or boredom reduced
    int cost;
};
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
    int coins = 100; // Initial wallet balance
    std::vector<Item> inventory;
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
    void OnTick(uint32_t secondsPassed); //
    
    // --- Basic Actions ---
    void Feed(float amount);    //
    void Sleep(bool start);     //
    void Clean();               //
    void Heal(float amount);    //
    void Play(float funValue);  //
    Mood GetCurrentMood() const; //

    // --- Inventory & Economy (New) ---
    /**
     * @brief Purchases an item if sufficient coins are available.
     * @return true if purchase was successful.
     */
    bool BuyItem(const Item& item);

    /**
     * @brief Uses an item from the inventory by name.
     * @return true if item was found and used.
     */
    bool UseItem(const std::string& itemName);

    /**
     * @brief Adds coins to the pet's wallet (e.g., from playing games).
     */
    void AddCoins(int amount) { stats.coins += amount; }

    // --- Data Access ---
    PetStats GetStats() const { return stats; } //
    void SetPersonality(Personality p) { stats.personality = p; } //

private:
    PetStats stats; //

    // --- Internal Logic ---
    void UpdateVitality(uint32_t dt); //
    void CheckEvolution();           //
    float CalculateMoodScore() const; //
};


#endif // PETLOGIC_H