#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include "PetLogic.h"
#include <string>

class DisplayManager {
public:
    // We pass the pet by reference so we can read its current stats
    void RefreshUI(const Tamagotchi& pet);

private:
    void DrawMoodIcon(Mood mood);
    void DrawProgressBar(int y, float value, const std::string& label);
};

#endif