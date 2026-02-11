#include <gtest/gtest.h>
#include "PetLogic.h"

//verify hunger decay over time
TEST(PetLogicTest, HungerDecaysOverTime) {
    Tamagotchi pet; 
    float initialHunger = pet.GetStats().hunger; // Starts at 100
    
    // Simulate 1 hour (3600 seconds) passing
    pet.OnTick(3600);
    
    // Based on our 8%/hour decay rate, hunger should now be 92
    EXPECT_NEAR(pet.GetStats().hunger, 92.0f, 0.01f);
}

// Test: Verify feeding increases hunger correctly
TEST(PetLogicTest, FeedingIncreasesHunger) {
    Tamagotchi pet;
    pet.OnTick(36000); // Make the pet very hungry first
    
    float hungryLevel = pet.GetStats().hunger;
    pet.Feed(20.0f);
    
    EXPECT_GT(pet.GetStats().hunger, hungryLevel);
    EXPECT_LE(pet.GetStats().hunger, 100.0f); // Should never exceed 100
}

TEST(PetLogicTest, SleepConservesEnergy) {
    Tamagotchi pet;
    float initialHunger = pet.GetStats().hunger;
    
    pet.Sleep(true);
    pet.OnTick(3600); // 1 hour passes
    
    // Hunger should have dropped by 4% (half of 8%)
    EXPECT_NEAR(pet.GetStats().hunger, initialHunger - 4.0f, 0.01f);
}