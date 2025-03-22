#pragma once

#include <string>
// Класс для указания типов аргументов
enum class ArgType { STRING,
                     INT,
                     BOOL };

// Базовый класс для всех аргументов
class Argument {
   public:
    virtual ~Argument() = default;
    virtual ArgType GetType() const = 0;

    virtual void ParseValue(const std::string& value) = 0;

    virtual void SetLongName(std::string long_name) = 0;
    virtual void SetShortName(char short_name) = 0;
    virtual void SetDescription(std::string description) = 0;

    virtual void SetMultiValue(int min_values) = 0;
    virtual void SetPositional(bool value) = 0;

    virtual int GetMultiValuesCount() const = 0;
    virtual int GetMinMultiValues() const = 0;
    virtual std::string GetShortName() const = 0;
    virtual std::string GetLongName() const = 0;
    virtual std::string GetDescription() const = 0;
    virtual std::string GetDefaultValue() const = 0;

    virtual bool IsPositional() const = 0;
    virtual bool IsMultiValue() const = 0;
    virtual bool IsInitialized() const = 0;
    virtual bool HasDefaultValue() const = 0;

    template <typename T>
    void StoreValues(std::vector<T>& values) {
        throw std::runtime_error("StoreValues is not supported for this type.");
    }

    template <typename T>
    void StoreValue(std::vector<T>& value) {
        throw std::runtime_error("StoreValue is not supported for this type.");
    }

    template <typename T>
    void SetDefault(T& value) {
        throw std::runtime_error("Default is not supported for this type.");
    }
};
