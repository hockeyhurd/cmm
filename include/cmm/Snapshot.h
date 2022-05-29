/**
 * Represents a snapshot in the lexer.
 *
 * @author hockeyhurd
 * @version 2022-05-14
 */

#pragma once

#ifndef CMM_SNAPSHOT_H
#define CMM_SNAPSHOT_H

#include <cstdlib>

namespace cmm
{
    class Snapshot
    {
    public:
        Snapshot(const std::size_t pos);
        Snapshot(const Snapshot&) noexcept = default;
        Snapshot(Snapshot&&) noexcept = default;
        ~Snapshot() = default;

        Snapshot& operator= (const Snapshot&) noexcept = default;
        Snapshot& operator= (Snapshot&&) noexcept = default;

        std::size_t getPosition() const noexcept;

    private:
        std::size_t pos;
    };
}

#endif //!CMM_SNAPSHOT_H

