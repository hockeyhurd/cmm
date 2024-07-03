/**
 * A class for starting ChildProcesses.
 *
 * @author hockeyhurd
 * @version 2024-01-19
 */

#pragma once

#ifndef CMM_SYSTEM_CHILD_PROCESS_H
#define CMM_SYSTEM_CHILD_PROCESS_H

// Our includes
#include <cmm/Types.h>

// std includes
#include <initializer_list>
#include <string>
#include <vector>

namespace cmm
{
namespace system
{
    class ChildProcess
    {
    public:

        typedef void(*SignalHandler)(s32);

    public:

        /**
         * Constructor.
         */
        ChildProcess(const std::string& path, std::vector<std::string>&& args, const s32 pipeFD = -1);
        ChildProcess(const ChildProcess&) = delete;
        ChildProcess(ChildProcess&&) CMM_NOEXCEPT = default;
        ~ChildProcess() = default;

        ChildProcess& operator= (const ChildProcess&) = delete;
        ChildProcess& operator= (ChildProcess&&) CMM_NOEXCEPT = default;

        s32 getPID() const CMM_NOEXCEPT;
        bool isRunning() const;

        bool setupSignalHandler(std::initializer_list<s32> signals, SignalHandler signalHandler);

        void start();
        s32 stop(const s32 signal = 15);
        s32 wait(const s32 waitType = 0) const;

    private:

        void setupPipeOutput();

    private:

        std::string path;
        std::vector<std::string> args;
        s32 pipeFD;
        s32 pid;
    };
}
}

#endif //!CMM_SYSTEM_CHILD_PROCESS_H

