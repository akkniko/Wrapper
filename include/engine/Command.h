#pragma once

#include <string>
#include <map>
#include <any>
#include <vector>

namespace Engine
{
    //поддержка любых типов 
    using Value = std::any;
    using Any = std::any;
    using ArgsMap = std::map<std::string, Any>;

    class Command
    {
    public:
        virtual ~Command() = default;
        
        virtual Any execute(const ArgsMap &args) = 0;
    };

}