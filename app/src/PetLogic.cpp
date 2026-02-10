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