/**
 * A class representinng a frame of reference.
 *
 * @author hockeyhurd
 * @version 2022-08-28
 */

// Our includes
#include <cmm/Scope.h>

namespace cmm
{
    Frame::Frame() CMM_NOEXCEPT : parent(nullptr)
    {
    }

    Frame::Frame(Frame* parent) CMM_NOEXCEPT : parent(parent)
    {
    }

    void Frame::add(const std::string& variable, const VariableContext& context)
    {
        variables.emplace(variable, context);
    }

    void Frame::add(std::string&& variable, const VariableContext& context)
    {
        variables.emplace(std::move(variable), context);
    }

    VariableContext* Frame::find(const std::string& variable)
    {
        const auto findResult = variables.find(variable);

        if (findResult != variables.cend())
        {
            return &findResult->second;;
        }

        // See if we can check the parent
        else if (parent != nullptr)
        {
            return parent->find(variable);
        }

        return nullptr;
    }

    const VariableContext* Frame::find(const std::string& variable) const
    {
        const auto findResult = variables.find(variable);

        if (findResult != variables.cend())
        {
            return &findResult->second;;
        }

        // See if we can check the parent
        else if (parent != nullptr)
        {
            return parent->find(variable);
        }

        return nullptr;
    }

    Frame::iterator Frame::begin() CMM_NOEXCEPT
    {
        return variables.begin();
    }

    Frame::const_iterator Frame::cbegin() const CMM_NOEXCEPT
    {
        return variables.cbegin();
    }

    Frame::iterator Frame::end() CMM_NOEXCEPT
    {
        return variables.end();
    }

    Frame::const_iterator Frame::cend() const CMM_NOEXCEPT
    {
        return variables.cend();
    }
}

