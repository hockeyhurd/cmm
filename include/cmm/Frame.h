/**
 * A class representinng a frame of reference.
 *
 * @author hockeyhurd
 * @version 2022-08-28
 */

#pragma once

#ifndef CMM_FRAME_H
#define CMM_FRAME_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <optional>
#include <string>
#include <unordered_map>

namespace cmm
{
    class Frame
    {
    public:

        using VarMap = std::unordered_map<std::string, EnumCType>;
        using iterator = VarMap::iterator;
        using const_iterator = VarMap::const_iterator;

    public:

        /**
         * Default constructor.
         */
        Frame() CMM_NOEXCEPT;

        /**
         * Constructor.
         *
         * @param the parent Frame pointer.
         */
        Frame(Frame* parent) CMM_NOEXCEPT;

        /**
         * Copy constructor.
         */
        Frame(const Frame&) = delete;

        /**
         * Move constructor.
         */
        Frame(Frame&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~Frame() = default;

        /**
         * Copy assignment operator.
         *
         * @return Frame reference.
         */
        Frame& operator= (const Frame&) = delete;

        /**
         * Move assignment operator.
         *
         * @return Frame reference.
         */
        Frame& operator= (Frame&&) CMM_NOEXCEPT = default;

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

        /**
         * The beginning of the frame.
         */
        iterator begin() CMM_NOEXCEPT;

        /**
         * The beginning of the frame.
         */
        const_iterator cbegin() const CMM_NOEXCEPT;

        /**
         * The end of the frame.
         */
        iterator end() CMM_NOEXCEPT;

        /**
         * The end of the frame.
         */
        const_iterator cend() const CMM_NOEXCEPT;

    private:

        // The parent frame.
        Frame* parent;

        // The variables in the current frame.
        VarMap variables;
    };
}

#endif //!CMM_FRAME_H
