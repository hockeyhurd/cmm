/**
 * A class representing a C scope data structure.
 *
 * @author hockeyhurd
 * @version 2022-08-28
 */

// Our includes
#include <cmm/Scope.h>
#include <cmm/VariableContext.h>

// std includes
#include <memory>

namespace cmm
{
    Scope::Scope()
    {
        // We always push a frame at the beginning to handle global scope.
        frames.emplace_back(Frame());
    }

    Frame& Scope::getCurrentFrame() CMM_NOEXCEPT
    {
        return frames.back();
    }

    const Frame& Scope::getCurrentFrame() const CMM_NOEXCEPT
    {
        return frames.back();
    }

    void Scope::push(const bool canSeeParent)
    {
        Frame* parent = nullptr;

        if (canSeeParent)
        {
            auto& last = frames.back();
            parent = std::addressof(last);
        }

        frames.emplace_back(parent);
    }

    void Scope::pop()
    {
        // Make sure we don't pop the first frame
        if (frames.size() > 1)
        {
            frames.pop_back();
        }
    }

    void Scope::add(const std::string& variable, const VariableContext& context)
    {
        auto& frame = getCurrentFrame();
        frame.add(variable, context);
    }

    void Scope::add(std::string&& variable, const VariableContext& context)
    {
        auto& frame = getCurrentFrame();
        frame.add(std::move(variable), context);
    }

    VariableContext* Scope::find(const std::string& variable)
    {
        auto& frame = getCurrentFrame();
        return frame.find(variable);
    }

    const VariableContext* Scope::find(const std::string& variable) const
    {
        const auto& frame = getCurrentFrame();
        return frame.find(variable);
    }
}

