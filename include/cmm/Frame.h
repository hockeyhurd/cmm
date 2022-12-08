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
#include <cmm/StructOrUnionContext.h>
#include <cmm/VariableContext.h>

// std includes
#include <string>
#include <unordered_map>

namespace cmm
{
    class Frame
    {
    public:

        using StructOrUnionMap = std::unordered_map<std::string, StructOrUnionContext>;
        using VarMap = std::unordered_map<std::string, VariableContext>;

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
         * Adds the struct or union to the frame.
         *
         * @param name the name of the struct or union to add.
         * @param context the context of the struct or union.
         */
        void add(const std::string& name, const StructOrUnionContext& context);

        /**
         * Adds the struct or union to the frame.
         *
         * @param name the name of the struct or union to add.
         * @param context the context of the struct or union.
         */
        void add(std::string&& name, const StructOrUnionContext& context);

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
         * Attempts to lookup the struct or union type in the current frame (only).
         *
         * @param name the name of the struct or union to lookup.
         * @return pointer to the StructOrUnionContext if found, else nullptr.
         */
        StructOrUnionContext* findStructOrUnion(const std::string& name);

        /**
         * Attempts to lookup the struct or union type in the current frame (only).
         *
         * @param name the name of the struct or union to lookup.
         * @return const pointer to the StructOrUnionContext if found, else nullptr.
         */
        const StructOrUnionContext* findStructOrUnion(const std::string& name) const;

        /**
         * Attempts to lookup the struct or union type in the frame or parent frame (if applicable).
         *
         * @param name the struct or union type to lookup.
         * @return pointer to the StructOrUnionContext if found, else nullptr.
         */
        StructOrUnionContext* findAnyStructOrUnion(const std::string& name);

        /**
         * Attempts to lookup the struct or union type in the frame or parent frame (if applicable).
         *
         * @param name the struct or union type to lookup.
         * @return const pointer to the StructOrUnionContext if found, else nullptr.
         */
        const StructOrUnionContext* findAnyStructOrUnion(const std::string& name) const;

        /**
         * Attempts to lookup the variable in the current frame (only).
         *
         * @param variable the variable to lookup.
         * @return pointer to the VariableContext if found, else nullptr.
         */
        VariableContext* findVariable(const std::string& variable);

        /**
         * Attempts to lookup the variable in the frame (only).
         *
         * @param variable the variable to lookup.
         * @return const pointer to the VariableContext if found, else nullptr.
         */
        const VariableContext* findVariable(const std::string& variable) const;

        /**
         * Attempts to lookup the variable in the frame or parent frame (if applicable).
         *
         * @param variable the variable to lookup.
         * @return pointer to the VariableContext if found, else nullptr.
         */
        VariableContext* findAnyVariable(const std::string& variable);

        /**
         * Attempts to lookup the variable in the frame or parent frame (if applicable).
         *
         * @param variable the variable to lookup.
         * @return const pointer to the VariableContext if found, else nullptr.
         */
        const VariableContext* findAnyVariable(const std::string& variable) const;

    private:

        /**
         * Common struct or union find function.
         *
         * @param map reference to Map to lookup.
         * @param name std::string name of variable, struct, or union.
         * @param allowParent bool flag whether can use parent frame to do lookup.
         * @return pointer to the context T if found, else nullptr.
         */
        template<class T, class Map>
        T* commonFindStructOrUnion(Map& map, const std::string& name, const bool allowParent)
        {
            const auto findResult = map.find(name);

            if (findResult != map.cend())
            {
                return &findResult->second;
            }

            // See if we can check the parent
            else if (parent != nullptr && allowParent)
            {
                return parent->commonFindStructOrUnion<T, Map>(parent->structsAndUnions, name, allowParent);
            }

            return nullptr;
        }

        /**
         * Common struct or union find function.
         *
         * @param map const reference to Map to lookup.
         * @param name std::string name of variable, struct, or union.
         * @param allowParent bool flag whether can use parent frame to do lookup.
         * @return const pointer to the context T if found, else nullptr.
         */
        template<class T, class Map>
        const T* commonFindStructOrUnion(const Map& map, const std::string& name, const bool allowParent) const
        {
            const auto findResult = map.find(name);

            if (findResult != map.cend())
            {
                return &findResult->second;
            }

            // See if we can check the parent
            else if (parent != nullptr && allowParent)
            {
                return parent->commonFindStructOrUnion<T, Map>(parent->structsAndUnions, name, allowParent);
            }

            return nullptr;
        }

        /**
         * Common variable find function.
         *
         * @param map reference to Map to lookup.
         * @param name std::string name of variable, struct, or union.
         * @param allowParent bool flag whether can use parent frame to do lookup.
         * @return pointer to the context T if found, else nullptr.
         */
        template<class T, class Map>
        T* commonFindVariable(Map& map, const std::string& name, const bool allowParent)
        {
            const auto findResult = map.find(name);

            if (findResult != map.cend())
            {
                return &findResult->second;
            }

            // See if we can check the parent
            else if (parent != nullptr && allowParent)
            {
                return parent->commonFindVariable<T, Map>(parent->variables, name, allowParent);
            }

            return nullptr;
        }

        /**
         * Common find function.
         *
         * @param map const reference to Map to lookup.
         * @param name std::string name of variable, struct, or union.
         * @param allowParent bool flag whether can use parent frame to do lookup.
         * @return const pointer to the context T if found, else nullptr.
         */
        template<class T, class Map>
        const T* commonFindVariable(const Map& map, const std::string& name, const bool allowParent) const
        {
            const auto findResult = map.find(name);

            if (findResult != map.cend())
            {
                return &findResult->second;
            }

            // See if we can check the parent
            else if (parent != nullptr && allowParent)
            {
                return parent->commonFindVariable<T, Map>(parent->variables, name, allowParent);
            }

            return nullptr;
        }

    private:

        // The parent frame.
        Frame* parent;

        // The structs and/or unions in the current frame.
        StructOrUnionMap structsAndUnions;

        // The variables in the current frame.
        VarMap variables;
    };
}

#endif //!CMM_FRAME_H

