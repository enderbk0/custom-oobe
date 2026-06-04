#include "jsonutils.h"
#include <sstream>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <stdexcept>

namespace JsonUtils {

std::string JsonValue::AsString(const std::string& defaultVal) const {
    return type == Type::String ? stringValue : defaultVal;
}

int JsonValue::AsInt(int defaultVal) const {
    if (type == Type::Number) return static_cast<int>(numberValue);
    if (type == Type::String) {
        try { return std::stoi(stringValue); } catch (...) {}
    }
    return defaultVal;
}

bool JsonValue::AsBool(bool defaultVal) const {
    if (type == Type::Bool) return boolValue;
    if (type == Type::String) return stringValue == "true";
    return defaultVal;
}

double JsonValue::AsNumber(double defaultVal) const {
    return type == Type::Number ? numberValue : defaultVal;
}

bool JsonValue::Has(const std::string& key) const {
    return type == Type::Object && objectValues.find(key) != objectValues.end();
}

const JsonValue& JsonValue::Get(const std::string& key) const {
    static JsonValue nullValue;
    if (type != Type::Object) return nullValue;
    auto it = objectValues.find(key);
    if (it != objectValues.end()) return it->second;
    return nullValue;
}

std::string EscapeString(const std::string& str) {
    std::string result;
    result.reserve(str.size() + 2);
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    char buf[8];
                    snprintf(buf, sizeof(buf), "\\u%04x", c);
                    result += buf;
                } else {
                    result += c;
                }
        }
    }
    return result;
}

std::string UnescapeString(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\' && i + 1 < str.size()) {
            switch (str[i + 1]) {
                case '"': result += '"'; ++i; break;
                case '\\': result += '\\'; ++i; break;
                case '/': result += '/'; ++i; break;
                case 'b': result += '\b'; ++i; break;
                case 'f': result += '\f'; ++i; break;
                case 'n': result += '\n'; ++i; break;
                case 'r': result += '\r'; ++i; break;
                case 't': result += '\t'; ++i; break;
                case 'u': {
                    if (i + 5 < str.size()) {
                        std::string hex = str.substr(i + 2, 4);
                        unsigned int codepoint;
                        sscanf(hex.c_str(), "%x", &codepoint);
                        result += static_cast<char>(codepoint & 0xFF);
                        i += 5;
                    }
                    break;
                }
                default: result += str[i]; break;
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

static void SkipWhitespace(const std::string& json, size_t& pos) {
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' ||
           json[pos] == '\n' || json[pos] == '\r')) {
        ++pos;
    }
}

static std::string ParseString(const std::string& json, size_t& pos) {
    if (pos >= json.size() || json[pos] != '"') return {};
    ++pos;
    std::string result;
    while (pos < json.size() && json[pos] != '"') {
        if (json[pos] == '\\' && pos + 1 < json.size()) {
            switch (json[pos + 1]) {
                case '"': result += '"'; ++pos; break;
                case '\\': result += '\\'; ++pos; break;
                case '/': result += '/'; ++pos; break;
                case 'n': result += '\n'; ++pos; break;
                case 'r': result += '\r'; ++pos; break;
                case 't': result += '\t'; ++pos; break;
                default: result += json[pos]; break;
            }
            ++pos;
        } else {
            result += json[pos];
            ++pos;
        }
    }
    if (pos < json.size() && json[pos] == '"') ++pos;
    return result;
}

static JsonValue ParseValue(const std::string& json, size_t& pos);

static JsonValue ParseObject(const std::string& json, size_t& pos) {
    JsonValue value;
    value.type = JsonValue::Type::Object;
    if (pos >= json.size() || json[pos] != '{') return value;
    ++pos;
    SkipWhitespace(json, pos);
    if (pos < json.size() && json[pos] == '}') { ++pos; return value; }
    while (pos < json.size()) {
        SkipWhitespace(json, pos);
        if (pos >= json.size() || json[pos] != '"') break;
        std::string key = ParseString(json, pos);
        SkipWhitespace(json, pos);
        if (pos < json.size() && json[pos] == ':') ++pos;
        SkipWhitespace(json, pos);
        value.objectValues[key] = ParseValue(json, pos);
        SkipWhitespace(json, pos);
        if (pos < json.size() && json[pos] == ',') ++pos;
        else if (pos < json.size() && json[pos] == '}') { ++pos; break; }
        else break;
    }
    return value;
}

static JsonValue ParseArray(const std::string& json, size_t& pos) {
    JsonValue value;
    value.type = JsonValue::Type::Array;
    if (pos >= json.size() || json[pos] != '[') return value;
    ++pos;
    SkipWhitespace(json, pos);
    if (pos < json.size() && json[pos] == ']') { ++pos; return value; }
    while (pos < json.size()) {
        value.arrayValues.push_back(ParseValue(json, pos));
        SkipWhitespace(json, pos);
        if (pos < json.size() && json[pos] == ',') ++pos;
        else if (pos < json.size() && json[pos] == ']') { ++pos; break; }
        else break;
    }
    return value;
}

static JsonValue ParseNumber(const std::string& json, size_t& pos) {
    JsonValue value;
    value.type = JsonValue::Type::Number;
    size_t start = pos;
    if (pos < json.size() && json[pos] == '-') ++pos;
    while (pos < json.size() && std::isdigit(json[pos])) ++pos;
    if (pos < json.size() && json[pos] == '.') {
        ++pos;
        while (pos < json.size() && std::isdigit(json[pos])) ++pos;
    }
    if (pos < json.size() && (json[pos] == 'e' || json[pos] == 'E')) {
        ++pos;
        if (pos < json.size() && (json[pos] == '+' || json[pos] == '-')) ++pos;
        while (pos < json.size() && std::isdigit(json[pos])) ++pos;
    }
    try {
        value.numberValue = std::stod(json.substr(start, pos - start));
    } catch (...) {
        value.numberValue = 0;
    }
    return value;
}

static JsonValue ParseValue(const std::string& json, size_t& pos) {
    SkipWhitespace(json, pos);
    if (pos >= json.size()) return {};
    if (json[pos] == '"') {
        JsonValue val;
        val.type = JsonValue::Type::String;
        val.stringValue = ParseString(json, pos);
        return val;
    }
    if (json[pos] == '{') return ParseObject(json, pos);
    if (json[pos] == '[') return ParseArray(json, pos);
    if (json[pos] == 't' && json.substr(pos, 4) == "true") {
        JsonValue val;
        val.type = JsonValue::Type::Bool;
        val.boolValue = true;
        pos += 4;
        return val;
    }
    if (json[pos] == 'f' && json.substr(pos, 5) == "false") {
        JsonValue val;
        val.type = JsonValue::Type::Bool;
        val.boolValue = false;
        pos += 5;
        return val;
    }
    if (json[pos] == 'n' && json.substr(pos, 4) == "null") {
        pos += 4;
        return {};
    }
    if (json[pos] == '-' || std::isdigit(json[pos])) {
        return ParseNumber(json, pos);
    }
    return {};
}

JsonValue Parse(const std::string& json) {
    size_t pos = 0;
    return ParseValue(json, pos);
}

static void SerializeValue(std::stringstream& ss, const JsonValue& value,
                            bool pretty, int indent) {
    auto indentStr = [](int level) -> std::string {
        return std::string(static_cast<size_t>(level) * 2, ' ');
    };

    switch (value.type) {
        case JsonValue::Type::Null:
            ss << "null";
            break;
        case JsonValue::Type::String:
            ss << '"' << EscapeString(value.stringValue) << '"';
            break;
        case JsonValue::Type::Number: {
            double intPart;
            if (std::modf(value.numberValue, &intPart) == 0.0 &&
                value.numberValue >= -9007199254740992.0 &&
                value.numberValue <= 9007199254740992.0) {
                ss << static_cast<long long>(value.numberValue);
            } else {
                ss << value.numberValue;
            }
            break;
        }
        case JsonValue::Type::Bool:
            ss << (value.boolValue ? "true" : "false");
            break;
        case JsonValue::Type::Object: {
            ss << '{';
            if (pretty) ss << '\n';
            bool first = true;
            for (const auto& [key, val] : value.objectValues) {
                if (!first) { ss << ','; if (pretty) ss << '\n'; }
                first = false;
                if (pretty) ss << indentStr(indent + 1);
                ss << '"' << EscapeString(key) << '"' << ':';
                if (pretty) ss << ' ';
                SerializeValue(ss, val, pretty, indent + 1);
            }
            if (pretty && !value.objectValues.empty()) {
                ss << '\n' << indentStr(indent);
            }
            ss << '}';
            break;
        }
        case JsonValue::Type::Array: {
            ss << '[';
            bool first = true;
            for (const auto& val : value.arrayValues) {
                if (!first) ss << ',';
                first = false;
                if (pretty) ss << ' ';
                SerializeValue(ss, val, pretty, indent);
            }
            ss << ']';
            break;
        }
    }
}

std::string Serialize(const JsonValue& value, bool pretty, int indent) {
    std::stringstream ss;
    SerializeValue(ss, value, pretty, indent);
    return ss.str();
}

bool ParseKeyValuePairs(const std::string& json,
                        std::unordered_map<std::string, std::string>& outMap) {
    JsonValue root = Parse(json);
    if (root.type != JsonValue::Type::Object) return false;
    for (const auto& [key, val] : root.objectValues) {
        switch (val.type) {
            case JsonValue::Type::String:
                outMap[key] = val.stringValue;
                break;
            case JsonValue::Type::Number:
                outMap[key] = std::to_string(val.numberValue);
                break;
            case JsonValue::Type::Bool:
                outMap[key] = val.boolValue ? "true" : "false";
                break;
            case JsonValue::Type::Null:
                outMap[key] = "";
                break;
            default:
                outMap[key] = Serialize(val, false);
                break;
        }
    }
    return true;
}

} // namespace JsonUtils
