#include <cmm/Snapshot.h>

namespace cmm
{
    Snapshot::Snapshot(const std::size_t pos) : pos(pos)
    {
    }

    std::size_t Snapshot::getPosition() const noexcept
    {
        return pos;
    }
}

