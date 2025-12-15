#include "engine/Engine.h"
#include <iostream>

namespace Engine
{
    void Engine::register_command(Command *command, const std::string &commandName)
    {
        if (!command)
        {
            throw std::invalid_argument("Engine: Cannot register null command");
        }
        commands_[commandName] = command;
        std::cout << "[Engine] Command '" << commandName << "' registered." << std::endl;
    }

    Any Engine::execute(const std::string &commandName, const ArgsMap &args)
    {
        auto it = commands_.find(commandName);
        if (it == commands_.end())
        {
            throw std::runtime_error("Engine: Command '" + commandName + "' not found.");
        }

        return it->second->execute(args);
    }

}