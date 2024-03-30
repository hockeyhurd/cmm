/**
 * A utility class for parsing commandline-args.
 *
 * @author hockeyhurd
 * @version 2023-05-18
 */

#pragma once

#ifndef CMM_UTILS_STRING_UTILS_H
#define CMM_UTILS_STRING_UTILS_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <optional>

namespace cmm
{
namespace StringUtils
{
    template<class Out, class In = Out>
    std::optional<Out> getBaseFileName(const In& inputFile)
    {
        if (inputFile.empty())
        {
            return Out();
        }

        Out output;
        output.reserve(inputFile.size());

        const auto endIter = inputFile.crend();
        auto iter = inputFile.crbegin();
        auto end = static_cast<typename Out::size_type>(static_cast<typename In::size_type>(inputFile.size() - 1));

        for (; iter != endIter; ++iter)
        {
            const auto ch = *iter;

            if (ch == '.')
            {
                return std::make_optional<Out>(inputFile.substr(0, end));
            }

            output += ch;
            --end;
        }

        return std::nullopt;
    }

    template<class T>
    bool isWhitespace(const T& str)
    {
        for (const char ch :  str)
        {
            if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r')
            {
                return false;
            }
        }

        return true;
    }

    // TODO: Do we need this to be more robust?
    template<class T>
    bool startsWith(const T& str, const char value)
    {
        return str[0] == value;
    }
}
}

#endif //!CMM_UTILS_STRING_UTILS_H

