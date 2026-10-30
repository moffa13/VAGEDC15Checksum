#include "Core/CommandInterpreter.h"
#include "EDC15/EDC15Checksum.h"
#include "Logging/Logger.h"
#include "version.h"
#include <string_view>
#include <format>

void printUsage(std::string_view errorMessage = "");

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
    Logger::level = Logger::Level::Debug;
#else
    Logger::level = Logger::Level::Info;
#endif

    CommandInterpreter interpreter{ argc, argv };

    if (interpreter.getCommand().has_value()) {

       
        if (interpreter.isCommand("checksum")) {

            interpreter.addNeedsValue("out");
            auto romOpt = interpreter.getOption("out");

            if (interpreter.countFreeArgs() != 1) {
                printUsage("checksum takes exactly one free argument: a binary file");
                return 1;
            }
            try {

                EDC15File edc15File{ interpreter.freeArg(0) };
                EDC15Checksum edc15Checksum{ edc15File };
                edc15Checksum.checksum(romOpt);
            }
            catch (const std::exception& e) {
                Logger::error("Unknown exception: {}", e.what());
                return 2;
            }

            return 0;
        }
        else {
            printUsage();
            return 1;
        }
    }
    else {
        printUsage();
        return 1;
    }
}
    

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
