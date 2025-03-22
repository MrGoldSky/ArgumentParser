#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "Argument.h"

template <typename T>
class TypedArgument : public Argument {
   public:
    TypedArgument(ArgType type) : type_(type) {}

    void SetLongName(std::string long_name) { long_name_ = long_name; }
    void SetShortName(char short_name) { short_name_ = short_name; }
    void SetDescription(std::string description) { description_ = description; }
    void SetValue(const T& value) {
        value_ = value;
        if (external_value_) {
            *external_value_ = value_;
        }
    }

    void AddValue(const T& value) {
        is_initialized_ = true;
        if (is_multi_value_){
            multi_values_->push_back(value);}
        else{
            SetValue(value);}
    }

    void SetMultiValue(int min_values = INT_MIN) override {
        is_multi_value_ = true;
        min_multi_values_ = min_values;
    }

    void SetPositional(bool value) override {
        is_positional_ = value;
    }

    void StoreValues(std::vector<T>& values) {
        multi_values_ = &values;
    }

    void StoreValue(T& value) {
        external_value_ = &value;
    }

    void SetDefault(T& value) {
        has_default_value_ = true;
        is_initialized_ = true;
        default_value_ = value;
        if (is_multi_value_) {
            for (int i = 0; i < GetMinMultiValues(); ++i) {
                AddValue(value);
            }
        } else {
            SetValue(value);
        }
    }

    bool IsPositional() const override { return is_positional_; }
    bool IsMultiValue() const override { return is_multi_value_; }
    bool IsInitialized() const override { return is_initialized_; }
    bool HasDefaultValue() const override { return has_default_value_; }

    int GetMultiValuesCount() const override {
        if (multi_values_->size()) {
            return multi_values_->size();
        } else {
            return 0;
        }
    }

    int GetMinMultiValues() const override {
        return min_multi_values_;
    }

    std::string GetDefaultValue() const override {
        if (!has_default_value_) {
            throw std::runtime_error("Default value is not set.");
        }

        if constexpr (std::is_same<T, int>::value) {
            return std::to_string(default_value_);
        } else if constexpr (std::is_same<T, bool>::value) {
            return default_value_ ? "true" : "false";
        } else if constexpr (std::is_same<T, std::string>::value) {
            return default_value_;
        }
        throw std::runtime_error("Unsupported type for argument default value.");
    }

    T GetValue(int index = 0) const {
        if (is_multi_value_ && multi_values_ && !multi_values_->empty()) {
            if (index >= 0 && index < static_cast<int>(multi_values_->size())) {
                return (*multi_values_)[index];
            } else {
                throw std::out_of_range("Index out of range for multi-value argument.");
            }
        } else if (external_value_)
            return *external_value_;
        return value_;
    }

    std::string GetLongName() const override { return long_name_; }
    std::string GetShortName() const override { return std::string(1, short_name_); }
    std::string GetDescription() const override { return description_; }

    ArgType GetType() const override {
        return type_;
    }

    void ParseValue(const std::string& value) override {
        if constexpr (std::is_same<T, int>::value) {
            AddValue(std::stoi(value));
        } else if constexpr (std::is_same<T, bool>::value) {
            AddValue(bool(value == "true" || value == "1"));
        } else if constexpr (std::is_same<T, std::string>::value) {
            AddValue(value);
        } else {
            throw std::runtime_error("Unsupported type for argument");
        }

        if (external_value_) {
            *external_value_ = value_;
        }
    }

   private:
    char short_name_{};
    std::string long_name_{};
    std::string description_{};

    ArgType type_;

    T value_{};
    T* external_value_ = nullptr;
    std::vector<T>* multi_values_ = nullptr;

    T default_value_{};

    bool is_multi_value_ = false;
    bool is_positional_ = false;
    bool is_initialized_ = false;
    bool has_default_value_ = false;
    int min_multi_values_ = INT_MIN;
};
