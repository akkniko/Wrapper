#pragma once

#include "Command.h"
#include <unordered_map>
#include <string>
#include <stdexcept>

namespace Engine {

    class Engine {
    public:
        void register_command(Command* command, const std::string& commandName);

        Any execute(const std::string& commandName, const ArgsMap& args);

    private:
        std::unordered_map<std::string, Command*> commands_;
    };

    
}