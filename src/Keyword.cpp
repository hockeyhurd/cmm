/**
 * A class for representing keywords in this language.
 *
 * @author hockeyhurd
 * @version 2022-09-09
 */

// Our includes
#include <cmm/Keyword.h>

namespace cmm
{
    /* static */
    std::map<std::string, const Keyword*> Keyword::keywords;

    /* static */
    std::unordered_set<const Keyword*> Keyword::primitiveTypes;

    /* static */
    const Keyword Keyword::CHAR("char", true);
    /* static */
    const Keyword Keyword::DOUBLE("double", true);
    /* static */
    const Keyword Keyword::ELSE("else", false);
    /* static */
    const Keyword Keyword::ENUM("enum", true);
    /* static */
    const Keyword Keyword::FLOAT("float", true);
    /* static */
    const Keyword Keyword::IF("if", false);
    /* static */
    const Keyword Keyword::INT("int", true);
    /* static */
    const Keyword Keyword::LONG("long", true);
    /* static */
    const Keyword Keyword::RETURN("return", false);
    /* static */
    const Keyword Keyword::SHORT("short", true);
    /* static */
    const Keyword Keyword::STRUCT("struct", true);
    /* static */
    const Keyword Keyword::VOID("void", true);
    /* static */
    const Keyword Keyword::WHILE("while", false);

    Keyword::Keyword(std::string&& name, const bool isAType) CMM_NOEXCEPT : name(std::move(name)), isAType(isAType)
    {
    }

    std::string& Keyword::getName() CMM_NOEXCEPT
    {
        return name;
    }

    const std::string& Keyword::getName() const CMM_NOEXCEPT
    {
        return name;
    }

    bool Keyword::isType() const CMM_NOEXCEPT
    {
        return isAType;
    }

    /* static */
    const Keyword* Keyword::isKeyword(const std::string& str)
    {
        oneTimeInitKeywords();

        const auto findResult = keywords.find(str);
        return findResult != keywords.cend() ? findResult->second : nullptr;
    }

    /* static */
    const Keyword* Keyword::isTypeKeyword(const std::string& str)
    {
        oneTimeInitKeywords();

        const auto findResult = keywords.find(str);
        return findResult != keywords.cend() && findResult->second->isType() ? findResult->second : nullptr;
    }

    /* static */
    void Keyword::oneTimeInitKeywords()
    {
        if (keywords.empty())
        {
            // Helper function for quickly creating Keyword entries into the map.
            static auto addKeyword = [&] (const Keyword* keyword)
            {
                // Safe to cast away const since this is an internal lambda function.
                const auto [iter, flag] = keywords.emplace(keyword->getName(), keyword);

                if (flag && iter->second->isType())
                {
                    primitiveTypes.emplace(iter->second);
                }
            };

            // TODO: Commenting out what we aren't currently supporting.
            // addKeyword("auto", false);
            // addKeyword("break", false);
            // addKeyword("case", false);
            addKeyword(&Keyword::CHAR);
            // addKeyword("const", false);
            // addKeyword("continue", false);
            // addKeyword("default", false);
            // addKeyword("do", false);
            addKeyword(&Keyword::DOUBLE);
            addKeyword(&Keyword::ELSE);
            addKeyword(&Keyword::ENUM);
            // addKeyword("extern", false);
            addKeyword(&Keyword::FLOAT);
            // addKeyword("for", false);
            // addKeyword("goto", false);
            addKeyword(&Keyword::IF);
            addKeyword(&Keyword::INT);
            addKeyword(&Keyword::LONG);
            // addKeyword("register", false);
            addKeyword(&Keyword::RETURN);
            addKeyword(&Keyword::STRUCT);
            addKeyword(&Keyword::SHORT);
            // addKeyword("signed", false);
            // addKeyword("sizeof", false);
            // addKeyword("static", false);
            // addKeyword("struct", true);
            // addKeyword("switch", false);
            // addKeyword("typedef", false);
            // addKeyword("union", false);
            // addKeyword("unsigned", false);
            addKeyword(&Keyword::VOID);
            // addKeyword("volatile", false);
            addKeyword(&Keyword::WHILE);
        }
    }
}

