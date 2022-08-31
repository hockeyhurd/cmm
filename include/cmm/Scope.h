/**
 * A class representing a C scope data structure.
 *
 * @author hockeyhurd
 * @version 2022-08-28
 */

#pragma once

#ifndef CMM_SCOPE_H
#define CMM_SCOPE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Frame.h>

// std includes
#include <optional>
#include <string>
#include <vector>

namespace cmm
{
    class Scope
    {
    public:

        /**
         * Default constructor.
         */
        Scope();

        /**
         * Copy constructor.
         */
        Scope(const Scope&) = delete;

        /**
         * Move constructor.
         */
        Scope(Scope&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~Scope() = default;

        /**
         * Copy assignment operator.
         *
         * @return Scope reference.
         */
        Scope& operator= (const Scope&) = delete;

        /**
         * Move assignment operator.
         *
         * @return Scope reference.
         */
        Scope& operator= (Scope&&) CMM_NOEXCEPT = default;

        /**
         * Gets a reference to the current Frame.
         */
        Frame& getCurrentFrame() CMM_NOEXCEPT;

        /**
         * Gets a const reference to the current Frame.
         */
        const Frame& getCurrentFrame() const CMM_NOEXCEPT;

        /**
         * Pushes a new frame onto the stack.
         *
         * @param canSeeParent flag for whether the new frame can see
         *        the previous frame's information.
         */
        void push(const bool canSeeParent);

        /**
         * Pops a frame off the stack.
         */
        void pop();

        // Convenience functions to call directly into the top Frame:

        /**
         * Adds the variable to the frame.
         *
         * @param variable the variable to add.
         * @param type the type of the variable.
         */
        void add(const std::string& variable, const EnumCType type);

        /**
         * Adds the variable to the frame.
         *
         * @param variable the variable to add.
         * @param type the type of the variable.
         */
        void add(std::string&& variable, const EnumCType type);

        /**
         * Attempts to lookup the variable in the frame.
         *
         * @param variable the variable to lookup.
         * @return optional EnumCType value if found, else std::nullopt.
         */
        std::optional<EnumCType> find(const std::string& variable) const;

        // End of convenience functions section.

    private:

        // A vector of scope based frames.
        std::vector<Frame> frames;
    };
}

#endif //!CMM_SCOPE_H
