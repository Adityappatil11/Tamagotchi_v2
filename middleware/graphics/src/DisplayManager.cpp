#include "DisplayManager.h"
// #include "OLED_Driver.h" // This will come from your drivers folder later

void DisplayManager::RefreshUI(const Tamagotchi& pet) {
    auto stats = pet.GetStats(); //

    // Clear the screen (via Driver)
    // Driver_Clear();

    // 1. Draw Pet based on Stage and Mood
    DrawMoodIcon(pet.GetCurrentMood());

    // 2. Draw Vital Bars
    DrawProgressBar(0, stats.hunger, "HGR");
    DrawProgressBar(10, stats.happiness, "HAP");

    // Flush to hardware
    // Driver_DisplayUpdate();
}