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

// std includes
#include <optional>
#include <string>
#include <unordered_map>

namespace cmm
{
    class StructTable
    {
    public:

        using Map = std::unordered_map<std::string, EnumSymState>;
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
         * Adds the struct to the table.  If an entry already exists, the EnumSymState
         * will be updated to the passed value.
         *
         * @param name the std::string name of the struct.
         * @param state the EnumSymState of the struct.
         */
        void addOrUpdate(const std::string& name, const EnumSymState state);

        /**
         * Adds the struct to the table.  If an entry already exists, the EnumSymState
         * will be updated to the passed value.
         *
         * @param name the std::string name of the struct.
         * @param state the EnumSymState of the struct.
         */
        void addOrUpdate(std::string&& name, const EnumSymState);

        /**
         * Gets the EnumSymState if struct is in the table by name.
         *
         * @param name the std::string name of the struct to lookup.
         * @return optional EnumSymState if found, else std::nullopt.
         */
        std::optional<EnumSymState> get(const std::string& name) const CMM_NOEXCEPT;

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

