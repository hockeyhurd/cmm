/**
 * A class representing a C scope data structure.
 *
 * @author hockeyhurd
 * @version 2022-08-28
 */

// Our includes
#include <cmm/ScopeManager.h>
#include <cmm/VariableContext.h>

// std includes
#include <memory>

namespace cmm
{
    ScopeManager::ScopeManager()
    {
        // We always push a frame at the beginning to handle global scope.
        frames.emplace_back(Frame());
    }

    Frame& ScopeManager::getCurrentFrame() CMM_NOEXCEPT
    {
        return frames.back();
    }

    const Frame& ScopeManager::getCurrentFrame() const CMM_NOEXCEPT
    {
        return frames.back();
    }

    void ScopeManager::push(const bool canSeeParent)
    {
        Frame* parent = nullptr;

        if (canSeeParent)
        {
            auto& last = frames.back();
            parent = std::addressof(last);
        }

        frames.emplace_back(parent);
    }

    void ScopeManager::pop()
    {
        // Make sure we don't pop the first frame
        if (frames.size() > 1)
        {
            frames.pop_back();
        }
    }

    void ScopeManager::add(const std::string& variable, const VariableContext& context)
    {
        auto& frame = getCurrentFrame();
        frame.add(variable, context);
    }

    void ScopeManager::add(std::string&& variable, const VariableContext& context)
    {
        auto& frame = getCurrentFrame();
        frame.add(std::move(variable), context);
    }

    VariableContext* ScopeManager::find(const std::string& variable)
    {
        auto& frame = getCurrentFrame();
        return frame.find(variable);
    }

    const VariableContext* ScopeManager::find(const std::string& variable) const
    {
        const auto& frame = getCurrentFrame();
        return frame.find(variable);
    }

    VariableContext* ScopeManager::findAny(const std::string& variable)
    {
        auto& frame = getCurrentFrame();
        return frame.findAny(variable);
    }

    const VariableContext* ScopeManager::findAny(const std::string& variable) const
    {
        const auto& frame = getCurrentFrame();
        return frame.findAny(variable);
    }
}

