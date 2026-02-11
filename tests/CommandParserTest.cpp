#include <gtest/gtest.h>
#include "PetLogic.h"
#include "CommandParser.h"

// Test that the parser correctly triggers feeding
TEST(CommandParserTest, HandlesFeedCommandPhrases) {
    Tamagotchi pet;
    CommandParser parser(&pet);
    
    // Setup: Make the pet hungry first
    pet.OnTick(36000); 
    float hungerBefore = pet.GetStats().hunger;
    
    // Action: Simulate an OpenAI or User string
    bool success = parser.ParseAndExecute("The pet looks hungry, please feed it!");
    
    // Assert
    EXPECT_TRUE(success);
    EXPECT_GT(pet.GetStats().hunger, hungerBefore);
}

// Test that synonyms like "wash" and "clean" work
TEST(CommandParserTest, HandlesCleanSynonyms) {
    Tamagotchi pet;
    CommandParser parser(&pet);
    
    // Simulate time to decrease cleanliness
    pet.OnTick(3600);
    
    parser.ParseAndExecute("Give the pet a wash");
    EXPECT_FLOAT_EQ(pet.GetStats().cleanliness, 100.0f);
}

// Test for unknown commands
TEST(CommandParserTest, ReturnsFalseForUnknownInput) {
    Tamagotchi pet;
    CommandParser parser(&pet);
    
    bool success = parser.ParseAndExecute("Dance for me!");
    EXPECT_FALSE(success);
}