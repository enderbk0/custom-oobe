#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace JsonUtils {

struct JsonValue {
    enum class Type { Null, String, Number, Bool, Object, Array };
    Type type = Type::Null;
    std::string stringValue;
    double numberValue = 0;
    bool boolValue = false;
    std::unordered_map<std::string, JsonValue> objectValues;
    std::vector<JsonValue> arrayValues;

    std::string AsString(const std::string& defaultVal = "") const;
    int AsInt(int defaultVal = 0) const;
    bool AsBool(bool defaultVal = false) const;
    double AsNumber(double defaultVal = 0.0) const;
    bool IsNull() const { return type == Type::Null; }
    bool Has(const std::string& key) const;
    const JsonValue& Get(const std::string& key) const;
};

JsonValue Parse(const std::string& json);
std::string Serialize(const JsonValue& value, bool pretty = false, int indent = 0);
std::string EscapeString(const std::string& str);
std::string UnescapeString(const std::string& str);

bool ParseKeyValuePairs(const std::string& json,
                        std::unordered_map<std::string, std::string>& outMap);

} // namespace JsonUtils
