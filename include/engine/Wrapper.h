#pragma once

#include "Command.h"
#include <functional>
#include <stdexcept>
#include <tuple>

namespace Engine {

/*
T-тип класса, чей метод мы оборачиваем
R- тип возвращаемого значения
Args - типы аргументов
*/
    template<typename T, typename R, typename... Args>
    class Wrapper : public Command {
    public:

        using MethodPtr = R(T::*)(Args...);

        /**
         obj кказатель на экземпляр класса
         method указатель на метод
        args_def список аргументов: {{"name", default_val}, ...}.
         */
        Wrapper(T* obj, MethodPtr method, const std::vector<std::pair<std::string, Any>>& args_def)
            : obj_(obj), method_(method)
        {
            for (const auto& pair : args_def) {
                arg_names_.push_back(pair.first);
                default_values_[pair.first] = pair.second;
            }

            // кол-во имен должно совпадать с кол-вом аргументов метода
            if (arg_names_.size() != sizeof...(Args)) {
                throw std::runtime_error("Wrapper: Mismatch between argument count and provided names.");
            }
        }

        Any execute(const ArgsMap& input_args) override {
            return execute_impl(input_args, std::make_index_sequence<sizeof...(Args)>{});
        }

    private:
        T* obj_;
        MethodPtr method_;

        std::vector<std::string> arg_names_;
        ArgsMap default_values_;

        template<std::size_t I, typename ArgType>
        ArgType get_arg_value(const ArgsMap& input_args) {
            const std::string& name = arg_names_[I];

            //  поиск в переданных аргументах
            auto it = input_args.find(name);
            if (it != input_args.end()) {
                try {
                    return std::any_cast<ArgType>(it->second);
                } catch (const std::bad_any_cast&) {
                    throw std::runtime_error("Wrapper: Argument '" + name + "' has wrong type or value.");
                }
            }

            auto it_def = default_values_.find(name);
            if (it_def != default_values_.end()) {
                 try {
                    return std::any_cast<ArgType>(it_def->second);
                } catch (const std::bad_any_cast&) {
                     throw std::runtime_error("Wrapper: Default argument '" + name + "' has wrong type or value.");
                }
            }

            throw std::runtime_error("Wrapper: Missing argument '" + name + "' and no default value set.");
        }

        //распаковka последовательности индексов
        template<std::size_t... Is>
        Any execute_impl(const ArgsMap& input_args, std::index_sequence<Is...>) {
            
            if constexpr (std::is_same_v<R, void>) {
                (obj_->*method_)(
                    get_arg_value<Is, std::tuple_element_t<Is, std::tuple<Args...>>>(input_args)...
                );
                return {}; 
            } else {
                return (obj_->*method_)(
                    get_arg_value<Is, std::tuple_element_t<Is, std::tuple<Args...>>>(input_args)...
                );
            }
        }
    };

}