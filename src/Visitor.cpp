/**
 * A base visitor class implementing the visitor pattern.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

// Our includes
#include <cmm/Visitor.h>
#include <cmm/NodeList.h>

namespace cmm
{
    VisitorResult::VisitorResult() CMM_NOEXCEPT : owned(true), resultType(EnumVisitorResultType::NULL_T)
    {
        result.null = nullptr;
    }

    VisitorResult::VisitorResult(Node* node, const bool owned) CMM_NOEXCEPT : owned(owned),
        resultType(EnumVisitorResultType::NODE)
    {
        result.node = node;
    }

    VisitorResult::VisitorResult(std::string* str) CMM_NOEXCEPT : owned(true),
        resultType(EnumVisitorResultType::STRING)
    {
        result.str = str;
    }

    VisitorResult::~VisitorResult()
    {
        if (owned)
        {
            switch (resultType)
            {
            case EnumVisitorResultType::NODE:
                delete result.node;
                result.node = nullptr;
                break;
            case EnumVisitorResultType::STRING:
                delete result.str;
                result.str = nullptr;
                break;
            default:
                // No-op
                break;
            }
        }
    }
}

