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
        frames.emplace_back(std::make_unique<Frame>(nullptr));
    }

    Frame& ScopeManager::getCurrentFrame() CMM_NOEXCEPT
    {
        return *frames.back();
    }

    const Frame& ScopeManager::getCurrentFrame() const CMM_NOEXCEPT
    {
        return *frames.back();
    }

    void ScopeManager::push(const bool canSeeParent)
    {
        Frame* parent = nullptr;

        if (canSeeParent && !frames.empty())
        {
            parent = frames.back().get();
        }

        frames.emplace_back(std::make_unique<Frame>(parent));
    }

    void ScopeManager::pop()
    {
        // Make sure we don't pop the first frame
        if (frames.size() > 1)
        {
            frames.pop_back();
        }
    }

    void ScopeManager::add(const std::string& name, const StructOrUnionContext& context)
    {
        auto& frame = getCurrentFrame();
        frame.add(name, context);
    }

    void ScopeManager::add(std::string&& name, const StructOrUnionContext& context)
    {
        auto& frame = getCurrentFrame();
        frame.add(std::move(name), context);
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

    StructOrUnionContext* ScopeManager::findStructOrUnion(const std::string& name)
    {
        auto& frame = getCurrentFrame();
        return frame.findStructOrUnion(name);
    }

    const StructOrUnionContext* ScopeManager::findStructOrUnion(const std::string& name) const
    {
        const auto& frame = getCurrentFrame();
        return frame.findStructOrUnion(name);
    }

    StructOrUnionContext* ScopeManager::findAnyStructOrUnion(const std::string& name)
    {
        auto& frame = getCurrentFrame();
        return frame.findAnyStructOrUnion(name);
    }

    const StructOrUnionContext* ScopeManager::findAnyStructOrUnion(const std::string& name) const
    {
        const auto& frame = getCurrentFrame();
        return frame.findAnyStructOrUnion(name);
    }

    VariableContext* ScopeManager::findVariable(const std::string& variable)
    {
        auto& frame = getCurrentFrame();
        return frame.findVariable(variable);
    }

    const VariableContext* ScopeManager::findVariable(const std::string& variable) const
    {
        const auto& frame = getCurrentFrame();
        return frame.findVariable(variable);
    }

    VariableContext* ScopeManager::findAnyVariable(const std::string& variable)
    {
        auto& frame = getCurrentFrame();
        return frame.findAnyVariable(variable);
    }

    const VariableContext* ScopeManager::findAnyVariable(const std::string& variable) const
    {
        const auto& frame = getCurrentFrame();
        return frame.findAnyVariable(variable);
    }
}

