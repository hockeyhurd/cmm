/**
 * Represents a snapshot in the lexer.
 *
 * @author hockeyhurd
 * @version 2022-05-14
 */

#pragma once

#ifndef CMM_SNAPSHOT_H
#define CMM_SNAPSHOT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Location.h>

// std includes
#include <cstdlib>

namespace cmm
{
    class Snapshot
    {
    public:
        Snapshot(const std::size_t index, const Location& location);
        Snapshot(const Snapshot&) CMM_NOEXCEPT = default;
        Snapshot(Snapshot&&) CMM_NOEXCEPT = default;
        ~Snapshot() = default;

        Snapshot& operator= (const Snapshot&) CMM_NOEXCEPT = default;
        Snapshot& operator= (Snapshot&&) CMM_NOEXCEPT = default;

        std::size_t getIndex() const CMM_NOEXCEPT;
        Location& getLocation() CMM_NOEXCEPT;
        const Location& getLocation() const CMM_NOEXCEPT;

    private:
        std::size_t index;
        Location location;
    };
}

#endif //!CMM_SNAPSHOT_H

