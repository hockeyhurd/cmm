/**
 * A class for representing keywords in this language.
 *
 * @author hockeyhurd
 * @version 2022-09-09
 */

#pragma once

#ifndef CMM_KEYWORD_H
#define CMM_KEYWORD_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <map>
#include <string>
#include <unordered_set>

namespace cmm
{
    class Keyword
    {
    private:

        /**
         * Constructor.
         *
         * @param name the std::string representation of the keyword.
         * @param isAType flags this keyword as a primitive type.
         */
        Keyword(std::string&& name, const bool isAType) CMM_NOEXCEPT;

    public:

        /**
         * Copy constructor.
         */
        Keyword(const Keyword&) = delete;

        /**
         * Move constructor.
         */
        Keyword(Keyword&&) CMM_NOEXCEPT = default;

        /**
         * Destructor.
         */
        ~Keyword() = default;

        /**
         * Copy assignment operator.
         */
        Keyword& operator= (const Keyword&) = delete;

        /**
         * Move assignment operator.
         */
        Keyword& operator= (Keyword&&) CMM_NOEXCEPT = default;

        /**
         * Gets the name of this Keyword as an std::string.
         *
         * @return reference to it's std::string name.
         */
        std::string& getName() CMM_NOEXCEPT;

        /**
         * Gets the name of this Keyword as an std::string.
         *
         * @return const reference to it's std::string name.
         */
        const std::string& getName() const CMM_NOEXCEPT;

        /**
         * Gets whether this Keyword is a primitive type or not.
         *
         * @return bool.
         */
        bool isType() const CMM_NOEXCEPT;

        /**
         * Checks if a std::string is a Keyword or not.
         *
         * @param str the std::string to check.
         * @return const pointer to the actual Keyword if found, else returns nullptr.
         */
        static const Keyword* isKeyword(const std::string& str);

        /**
         * Checks if a std::string is a Keyword that represents a primitive type or not.
         *
         * @param str the std::string to check.
         * @return const pointer to the actual Keyword if found, else returns nullptr.
         */
        static const Keyword* isTypeKeyword(const std::string& str);

        template<class Func>
        static void registerPrimitiveKeywordsByName(Func func)
        {
            oneTimeInitKeywords();

            for (const auto* keywordPtr : primitiveTypes)
            {
                func(keywordPtr->getName());
            }
        }

    private:

        /**
         * Performs one time initialization of the static map of Keywords.
         */
        static void oneTimeInitKeywords();

    private:

        // Container for holding all valid keywords available.
        static std::map<std::string, const Keyword*> keywords;

        // Container for holding all valid keywords available that are primitive types.
        static std::unordered_set<const Keyword*> primitiveTypes;

    public:

        static const Keyword CHAR;
        static const Keyword DOUBLE;
        static const Keyword ELSE;
        static const Keyword ENUM;
        static const Keyword FLOAT;
        static const Keyword IF;
        static const Keyword INT;
        static const Keyword LONG;
        static const Keyword RETURN;
        static const Keyword SHORT;
        static const Keyword STRUCT;
        static const Keyword VOID;
        static const Keyword WHILE;

    private:

        std::string name;
        bool isAType;

    };
}

#endif //!CMM_KEYWORD_H

