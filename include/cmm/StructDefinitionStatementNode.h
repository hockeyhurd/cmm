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
#include <cmm/StatementNode.h>

// std includes
#include <string>

namespace cmm
{
    class StructDefinitionStatementNode : public StatementNode
    {
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

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The name of the struct
        std::string name;

        // The block of fields.
        BlockNode blockNode;
    };
}

#endif //!CMM_STRUCT_DEFINITION_STATEMENT_H


