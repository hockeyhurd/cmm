/**
 * An AST node for a struct definition statement ast nodes.
 *
 * @author hockeyhurd
 * @version 2022-10-26
 */

#pragma once

#ifndef CMM_STRUCT_DEFINITION_STATEMENT_H
#define CMM_STRUCT_DEFINITION_STATEMENT_H

// Our includes
#include <cmm/Types.h>
#include <cmm/BlockNode.h>
#include <cmm/Field.h>
#include <cmm/StatementNode.h>

// std includes
#include <optional>
#include <string>
#include <unordered_map>

namespace cmm
{
    class StructDefinitionStatementNode : public StatementNode
    {
    public:

        using FieldMap = std::unordered_map<std::string, Field>;
        using FieldMapIter = FieldMap::iterator;
        using FieldMapConstIter = FieldMap::const_iterator;

    public:

        /**
         * Default constructor.
         *
         * @param location the location of this node.
         * @param name the name of the struct.
         * @param blockNode the block of fields.
         */
        StructDefinitionStatementNode(const Location& location, const std::string& name, BlockNode&& blockNode);

        /**
         * Copy constructor.
         */
        StructDefinitionStatementNode(const StructDefinitionStatementNode&) = delete;

        /**
         * Move constructor.
         */
        StructDefinitionStatementNode(StructDefinitionStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Destructor
         */
        ~StructDefinitionStatementNode() = default;

        /**
         * Copy assignment operator.
         *
         * @return StructDefinitionStatementNode reference.
         */
        StructDefinitionStatementNode& operator= (const StructDefinitionStatementNode&) = delete;

        /**
         * Move assignment operator.
         *
         * @return StructDefinitionStatementNode reference.
         */
        StructDefinitionStatementNode& operator= (StructDefinitionStatementNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the name of the struct.
         *
         * @return std::string.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of the struct.
         *
         * @return std::string.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets the block of the struct.
         *
         * @return Reference to the BlockNode.
         */
        BlockNode& getBlockNode() CMM_NOEXCEPT;

        /**
         * Gets the block of the struct.
         *
         * @return Const reference to the BlockNode.
         */
        const BlockNode& getBlockNode() const CMM_NOEXCEPT;

        /**
         * Gets whether there are no fields in the structs definition or not.
         *
         * @return bool true if empty, else false.
         */
        bool empty() const CMM_NOEXCEPT;

        /**
         * Gets the number of fields in the structs definition.
         *
         * @return std::size_t count.
         */
        std::size_t size() const CMM_NOEXCEPT;

        /**
         * Handles converting the BlockNode of "VariableDeclarationStatementNodes"
         * to a proper Field format.
         *
         * Note: This does NOT change the BlockNode.
         * Note #2: The returned value is the first duplicate field (only).
         *
         * @return optional std::string of field with an error.
         */
        std::optional<std::string> setupFieldTable();

        /**
         * Gets an iterator to the beginning of the FieldMap.
         *
         * Note: The map is backed by an std::unordered_map and thus will
         *       NOT be guranteed to be in 'index' order (i.e. as appears in
         *       the struct definition).
         *
         * @return FieldMapIter.
         */
        FieldMapIter begin() CMM_NOEXCEPT;

        /**
         * Gets a const iterator to the beginning of the FieldMap.
         *
         * Note: The map is backed by an std::unordered_map and thus will
         *       NOT be guranteed to be in 'index' order (i.e. as appears in
         *       the struct definition).
         *
         * @return FieldMapConstIter.
         */
        FieldMapConstIter cbegin() const CMM_NOEXCEPT;

        /**
         * Gets an iterator to the end of the FieldMap.
         *
         * Note: The map is backed by an std::unordered_map and thus will
         *       NOT be guranteed to be in 'index' order (i.e. as appears in
         *       the struct definition).
         *
         * @return FieldMapIter.
         */
        FieldMapIter end() CMM_NOEXCEPT;

        /**
         * Gets a const iterator to the end of the FieldMap.
         *
         * Note: The map is backed by an std::unordered_map and thus will
         *       NOT be guranteed to be in 'index' order (i.e. as appears in
         *       the struct definition).
         *
         * @return FieldMapConstIter.
         */
        FieldMapConstIter cend() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the struct
        std::string name;

        // The block of fields.
        BlockNode blockNode;

        // The map of fields.
        // TODO: Consider moving this to the StructTable (if it makes sense) to reduce duplicate data.
        FieldMap fieldMap;
    };
}

#endif //!CMM_STRUCT_DEFINITION_STATEMENT_H


