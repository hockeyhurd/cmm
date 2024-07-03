/**
 * A class for starting ChildProcesses.
 *
 * @author hockeyhurd
 * @version 2024-01-19
 */

// Our includes
#include <cmm/system/ChildProcess.h>

// std includes
#include <cstring>
#include <iostream>
#include <memory>

// System includes
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#if OS_WIN
#ifndef WIFEXITED
#define WIFEXITED(x) ((x) != 3)
#endif // !WIFEXITED
#endif // !OS_WIN

namespace cmm
{
namespace system
{
    ChildProcess::ChildProcess(const std::string& path, std::vector<std::string>&& args, const s32 pipeFD) :
        path(path), args(std::move(args)), pipeFD(pipeFD), pid(-1)
    {
    }

    s32 ChildProcess::getPID() const CMM_NOEXCEPT
    {
        return pid;
    }

    bool ChildProcess::isRunning() const
    {
        if (pid <= 0 || WIFEXITED(pid) || WIFSTOPPED(pid))
        {
            return false;
        }

        const s32 signaled = WIFSIGNALED(pid);

        // If we were signaled and the value is one of these, then report status as STOPPED.
        if (signaled && (pid == 9 || pid == 15 || pid == 19))
        {
            return false;
        }

        return true;
    }

    bool ChildProcess::setupSignalHandler(std::initializer_list<s32> signals, SignalHandler signalHandler)
    {
        struct sigaction sigAct;
        std::memset(&sigAct, '\0', sizeof(sigAct));

        sigAct.sa_handler = signalHandler;
        sigAct.sa_flags = SA_RESTART;
        sigfillset(&sigAct.sa_mask);

        for (auto &sig : signals)
        {
            if (sigaction(sig, &sigAct, NULL) == -1)
            {
                // Debug::error("Could not setup signal handler...");
                return false;
            }
        }

        return true;
    }

    void ChildProcess::start()
    {
        if (isRunning())
        {
            return;
        }

        pid = fork();

        switch (pid)
        {
        // Error: Failed to start child process.
        case -1:
            std::cerr << "Compiler error: failed to start '" << path << "'\n";
            break;
        // Success: We are running as the child process.
        case 0:
        {
            if (pipeFD >= 0)
            {
                setupPipeOutput();
            }

            const auto argv = std::make_unique<char*[]>(args.size() + 1);
            argv[args.size()] = nullptr;

            for (std::size_t i = 0; i < args.size(); ++i)
            {
                const auto& str = args[i];
                char** cur = &argv[i];
                *cur = new char[str.size() + 1];
                std::strcpy(*cur, str.c_str());
            }

            const int errorCode = execvp(path.c_str(), argv.get());
            std::cout << "Compiler error: unexpected return from execvp (" << errorCode << ").\n";

            for (std::size_t i = 0; i < args.size(); ++i)
            {
                char** cur = &argv[i];
                delete[] *cur;
                *cur = nullptr;
            }
        }
            break;
        // Success: We are the parrent and the child process has at least started.
        default:
            break;
        }
    }

    s32 ChildProcess::stop(const s32 signal)
    {
        if (!isRunning())
        {
            return -1;
        }

        s32 returnValue = kill(pid, signal);

        if (returnValue < 0)
        {
            returnValue = errno;
        }

        else
        {
            returnValue = 0;
            pid = -1;
        }

        return returnValue;
    }

    s32 ChildProcess::wait(const s32 waitType) const
    {
        s32 waitStatus;
        const s32 result = waitpid(pid, &waitStatus, waitType);

        if (result != 0)
        {
            waitStatus = result;
        }

        return waitStatus;
    }

    void ChildProcess::setupPipeOutput()
    {
        const auto errorCode = dup2(pipeFD, STDOUT_FILENO);

        if (errorCode < 0)
        {
            std::cerr << "Failed to pipe STDOUT_FILENO to pipeFD (error code: " << errorCode << ")\n";
            std::exit(-1);
        }
    }
}
}

