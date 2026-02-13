#include "CommandParser.h"
#include <algorithm>
#include <cctype>

CommandParser::CommandParser(Tamagotchi *pet) : _pet(pet)
{
    // Registering synonyms for the same commands to make the AI feel natural
    _commandMap["feed"] = std::make_unique<FeedCommand>(_pet, 20.0f);
    _commandMap["eat"] = std::make_unique<FeedCommand>(_pet, 20.0f);

    _commandMap["sleep"] = std::make_unique<SleepCommand>(_pet, true);
    _commandMap["bed"] = std::make_unique<SleepCommand>(_pet, true);
    _commandMap["wake"] = std::make_unique<SleepCommand>(_pet, false);

    _commandMap["clean"] = std::make_unique<CleanCommand>(_pet);
    _commandMap["wash"] = std::make_unique<CleanCommand>(_pet);
    _commandMap["bath"] = std::make_unique<CleanCommand>(_pet);

    _commandMap["play"] = std::make_unique<PlayCommand>(_pet, 15.0f);
    _commandMap["game"] = std::make_unique<PlayCommand>(_pet, 15.0f);
    _commandMap["fun"] = std::make_unique<PlayCommand>(_pet, 15.0f);
}

bool CommandParser::ParseAndExecute(std::string input) {
    std::string cleanInput = ToLowerCase(input);

    // 1. Handle "Use" commands
    size_t usePos = cleanInput.find("use ");
    if (usePos != std::string::npos) {
        // Extract everything after "use "
        std::string target = cleanInput.substr(usePos + 4);
        
        // Basic cleanup: Remove punctuation if present
        target.erase(std::remove_if(target.begin(), target.end(), ispunct), target.end());

        // Check against inventory (Case-Insensitive search)
        return _pet->UseItem(target); 
    }

    // 2. Existing keyword mapping
    for (auto const& [keyword, command] : _commandMap) {
        if (cleanInput.find(keyword) != std::string::npos) {
            command->execute();
            return true;
        }
    }
    return false;
}

std::string CommandParser::ToLowerCase(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return str;
}