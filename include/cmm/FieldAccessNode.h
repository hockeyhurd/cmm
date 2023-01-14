/**
 * A class for representing an struct or union member field access in an AST.
 *
 * @author hockeyhurd
 * @version 2023-01-10
 */

#pragma once

#ifndef CMM_FIELD_ACCESS_NODE_H
#define CMM_FIELD_ACCESS_NODE_H

// Our includes
#include <cmm/Types.h>
#include <cmm/ExpressionNode.h>
#include <cmm/Field.h>

// std includes
#include <memory>

namespace cmm
{
    class FieldAccessNode : public ExpressionNode
    {
    public:

        /**
         * Constructor with copy semantics.
         *
         * @param location the Location of this node.
         * @param expr the ExpressionNode struct that contains this Field.
         * @param fieldName std::string name of the field.
         * @param accessType the EnumFieldAccessType.
         */
        FieldAccessNode(const Location& location, std::unique_ptr<ExpressionNode>&& expr,
            const std::string& fieldName, const EnumFieldAccessType accessType);

        /**
         * Constructor with move semantics.
         *
         * @param location the Location of this node.
         * @param expr the ExpressionNode struct that contains this Field.
         * @param fieldName std::string name of the field.
         * @param accessType the EnumFieldAccessType.
         */
        FieldAccessNode(const Location& location, std::unique_ptr<ExpressionNode>&& expr,
            std::string&& fieldName, const EnumFieldAccessType accessType) CMM_NOEXCEPT;

        /**
         * Default copy constructor.
         */
        FieldAccessNode(const FieldAccessNode&) = delete;

        /**
         * Default move constructor.
         */
        FieldAccessNode(FieldAccessNode&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor.
         */
        ~FieldAccessNode() = default;

        /**
         * Default copy assignment operator.
         */
        FieldAccessNode& operator= (const FieldAccessNode&) = delete;

        /**
         * Default move assignment operator.
         */
        FieldAccessNode& operator= (FieldAccessNode&&) CMM_NOEXCEPT = default;

        /**
         * Gets the base ExpressionNode (i.e. the function variable or function
         * call where the field is to be used with).
         *
         * @return pointer to the ExpressionNode.
         */
        ExpressionNode* getExpression() CMM_NOEXCEPT;

        /**
         * Gets the base ExpressionNode (i.e. the function variable or function
         * call where the field is to be used with).
         *
         * @return const pointer to the ExpressionNode.
         */
        const ExpressionNode* getExpression() const CMM_NOEXCEPT;

        /**
         * Gets the underlying Field object.
         *
         * @return reference to the wrapped Field.
         */
        Field& getField() CMM_NOEXCEPT;

        /**
         * Gets the underlying Field object.
         *
         * @return const reference to the wrapped Field.
         */
        const Field& getField() const CMM_NOEXCEPT;

        /**
         * Gets the std::string name of this FieldAccessNode.
         *
         * @return std::string reference.
         */
        std::string& getFieldName() CMM_NOEXCEPT;

        /**
         * Gets the std::string name of this FieldAccessNode.
         *
         * @return std::string const reference.
         */
        const std::string& getFieldName() const CMM_NOEXCEPT;

        /**
         * Gets the type of EnumFieldAccessType.
         *
         * @return EnumFieldAccessType.
         */
        EnumFieldAccessType getFieldAccessType() const CMM_NOEXCEPT;

        /**
         * Gets the CType of this FieldAccessNode.
         *
         * @return CType reference.
         */
        // CType& getDatatype() CMM_NOEXCEPT override;

        /**
         * Gets the CType of this FieldAccessNode.
         *
         * @return CType const reference.
         */
        // const CType& getDatatype() const CMM_NOEXCEPT override;

        /**
         * Gets the index of the FieldAccessNode within its struct.
         *
         * @return s32 index.
         */
        s32 getIndex() const CMM_NOEXCEPT;

        VisitorResult accept(Visitor* visitor) override;
        std::string toString() const override;

    private:

        // The expression where the field should live.
        std::unique_ptr<ExpressionNode> expr;

        // The field we are wrapping.
        Field field;

        // The type of field access operator.
        EnumFieldAccessType accessType;
    };
}

#endif //!CMM_FIELD_ACCESS_NODE_H

