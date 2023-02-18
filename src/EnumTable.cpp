/**
 * A class for storing all available enums.
 *
 * @author hockeyhurd
 * @version 2023-02-03
 */

// Our includes
#include <cmm/EnumTable.h>

// std includes
#include <sstream>

namespace cmm
{
    EnumData::EnumData(std::unordered_map<std::string, Enumerator>&& enumeratorMap) :
        enumeratorMap(std::move(enumeratorMap)), name(nullptr)
    {
    }

    bool EnumData::findEnumerator(const std::string& name) const CMM_NOEXCEPT
    {
        return enumeratorMap.find(name) != enumeratorMap.cend();
    }

    bool EnumTable::empty() const CMM_NOEXCEPT
    {
        return enumMap.empty();
    }

    EnumTable::EnumMap::size_type EnumTable::size() const CMM_NOEXCEPT
    {
        return enumMap.size();
    }

    void EnumTable::clear() CMM_NOEXCEPT
    {
        enumMap.clear();
    }

    EnumData* EnumTable::addOrUpdate(const std::string& name, EnumData&& data, std::string* reason)
    {
        EnumData* result;
        const auto findEnumResult = enumMap.find(name);

        if (findEnumResult != enumMap.end())
        {
            findEnumResult->second = std::move(data);
            result = &findEnumResult->second;
            return result;
        }

        // Now need to check to see if individual defined enumerators could cause a redefinition.
        // ex. enum A { X, Y }; enum B { X, Z }; would cause a redefinition on 'X'.
        for (const auto& [enumeratorName, enumerator] : data.enumeratorMap)
        {
            const auto findEnumeratorNameResult  = enumeratorNameSet.find(enumeratorName);

            // enumerator already exists
            if (findEnumeratorNameResult != enumeratorNameSet.cend())
            {
                if (reason != nullptr)
                {
                    std::ostringstream os;
                    os << "enumerator with name '" << enumeratorName << "' is already previously defined";
                    *reason = os.str();
                }

                return nullptr;
            }

            // New enumerator, add it to our set.
            enumeratorNameSet.emplace(enumeratorName);
        }

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

        return result;
    }

    EnumData* EnumTable::addOrUpdate(std::string&& name, EnumData&& data, std::string* reason)
    {
        EnumData* result;
        const auto findEnumResult = enumMap.find(name);

        if (findEnumResult != enumMap.end())
        {
            findEnumResult->second = std::move(data);
            result = &findEnumResult->second;
            return result;
        }

        // Now need to check to see if individual defined enumerators could cause a redefinition.
        // ex. enum A { X, Y }; enum B { X, Z }; would cause a redefinition on 'X'.
        for (const auto& [enumeratorName, enumerator] : data.enumeratorMap)
        {
            const auto findEnumeratorNameResult  = enumeratorNameSet.find(enumeratorName);

            // enumerator already exists
            if (findEnumeratorNameResult != enumeratorNameSet.cend())
            {
                if (reason != nullptr)
                {
                    std::ostringstream os;
                    os << "enumerator with name '" << enumeratorName << "' is already previously defined";
                    *reason = os.str();
                }

                return nullptr;
            }

            // New enumerator, add it to our set.
            enumeratorNameSet.emplace(enumeratorName);
        }

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

