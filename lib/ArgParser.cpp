#include "ArgParser.h"

#include <iostream>
using namespace ArgumentParser;

template <typename T>
ArgParser& ArgParser::AddArgument(char short_name, const std::string& long_name, const std::string& description) {
    ArgType type;
    if constexpr (std::is_same_v<T, int>) {
        type = ArgType::INT;
    } else if constexpr (std::is_same_v<T, bool>) {
        type = ArgType::BOOL;
    } else if constexpr (std::is_same_v<T, std::string>) {
        type = ArgType::STRING;
    } else {
        throw std::invalid_argument("Unsupported type");
    }

    TypedArgument<T>* arg = new TypedArgument<T>(type);
    arg->SetLongName(long_name);
    arg->SetShortName(short_name);
    arg->SetDescription(description);

    arguments_[long_name] = arg;
    short_name_map_[short_name] = arg;

    last_added_argument_ = arg;

    return *this;
}

template <typename T>
ArgParser& ArgParser::AddArgument(const std::string& long_name, const std::string& description) {
    ArgType type;
    if constexpr (std::is_same_v<T, int>) {
        type = ArgType::INT;
    } else if constexpr (std::is_same_v<T, bool>) {
        type = ArgType::BOOL;
    } else if constexpr (std::is_same_v<T, std::string>) {
        type = ArgType::STRING;
    } else {
        throw std::invalid_argument("Unsupported type");
    }

    TypedArgument<T>* arg = new TypedArgument<T>(type);

    arg->SetLongName(long_name);
    arg->SetDescription(description);

    arguments_[long_name] = arg;

    last_added_argument_ = arg;
    return *this;
}

ArgParser& ArgParser::AddStringArgument(char short_name, const std::string& long_name, const std::string& description) {
    return AddArgument<std::string>(short_name, long_name, description);
}

ArgParser& ArgParser::AddStringArgument(const std::string& long_name, const std::string& description) {
    return AddArgument<std::string>(long_name, description);
}

ArgParser& ArgParser::AddIntArgument(char short_name, const std::string& long_name, const std::string& description) {
    return AddArgument<int>(short_name, long_name, description);
}

ArgParser& ArgParser::AddIntArgument(const std::string& long_name, const std::string& description) {
    return AddArgument<int>(long_name, description);
}

ArgParser& ArgParser::AddFlag(char short_name, const std::string& long_name, const std::string& description) {
    return AddArgument<bool>(short_name, long_name, description);
}

ArgParser& ArgParser::AddFlag(const std::string& long_name, const std::string& description) {
    return AddArgument<bool>(long_name, description);
}

ArgParser& ArgParser::Default(const char* value) {
    std::string new_value = std::string(value);
    return MakeDefault(new_value);
}

ArgParser& ArgParser::Default(int value) {
    return MakeDefault(value);
}

ArgParser& ArgParser::Default(bool value) {
    return MakeDefault(value);
}

template <typename T>
ArgParser& ArgParser::MakeDefault(T& value) {
    if (!last_added_argument_) {
        throw std::runtime_error("No argument added to configure.");
    }

    auto* typed_arg = dynamic_cast<TypedArgument<T>*>(last_added_argument_);
    if (typed_arg) {
        typed_arg->SetDefault(value);
        return *this;
    }

    throw std::invalid_argument("Argument does not support default values of this type.");
}

ArgParser& ArgParser::MultiValue(int min_values) {
    if (!last_added_argument_) {
        throw std::runtime_error("No argument added to configure.");
    }

    last_added_argument_->SetMultiValue(min_values);
    return *this;
}

template <typename T>
ArgParser& ArgParser::MakeStoreValues(std::vector<T>& values) {
    if (!last_added_argument_) {
        throw std::runtime_error("No argument added to configure.");
    }

    auto* typed_arg = dynamic_cast<TypedArgument<T>*>(last_added_argument_);
    if (!typed_arg) {
        throw std::invalid_argument("Argument does not support storing values of this type.");
    }

    typed_arg->StoreValues(values);
    return *this;
}

ArgParser& ArgParser::StoreValues(std::vector<int>& values) {
    return MakeStoreValues(values);
}

ArgParser& ArgParser::StoreValues(std::vector<std::string>& values) {
    return MakeStoreValues(values);
}

ArgParser& ArgParser::StoreValues(std::vector<bool>& values) {
    return MakeStoreValues(values);
}

template <typename T>
ArgParser& ArgParser::MakeStoreValue(T& value) {
    if (!last_added_argument_) {
        throw std::runtime_error("No argument added to configure.");
    }

    auto* typed_arg = dynamic_cast<TypedArgument<T>*>(last_added_argument_);
    if (!typed_arg) {
        throw std::invalid_argument("Argument does not support storing value of this type.");
    }

    typed_arg->StoreValue(value);
    return *this;
}

ArgParser& ArgParser::StoreValue(std::string& value) {
    return MakeStoreValue(value);
}

ArgParser& ArgParser::StoreValue(bool& value) {
    return MakeStoreValue(value);
}

ArgParser& ArgParser::StoreValue(int& value) {
    return MakeStoreValue(value);
}

ArgParser& ArgParser::Positional(bool value) {
    if (!last_added_argument_) {
        throw std::runtime_error("No argument added to configure.");
    }

    last_added_argument_->SetPositional(value);
    return *this;
}

template <typename T>
T ArgParser::GetValue(const std::string& long_name) {
    if (arguments_.find(long_name) == arguments_.end())
        throw std::invalid_argument("Argument not found");

    Argument* arg = arguments_[long_name];
    TypedArgument<T>* typedArg = dynamic_cast<TypedArgument<T>*>(arg);
    if (!typedArg) {
        throw std::bad_cast();
    }
    return typedArg->GetValue();
}

template <typename T>
T ArgParser::GetValue(const char& short_name) {
    if (short_name_map_.find(short_name) == short_name_map_.end())
        throw std::invalid_argument("Argument not found");

    Argument* arg = short_name_map_[short_name];
    TypedArgument<T>* typedArg = dynamic_cast<TypedArgument<T>*>(arg);
    if (!typedArg)
        throw std::bad_cast();
    return typedArg->GetValue();
}

int ArgParser::GetIntValue(const std::string& argument, const int& multi_value) {
    return GetValue<int>(argument);
}

int ArgParser::GetIntValue(const char& argument, const int& multi_value) {
    return GetValue<int>(argument);
}

std::string ArgParser::GetStringValue(const std::string& argument, const int& multi_value) {
    return GetValue<std::string>(argument);
}

std::string ArgParser::GetStringValue(const char& argument, const int& multi_value) {
    return GetValue<std::string>(argument);
}

bool ArgParser::GetFlag(const std::string& argument, const int& multi_value) {
    return GetValue<bool>(argument);
}

bool ArgParser::GetFlag(const char& argument, const int& multi_value) {
    return GetValue<bool>(argument);
}

int ArgParser::Parse(const std::vector<std::string>& parse_values) {
    std::vector<const char*> argv;
    for (const auto& value : parse_values)
        argv.push_back(value.c_str());

    return Parse(static_cast<int>(argv.size()), const_cast<char**>(argv.data()));
}

bool ArgParser::ParseLongArgument(std::string arg, Argument*& current_argument) {
    size_t equal_pos = arg.find('=');
    std::string long_name = arg.substr(2, equal_pos - 2);
    if (CheckHelp(long_name)) {
        return true;
    }
    if (arguments_.find(long_name) != arguments_.end()) {
        current_argument = arguments_[long_name];
        if (equal_pos != std::string::npos) {
            std::string value = arg.substr(equal_pos + 1);
            current_argument->ParseValue(value);
            current_argument = nullptr;
        } else if (current_argument->GetType() == ArgType::BOOL) {
            dynamic_cast<TypedArgument<bool>*>(current_argument)->AddValue(true);
        }
    } else {
        throw std::runtime_error("Unknown argument: " + long_name);
    }
    return false;
}

int ArgParser::Parse(int argc, char** argv) {
    std::string_view current_arg;
    Argument* current_argument = nullptr;

    std::vector<Argument*> positional_args;

    for (auto& [name, argument] : arguments_) {
        if (argument->IsPositional()) {
            positional_args.push_back(argument);
        }
    }

    size_t positional_index = 0;

    for (size_t i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        // Проверка на длинный аргумент
        if (arg.rfind("--", 0) == 0) {
            if (ParseLongArgument(arg, current_argument)) {
                return true;
            }
        }
        // Проверка на короткий аргумент или цепочку коротких флагов
        else if (arg.rfind("-", 0) == 0 && arg.size() > 1) {
            for (size_t j = 1; j < arg.size(); ++j) {
                char short_name = arg[j];
                if (CheckHelp(std::string(arg))) {
                    return true;
                }
                if (short_name_map_.find(short_name) == short_name_map_.end()) {
                    throw std::runtime_error("Unknown argument: -" + std::string(1, short_name));
                } else {
                    current_argument = short_name_map_[short_name];
                    if (current_argument->GetType() != ArgType::BOOL) {
                        if (j == arg.size() - 1) {
                            if (i + 1 < argc) {
                                current_argument->ParseValue(argv[++i]);
                            }
                        } else if (arg[j + 1] == '=') {
                            current_argument->ParseValue(arg.substr(j + 2));
                            break;
                        }
                    } else {
                        dynamic_cast<TypedArgument<bool>*>(current_argument)->AddValue(true);
                    }
                }
            }
            current_argument = nullptr;
        } else if (current_argument) {
            current_argument->ParseValue(arg);
            current_argument = nullptr;
        }
        // Обработка позиционных аргументов
        else if (positional_index < positional_args.size()) {
            Argument* positional_arg = positional_args[positional_index];
            positional_arg->ParseValue(arg);
            if (!positional_arg->IsMultiValue()) {
                positional_index++;
            }
        }
    }

    return CheckMultiValueValid() && CheckValuesValid();
}

bool ArgParser::CheckHelp(std::string arg) {
    if (arg == help_short_ || arg == help_long_)
        return true;
    return false;
}

bool ArgParser::CheckMultiValueValid() {
    for (auto& [name, argument] : arguments_) {
        if (argument->IsMultiValue() && argument->GetMultiValuesCount() < argument->GetMinMultiValues()) {
            return false;
        }
    }
    return true;
}

bool ArgParser::CheckValuesValid() {
    for (auto& [name, argument] : arguments_) {
        if (!argument->IsInitialized() && (argument->GetType() != ArgType::BOOL) && argument->GetLongName() != help_long_) {
            return false;
        }
    }
    return true;
}

bool ArgParser::Help() {
    return help_initialized;
}

ArgParser& ArgParser::AddHelp(const char short_name, const std::string& long_name, const std::string& description) {
    help_initialized = true;
    help_long_ = long_name;
    help_short_ = short_name;
    help_description_ = description;
    return AddArgument<std::string>(short_name, long_name, description);
}

std::string ArgParser::HelpDescription() {
    std::ostringstream help;

    help << name_ << "\n";
    if (!help_description_.empty()) {
        help << help_description_ << "\n\n";
    }

    for (const auto& [name, argument] : arguments_) {
        std::string short_name = argument->GetShortName().empty() ? "" : "-" + argument->GetShortName() + ", ";
        std::string long_name = "--" + name;

        help << short_name << long_name;
        help << ",  " << argument->GetDescription();

        if (argument->IsMultiValue()) {
            help << " [MultiValue";
            if (argument->GetMinMultiValues() > 0) {
                help << ", min args = " << argument->GetMinMultiValues();
            }
            help << "]";
        }

        if (argument->IsPositional()) {
            help << " [Positional]";
        }
        if (argument->HasDefaultValue()) {
            auto type = argument->GetType();
            help << " [default = " << argument->GetDefaultValue() << "]";
        }

        help << "\n";
    }

    return help.str();
}
