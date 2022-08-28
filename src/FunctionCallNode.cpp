/**
 * An AST node for a function call node.
 *
 * @author hockeyhurd
 * @version 2022-07-24
 */

// Our includes
#include <cmm/FunctionCallNode.h>

namespace cmm
{

    FunctionCallNode::FunctionCallNode(std::string name, ArgList&& args) CMM_NOEXCEPT :
        ExpressionNode(NodeType::FUNCTION_CALL), name(std::move(name)), args(std::move(args))
    {
    }

    bool FunctionCallNode::empty() const CMM_NOEXCEPT
    {
        return args.empty();
    }

    FunctionCallNode::size_type FunctionCallNode::size() const CMM_NOEXCEPT
    {
        return args.size();
    }

    std::string& FunctionCallNode::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& FunctionCallNode::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    FunctionCallNode::ArgListIter FunctionCallNode::begin() CMM_NOEXCEPT
    {
        return args.begin();
    }

    const FunctionCallNode::ArgListConstIter FunctionCallNode::cbegin() const CMM_NOEXCEPT
    {
        return args.cbegin();
    }

    FunctionCallNode::ArgListIter FunctionCallNode::end() CMM_NOEXCEPT
    {
        return args.end();
    }

    const FunctionCallNode::ArgListConstIter FunctionCallNode::cend() const CMM_NOEXCEPT
    {
        return args.cend();
    }

    VisitorResult FunctionCallNode::accept(Visitor* visitor) /* override */
    {
        return visitor->visit(*this);
    }

    std::string FunctionCallNode::toString() const /* override */
    {
        return "FunctionCallNode";
    }
}

