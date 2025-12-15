#include <iostream>
#include <string>
#include <stdexcept>


#include "include/subject/Subject.h"
#include "include/engine/Engine.h"
#include "include/engine/Wrapper.h"


template<typename T>
void print_result(const std::string& command_name, const Engine::Any& result) {
    if constexpr (std::is_same_v<T, void>) {
        std::cout << "Result of '" << command_name << "': VOID (Operation complete)\n";
    } else {
        try {
            std::cout << "Result of '" << command_name << "': "
                      << std::any_cast<T>(result) << std::endl;
        } catch (const std::bad_any_cast&) {
            std::cout << "Result of '" << command_name << "': bad any cast\n";
        }
    }
}

int main() {
    try {
        Subject subj;
        Engine::Engine engine;
        
        //1
        std::cout << "Test 1: Subject::f3 (int, int) -> int" << std::endl;

        Engine::Wrapper<Subject, int, int, int> wrapper_f3(
            &subj, 
            &Subject::f3, 
            {
                {"arg1", Engine::Any(0)}, 
                {"arg2", Engine::Any(0)}
            }
        ); 
        
        engine.register_command(&wrapper_f3, "command1");
        

        //2
        Engine::Any result1 = engine.execute("command1", { {"arg1", Engine::Any(4)}, {"arg2", Engine::Any(5)} });
        print_result<int>("command1", result1); //9
        
        std::cout << "Test 2: Subject::f3 (Using default arg)" << std::endl;
        

        //3
        Engine::Any result2 = engine.execute("command1", { {"arg1", Engine::Any(100)} });
        print_result<int>("command1 (partial)", result2); //100 

        std::cout << "Test 3: Subject::print_data (string, double) -> void" << std::endl;


        //4
        Engine::Wrapper<Subject, void, const std::string&, double> wrapper_print(
            &subj, 
            &Subject::print_data, 
            {
                {"label", Engine::Any(std::string("Default"))}, 
                {"value", Engine::Any(1.23)}
            }
        );
        
        engine.register_command(&wrapper_print, "print_cmd");
        

        // 5
        Engine::Any result3 = engine.execute("print_cmd", { 
            {"label", Engine::Any(std::string("Temperature"))}, 
            {"value", Engine::Any(25.5)} 
        });
        print_result<void>("print_cmd", result3);
        

    } catch (const std::exception& e) {
        std::cerr << "Runtime Error: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "An unknown error occurred." << std::endl;
        return 1;
    }

    return 0;
}