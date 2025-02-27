///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_json.h
// Purpose:     BWX_SDK Library; JSON parsing utilities
// Author:      Bartosz Warzocha
// Created:     2023-12-25
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWXJSON_H_
#define _BWXJSON_H_

#include <wx/file.h>
#include <wx/log.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/wfstream.h>

#include <memory>
#include <optional>
#include <unordered_map>
#include <variant>
#include <vector>

namespace bwx_sdk {

class bwxJSON;  // Forward declarations
struct bwxJsonValueHelper;

using bwxJsonValue = std::optional<std::variant<std::nullptr_t, int, int64_t, uint64_t, double, bool, std::string,
                                                wxString, std::shared_ptr<bwxJSON>, std::vector<bwxJsonValueHelper>>>;

struct bwxJsonValueHelper {
    bwxJsonValue value;

    bool operator==(const bwxJsonValueHelper& other) const { return value == other.value; }
};

class bwxJSON {
public:
    bwxJSON() = default;

    explicit bwxJSON(const wxString& jsonText);

    explicit bwxJSON(const bwxJsonValue& defaultValue);

    bwxJSON(const bwxJSON& other);

    bwxJSON(bwxJSON&& other) noexcept;

    bool LoadFromFile(const wxString& filename);

    bool SaveToFile(const wxString& filename) const;

    bool LoadFromStream(wxInputStream& stream);

    bool SaveToStream(wxOutputStream& stream) const;

    bool ParseFromString(const wxString& jsonText);

    wxString SerializeToString() const;

    bool HasKey(const wxString& key) const;

    void RemoveKey(const wxString& key);

    std::vector<wxString> GetKeys() const;

    void SetValue(const wxString& key, const bwxJsonValue& value);

    bwxJsonValue GetValue(const wxString& key, const bwxJsonValue& defaultValut = std::nullopt) const;

    bwxJsonValue FindValue(const wxString& key) const;

    void AppendToArray(const wxString& key, const bwxJsonValue& value);

    std::vector<bwxJsonValue> GetArray(const wxString& key) const;

    void RemoveFromArray(const wxString& key, size_t index);

    size_t GetSize() const;

    wxString GetKey(size_t index) const;

    void SortKeys();

    bool IsValid() const;

    wxString SerializeCompact() const;

    wxString SerializePretty(int indentLevel = 4) const;

    wxString GetLastError() const { return lastError; }

    //

    int AsInt(const wxString& key, int defaultValue = 0) const;

    int64_t AsInt64(const wxString& key, int64_t defaultValue = 0) const;

    uint64_t AsUInt64(const wxString& key, uint64_t defaultValue = 0) const;

    double AsDouble(const wxString& key, double defaultValue = 0.0) const;

    bool AsBool(const wxString& key, bool defaultValue = false) const;

    wxString AsWxString(const wxString& key, const wxString& defaultValue = "") const;

    std::string AsStdString(const wxString& key, const std::string& defaultValue = "") const;

    //

    using iterator = std::unordered_map<wxString, bwxJsonValue>::iterator;
    using const_iterator = std::unordered_map<wxString, bwxJsonValue>::const_iterator;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    //

    bool IsNull(const wxString& key) const;

    template <typename T>
    bool IsType(const wxString& key) const {
        if (!HasKey(key)) return false;
        const auto& value = GetValue(key);
        return value.has_value() && std::holds_alternative<T>(*value);
    }

    //

    std::shared_ptr<bwxJSON> Clone() const;

    //

    bwxJSON& operator=(const bwxJSON& other);

    bool operator==(const bwxJSON& other) const;

    bool operator!=(const bwxJSON& other) const;

    bwxJsonValue& operator[](const wxString& key);

    const bwxJsonValue& operator[](const wxString& key) const;

    //

    void Merge(const bwxJSON& other, bool overwriteExisting = true);

    bwxJSON Diff(const bwxJSON& other) const;

    void Patch(const bwxJSON& patchData);

    bwxJSON Flatten(const wxString& separator = ".") const;

    static bwxJSON Unflatten(const bwxJSON& flatJson, const wxString& separator = ".");

    std::vector<wxString> Search(const wxString& key) const;

    std::vector<wxString> SearchValue(const bwxJsonValue& value) const;

    bwxJSON Filter(std::function<bool(const wxString&, const bwxJsonValue&)> predicate) const;

    void Transform(std::function<void(wxString&, bwxJsonValue&)> rule);

    bwxJSON Map(std::function<bwxJsonValue(const wxString&, const bwxJsonValue&)> transform) const;

private:
    std::unordered_map<wxString, bwxJsonValue> data;

    wxString lastError;

    wxString EscapeString(const wxString& str) const;

    wxString JsonValueToString(const bwxJsonValue& value) const;

    size_t SkipWhitespace(const wxString& text, size_t start) const;

    bwxJsonValue ParseValue(const wxString& text, size_t& pos);

    wxString ParseString(const wxString& text, size_t& pos);

    bwxJsonValue ParseNumber(const wxString& text, size_t& pos);

    bwxJsonValue ParseObject(const wxString& text, size_t& pos);

    bwxJsonValue ParseArray(const wxString& text, size_t& pos);
};

}  // namespace bwx_sdk

#endif
