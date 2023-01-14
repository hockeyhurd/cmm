/**
 * A class for storing all available structs, declared or defined.
 *
 * @author hockeyhurd
 * @version 2022-10-13
 */

// Our includes
#include <cmm/StructTable.h>

namespace cmm
{
    StructData::StructData(const EnumSymState symState, std::unordered_map<std::string, Field>&& fieldMap) :
        symState(symState), fieldMap(std::move(fieldMap))
    {
    }

    bool StructTable::empty() const CMM_NOEXCEPT
    {
        return map.empty();
    }

    StructTable::Map::size_type StructTable::size() const CMM_NOEXCEPT
    {
        return map.size();
    }

    void StructTable::addOrUpdate(const std::string& name, StructData&& data)
    {
        const auto findResult = map.find(name);

        if (findResult != map.end())
        {
            findResult->second = std::move(data);
        }

        else
        {
            [[maybe_unused]]
            auto [iter, wasInserted] = map.emplace(name, std::move(data));
            iter->second.name = &iter->first;
        }
    }

    void StructTable::addOrUpdate(std::string&& name, StructData&& data)
    {
        const auto findResult = map.find(name);

        if (findResult != map.end())
        {
            findResult->second = std::move(data);
        }

        else
        {
            [[maybe_unused]]
            auto [iter, wasInserted] = map.emplace(std::move(name), std::move(data));
            iter->second.name = &iter->first;
        }
    }

    StructData* StructTable::get(const std::string& name) CMM_NOEXCEPT
    {
        const auto findResult = map.find(name);
        return findResult != map.cend() ? &findResult->second : nullptr;
    }

    const StructData* StructTable::get(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = map.find(name);
        return findResult != map.cend() ? &findResult->second : nullptr;
    }

    bool StructTable::has(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = map.find(name);
        return findResult != map.cend();
    }
}

