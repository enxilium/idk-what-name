#include <stdint.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

#include <chlorine/logs/logs.hpp>

#pragma once

namespace chlorine::scene
{
    // Note to anyone using these in the future:
    // All of these functions need to exist, and
    // BE PROPERLY DEFINED!!!
    class component
    {
    public:
        std::string componentName;
        component() = default;
        virtual void dumpData(::chlorine::logging::logBase* logOut) { logOut->log(componentName+"\n"); }
        virtual bool loadFile(std::fstream& componentFile, std::string filePath,  ::chlorine::logging::logBase* logOut) {return false;};
        virtual bool saveFile(std::string filePath, ::chlorine::logging::logBase* logOut) {return false;};

    };

}
