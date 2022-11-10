#include "Option.h"

#include <stdexcept>
#include <algorithm>
#include <regex>

namespace op {
    Option::Option(const std::string& name)
        : has_read(false)
        , expect_value(false)
        , is_mandatory(false)
        , has_default(false)
    {
        std::regex expr("[a-zA-Z][A-Za-z0-9_-]*");
        if (std::regex_match(name, expr)) {
            Option::names.push_back(name);
        } else {
            throw std::runtime_error("bad format for option name");
        }
    }

    const std::vector<std::string>& Option::getNames() const {
        return Option::names;
    }

    const std::string& Option::getValue() const {
        if (expect_value && !value.empty()) {
            return value;
        }
        throw std::runtime_error("getValue");
    }

    bool Option::expectValue() const {
        return expect_value;
    }

    Option& Option::setDefaultValue(const std::string& value1) {
        Option::value = value1;
        has_default = true;
        expect_value = true;
        return *this;
    }

    Option& Option::hasValue() {
        expect_value = true;
        return *this;
    }

    Option& Option::addAlias(const std::string& alias) {
        if (std::find(names.begin(), names.end(), alias) == names.end()){
            // alias not already added
            Option::names.push_back(alias);
            return *this;
        }
        throw std::runtime_error("alias already added");
    }

    Option& Option::setMandatory() {
        is_mandatory = true;
        return *this;
    }

    bool Option::isMandatory() const {
        return is_mandatory;
    }

    void Option::parsed(){
        has_read = true;
    }

    bool Option::operator==(const std::string& arg) const {
        if (std::find(names.begin(), names.end(), arg) != names.end()){
            return true;
        }
        return false;
    }

    bool Option::operator!=(const std::string& arg) const {
        return !(operator==(arg));
    }

    Option& Option::operator=(const std::string& value1) {
        if(Option::expect_value){
            Option::value = value1;
            return *this;
        }
        throw std::runtime_error("no value expected");
    }

    Option::operator bool() const {
        return has_read;
    }

    bool Option::hasDefault() const {
        return has_default;
    }
}
