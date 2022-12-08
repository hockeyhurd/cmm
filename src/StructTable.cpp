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
    bool StructTable::empty() const CMM_NOEXCEPT
    {
        return map.empty();
    }

    StructTable::Map::size_type StructTable::size() const CMM_NOEXCEPT
    {
        return map.size();
    }

    void StructTable::addOrUpdate(const std::string& name, const EnumSymState state)
    {
        map[name] = state;
    }

    void StructTable::addOrUpdate(std::string&& name, const EnumSymState state)
    {
        map[std::move(name)] = state;
    }

    std::optional<EnumSymState> StructTable::get(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = map.find(name);
        return findResult != map.cend() ? std::make_optional(findResult->second) : std::nullopt;
    }

    bool StructTable::has(const std::string& name) const CMM_NOEXCEPT
    {
        const auto findResult = map.find(name);
        return findResult != map.cend();
    }
}

