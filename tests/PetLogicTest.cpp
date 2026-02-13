#include <gtest/gtest.h>
#include "PetLogic.h"
#include "CommandParser.h"

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

TEST(PetLogicTest, EnergeticPetLosesEnergyFaster) {
    Tamagotchi normalPet;
    Tamagotchi activePet;
    
    // Correctly modify the object state
    activePet.SetPersonality(Personality::ENERGETIC);
    
    // Simulate 1 hour (3600 seconds)
    activePet.OnTick(3600); 
    normalPet.OnTick(3600);
    
    // Energetic (1.5x decay) should have lower energy than Normal (1.0x decay)
    EXPECT_LT(activePet.GetStats().energy, normalPet.GetStats().energy);
}

TEST(PetLogicTest, MoodDegradesWithHunger) {
    Tamagotchi pet;
    
    // Initial state should be HAPPY (Score near 100)
    EXPECT_EQ(pet.GetCurrentMood(), Mood::HAPPY);
    
    // Simulate 12 hours of hunger decay (12 * 8 = 96% loss)
    pet.OnTick(3600 * 12); 
    
    // Pet should no longer be happy
    Mood currentMood = pet.GetCurrentMood();
    EXPECT_NE(currentMood, Mood::HAPPY);
    EXPECT_TRUE(currentMood == Mood::SAD || currentMood == Mood::ANGRY || currentMood == Mood::STRESSED);
}

TEST(PetLogicTest, ItemPurchaseAndUsage) {
    Tamagotchi pet;
    Item apple = {"Apple", ItemType::FOOD, 20.0f, 10};
    
    // 1. Test Purchase
    bool bought = pet.BuyItem(apple);
    EXPECT_TRUE(bought);
    EXPECT_EQ(pet.GetStats().coins, 90);
    EXPECT_EQ(pet.GetStats().inventory.size(), 1);

    // 2. Test Usage
    pet.OnTick(3600 * 5); // Make pet hungry
    float hungerBefore = pet.GetStats().hunger;
    
    bool used = pet.UseItem("Apple");
    EXPECT_TRUE(used);
    EXPECT_GT(pet.GetStats().hunger, hungerBefore);
    EXPECT_EQ(pet.GetStats().inventory.size(), 0);
}

TEST(CommandParserTest, HandlesUseItemWithArgument) {
    Tamagotchi pet;
    CommandParser parser(&pet);
    
    // Setup: Buy an item first
    Item apple = {"Apple", ItemType::FOOD, 20.0f, 10};
    pet.BuyItem(apple);
    
    // Action: Parse a string with an argument
    bool success = parser.ParseAndExecute("Please use the apple");
    
    // Assert
    EXPECT_TRUE(success);
    EXPECT_EQ(pet.GetStats().inventory.size(), 0);
}