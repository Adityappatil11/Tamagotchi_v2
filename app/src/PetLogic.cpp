#include "PetLogic.h"
#include <algorithm>

// Configuration Constants for game balance
namespace PetConfig {
    const float HUNGER_DECAY_BASE = 8.0f;     // % per hour
    const float HYGIENE_DECAY_BASE = 2.0f;    // % per hour
    const float HEALTH_DECAY_NEGLECT = 5.0f;  // % per hour if starving/dirty
    const float HEALTH_RECOVERY_SLEEP = 2.0f; // % per hour if well-fed
}

void Tamagotchi::OnTick(uint32_t secondsPassed) {
    if (stats.stage == LifeStage::DEAD) return;
    
    stats.ageSeconds += secondsPassed;
    UpdateVitality(secondsPassed);
    CheckEvolution();
}

void Tamagotchi::Feed(float amount) {
    if (stats.isSleeping || stats.stage == LifeStage::DEAD) return; 
    stats.hunger = std::min(100.0f, stats.hunger + amount);
}

void Tamagotchi::Sleep(bool start) {
    if (stats.stage == LifeStage::DEAD) return;
    stats.isSleeping = start;
}

void Tamagotchi::Clean() {
    if (stats.stage == LifeStage::DEAD) return;
    stats.cleanliness = 100.0f; // Reset hygiene to max
}

void Tamagotchi::Heal(float amount) {
    if (stats.stage == LifeStage::DEAD) return;
    stats.health = std::min(100.0f, stats.health + amount);
}

void Tamagotchi::UpdateVitality(uint32_t dt) {
    // 1. Hunger Logic
    float hungerMultiplier = stats.isSleeping ? 0.5f : 1.0f;
    float hungerDecay = (PetConfig::HUNGER_DECAY_BASE / 3600.0f) * dt * hungerMultiplier;
    stats.hunger = std::max(0.0f, stats.hunger - hungerDecay);

    // 2. Hygiene Logic
    float hygieneDecay = (PetConfig::HYGIENE_DECAY_BASE / 3600.0f) * dt;
    stats.cleanliness = std::max(0.0f, stats.cleanliness - hygieneDecay);
    
    // 3. Health & Sickness Logic
    if (stats.cleanliness < 20.0f || stats.hunger <= 0.0f) {
        stats.health -= (PetConfig::HEALTH_DECAY_NEGLECT / 3600.0f) * dt;
    } else if (stats.hunger > 50.0f && stats.isSleeping) {
        stats.health = std::min(100.0f, stats.health + (PetConfig::HEALTH_RECOVERY_SLEEP / 3600.0f) * dt);
    }

    // 4. Death Check
    if (stats.health <= 0) {
        stats.health = 0;
        stats.stage = LifeStage::DEAD;
    }

    //  Energy & Boredom Logic
    float hours = dt / 3600.0f;

    // personality modifiers
    float energyMod = (stats.personality == Personality::ENERGETIC) ? 1.5f : 1.0f;

    if (stats.isSleeping) {
        stats.energy = std::min(100.0f, stats.energy + (10.0f * hours));
    } else {
        stats.energy = std::max(0.0f, stats.energy - (PetConfig::ENERGY_DECAY_BASE * hours * energyMod));
        stats.boredom = std::min(100.0f, stats.boredom + (PetConfig::BOREDOM_INCREASE_RATE * hours));
    }

    //  Happiness Logic: Drops if hungry, dirty, or bored
    if (stats.hunger < 20.0f || stats.cleanliness < 20.0f || stats.boredom > 70.0f) {
        stats.happiness = std::max(0.0f, stats.happiness - (PetConfig::HAPPINESS_DECAY_BASE * hours));
    }
}

void Tamagotchi::CheckEvolution() {
    // 300s = 5m, 3600s = 1h
    if (stats.stage == LifeStage::EGG && stats.ageSeconds > 300) {
        stats.stage = LifeStage::BABY;
    }
    else if (stats.stage == LifeStage::BABY && stats.ageSeconds > 3600) {
        stats.stage = LifeStage::TEEN;
    }
}

Mood Tamagotchi::GetCurrentMood() const {
    if (stats.stage == LifeStage::DEAD) return Mood::SAD;

    float score = CalculateMoodScore();

    if (score > 85.0f) return Mood::HAPPY;
    if (score > 60.0f) return Mood::NEUTRAL;
    if (score > 40.0f) return Mood::BORED;
    if (score > 20.0f) return Mood::SAD;
    if (score > 5.0f)  return Mood::ANGRY;
    
    return Mood::STRESSED;
}

void Tamagotchi::Play(float funValue) {
    if (stats.isSleeping || stats.stage == LifeStage::DEAD) return;
    
    stats.boredom = std::max(0.0f, stats.boredom - funValue);
    stats.happiness = std::min(100.0f, stats.happiness + (funValue / 2.0f));
    stats.energy = std::max(0.0f, stats.energy - PetConfig::PLAY_COST_ENERGY); // Playing costs energy
}

float Tamagotchi::CalculateMoodScore() const {
    // Weighted logic: Physical survival is 70% of the score, Mental state is 30%
    float physicalNeeds = (stats.health * 0.6f) + (stats.hunger * 0.4f);
    float mentalNeeds = (stats.happiness * 0.7f) - (stats.boredom * 0.3f);
    
    // Final score normalized to 0-100
    float score = (physicalNeeds * 0.7f) + (mentalNeeds * 0.3f);
    
    return std::clamp(score, 0.0f, 100.0f);
}

/**
 * @brief Purchases an item if the pet has enough coins.
 */
bool Tamagotchi::BuyItem(const Item& item) {
    if (stats.coins >= item.cost) {
        stats.coins -= item.cost;
        stats.inventory.push_back(item);
        return true;
    }
    return false;
}

/**
 * @brief Uses an item from the inventory and applies its effects.
 */
bool Tamagotchi::UseItem(const std::string& itemName) {
    if (stats.stage == LifeStage::DEAD || stats.isSleeping) return false;

    std::string searchName = itemName;
    std::transform(searchName.begin(), searchName.end(), searchName.begin(), ::tolower);

    auto it = std::find_if(stats.inventory.begin(), stats.inventory.end(),
        [&](const Item& i) {
            std::string currentItemName = i.name;
            std::transform(currentItemName.begin(), currentItemName.end(), currentItemName.begin(), ::tolower);
            return currentItemName == searchName;
        });

    if (it != stats.inventory.end()) {
        // Apply effects
        if (it->type == ItemType::FOOD) Feed(it->effectValue);
        else if (it->type == ItemType::TOY) Play(it->effectValue);
        else if (it->type == ItemType::MEDICINE) Heal(it->effectValue);

        stats.inventory.erase(it);
        return true;
    }
    return false;
}