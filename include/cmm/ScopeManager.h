/**
 * A class representing a C scope data structure.
 *
 * @author hockeyhurd
 * @version 2022-08-28
 */

#pragma once

#ifndef CMM_SCOPE_MANAGER_H
#define CMM_SCOPE_MANAGER_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Frame.h>

// std includes
#include <string>
#include <vector>

namespace cmm
{
    // Forward declarations:
    class VariableContext;

    class ScopeManager
    {
    public:

        /**
         * Default constructor.
         */
        ScopeManager();

        /**
         * Copy constructor.
         */
        ScopeManager(const ScopeManager&) = delete;

        /**
         * Move constructor.
         */
        ScopeManager(ScopeManager&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~ScopeManager() = default;

        /**
         * Copy assignment operator.
         *
         * @return ScopeManager reference.
         */
        ScopeManager& operator= (const ScopeManager&) = delete;

        /**
         * Move assignment operator.
         *
         * @return ScopeManager reference.
         */
        ScopeManager& operator= (ScopeManager&&) CMM_NOEXCEPT = default;

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
         * @param context the context of the variable.
         */
        void add(const std::string& variable, const VariableContext& context);

        /**
         * Adds the variable to the frame.
         *
         * @param variable the variable to add.
         * @param context the context of the variable.
         */
        void add(std::string&& variable, const VariableContext& context);

        /**
         * Attempts to lookup the variable in the frame (only).
         *
         * @param variable the variable to lookup.
         * @return pointer to the VariableContext if found, else nullptr.
         */
        VariableContext* find(const std::string& variable);

        /**
         * Attempts to lookup the variable in the frame (only).
         *
         * @param variable the variable to lookup.
         * @return const pointer to the VariableContext if found, else nullptr.
         */
        const VariableContext* find(const std::string& variable) const;

        /**
         * Attempts to lookup the variable in the frame or parent frame (if applicable).
         *
         * @param variable the variable to lookup.
         * @return pointer to the VariableContext if found, else nullptr.
         */
        VariableContext* findAny(const std::string& variable);

        /**
         * Attempts to lookup the variable in the frame or parent frame (if applicable).
         *
         * @param variable the variable to lookup.
         * @return const pointer to the VariableContext if found, else nullptr.
         */
        const VariableContext* findAny(const std::string& variable) const;

        // End of convenience functions section.

    private:

        // A vector of scope based frames.
        std::vector<Frame> frames;
    };
}

#endif //!CMM_SCOPE_MANAGER_H

