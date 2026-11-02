#include "Core/CommandInterpreter.h"
#include "EDC15/EDC15Checksum.h"
#include "Logging/Logger.h"
#include "version.h"
#include <string_view>
#include <format>
#include <vector>
#include <optional>

namespace {

    void printUsage(std::string_view errorMessage = "");

    /// <summary>
    /// What each command accepts on the command line. Must stay in sync with printUsage().
    /// </summary>
    const std::vector<CommandInterpreter::CommandSpec> specs{
        {
            .name = "checksum",
            .valued = { "out" },
            .minFreeArgs = 1,
            .maxFreeArgs = 1,
            .freeArgsError = "checksum takes exactly one free argument: a binary file"
        },
    };
}

/// <summary>
///
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns>
/// 0 means success
/// 1 means command-line error
/// 2 means that the program failed
/// </returns>
int main(int argc, char* argv[])
{

    Logger::print("------------- VAGEDC15Checksum tool " VERSION_FULL " -------------");
    Logger::print("                 Created By Moffa13@moffa13.com                ");
    Logger::breakLine(1);

#ifdef NDEBUG
    Logger::level = Logger::Level::Info;
#else
    Logger::level = Logger::Level::Debug;
#endif

    CommandInterpreter interpreter{ argc, argv };
    interpreter.setCommandSpecs(&specs);

    // No command at all: just show the usage, without an error message
    if (!interpreter.getCommand().has_value()) {
        printUsage();
        return 1;
    }

    if (auto error = interpreter.validateCommandLine()) {
        printUsage(*error);
        return 1;
    }

    if (interpreter.isCommand("checksum")) {

        // --out given without a file name
        if (interpreter.hasOption("out") && !interpreter.optionHasValue("out")) {
            printUsage("--out requires a file name");
            return 1;
        }

        try {
            EDC15File edc15File{ interpreter.freeArg(0) };
            EDC15Checksum edc15Checksum{ edc15File };
            edc15Checksum.checksum(interpreter.getOption("out"));
        }
        catch (const std::exception& e) {
            Logger::error("Unknown exception: {}", e.what());
            return 2;
        }

        return 0;
    }

    // Only reachable if a command is in specs but has no branch here
    printUsage(std::format("command '{}' is not implemented", *interpreter.getCommand()));
    return 1;
}


namespace {

    void printUsage(std::string_view errorMessage) {

        if (!errorMessage.empty()) {
            Logger::error(errorMessage);
            Logger::breakLine(1);
        }
        Logger::print("Usage: VAGEDC15Checksum.exe <command> [options]");
        Logger::breakLine(1);
        Logger::print("Commands:");
        Logger::print("  checksum <binary-file> [--out <file>]   Calculate the checksum of the EDC15 File and write it with the <out> option");

        Logger::breakLine(1);
        Logger::print("Examples:");
        Logger::print("  VAGEDC15Checksum.exe checksum 038906019NJ.bin --out 038906019NJ.cs.bin");
    }
}