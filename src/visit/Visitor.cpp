/**
 * A base visitor class implementing the visitor pattern.
 *
 * @author hockeyhurd
 * @version 2022-08-24
 */

// Our includes
#include <cmm/NodeList.h>
#include <cmm/visit/Visitor.h>

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

    VisitorResult::VisitorResult(std::string* str, const bool owned) CMM_NOEXCEPT : owned(owned),
        resultType(EnumVisitorResultType::STRING)
    {
        result.str = str;
    }

    VisitorResult::VisitorResult(VisitorResult&& other) CMM_NOEXCEPT : owned(other.owned),
        resultType(other.resultType)
    {
        switch (resultType)
        {
            case EnumVisitorResultType::NODE:
                result.node = other.result.node;
                other.result.node = nullptr;
                break;
            case EnumVisitorResultType::STRING:
                result.str = other.result.str;
                other.result.str = nullptr;
                break;
            default:
                // No-op
                break;
        }

        other.owned = false;
    }

    VisitorResult& VisitorResult::operator= (VisitorResult&& other) CMM_NOEXCEPT
    {
        clean();

        switch (resultType)
        {
            case EnumVisitorResultType::NODE:
                result.node = other.result.node;
                other.result.node = nullptr;
                break;
            case EnumVisitorResultType::STRING:
                result.str = other.result.str;
                other.result.str = nullptr;
                break;
            default:
                // No-op
                break;
        }

        other.owned = false;
        return *this;
    }

    VisitorResult::~VisitorResult()
    {
        clean();
    }

    void VisitorResult::clean() CMM_NOEXCEPT
    {
        switch (resultType)
        {
            case EnumVisitorResultType::NODE:
            {
                if (owned) delete result.node;
                result.node = nullptr;
                break;
            }
            case EnumVisitorResultType::STRING:
            {
                if (owned) delete result.str;
                result.str = nullptr;
                break;
            }
            default:
                // No-op
                break;
        }
    }
}

