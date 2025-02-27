///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_json.h
// Purpose:     BWX_SDK Library; JSON parsing utilities
// Author:      Bartosz Warzocha
// Created:     2023-12-25
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWXJSON_H_
#define _BWXJSON_H_

#include <wx/string.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/wfstream.h>
#include <wx/txtstrm.h>
#include <wx/log.h>

#include <unordered_map>
#include <vector>
#include <variant>
#include <memory>
#include <optional>

/**
 * @file bwx_json.h
 * @brief Provides JSON parsing utilities for the BWXSDK library.
 */

namespace bwx_sdk {

    class bwxJSON; // Forward declarations
    struct bwxJsonValueHelper;

    /**
     * @brief Alias for the JSON value type.
     *
     * This type represents a JSON value and can be one of the following:
     * - std::nullptr_t for null values.
     * - int, int64_t, uint64_t, double, bool for numeric and boolean values.
     * - std::string or wxString for textual data.
     * - std::shared_ptr<bwxJSON> for nested JSON objects.
     * - std::vector<bwxJsonValueHelper> for JSON arrays.
     *
     * The value is wrapped in a std::optional, so an unset value is represented by std::nullopt.
     *
     * Example:
     * @code
     *   bwxJsonValue value = std::string("example");
     *   if (value.has_value() && std::holds_alternative<std::string>(*value)) {
     *       std::string s = std::get<std::string>(*value);
     *   }
     * @endcode
     */
    using bwxJsonValue = std::optional<std::variant<
        std::nullptr_t,
        int,
        int64_t,
        uint64_t,
        double,
        bool,
        std::string,
        wxString,
        std::shared_ptr<bwxJSON>,
        std::vector<bwxJsonValueHelper>
    >>;

    /**
     * @brief Helper structure for recursive variant type.
     */
    struct bwxJsonValueHelper {
        bwxJsonValue value;

        /**
         * @brief Equality operator for bwxJsonValueHelper.
         * @param other The other helper to compare with.
         * @return true if the underlying values are equal, false otherwise.
         */
        bool operator==(const bwxJsonValueHelper& other) const {
            return value == other.value;
        }
    };

    /**
     * @brief A lightweight JSON parser and serializer.
     *
     * The bwxJSON class provides functionality to parse, manipulate, and serialize JSON data.
     * It supports loading from files or streams, accessing and modifying data by key,
     * handling nested objects and arrays, and many utility functions.
     *
     * Example:
     * @code
     *   // Parsing JSON from a string:
     *   wxString jsonStr = "{\"name\": \"Alice\", \"age\": 30}";
     *   bwxJSON json(jsonStr);
     *
     *   // Accessing a value:
     *   auto nameValue = json.GetValue("name");
     *   if (nameValue.has_value() && std::holds_alternative<wxString>(*nameValue)) {
     *       wxString name = std::get<wxString>(*nameValue);
     *       wxLogMessage("Name: %s", name);
     *   }
     * @endcode
     */
    class bwxJSON
    {
    public:

		/**
		 * @brief Default constructor.
		 */
        bwxJSON() = default;
        
		/**
		* @brief Copy constructor.
        */
        explicit bwxJSON(const wxString& jsonText);
        
		/**
		* @brief Copy constructor.
		*/
        explicit bwxJSON(const bwxJsonValue& defaultValue);
        
        /**
		* @brief Copy constructor.
		*/
        bwxJSON(const bwxJSON& other);
        
        /**
		* @brief Move constructor.
		*/
        bwxJSON(bwxJSON&& other) noexcept;

        /**
         * @brief Loads JSON data from a file.
         * @param filename The path to the file.
         * @return true if successful, false otherwise.
         *
         * Example:
         * @code
         *   bwxJSON json;
         *   if (!json.LoadFromFile("data.json"))
         *       wxLogError("Failed to load JSON from file");
         * @endcode
         */
        bool LoadFromFile(const wxString& filename);

		/**
		 * @brief Saves JSON data to a file.
		 * @param filename The path to the file.
		 * @return true if successful, false otherwise.
		 *
		 * Example:
		 * @code
		 *   bwxJSON json;
		 *   json.SetValue("name", "Alice");
		 *   json.SetValue("age", 30);
		 *   if (!json.SaveToFile("data.json"))
		 *       wxLogError("Failed to save JSON to file");
		 * @endcode
		 */
        bool SaveToFile(const wxString& filename) const;

		/**
		 * @brief Loads JSON data from a stream.
		 * @param stream The input stream.
		 * @return true if successful, false otherwise.
		 *
		 * Example:
		 * @code
		 *   wxFileInputStream stream("data.json");
		 *   bwxJSON json;
		 *   if (!json.LoadFromStream(stream))
		 *       wxLogError("Failed to load JSON from stream");
		 * @endcode
		 */
        bool LoadFromStream(wxInputStream& stream);

        /**
         * @brief Saves JSON data to a stream.
         * @param stream The output stream.
         * @return true if successful, false otherwise.
         */
        bool SaveToStream(wxOutputStream& stream) const;

		/**
		 * @brief Parses JSON data from a string.
		 * @param jsonText The JSON text to parse.
		 * @return true if successful, false otherwise.
		 */
        bool ParseFromString(const wxString& jsonText);

        /**
         * @brief Serializes the JSON object to a formatted string.
         * @return A string representing the JSON data.
         */
        wxString SerializeToString() const;

		/**
		 * @brief Serializes the JSON object to a compact string.
		 * @return A string representing the JSON data.
		 */
        bool HasKey(const wxString& key) const;
        
        /**
         * @brief Serializes the JSON object to a formatted string.
         * @return A string representing the JSON data.
         */
        void RemoveKey(const wxString& key);

		/**
		 * @brief Serializes the JSON object to a formatted string.
		 * @return A string representing the JSON data.
		 */
        std::vector<wxString> GetKeys() const;

        /**
         * @brief Sets the value for a given key.
         * @param key The key.
         * @param value The JSON value to assign.
         */
        void SetValue(const wxString& key, const bwxJsonValue& value);

        /**
         * @brief Retrieves the value associated with a key.
         * @param key The key.
         * @param defaultValut The default value if the key does not exist.
         * @return The JSON value, or defaultValut if the key is not found.
         */
        bwxJsonValue GetValue(const wxString& key, const bwxJsonValue& defaultValut = std::nullopt) const;

		/**
		 * @brief Finds a value by key in the JSON object.
		 * @param key The key to search for.
		 * @return The JSON value, or std::nullopt if not found.
		 */
        bwxJsonValue FindValue(const wxString& key) const;

        /**
         * @brief Appends a value to an array identified by a key.
         * @param key The key identifying the array.
         * @param value The value to append.
         *
         * If the key does not exist, an array is created.
         */
        void AppendToArray(const wxString& key, const bwxJsonValue& value);

		/**
		 * @brief Retrieves an array of values associated with a key.
		 * @param key The key.
		 * @return A vector of JSON values.
		 */
        std::vector<bwxJsonValue> GetArray(const wxString& key) const;

		/**
		 * @brief Removes an element from an array identified by a key.
		 * @param key The key identifying the array.
		 * @param index The index of the element to remove.
		 */
        void RemoveFromArray(const wxString& key, size_t index);

		/**
		 * @brief Retrieves the number of key-value pairs in the JSON object.
		 * @return The number of key-value pairs.
		 */
        size_t GetSize() const;

        /**
         * @brief Retrieves the key at a given index.
         * @param index The index (order is not guaranteed since the underlying container is unordered).
         * @return The key as a wxString, or an empty string if index is out of bounds.
         */
        wxString GetKey(size_t index) const;

		/**
		 * @brief Sorts the keys in the JSON object.
		 *
		 * This function sorts the keys in the JSON object in lexicographical order.
		 */
        void SortKeys();

		/**
		 * @brief Checks if the JSON object is valid.
		 * @return true if the object is valid (not empty), false otherwise.
		 */
        bool IsValid() const;

        /**
         * @brief Serializes the JSON object to a compact string (no extra whitespace).
         * @return The compact JSON string.
         */
        wxString SerializeCompact() const;

		/**
		 * @brief Serializes the JSON object to a pretty formatted string.
		 * @param indentLevel The number of spaces for each indentation level.
		 * @return The formatted JSON string.
		 */
        wxString SerializePretty(int indentLevel = 4) const;

		/**
		 * @brief Retrieves the last error message from the JSON parser.
		 * @return The error message.
		 */
        wxString GetLastError() const { return lastError; }

        //

        /**
         * @brief Returns the value associated with a key as an int.
         * @param key The key.
         * @param defaultValue The default value if conversion fails.
         * @return The int value.
         */
        int AsInt(const wxString& key, int defaultValue = 0) const;

		/**
		 * @brief Returns the value associated with a key as an int64_t.
		 * @param key The key.
		 * @param defaultValue The default value if conversion fails.
		 * @return The int64_t value.
		 */
        int64_t AsInt64(const wxString& key, int64_t defaultValue = 0) const;
        
        /**
         * @brief Returns the value associated with a key as a uint64_t.
         * @param key The key.
         * @param defaultValue The default value if conversion fails.
         * @return The uint64_t value.
         */
        uint64_t AsUInt64(const wxString& key, uint64_t defaultValue = 0) const;

		/**
		 * @brief Returns the value associated with a key as a double.
		 * @param key The key.
		 * @param defaultValue The default value if conversion fails.
		 * @return The double value.
		 */
        double AsDouble(const wxString& key, double defaultValue = 0.0) const;

        /**
         * @brief Returns the value associated with a key as a uint64_t.
         * @param key The key.
         * @param defaultValue The default value if conversion fails.
         * @return The uint64_t value.
         */
        bool AsBool(const wxString& key, bool defaultValue = false) const;

		/**
		 * @brief Returns the value associated with a key as a wxString.
		 * @param key The key.
		 * @param defaultValue The default value if conversion fails.
		 * @return The wxString value.
		 */
        wxString AsWxString(const wxString& key, const wxString& defaultValue = "") const;

		/**
		 * @brief Returns the value associated with a key as a std::string.
		 * @param key The key.
		 * @param defaultValue The default value if conversion fails.
		 * @return The std::string value.
		 */
        std::string AsStdString(const wxString& key, const std::string& defaultValue = "") const;

        //

		/**
		 * @brief Returns the value associated with a key as a string.
		 * @param key The key.
		 * @param defaultValue The default value if conversion fails.
		 * @return The string value.
		 */
        using iterator = std::unordered_map<wxString, bwxJsonValue>::iterator;
        using const_iterator = std::unordered_map<wxString, bwxJsonValue>::const_iterator;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

        //

        /**
         * @brief Returns the value associated with a key as a uint64_t.
         * @param key The key.
         * @param defaultValue The default value if conversion fails.
         * @return The uint64_t value.
         */
        bool IsNull(const wxString& key) const;

        /**
         * @brief Checks if the value associated with a key is of type T.
         * @tparam T The type to check.
         * @param key The key.
         * @return true if the value is of type T, false otherwise.
         *
         * Example:
         * @code
         *   if (json.IsType<int>("age")) { ... }
         * @endcode
         */
        template <typename T>
        bool IsType(const wxString& key) const
        {
            if (!HasKey(key)) return false;
            const auto& value = GetValue(key);
            return value.has_value() && std::holds_alternative<T>(*value);
        }

        //

        /**
         * @brief Creates a deep copy (clone) of the JSON object.
         * @return A shared pointer to the cloned bwxJSON object.
         *
         * Example:
         * @code
         *   auto cloned = json.Clone();
         * @endcode
         */
        std::shared_ptr<bwxJSON> Clone() const;

        //

		/**
		 * @brief Assignment operator.
		 * @param other The JSON object to assign.
		 * @return A reference to this object.
		 */
        bwxJSON& operator=(const bwxJSON& other);

		/**
		 * @brief Equality operator.
		 * @param other The JSON object to compare with.
		 * @return true if the objects are equal, false otherwise.
		 */
        bool operator==(const bwxJSON& other) const;

        /**
         * @brief Inequality operator.
         * @param other The JSON object to compare with.
         * @return true if not equal, false otherwise.
         */
        bool operator!=(const bwxJSON& other) const;

        /**
         * @brief Subscript operator for non-const access.
         * @param key The key.
         * @return A reference to the JSON value.
         *
         * Example:
         * @code
         *   json["name"] = "Alice";
         * @endcode
         */
        bwxJsonValue& operator[](const wxString& key);

        /**
         * @brief Subscript operator for const access.
         * @param key The key.
         * @return A const reference to the JSON value.
         *
         * Example:
         * @code
         *   auto val = json["age"];
         * @endcode
         */
        const bwxJsonValue& operator[](const wxString& key) const;

        //

		/**
		 * @brief Merges another JSON object into this one.
		 * @param other The JSON object to merge.
		 * @param overwriteExisting Whether to overwrite existing keys.
		 *
		 * Example:
		 * @code
		 *   bwxJSON json1("{\"name\": \"Alice\"}");
		 *   bwxJSON json2("{\"age\": 30}");
		 *   json1.Merge(json2);
		 * @endcode
		 */
        void Merge(const bwxJSON& other, bool overwriteExisting = true);

		/**
		 * @brief Computes the difference between two JSON objects.
		 * @param other The other JSON object.
		 * @return A new JSON object containing the differences.
		 *
		 * Example:
		 * @code
		 *   bwxJSON diff = json1.Diff(json2);
		 * @endcode
		 */
        bwxJSON Diff(const bwxJSON& other) const;

        /**
         * @brief Applies a patch (set of changes) to the JSON object.
         * @param patchData The JSON object containing changes.
         *
         * Example:
         * @code
         *   json.Patch(patch);
         * @endcode
         */
        void Patch(const bwxJSON& patchData);

        /**
         * @brief Flattens a nested JSON object into a single-level JSON with dot-separated keys.
         * @param separator The separator used between keys (default is ".").
         * @return A new flattened bwxJSON object.
         *
         * Example:
         * @code
         *   bwxJSON flat = json.Flatten();
         * @endcode
         */
        bwxJSON Flatten(const wxString& separator = ".") const;

		/**
		 * @brief Unflattens a single-level JSON object into a nested JSON object.
		 * @param flatJson The flattened JSON object.
		 * @param separator The separator used between keys (default is ".").
		 * @return A new unflattened bwxJSON object.
		 *
		 * Example:
		 * @code
		 *   bwxJSON nested = bwxJSON::Unflatten(flat);
		 * @endcode
		 */
        static bwxJSON Unflatten(const bwxJSON& flatJson, const wxString& separator = ".");

        /**
         * @brief Searches for a key in the JSON object recursively.
         * @param key The key to search for.
         * @return A vector of paths (in dot notation) where the key was found.
         *
         * Example:
         * @code
         *   auto paths = json.Search("name");
         * @endcode
         */
        std::vector<wxString> Search(const wxString& key) const;

		/**
		 * @brief Searches for a value in the JSON object recursively.
		 * @param value The value to search for.
		 * @return A vector of paths (in dot notation) where the value was found.
		 *
		 * Example:
		 * @code
		 *   auto paths = json.SearchValue("Alice");
		 * @endcode
		 */
        std::vector<wxString> SearchValue(const bwxJsonValue& value) const;

        /**
         * @brief Filters the JSON object based on a predicate.
         * @param predicate A function that receives a key and value and returns true if the element should be kept.
         * @return A new bwxJSON containing only elements that satisfy the predicate.
         *
         * Example:
         * @code
         *   bwxJSON filtered = json.Filter([](const wxString& key, const bwxJsonValue& value) {
         *       return key.StartsWith("a");
         *   });
         * @endcode
         */
        bwxJSON Filter(std::function<bool(const wxString&, const bwxJsonValue&)> predicate) const;

        /**
         * @brief Transforms the JSON object by applying a transformation function to every key-value pair.
         * @param rule A function that receives a key (by reference) and a value (by reference) and can modify them.
         *
         * Example:
         * @code
         *   json.Transform([](wxString& key, bwxJsonValue& value) {
         *       key.MakeUpper();
         *   });
         * @endcode
         */
        void Transform(std::function<void(wxString&, bwxJsonValue&)> rule);

        /**
         * @brief Maps the JSON object to a new JSON object by transforming each key-value pair.
         * @param transform A function that takes a key and value and returns a new JSON value.
         * @return A new bwxJSON with transformed values.
         *
         * Example:
         * @code
         *   bwxJSON mapped = json.Map([](const wxString& key, const bwxJsonValue& value) -> bwxJsonValue {
         *       if (key == "age" && value.has_value() && std::holds_alternative<int>(*value))
         *           return std::get<int>(*value) * 2;
         *       return value;
         *   });
         * @endcode
         */
        bwxJSON Map(std::function<bwxJsonValue(const wxString&, const bwxJsonValue&)> transform) const;


    private:
		std::unordered_map<wxString, bwxJsonValue> data; ///< The JSON data.

		wxString lastError; ///< The last error message.

        /**
         * @brief Escapes a string for JSON output.
         * @param str The input string.
         * @return The escaped string enclosed in quotes.
         *
         * Example:
         * @code
         *   wxString escaped = EscapeString("Hello \"World\"");
         *   // Result: "Hello \"World\""
         * @endcode
         */
        wxString EscapeString(const wxString& str) const;

		/**
		 * @brief Converts a JSON value to a string.
		 * @param value The JSON value.
		 * @return The string representation of the value.
		 */
        wxString JsonValueToString(const bwxJsonValue& value) const;

        /**
         * @brief Skips whitespace and comments in the JSON text.
         * @param text The JSON text.
         * @param start The starting index.
         * @return The index of the first non-whitespace (and non-comment) character.
         */
        size_t SkipWhitespace(const wxString& text, size_t start) const;

		/**
		 * @brief Parses a JSON value from a string.
		 * @param text The JSON text.
		 * @param pos The starting position.
		 * @return The parsed JSON value.
		 */
        bwxJsonValue ParseValue(const wxString& text, size_t& pos);

		/**
		 * @brief Parses a JSON string from a string.
		 * @param text The JSON text.
		 * @param pos The starting position.
		 * @return The parsed JSON string.
		 */
        wxString ParseString(const wxString& text, size_t& pos);

        /**
         * @brief Parses a JSON number.
         * @param text The JSON text.
         * @param pos The current position (will be updated past the number).
         * @return The parsed numeric value.
         */
        bwxJsonValue ParseNumber(const wxString& text, size_t& pos);

        /**
         * @brief Parses a JSON object.
         * @param text The JSON text.
         * @param pos The current position (will be updated past the object).
         * @return The parsed object as a shared pointer to bwxJSON.
         */
        bwxJsonValue ParseObject(const wxString& text, size_t& pos);

		/**
		 * @brief Parses a JSON array.
		 * @param text The JSON text.
		 * @param pos The current position (will be updated past the array).
		 * @return The parsed array as a vector of JSON values.
		 */
        bwxJsonValue ParseArray(const wxString& text, size_t& pos);
    };

}

#endif
