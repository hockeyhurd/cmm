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

    Enumerator* EnumData::findEnumerator(const std::string& name) CMM_NOEXCEPT
    {
        auto findResult = enumeratorMap.find(name);
        return findResult != enumeratorMap.cend() ? &findResult->second : nullptr;
    }

    const Enumerator* EnumData::findEnumerator(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = enumeratorMap.find(name);
        return findResult != enumeratorMap.cend() ? &findResult->second : nullptr;
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
        std::string nameCopy = name;
        return addOrUpdate(std::move(nameCopy), std::move(data), reason);
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

        // We insert first so that we can capture the pointer to the inserted/emplaced EnumTable.
        // Note: We will need to make sure to remove this entry later if this function were
        // to return 'unsuccessfully'.
        auto [iter, wasInserted] = enumMap.emplace(std::move(name), std::move(data));
        iter->second.name = &iter->first;

        if (!wasInserted)
        {
            return nullptr;
        }

        result = &iter->second;

        // Now need to check to see if individual defined enumerators could cause a redefinition.
        // ex. enum A { X, Y }; enum B { X, Z }; would cause a redefinition on 'X'.
        // for (const auto& [enumeratorName, enumerator] : data.enumeratorMap)
        for (const auto& [enumeratorName, enumerator] : result->enumeratorMap)
        {
            const auto findEnumeratorNameResult  = enumeratorNameMap.find(enumeratorName);

            // enumerator already exists
            if (findEnumeratorNameResult != enumeratorNameMap.cend())
            {
                if (reason != nullptr)
                {
                    std::ostringstream os;
                    os << "enumerator with name '" << enumeratorName << "' is already previously defined";
                    *reason = os.str();
                }

                // Perform the undo as previously mentioned before returning error/nullptr.
                enumMap.erase(iter);

                return nullptr;
            }

            // New enumerator, add it to our set.
            enumeratorNameMap.emplace(enumeratorName, result);
        }

        return result;
    }

    EnumData* EnumTable::get(const std::string& name)
    {
        const auto findResult = enumMap.find(name);
        return findResult != enumMap.cend() ? &findResult->second : nullptr;
    }

    const EnumData* EnumTable::get(const std::string& name) const
    {
        const auto findResult = enumMap.find(name);
        return findResult != enumMap.cend() ? &findResult->second : nullptr;
    }

    bool EnumTable::has(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = enumMap.find(name);
        return findResult != enumMap.cend();
    }

    EnumData* EnumTable::findEnumFromEnumeratorName(const std::string& name)
    {
        const auto findResult = enumeratorNameMap.find(name);
        return findResult != enumeratorNameMap.cend() ? findResult->second : nullptr;
    }

    const EnumData* EnumTable::findEnumFromEnumeratorName(const std::string& name) const
    {
        const auto findResult = enumeratorNameMap.find(name);
        return findResult != enumeratorNameMap.cend() ? findResult->second : nullptr;
    }
}

