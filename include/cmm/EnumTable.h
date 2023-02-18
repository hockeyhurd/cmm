/**
 * A class for storing all available enums.
 *
 * @author hockeyhurd
 * @version 2023-02-03
 */

#pragma once

#ifndef CMM_ENUM_TABLE_H
#define CMM_ENUM_TABLE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Enumerator.h>

// std includes
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace cmm
{
    struct EnumData
    {
        std::unordered_map<std::string, Enumerator> enumeratorMap;
        const std::string* name;

        explicit EnumData(std::unordered_map<std::string, Enumerator>&& enumeratorMap = {});
        EnumData(const EnumData&) = delete;
        EnumData(EnumData&&) CMM_NOEXCEPT = default;
        ~EnumData() = default;

        EnumData& operator= (const EnumData&) = delete;
        EnumData& operator= (EnumData&&) CMM_NOEXCEPT = default;

        /**
         * Helper function for looking up an enumerator in the internal enumeratorMap.
         *
         * @param name the std::string name of the enumerator to find.
         * @return bool true if found, else false.
         */
        bool findEnumerator(const std::string& name) const CMM_NOEXCEPT;
    };

    class EnumTable
    {
    public:

        // Useful typedefs
        using EnumMap = std::unordered_map<std::string, EnumData>;
        using iterator = EnumMap::iterator;
        using const_iterator = EnumMap::const_iterator;

        using EnumeratorNameSet = std::unordered_set<std::string>;

    public:

        /**
         * Default constructor.
         */
        EnumTable() = default;

        /**
         * Deleted copy constructor.
         */
        EnumTable(const EnumTable&) = delete;

        /**
         * Default move constructor.
         */
        EnumTable(EnumTable&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        ~EnumTable() = default;

        /**
         * Deleted copy constructor.
         */
        EnumTable& operator= (const EnumTable&) = delete;

        /**
         * Default move constructor.
         */
        EnumTable& operator= (EnumTable&&) CMM_NOEXCEPT = default;

        /**
         * Gets whether the table is empty or note.
         *
         * @return bool.
         */
        bool empty() const CMM_NOEXCEPT;

        /**
         * Gets the size of the table.
         *
         * @return EnumMap::size_type.
         */
        EnumMap::size_type size() const CMM_NOEXCEPT;

        /**
         * Clears the underlying table.
         */
        void clear() CMM_NOEXCEPT;

        /**
         * Adds the enum to the table.  If an entry already exists, the EnumData
         * will be updated to the passed value.
         *
         * @param name the std::string name of the enum.
         * @param data the enum's data (name, field, etc.).
         * @param reason optional std::string pointer to write an error message if unsuccessfull.
         * @return pointer to the added or updated EnumData if successful, else nullptr.
         */
        EnumData* addOrUpdate(const std::string& name, EnumData&& data, std::string* reason = nullptr);

        /**
         * Adds the enum to the table.  If an entry already exists, the EnumData
         * will be updated to the passed value.
         *
         * @param name the std::string name of the enum.
         * @param data the enum's data (name, field, etc.).
         * @param reason optional std::string pointer to write an error message if unsuccessfull.
         * @return pointer to the added or updated EnumData if successful, else nullptr.
         */
        EnumData* addOrUpdate(std::string&& name, EnumData&& data, std::string* reason = nullptr);

        /**
         * Gets the EnumData if enum is in the table by name.
         *
         * @param name the std::string name of the enum to lookup.
         * @return pointer to the EnumData if found, else nullptr.
         */
        EnumData* get(const std::string& name) CMM_NOEXCEPT;

        /**
         * Gets the EnumData if enum is in the table by name.
         *
         * @param name the std::string name of the enum to lookup.
         * @return const a pointer to the EnumData if found, else nullptr.
         */
        const EnumData* get(const std::string& name) const CMM_NOEXCEPT;

        /**
         * Gets the EnumData if enum is in the table by name.
         *
         * @param name the std::string name of the enum to lookup.
         * @return bool true if found, else false.
         */
        bool has(const std::string& name) const CMM_NOEXCEPT;

    private:

        // The underlying enum map
        EnumMap enumMap;

        // A table for enumerator names all.
        EnumeratorNameSet enumeratorNameSet;
    };
}

#endif //!CMM_ENUM_TABLE_H

