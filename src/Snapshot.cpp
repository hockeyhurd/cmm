#include <cmm/Snapshot.h>

namespace cmm
{
    Snapshot::Snapshot(const std::size_t index, const Location& location) : index(index)
    {
    }

    std::size_t Snapshot::getIndex() const CMM_NOEXCEPT
    {
        return index;
    }

    Location& Snapshot::getLocation() CMM_NOEXCEPT
    {
        return location;
    }

    const Location& Snapshot::getLocation() const CMM_NOEXCEPT
    {
        return location;
    }
}

