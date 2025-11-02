///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_json.cpp
// Purpose:     BWX_SDK Library; JSON parsing utilities
// Author:      Bartosz Warzocha
// Created:     2023-12-25
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "bwx_json.h"

#include <wx/log.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>

#include <sstream>
#include <algorithm>
#include <cctype>

namespace bwx_sdk {

    bwxJSON::bwxJSON(const wxString& jsonText)
    {
        ParseFromString(jsonText);
    }

    bwxJSON::bwxJSON(const bwxJsonValue& defaultValue)
    {
        if (defaultValue.has_value())
        {
            m_data["default"] = defaultValue;
        }
    }
    
    bwxJSON::bwxJSON(const bwxJSON& other)
    : m_data(other.m_data), m_lastError(other.m_lastError) {}
    
    bwxJSON::bwxJSON(bwxJSON&& other) noexcept
    : m_data(std::move(other.m_data)), m_lastError(std::move(other.m_lastError)) {}

    bool bwxJSON::LoadFromFile(const wxString& filename)
    {
        wxTextFile file;
        if (!file.Open(filename))
            return false;

        wxString jsonText;
        for (size_t i = 0; i < file.GetLineCount(); ++i)
            jsonText += file.GetLine(i);

        file.Close();
        return ParseFromString(jsonText);
    }

    bool bwxJSON::SaveToFile(const wxString& filename) const
    {
        wxFile file;
        if (!file.Open(filename, wxFile::write))
            return false;

        wxString jsonText = SerializeToString();
        file.Write(jsonText);
        file.Close();
        return true;
    }

    bool bwxJSON::LoadFromStream(wxInputStream& stream)
    {
        wxTextInputStream textStream(stream);
        wxString jsonText;

        while (!stream.Eof())
        {
            jsonText += textStream.ReadLine();
        }

        return ParseFromString(jsonText);
    }

    bool bwxJSON::SaveToStream(wxOutputStream& stream) const
    {
        wxTextOutputStream textStream(stream);
        textStream << SerializeToString();
        return true;
    }

    bool bwxJSON::ParseFromString(const wxString& jsonText)
    {
        size_t pos = 0;
        m_data.clear();
        m_lastError.Clear();

        pos = SkipWhitespace(jsonText, pos);

        bwxJsonValue parsedValue = ParseValue(jsonText, pos);
        if (parsedValue.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*parsedValue))
        {
            *this = *std::get<std::shared_ptr<bwxJSON>>(*parsedValue);
            return true;
        }

        m_lastError = wxString::Format(wxT("Parse JSON error at %zu: wrong structure"), pos);
        return false;
    }

    wxString bwxJSON::SerializeToString() const
    {
        wxString jsonText = "{";

        for (const auto& [key, value] : m_data)
        {
            jsonText += "\"" + key + "\":" + JsonValueToString(value) + ",";
        }

        if (jsonText.EndsWith(","))
            jsonText.RemoveLast();

        jsonText += "}";
        return jsonText;
    }

    wxString bwxJSON::SerializeCompact() const
    {
        return SerializeToString();
    }

    bool bwxJSON::HasKey(const wxString& key) const
    {
        return m_data.find(key) != m_data.end();
    }

    void bwxJSON::RemoveKey(const wxString& key)
    {
        m_data.erase(key);
    }

    std::vector<wxString> bwxJSON::GetKeys() const
    {
        std::vector<wxString> keys;
        for (const auto& [key, _] : m_data)
            keys.push_back(key);
        return keys;
    }

    void bwxJSON::SetValue(const wxString& key, const bwxJsonValue& value)
    {
        m_data[key] = value;
    }

    bwxJsonValue bwxJSON::GetValue(const wxString& key, const bwxJsonValue& defaultValue) const
    {
        auto it = m_data.find(key);
        return (it != m_data.end()) ? it->second : defaultValue;
    }

    void bwxJSON::AppendToArray(const wxString& key, const bwxJsonValue& value)
    {
        if (!m_data[key].has_value())
        {
            m_data[key].emplace(std::vector<bwxJsonValueHelper>());
        }

        if (std::holds_alternative<std::vector<bwxJsonValueHelper>>(*m_data[key]))
        {
            std::get<std::vector<bwxJsonValueHelper>>(*m_data[key]).emplace_back(bwxJsonValueHelper{ value });
        }
    }

    std::vector<bwxJsonValue> bwxJSON::GetArray(const wxString& key) const
    {
        if (!HasKey(key) || !m_data.at(key).has_value() || !std::holds_alternative<std::vector<bwxJsonValueHelper>>(*m_data.at(key)))
            return {};

        std::vector<bwxJsonValue> result;
        for (const auto& val : std::get<std::vector<bwxJsonValueHelper>>(*m_data.at(key)))
        {
            result.push_back(val.value);
        }
        return result;
    }

    void bwxJSON::RemoveFromArray(const wxString& key, size_t index)
    {
        if (!HasKey(key) || !std::holds_alternative<std::vector<bwxJsonValueHelper>>(*m_data[key]))
            return;

        auto& arr = std::get<std::vector<bwxJsonValueHelper>>(*m_data[key]);
        if (index < arr.size())
            arr.erase(arr.begin() + index);
    }

    bwxJsonValue bwxJSON::FindValue(const wxString& key) const
    {
        if (HasKey(key))
            return GetValue(key);

        for (const auto& [_, v] : m_data)
        {
            if (v.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*v))
            {
                auto nested = std::get<std::shared_ptr<bwxJSON>>(*v);
                if (nested && nested->HasKey(key))
                    return nested->GetValue(key);
            }
        }
        return std::nullopt;
    }

    void bwxJSON::SortKeys()
    {
        std::vector<std::pair<wxString, bwxJsonValue>> sortedData;

        for (const auto& [key, value] : m_data)
        {
            sortedData.emplace_back(key, value);
        }

        std::sort(sortedData.begin(), sortedData.end(),
            [](const auto& a, const auto& b) { return a.first < b.first; });

        m_data.clear();
        for (const auto& [key, value] : sortedData)
        {
            m_data.emplace(key, value);
        }
    }

    bool bwxJSON::operator==(const bwxJSON& other) const
    {
        return m_data == other.m_data;
    }

    bool bwxJSON::operator!=(const bwxJSON& other) const
    {
        return !(*this == other);
    }

    bwxJsonValue& bwxJSON::operator[](const wxString& key)
    {
        return m_data[key];
    }

    const bwxJsonValue& bwxJSON::operator[](const wxString& key) const
    {
        auto it = m_data.find(key);
        if (it != m_data.end())
            return it->second;

        static bwxJsonValue emptyValue = std::nullopt;
        return emptyValue;
    }

    bool bwxJSON::IsValid() const
    {
        return !m_data.empty();
    }

    size_t bwxJSON::GetSize() const
	{
		return m_data.size();
	}

    wxString bwxJSON::GetKey(size_t index) const
	{
		if (index >= m_data.size()) return wxString();
		auto it = m_data.begin();
		std::advance(it, index);
		return it->first;
	}

    wxString bwxJSON::EscapeString(const wxString& str) const
    {
        wxString escaped;
        for (auto ch : str)
        {
            switch (ch.GetValue())
            {
            case '\"': escaped += "\\\""; break;
            case '\\': escaped += "\\\\"; break;
            case '\b': escaped += "\\b"; break;
            case '\f': escaped += "\\f"; break;
            case '\n': escaped += "\\n"; break;
            case '\r': escaped += "\\r"; break;
            case '\t': escaped += "\\t"; break;
            default: escaped += ch; break;
            }
        }
        return "\"" + escaped + "\"";
    }

    wxString bwxJSON::JsonValueToString(const bwxJsonValue& value) const
    {
        if (!value.has_value()) return wxString("null");

        return std::visit([this](auto&& arg) -> wxString {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::nullptr_t>) return wxString("null");
            else if constexpr (std::is_same_v<T, int>) return wxString::Format("%d", arg);
            else if constexpr (std::is_same_v<T, int64_t>) return wxString::Format("%lld", arg);
            else if constexpr (std::is_same_v<T, uint64_t>) return wxString::Format("%llu", arg);
            else if constexpr (std::is_same_v<T, double>) return wxString::Format("%g", arg);
            else if constexpr (std::is_same_v<T, bool>) return wxString(arg ? "true" : "false");
            else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, wxString>) return "\"" + wxString(arg) + "\"";
            else if constexpr (std::is_same_v<T, std::shared_ptr<bwxJSON>>) return arg ? arg->SerializeToString() : wxString("null");
            else { // std::vector<bwxJsonValueHelper>
                wxString jsonArray = "[";
                for (const auto& val : arg) {
                    jsonArray += JsonValueToString(val.value) + ",";
                }
                if (jsonArray.EndsWith(",")) jsonArray.RemoveLast();
                jsonArray += "]";
                return jsonArray;
            }
            }, *value);
    }

    size_t bwxJSON::SkipWhitespace(const wxString& text, size_t start) const
    {
        while (start < text.Length())
        {
            // SKip whitaspaces
            if (std::isspace(text[start].GetValue()))
            {
                start++;
                continue;
            }

            // Comments //
            if (text[start] == '/' && start + 1 < text.Length() && text[start + 1] == '/')
            {
                while (start < text.Length() && text[start] != '\n')
                    start++;
                continue;
            }

            // Comments /* ... */
            if (text[start] == '/' && start + 1 < text.Length() && text[start + 1] == '*')
            {
                start += 2; // Skip begin `/*`
                while (start + 1 < text.Length() && !(text[start] == '*' && text[start + 1] == '/'))
                    start++;
                if (start + 1 < text.Length()) start += 2; // Skip end `*/`
                continue;
            }

			// If not whitespace or comment, break loop...
            break;
        }

        return start;
    }

    bwxJsonValue bwxJSON::ParseValue(const wxString& text, size_t& pos)
    {
        pos = SkipWhitespace(text, pos);

        if (text[pos] == '"') return ParseString(text, pos);
        if (text[pos] == '{') return ParseObject(text, pos);
        if (text[pos] == '[') return ParseArray(text, pos);
        if (std::isdigit(text[pos].GetValue()) || text[pos] == '-') return ParseNumber(text, pos);

        if (text.Mid(pos, 4) == "true") { pos += 4; return bwxJsonValue(true); }
        if (text.Mid(pos, 5) == "false") { pos += 5; return bwxJsonValue(false); }
        if (text.Mid(pos, 4) == "null") { pos += 4; return bwxJsonValue(); }

        return bwxJsonValue();
    }

    wxString bwxJSON::ParseString(const wxString& text, size_t& pos)
    {
        pos++;
        size_t start = pos;
        while (pos < text.Length() && text[pos] != '"')
        {
            if (text[pos] == '\\') pos++;
            pos++;
        }
        wxString result = text.Mid(start, pos - start);
        pos++;
        return result;
    }

    bwxJsonValue bwxJSON::ParseNumber(const wxString& text, size_t& pos)
    {
        size_t start = pos;
        while (pos < text.Length() && (std::isdigit(text[pos].GetValue()) || text[pos] == '.' || text[pos] == '-'))
            pos++;

        wxString numberStr = text.Mid(start, pos - start);
        if (numberStr.Contains(".")) return bwxJsonValue(std::stod(numberStr.ToStdString()));
        return bwxJsonValue(std::stoi(numberStr.ToStdString()));
    }

    bwxJsonValue bwxJSON::ParseObject(const wxString& text, size_t& pos)
    {
        auto jsonObject = std::make_shared<bwxJSON>();
        pos++;

        while (pos < text.Length() && text[pos] != '}')
        {
            pos = SkipWhitespace(text, pos);
            if (pos >= text.Length()) {
                m_lastError = wxString::Format("Unexpected end of JSON at position %zu - missing closing `}`", pos);
                return std::nullopt;
            }

            wxString key = ParseString(text, pos);
            pos = SkipWhitespace(text, pos);

            if (text[pos] != ':') {
                m_lastError = wxString::Format("JSON syntax error at position %zu: Expected `:` after key", pos);
                return std::nullopt;
            }
            pos++;

            bwxJsonValue value = ParseValue(text, pos);
            jsonObject->SetValue(key, value);

            pos = SkipWhitespace(text, pos);
            if (text[pos] == ',') pos++;
        }

        if (pos >= text.Length()) {
            m_lastError = wxString::Format("Unexpected end of JSON at position %zu - missing closing `}`", pos);
            return std::nullopt;
        }

        pos++;
        return bwxJsonValue(jsonObject);
    }

    bwxJsonValue bwxJSON::ParseArray(const wxString& text, size_t& pos)
    {
        std::vector<bwxJsonValueHelper> jsonArray;
        pos++;

        while (pos < text.Length() && text[pos] != ']')
        {
            pos = SkipWhitespace(text, pos);
            if (pos >= text.Length()) {
                m_lastError = wxString::Format("Unexpected end of JSON at position %zu - missing closing `]`", pos);
                return std::nullopt;
            }

            jsonArray.push_back(bwxJsonValueHelper{ ParseValue(text, pos) });
            pos = SkipWhitespace(text, pos);
            if (text[pos] == ',') pos++;
        }

        if (pos >= text.Length()) {
            m_lastError = wxString::Format("Unexpected end of JSON at position %zu - missing closing `]`", pos);
            return std::nullopt;
        }

        pos++;
        return bwxJsonValue(jsonArray);
    }

    wxString bwxJSON::SerializePretty(int indentLevel) const
    {
        wxString jsonText;
        int currentIndent = 0;

        auto addIndent = [&]() {
            jsonText += wxString(' ', currentIndent);
            };

        std::function<void(const bwxJsonValue&, int)> serialize = [&](const bwxJsonValue& value, int level) {
            if (!value.has_value()) {
                jsonText += "null";
                return;
            }

            std::visit([&](auto&& arg) {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, std::nullptr_t>) {
                    jsonText += "null";
                }
                else if constexpr (std::is_same_v<T, int>) {
                    jsonText += wxString::Format("%d", arg);
                }
                else if constexpr (std::is_same_v<T, double>) {
                    jsonText += wxString::Format("%g", arg);
                }
                else if constexpr (std::is_same_v<T, bool>) {
                    jsonText += arg ? "true" : "false";
                }
                else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, wxString>) {
                    jsonText += "\"" + wxString(arg) + "\"";
                }
                else if constexpr (std::is_same_v<T, std::shared_ptr<bwxJSON>>) {
                    if (arg) jsonText += arg->SerializePretty(indentLevel);
                    else jsonText += "null";
                }
                else if constexpr (std::is_same_v<T, std::vector<bwxJsonValueHelper>>) {
                    jsonText += "[\n";
                    currentIndent += indentLevel;
                    for (size_t i = 0; i < arg.size(); ++i) {
                        addIndent();
                        serialize(arg[i].value, level + 1);
                        if (i < arg.size() - 1) jsonText += ",";
                        jsonText += "\n";
                    }
                    currentIndent -= indentLevel;
                    addIndent();
                    jsonText += "]";
                }
                }, *value);
            };

        jsonText += "{\n";
        currentIndent += indentLevel;
        size_t count = m_data.size();
        size_t index = 0;

        for (const auto& [key, value] : m_data) {
            addIndent();
            jsonText += "\"" + key + "\": ";
            serialize(value, 1);
            if (index++ < count - 1) jsonText += ",";
            jsonText += "\n";
        }

        currentIndent -= indentLevel;
        addIndent();
        jsonText += "}";

        return jsonText;
    }

	// As* functions
    int bwxJSON::AsInt(const wxString& key, int defaultValue) const
    {
        if (!HasKey(key)) return defaultValue;
        const auto& value = GetValue(key);
        if (!value.has_value()) return defaultValue;

        if (std::holds_alternative<int>(*value)) return std::get<int>(*value);
        if (std::holds_alternative<int64_t>(*value)) return static_cast<int>(std::get<int64_t>(*value));
        if (std::holds_alternative<uint64_t>(*value)) return static_cast<int>(std::get<uint64_t>(*value));
        if (std::holds_alternative<double>(*value)) return static_cast<int>(std::get<double>(*value));

        return defaultValue;
    }

    int64_t bwxJSON::AsInt64(const wxString& key, int64_t defaultValue) const
    {
        if (!HasKey(key)) return defaultValue;
        const auto& value = GetValue(key);
        if (!value.has_value()) return defaultValue;

        if (std::holds_alternative<int64_t>(*value)) return std::get<int64_t>(*value);
        if (std::holds_alternative<int>(*value)) return static_cast<int64_t>(std::get<int>(*value));
        if (std::holds_alternative<double>(*value)) return static_cast<int64_t>(std::get<double>(*value));

        return defaultValue;
    }

    uint64_t bwxJSON::AsUInt64(const wxString& key, uint64_t defaultValue) const
    {
        if (!HasKey(key)) return defaultValue;
        const auto& value = GetValue(key);
        if (!value.has_value()) return defaultValue;

        if (std::holds_alternative<uint64_t>(*value)) return std::get<uint64_t>(*value);
        if (std::holds_alternative<int>(*value)) return static_cast<uint64_t>(std::get<int>(*value));
        if (std::holds_alternative<int64_t>(*value)) return static_cast<uint64_t>(std::get<int64_t>(*value));
        if (std::holds_alternative<double>(*value)) return static_cast<uint64_t>(std::get<double>(*value));

        return defaultValue;
    }

    double bwxJSON::AsDouble(const wxString& key, double defaultValue) const
    {
        if (!HasKey(key)) return defaultValue;
        const auto& value = GetValue(key);
        if (!value.has_value()) return defaultValue;

        if (std::holds_alternative<double>(*value)) return std::get<double>(*value);
        if (std::holds_alternative<int>(*value)) return static_cast<double>(std::get<int>(*value));
        if (std::holds_alternative<int64_t>(*value)) return static_cast<double>(std::get<int64_t>(*value));
        if (std::holds_alternative<uint64_t>(*value)) return static_cast<double>(std::get<uint64_t>(*value));

        return defaultValue;
    }

    bool bwxJSON::AsBool(const wxString& key, bool defaultValue) const
    {
        if (!HasKey(key)) return defaultValue;
        const auto& value = GetValue(key);
        if (!value.has_value()) return defaultValue;

        if (std::holds_alternative<bool>(*value)) return std::get<bool>(*value);
        if (std::holds_alternative<int>(*value)) return std::get<int>(*value) != 0;
        if (std::holds_alternative<int64_t>(*value)) return std::get<int64_t>(*value) != 0;
        if (std::holds_alternative<uint64_t>(*value)) return std::get<uint64_t>(*value) != 0;
        if (std::holds_alternative<double>(*value)) return std::get<double>(*value) != 0.0;

        return defaultValue;
    }

    wxString bwxJSON::AsWxString(const wxString& key, const wxString& defaultValue) const
    {
        if (!HasKey(key)) return defaultValue;
        const auto& value = GetValue(key);
        if (!value.has_value()) return defaultValue;

        if (std::holds_alternative<wxString>(*value)) return std::get<wxString>(*value);
        if (std::holds_alternative<std::string>(*value)) return wxString(std::get<std::string>(*value));

        return defaultValue;
    }

    std::string bwxJSON::AsStdString(const wxString& key, const std::string& defaultValue) const
    {
        if (!HasKey(key)) return defaultValue;
        const auto& value = GetValue(key);
        if (!value.has_value()) return defaultValue;

        if (std::holds_alternative<std::string>(*value)) return std::get<std::string>(*value);
        if (std::holds_alternative<wxString>(*value)) return std::get<wxString>(*value).ToStdString();

        return defaultValue;
    }

    // Iterators
    bwxJSON::iterator bwxJSON::begin()
    {
        return m_data.begin();
    }

    bwxJSON::iterator bwxJSON::end()
    {
        return m_data.end();
    }

    bwxJSON::const_iterator bwxJSON::begin() const
    {
        return m_data.begin();
    }

    bwxJSON::const_iterator bwxJSON::end() const
    {
        return m_data.end();
    }

    //
    bool bwxJSON::IsNull(const wxString& key) const
    {
        if (!HasKey(key)) return false;
        const auto& value = GetValue(key);
        return value.has_value() && std::holds_alternative<std::nullptr_t>(*value);
    }

    //
    std::shared_ptr<bwxJSON> bwxJSON::Clone() const
    {
        auto copy = std::make_shared<bwxJSON>();
		copy->m_data = m_data; // Copy whole map of values
        return copy;
    }

	bwxJSON& bwxJSON::operator=(const bwxJSON& other)
	{
		if (this != &other)
		{
			m_data = other.m_data;
		}
		return *this;
	}

    //
    void bwxJSON::Merge(const bwxJSON& other, bool overwriteExisting)
    {
        for (const auto& [key, value] : other.m_data)
        {
            if (!HasKey(key) || overwriteExisting)
            {
                m_data[key] = value;
            }
            else if (std::holds_alternative<std::shared_ptr<bwxJSON>>(*m_data[key]) &&
                std::holds_alternative<std::shared_ptr<bwxJSON>>(*value))
            {
                std::get<std::shared_ptr<bwxJSON>>(*m_data[key])->Merge(*std::get<std::shared_ptr<bwxJSON>>(*value), overwriteExisting);
            }
        }
    }

    bwxJSON bwxJSON::Diff(const bwxJSON& other) const
    {
        bwxJSON diffResult;

        // Checking keys present in `this` but not in `other`
        for (const auto& [key, value] : m_data)
        {
            if (!other.HasKey(key))
            {
                diffResult.SetValue(key, value); // Key removed
            }
            else if (*value != *other.GetValue(key))
            {
                diffResult.SetValue(key, other.GetValue(key)); // Key changed
            }
        }

        // Checking keys present in `other` but not in `this`
        for (const auto& [key, value] : other.m_data)
        {
            if (!HasKey(key))
            {
                diffResult.SetValue(key, value); // Key added
            }
        }

        return diffResult;
    }

    void bwxJSON::Patch(const bwxJSON& patchData)
    {
        for (const auto& [key, value] : patchData.m_data)
        {
            if (value.has_value())
            {
				m_data[key] = value; // Update or add new value
            }
            else
            {
				m_data.erase(key); // Remove value
            }
        }
    }

    bwxJSON bwxJSON::Flatten(const wxString& separator) const
    {
        bwxJSON flatJson;

        std::function<void(const wxString&, const bwxJSON&, bwxJSON&)> flattenHelper =
            [&](const wxString& prefix, const bwxJSON& obj, bwxJSON& result)
            {
                for (const auto& [key, value] : obj.m_data)
                {
                    wxString newKey = prefix.IsEmpty() ? key : prefix + separator + key;

                    if (value.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*value))
                    {
                        flattenHelper(newKey, *std::get<std::shared_ptr<bwxJSON>>(*value), result);
                    }
                    else
                    {
                        result.SetValue(newKey, value);
                    }
                }
            };

        flattenHelper("", *this, flatJson);
        return flatJson;
    }

    bwxJSON bwxJSON::Unflatten(const bwxJSON& flatJson, const wxString& separator)
    {
        bwxJSON unflattenedJson;

        for (const auto& [flatKey, value] : flatJson.m_data)
        {
            wxString key = flatKey;
            bwxJSON* current = &unflattenedJson;

            while (key.Contains(separator))
            {
                wxString parentKey = key.BeforeFirst(separator[0]);
                wxString restKey = key.AfterFirst(separator[0]);

                if (!current->HasKey(parentKey) || !current->GetValue(parentKey).has_value() ||
                    !std::holds_alternative<std::shared_ptr<bwxJSON>>(*current->GetValue(parentKey)))
                {
                    current->SetValue(parentKey, std::make_shared<bwxJSON>());
                }

                current = std::get<std::shared_ptr<bwxJSON>>(*current->GetValue(parentKey)).get();
                key = restKey;
            }

            current->SetValue(key, value);
        }

        return unflattenedJson;
    }

    std::vector<wxString> bwxJSON::Search(const wxString& key) const
    {
        std::vector<wxString> results;

        std::function<void(const wxString&, const bwxJSON&, std::vector<wxString>&)> searchHelper =
            [&](const wxString& prefix, const bwxJSON& obj, std::vector<wxString>& results)
            {
                for (const auto& [currentKey, value] : obj.m_data)
                {
                    wxString newPath = prefix.IsEmpty() ? currentKey : prefix + "." + currentKey;

                    if (currentKey == key)
                    {
                        results.push_back(newPath);
                    }

                    if (value.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*value))
                    {
                        searchHelper(newPath, *std::get<std::shared_ptr<bwxJSON>>(*value), results);
                    }
                }
            };

        searchHelper("", *this, results);
        return results;
    }

    std::vector<wxString> bwxJSON::SearchValue(const bwxJsonValue& targetValue) const
    {
        std::vector<wxString> results;

        std::function<void(const wxString&, const bwxJSON&, std::vector<wxString>&)> searchHelper =
            [&](const wxString& prefix, const bwxJSON& obj, std::vector<wxString>& results)
            {
                for (const auto& [currentKey, value] : obj.m_data)
                {
                    wxString newPath = prefix.IsEmpty() ? currentKey : prefix + "." + currentKey;

                    if (value == targetValue)
                    {
                        results.push_back(newPath);
                    }

                    if (value.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*value))
                    {
                        searchHelper(newPath, *std::get<std::shared_ptr<bwxJSON>>(*value), results);
                    }
                }
            };

        searchHelper("", *this, results);
        return results;
    }

    bwxJSON bwxJSON::Filter(std::function<bool(const wxString&, const bwxJsonValue&)> predicate) const
    {
        bwxJSON filteredJson;

        for (const auto& [key, value] : m_data)
        {
            if (predicate(key, value))
            {
                filteredJson.SetValue(key, value);
            }
            else if (value.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*value))
            {
                auto nested = std::get<std::shared_ptr<bwxJSON>>(*value)->Filter(predicate);
                if (!nested.m_data.empty())
                {
                    filteredJson.SetValue(key, std::make_shared<bwxJSON>(nested));
                }
            }
        }

        return filteredJson;
    }

    void bwxJSON::Transform(std::function<void(wxString&, bwxJsonValue&)> rule)
    {
        std::unordered_map<wxString, bwxJsonValue> newData;

        for (auto& [key, value] : m_data)
        {
            wxString newKey = key;
            bwxJsonValue newValue = value;

            rule(newKey, newValue);

            if (newValue.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*newValue))
            {
                std::get<std::shared_ptr<bwxJSON>>(*newValue)->Transform(rule);
            }

            newData[newKey] = newValue;
        }

        m_data = std::move(newData);
    }

    bwxJSON bwxJSON::Map(std::function<bwxJsonValue(const wxString&, const bwxJsonValue&)> transform) const
    {
        bwxJSON mappedJson;

        for (const auto& [key, value] : m_data)
        {
            if (value.has_value() && std::holds_alternative<std::shared_ptr<bwxJSON>>(*value))
            {
                auto mappedNested = std::get<std::shared_ptr<bwxJSON>>(*value)->Map(transform);
                mappedJson.SetValue(key, std::make_shared<bwxJSON>(mappedNested));
            }
            else
            {
                mappedJson.SetValue(key, transform(key, value));
            }
        }

        return mappedJson;
    }

}
