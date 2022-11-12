/**
 * A class representinng a frame of reference.
 *
 * @author hockeyhurd
 * @version 2022-08-28
 */

// Our includes
#include <cmm/Frame.h>

namespace cmm
{
    Frame::Frame() CMM_NOEXCEPT : parent(nullptr)
    {
    }

    Frame::Frame(Frame* parent) CMM_NOEXCEPT : parent(parent)
    {
    }

    void Frame::add(const std::string& name, const StructOrUnionContext& context)
    {
        structsAndUnions.emplace(name, context);
    }

    void Frame::add(std::string&& name, const StructOrUnionContext& context)
    {
        structsAndUnions.emplace(std::move(name), context);
    }

    void Frame::add(const std::string& variable, const VariableContext& context)
    {
        variables.emplace(variable, context);
    }

    void Frame::add(std::string&& variable, const VariableContext& context)
    {
        variables.emplace(std::move(variable), context);
    }

    StructOrUnionContext* Frame::findStructOrUnion(const std::string& name)
    {
        return commonFindStructOrUnion<StructOrUnionContext, StructOrUnionMap>(structsAndUnions, name, false);
    }

    const StructOrUnionContext* Frame::findStructOrUnion(const std::string& name) const
    {
        return commonFindStructOrUnion<StructOrUnionContext, StructOrUnionMap>(structsAndUnions, name, false);
    }

    StructOrUnionContext* Frame::findAnyStructOrUnion(const std::string& name)
    {
        return commonFindStructOrUnion<StructOrUnionContext, StructOrUnionMap>(structsAndUnions, name, true);
    }

    const StructOrUnionContext* Frame::findAnyStructOrUnion(const std::string& name) const
    {
        return commonFindStructOrUnion<StructOrUnionContext, StructOrUnionMap>(structsAndUnions, name, true);
    }

    VariableContext* Frame::findVariable(const std::string& variable)
    {
        return commonFindVariable<VariableContext, VarMap>(variables, variable, false);
    }

    const VariableContext* Frame::findVariable(const std::string& variable) const
    {
        return commonFindVariable<VariableContext, VarMap>(variables, variable, false);
    }

    VariableContext* Frame::findAnyVariable(const std::string& variable)
    {
        return commonFindVariable<VariableContext, VarMap>(variables, variable, true);
    }

    const VariableContext* Frame::findAnyVariable(const std::string& variable) const
    {
        return commonFindVariable<VariableContext, VarMap>(variables, variable, true);
    }
}

