#include "CommandParser.h"
#include <algorithm>
#include <string>

bool CommandParser::ParseAndExecute(std::string input){
    std::string cleanInput = ToLowerCase(input);

    // keyword mapping logic
    if(cleanInput.find("feed")!=std::string::npos || cleanInput.find("eat")!=std::string::npos){
        pet->Feed(20.0f);
        return true;
    }
    if(cleanInput.find("clean")!=std::string::npos || cleanInput.find("wash")!=std::string::npos){
        pet->Clean();
        return true;
    }
    if(cleanInput.find("sleep")!=std::string::npos || cleanInput.find("bed")!=std::string::npos){
        pet->Sleep(true);
        return true;
    }
    if(cleanInput.find("wake")!=std::string::npos){
        pet->Sleep(false);
        return true;
    }
    return false;
}

std::string CommandParser::ToLowerCase(std::string str){
    std::transform(str.begin(),str.end(),str.begin(),std::tolower);
    return str;
}