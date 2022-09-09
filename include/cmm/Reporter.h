/**
 * The console output for reporting warnings, errors, etc.
 *
 * @author hockeyhurd
 * @version 2022-07-23
 */

#pragma once

#ifndef CMM_REPORTER_H
#define CMM_REPORTER_H

// Our includes
#include <cmm/Types.h>
#include <cmm/Location.h>

// std includes
#include <iostream>

namespace cmm
{
    class Reporter
    {
    private:

        /**
         * Private constructor for singleton instance.
         */
        Reporter() CMM_NOEXCEPT;

        /**
         * Deleted copy constructor.
         */
        Reporter(const Reporter&) CMM_NOEXCEPT = delete;

        /**
         * Default move constructor.
         */
        Reporter(Reporter&&) CMM_NOEXCEPT = default;

        /**
         * Deleted copy assignment operator.
         */
        Reporter& operator= (const Reporter&) CMM_NOEXCEPT = delete;

        /**
         * Default move assignment operator.
         */
        Reporter& operator= (Reporter&&) CMM_NOEXCEPT = default;

    public:

        /**
         * Destructor
         */
        ~Reporter();

        /**
         * Gets the reference to the singleton instance of the Reporter class.
         *
         * @return reference to singleton Reporter instance.
         */
        static Reporter& instance();

        /**
         * Gets the number of errors.
         *
         * @return s32 count.
         */
        s32 getErrorCount() const CMM_NOEXCEPT;

        /**
         * Gets the number of warnings.
         *
         * @return s32 count.
         */
        s32 getWarningCount() const CMM_NOEXCEPT;

        /**
         * Reports a bug in the compiler.
         *
         * @param msg the templated error message to provide.
         * @param location the Location in the file where the error occurred.
         */
        template<class T>
        void bug(const T& msg, const Location& location, const bool fatal)
        {
            std::cout << "bug: " << msg << " at " << location << std::endl;

            if (fatal)
            {
                std::exit(-1);
            }
        }

        /**
         * Reports an error to the console output.
         *
         * @param msg the templated error message to provide.
         * @param location the Location in the file where the error occurred.
         */
        template<class T>
        void error(const T& msg, const Location& location)
        {
            std::cout << "error: " << msg << " at " << location << std::endl;
            ++errors;
        }

        /**
         * Reports a warning to the console output.
         *
         * @param msg the templated warning message to provide.
         * @param location the Location in the file where the warning occurred.
         */
        template<class T>
        void warn(const T& msg, const Location& location)
        {
            std::cout << "warning: " << msg << " at " << location << std::endl;
            ++warnings;
        }

        /**
         * Resets tracked errors and warnings.
         */
        void reset();

    private:

        // The count of errors.
        s32 errors;

        // The count of warnings.
        s32 warnings;
    };
}

#endif //!CMM_REPORTER_H

