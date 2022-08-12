/**
 * A helper class for predicting what to parse next based on the current state.
 *
 * @author hockeyhurd
 * @version 2022-08-12
 */

#pragma once

#ifndef CMM_PARSER_PREDICTOR_H
#define CMM_PARSER_PREDICTOR_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Token.h>

// std includes
#include <functional>
#include <optional>
#include <unordered_map>

namespace cmm
{
    // Forward declarations
    class Token;

    template<class T>
    struct PredictionContext
    {
        std::function<T> func;

        explicit CMM_CONSTEXPR_FUNC PredictionContext(T func) : func(func) {}
    };

    template <class T>
    class ParserPredictor
    {
    public:

        /**
         * Constructor
         */
        ParserPredictor() = default;

        /**
         * Deleted copy constructor.
         */
        ParserPredictor(const ParserPredictor&) = delete;

        /**
         * Default move constructor.
         */
        ParserPredictor(ParserPredictor&&) CMM_NOEXCEPT = default;

        /**
         * Default destructor
         */
        ~ParserPredictor() = default;

        /**
         * Deleted copy assignment operator.
         */
        ParserPredictor& operator= (const ParserPredictor&) = delete;

        /**
         * Default move assignment operator.
         *
         * @return this ParserPredictor reference.
         */
        ParserPredictor& operator= (ParserPredictor&&) CMM_NOEXCEPT = default;

        /**
         * Gets whether this prediction table is empty/un-initialized or not.
         *
         * @return bool.
         */
        inline bool empty() const CMM_NOEXCEPT
        {
            return tokenTable.empty() && typeTable.empty();
        }

        /**
         * Calls the predicted function.
         */
        template<class ResultType, class... Args>
        ResultType call(const PredictionContext<T>& context, Args&&... args)
        {
            return context.func(std::forward<Args>(args)...);
        }

        /**
         * Attempts to predict the next parser function to call based on the provided token.
         *
         * @param token the Token to base our prediction from.
         * @return optional PredictionContext.
         */
        std::optional<PredictionContext<T>> predict(const Token& token) CMM_NOEXCEPT
        {
            std::optional<PredictionContext<T>> result = std::nullopt;
            const auto findResult = tokenTable.find(token);

            if (findResult != tokenTable.cend())
            {
                result = findResult->second;
            }

            // Try to find by type
            else
            {
                const auto findTypeResult = typeTable.find(token.getType());

                if (findTypeResult != typeTable.cend())
                {
                    result = findTypeResult->second;
                }
            }

            return result;
        }

        /**
         * Registers the function to the lookup table.
         */
        inline void registerFunction(const Token& token, T func)
        {
            tokenTable.emplace(token, func);
        }

        /**
         * Registers the function to the lookup table.
         */
        inline void registerFunction(const TokenType& type, T func)
        {
            typeTable.emplace(type, func);
        }

    private:

        // The lookup table by 'specific' Token
        std::unordered_map<Token, PredictionContext<T>, TokenHasher> tokenTable;

        // The lookup table by TokenType
        std::unordered_map<TokenType, PredictionContext<T>, TokenTypeHasher> typeTable;

    };
}

#endif //!CMM_PARSER_PREDICTOR_H

