#pragma once

#include <fstream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "TypedArgument.h"

namespace ArgumentParser {

class ArgParser {
   public:
    // Конструктор, деструктор
    ArgParser(const std::string& name) : name_(name) {};
    ~ArgParser() {
        for (auto& [key, value] : arguments_) {
            delete value;
        }
    }

    ArgParser& AddStringArgument(const char short_name_, const std::string& long_name_, const std::string& description = "");
    ArgParser& AddStringArgument(const std::string& long_name_, const std::string& description = "");
    ArgParser& AddIntArgument(char short_name_, const std::string& long_name_, const std::string& description = "");
    ArgParser& AddIntArgument(const std::string& long_name_, const std::string& description = "");
    ArgParser& AddFlag(char short_name_, const std::string& long_name_, const std::string& description = "");
    ArgParser& AddFlag(const std::string& long_name_, const std::string& description = "");

    template <typename T>
    ArgParser& AddArgument(char short_name, const std::string& long_name, const std::string& description = "");
    template <typename T>
    ArgParser& AddArgument(const std::string& long_name, const std::string& description = "");

    template <typename T>
    ArgParser& MakeDefault(T& values);
    ArgParser& Default(const char* value);
    ArgParser& Default(int value);
    ArgParser& Default(bool value);

    ArgParser& Positional(bool value = true);
    ArgParser& MultiValue(int min_values = INT_MIN);

    template <typename T>
    ArgParser& MakeStoreValues(std::vector<T>& values);
    ArgParser& StoreValues(std::vector<int>& values);
    ArgParser& StoreValues(std::vector<std::string>& values);
    ArgParser& StoreValues(std::vector<bool>& values);

    template <typename T>
    ArgParser& MakeStoreValue(T& value);
    ArgParser& StoreValue(std::string& value);
    ArgParser& StoreValue(bool& value);
    ArgParser& StoreValue(int& value);

    int GetIntValue(const std::string& argument, const int& multi_value = 0);
    int GetIntValue(const char& argument, const int& multi_value = 0);

    std::string GetStringValue(const std::string& argument, const int& multi_value = 0);
    std::string GetStringValue(const char& argument, const int& multi_value = 0);

    bool GetFlag(const std::string& argument, const int& multi_value = 0);
    bool GetFlag(const char& argument, const int& multi_value = 0);

    template <typename T>
    T GetValue(const std::string& long_name);
    template <typename T>
    T GetValue(const char& short_name);

    // Получение аргументов
    template <typename T>
    TypedArgument<T>& GetArgument(const std::string& long_name) {
        if (arguments_.find(long_name) == arguments_.end())
            throw std::invalid_argument("Argument not found");
        Argument* arg = arguments_[long_name];
        auto* typedArg = dynamic_cast<TypedArgument<T>*>(arg);
        if (!typedArg)
            throw std::bad_cast();
        return *typedArg;
    }

    template <typename T>
    TypedArgument<T>& GetArgument(const char& short_name) {
        if (short_name_map_.find(short_name) == short_name_map_.end())
            throw std::invalid_argument("Argument not found");

        Argument* arg = short_name_map_[short_name];
        auto* typedArg = dynamic_cast<TypedArgument<T>*>(arg);
        if (!typedArg)
            throw std::bad_cast();

        return *typedArg;
    }

    int Parse(const std::vector<std::string>& parse_values);
    int Parse(int argc, char** argv);
    bool CheckMultiValueValid();
    bool CheckValuesValid();
    bool ParseLongArgument(std::string arg, Argument*& current_argument);

    bool Help();
    ArgParser& AddHelp(const char short_name_, const std::string& long_name_, const std::string& description = "^_^");
    std::string HelpDescription();
    bool CheckHelp(std::string arg);

   private:
    std::string name_;
    std::unordered_map<std::string, Argument*> arguments_;
    std::unordered_map<char, Argument*> short_name_map_;
    Argument* last_added_argument_ = nullptr;

    std::string help_short_;
    std::string help_long_;
    std::string help_description_;
    bool help_initialized = false;
};

}  // namespace ArgumentParser
