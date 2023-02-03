/**
 * A class for storing all available enums.
 *
 * @author hockeyhurd
 * @version 2023-02-03
 */

// Our includes
#include <cmm/EnumTable.h>

namespace cmm
{
    EnumData::EnumData(std::unordered_set<std::string>&& enumeratorSet) : enumeratorSet(std::move(enumeratorSet)),
        name(nullptr)
    {
    }

    bool EnumData::findEnumerator(const std::string& name) const CMM_NOEXCEPT
    {
        return enumeratorSet.find(name) != enumeratorSet.cend();
    }

    bool EnumTable::empty() const CMM_NOEXCEPT
    {
        return enumMap.empty();
    }

    EnumTable::EnumMap::size_type EnumTable::size() const CMM_NOEXCEPT
    {
        return enumMap.size();
    }

    EnumData* EnumTable::addOrUpdate(const std::string& name, EnumData&& data)
    {
        EnumData* result;
        const auto findResult = enumMap.find(name);

        if (findResult != enumMap.end())
        {
            findResult->second = std::move(data);
            result = &findResult->second;
        }

        else
        {
            auto [iter, wasInserted] = enumMap.emplace(name, std::move(data));
            iter->second.name = &iter->first;

            if (wasInserted)
            {
                result = &iter->second;
            }

            else
            {
                result = nullptr;
            }
        }

        return result;
    }

    EnumData* EnumTable::addOrUpdate(std::string&& name, EnumData&& data)
    {
        EnumData* result;
        const auto findResult = enumMap.find(name);

        if (findResult != enumMap.end())
        {
            findResult->second = std::move(data);
            result = &findResult->second;
        }

        else
        {
            auto [iter, wasInserted] = enumMap.emplace(std::move(name), std::move(data));
            iter->second.name = &iter->first;

            if (wasInserted)
            {
                result = &iter->second;
            }

            else
            {
                result = nullptr;
            }
        }

        return result;
    }

    EnumData* EnumTable::get(const std::string& name) CMM_NOEXCEPT
    {
        const auto findResult = enumMap.find(name);
        return findResult != enumMap.cend() ? &findResult->second : nullptr;
    }

    const EnumData* EnumTable::get(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = enumMap.find(name);
        return findResult != enumMap.cend() ? &findResult->second : nullptr;
    }

    bool EnumTable::has(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = enumMap.find(name);
        return findResult != enumMap.cend();
    }
}

