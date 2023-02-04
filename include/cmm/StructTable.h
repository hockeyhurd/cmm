/**
 * A class for storing all available structs, declared or defined.
 *
 * @author hockeyhurd
 * @version 2022-10-13
 */

#pragma once

#ifndef CMM_STRUCT_TABLE_H
#define CMM_STRUCT_TABLE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Field.h>

// std includes
#include <string>
#include <unordered_map>

namespace cmm
{
    struct StructData
    {
        EnumSymState symState;
        std::unordered_map<std::string, Field> fieldMap;
        const std::string* name;

        StructData(const EnumSymState symState, std::unordered_map<std::string, Field>&& fieldMap = {});
        StructData(const StructData&) = delete;
        StructData(StructData&&) CMM_NOEXCEPT = default;
        ~StructData() = default;

        StructData& operator= (const StructData&) = delete;
        StructData& operator= (StructData&&) CMM_NOEXCEPT = default;

        /**
         * Helper function for looking up fields in the internal fieldMap.
         *
         * @param name the std::string name of the field to find.
         * @return pointer to the Field if found, else nullptr.
         */
        IField* findField(const std::string& name) CMM_NOEXCEPT;

        /**
         * Helper function for looking up fields in the internal fieldMap.
         *
         * @param name the std::string name of the field to find.
         * @return const pointer to the Field if found, else nullptr.
         */
        const IField* findField(const std::string& name) const CMM_NOEXCEPT;
    };

    class StructTable
    {
    public:

        using Map = std::unordered_map<std::string, StructData>;
        using iterator = Map::iterator;
        using const_iterator = Map::const_iterator;

    public:

        /**
         * Default constructor.
         */
        StructTable() = default;

        /**
         * Deleted copy constructor.
         */
        StructTable(const StructTable&) = delete;

        /**
         * Default move constructor.
         */
        StructTable(StructTable&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        ~StructTable() = default;

        /**
         * Deleted copy constructor.
         */
        StructTable& operator= (const StructTable&) = delete;

        /**
         * Default move constructor.
         */
        StructTable& operator= (StructTable&&) CMM_NOEXCEPT = default;

        /**
         * Gets whether the table is empty or note.
         *
         * @return bool.
         */
        bool empty() const CMM_NOEXCEPT;

        /**
         * Gets the size of the table.
         *
         * @return Map::size_type.
         */
        Map::size_type size() const CMM_NOEXCEPT;

        /**
         * Clears the underlying table.
         */
        void clear() CMM_NOEXCEPT;

        /**
         * Adds the struct to the table.  If an entry already exists, the EnumSymState
         * will be updated to the passed value.
         *
         * @param name the std::string name of the struct.
         * @param data the struct's data (name, field, etc.).
         * @return pointer to the added or updated StructData if successful, else nullptr.
         */
        StructData* addOrUpdate(const std::string& name, StructData&& data);

        /**
         * Adds the struct to the table.  If an entry already exists, the EnumSymState
         * will be updated to the passed value.
         *
         * @param name the std::string name of the struct.
         * @param data the struct's data (name, field, etc.).
         * @return pointer to the added or updated StructData if successful, else nullptr.
         */
        StructData* addOrUpdate(std::string&& name, StructData&& data);

        /**
         * Gets the EnumSymState if struct is in the table by name.
         *
         * @param name the std::string name of the struct to lookup.
         * @return pointer to the StructData if found, else nullptr.
         */
        StructData* get(const std::string& name) CMM_NOEXCEPT;

        /**
         * Gets the EnumSymState if struct is in the table by name.
         *
         * @param name the std::string name of the struct to lookup.
         * @return const a pointer to the StructData if found, else nullptr.
         */
        const StructData* get(const std::string& name) const CMM_NOEXCEPT;

        /**
         * Gets the EnumSymState if struct is in the table by name.
         *
         * @param name the std::string name of the struct to lookup.
         * @return bool true if found, else false.
         */
        bool has(const std::string& name) const CMM_NOEXCEPT;

    private:

        // The underlying map
        Map map;
    };
}

#endif //!CMM_STRUCT_TABLE_H

