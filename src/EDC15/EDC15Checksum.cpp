#include "EDC15Checksum.h"
#include "Logging/Logger.h"
#include "EDC15/EDC15File.h"
#include <array>
#include <tuple>
#include "Core/BitArithmetics.h"

EDC15Checksum::ChecksumResult EDC15Checksum::checksum(std::optional<std::filesystem::path> path)
{

    auto type = getChecksumType();

    ChecksumResult result{};
    std::string algorithmUsed{};

    if (type == ChecksumTYPE::TDI2002) {
        result = checksumV2002();
        algorithmUsed = "VAGTDI_V2002";
    }
    else if (type == ChecksumTYPE::TDI41) {
        result = checksumTDI41();
        algorithmUsed = "VAGTDI_V41";
    }
    else if (type == ChecksumTYPE::TDI41V2) {
        result = checksumTDI41V2();
        algorithmUsed = "VAGTDI_V41_V2";
    }
    else {
        throw std::runtime_error{ "Unknown detected checksum" };
    }

    Logger::info("Running {} algorithm, total {} checksum block, {} need calculation, {} fixed.", algorithmUsed, result.computedBlocks, result.checksumBlocksThatAreChecked, result.fixNeeded);

    if (result.fixNeeded == 0) {
        Logger::info("The file already has its checksums correct, no change to the file was made.");
    }
    
    if (path) {
        _file.writeFile(*path);
        Logger::info("File written at {}", path->string());
    }
        

    return result;
}

EDC15Checksum::ChecksumResult EDC15Checksum::checksumTDI41()
{

    ChecksumResult finalResult{};
    finalResult.fixNeeded = false;

    std::vector<std::tuple<uint32_t, uint32_t, size_t, size_t>> checksums;

    checksums.emplace_back(0x00000000, 0x00000000, 0x00000, 0x08000);
    checksums.emplace_back(0x20575354, 0x35504531, 0x08000, 0x10000);
    checksums.emplace_back(0x00000000, 0xEFCD8631, 0x10000, 0x14000);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x14000, 0x4C000);

    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x4C000, 0x50000);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x50000, 0x50B80);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x50B80, 0x5C000);

    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x5C000, 0x60000);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x60000, 0x60B80);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x60B80, 0x6C000);
    
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x6C000, 0x70000);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x70000, 0x70B80);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x70B80, 0x7C000);

    for (const auto& i : checksums) {
        if (_file.readWord(std::get<3>(i) - 4) == 0xC3C3) {
            continue;
        }
        auto res = checkAndFixChecksum(std::get<0>(i), std::get<2>(i), std::get<3>(i), std::get<1>(i));
        
        finalResult.computedBlocks++;
        finalResult.checksumBlocksThatAreChecked++;
        finalResult.fixNeeded += res.fixNeeded;
    }

    return finalResult;

}

EDC15Checksum::ChecksumTYPE EDC15Checksum::getChecksumType() {
    //seg010:000113E8       subc    r1, word_F8C0
    //seg010:000113EC       subc    r6, word_F8C2
    auto checksumTDI2000Addr = _file.findPattern("32 F1 C0 F8 32 F6 C2 F8", 0, 0x4C000);


    Logger::debug("Pattern for TDI2000 checksum was {} {}", checksumTDI2000Addr ? "found" : "not found", checksumTDI2000Addr ? std::to_string(*checksumTDI2000Addr) : "");

    //seg010:00011472       calls   0, checksum_calculate
    //seg010:00011476       cmp     DPP2, #35h; '5'
    auto checksumTDI41V2Addr = _file.findPattern("DA 00 84 15 46 02 35 00", 0, 0x4C000);
    Logger::debug("Pattern for TDI41V2 checksum was {} {}", checksumTDI41V2Addr ? "found" : "not found", checksumTDI41V2Addr ? std::to_string(*checksumTDI41V2Addr) : "");

    if (checksumTDI2000Addr && checksumTDI41V2Addr) {
        throw std::logic_error{ "Multiple checksum pattern detected, unable to determine checksum to use" };
    }
    else if (checksumTDI2000Addr) {
        return ChecksumTYPE::TDI2002;
    }
    else if (checksumTDI41V2Addr) {
        return ChecksumTYPE::TDI41V2;
    }
    else {
        return ChecksumTYPE::TDI41;
    }
}

EDC15Checksum::ChecksumResult EDC15Checksum::checksumTDI41V2()
{

    ChecksumResult finalResult{};
    finalResult.fixNeeded = false;

    std::vector<std::tuple<uint32_t, uint32_t, size_t, size_t>> checksums;

    checksums.emplace_back(0x00000000, 0x00000000, 0x00000, 0x08000);
    checksums.emplace_back(0x20575354, 0x35504531, 0x08000, 0x10000);
    checksums.emplace_back(0x00000000, 0xEFCD8631, 0x10000, 0x14000);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x14000, 0x58000);

    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x58000, 0x58B80);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x58B80, 0x64000);

    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x64000, 0x70000);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x70000, 0x70B80);
    checksums.emplace_back(0xEFCD8631, 0xEFCD8631, 0x70B80, 0x7C000);


    for (const auto& i : checksums) {
        if (_file.readWord(std::get<3>(i) - 4) == 0xC3C3) {
            continue;
        }
        auto res = checkAndFixChecksum(std::get<0>(i), std::get<2>(i), std::get<3>(i), std::get<1>(i));

        finalResult.computedBlocks++;
        finalResult.checksumBlocksThatAreChecked++;
        finalResult.fixNeeded += res.fixNeeded;
    }

    return finalResult;

}

EDC15Checksum::ChecksumResult EDC15Checksum::checksumV2002()
{

    ChecksumResult result{};
    ChecksumResult tmpResult{};

    uint32_t b0 = checksum(0x00000000, 0x00000, 0x08000);
    b0 = MemoryOperations::pack(MemoryOperations::lo(b0) - 0x79CF, MemoryOperations::hi(b0) - 0x1033);
    result.computedBlocks++;

    uint32_t seedIn = MemoryOperations::pack(MemoryOperations::lo(b0) ^ 0xD565, MemoryOperations::hi(b0) + 0x308A);
    
    // First check
    tmpResult = checkAndFixChecksum(seedIn, 0x08000, 0x10000, 0x35504531);
    result.computedBlocks++;
    result.checksumBlocksThatAreChecked++;
    result.fixNeeded += tmpResult.fixNeeded;

    // r1=0x8631
    // r6=0xEFCD
    // acc = r6:r1
    uint32_t acc = 0xEFCD8631; // F8C4:F8C6                 

    // In the file, checksum is done 0x4000 by 0x4000 from 0x14000 to 0x4C000 included
    // But the last checksum is not used.
    // This is equivalent to the line below of directly checksumming from 0x14000 to 0x48000
    uint32_t mainChecksum = checksum(acc, 0x14000, 0x48000);
    result.computedBlocks++;


    // revert mainChecksum here and add some things
    {
        uint16_t pHigh = MemoryOperations::hi(mainChecksum) + 0x2E0B ;   // F8C0 += 0x2E0B
        uint16_t pLow = MemoryOperations::lo(mainChecksum) + 0x5219 ;   // F8C2 += 0x5219
        mainChecksum = MemoryOperations::pack(pHigh, pLow);
    }

    // 2nd checksum
    tmpResult = checkAndFixChecksum(mainChecksum, 0x70000, 0x70000 + 0x0B80, mainChecksum);
    result.computedBlocks++;
    result.checksumBlocksThatAreChecked++;
    uint32_t ncs = tmpResult.sum;
    result.fixNeeded += tmpResult.fixNeeded;

    //DPP0=0x34
    std::array<uint32_t, 3> baseDPPs = { 0x3C, 0x38, 0x34 };

    for (auto dpp : baseDPPs) {

        uint32_t baseAddr = dpp * 0x4000 - 0x80000;

        if (_file.readWord(baseAddr) == 0xC3C3) continue;

        
        // No need for 70000 -> 70B80, already done before the loop
        // Because 0x70000 -> 0x70B80 has mainChecksum as its seed and the checksum must be equal to mainChecksum too
        // Then ncs is mainChecksum so it gives the same args and is therefore already fixed
        if (dpp != 0x3C) { 
            // 50000 -> 50B80, 60000 -> 60B80
            tmpResult = checkAndFixChecksum(ncs, baseAddr, baseAddr + 0xB80, mainChecksum);
            result.computedBlocks++;
            result.checksumBlocksThatAreChecked++;
            result.fixNeeded += tmpResult.fixNeeded;
        }

        // 50B80 -> 5C000, 60B80 -> 6C000, 70B80 -> 7C000
        uint32_t seed = checksum(mainChecksum, baseAddr + 0xB80, baseAddr + 0xC000);
        result.computedBlocks++;

        // 4C000 -> 50000, 5C000 -> 60000, 6C000 -> 70000
        tmpResult = checkAndFixChecksum(seed, baseAddr - 0x4000, baseAddr, mainChecksum);
        result.computedBlocks++;
        result.checksumBlocksThatAreChecked++;
        result.fixNeeded += tmpResult.fixNeeded;

    }

    return result;

}

EDC15Checksum::ChecksumResult EDC15Checksum::checkAndFixChecksum(uint32_t seed, size_t start, size_t stop, uint32_t result) {

    ChecksumResult res;
    res.fixNeeded = false;

    uint32_t acc = checksum(seed, start, stop);
    if (acc != result) {
        res.fixNeeded = 1;
        auto corr = correction(seed, start, stop - 4, result);
        _file.writeLittleEndian(stop - 4, static_cast<uint16_t>(corr >> 16));
        _file.writeLittleEndian(stop - 2, static_cast<uint16_t>(corr));
    }
    res.sum = result;
    return res;
}

uint32_t EDC15Checksum::checksum(uint32_t seed, size_t start, size_t stop)
{
    uint16_t low = seed & 0xFFFF;
    uint16_t high = seed >> 16;
    uint16_t carry;
    size_t pos = start;

    auto nextWord = [&]() {
        uint16_t w = _file.readWord(pos);
        pos += 2;
        return w;
    };

    while (true) {
        low = MemoryOperations::rolCarry(low ^ nextWord(), high, &carry);
        high = (high - nextWord() - carry) ^ low;
        if (pos >= stop)
            break;

        low = low - nextWord() + 0xDAAC;   // = low - w - 1 + 0xDAAD
        high = MemoryOperations::ror(high ^ nextWord(), low);
        if (pos >= stop)
            break;
    }

    return (uint32_t(high) << 16) | low;
}

uint32_t EDC15Checksum::correction(uint32_t seed, size_t start, size_t stop, uint32_t target)
{
    uint32_t state = checksum(seed, start, stop);
    uint16_t stateLow = state & 0xFFFF;
    uint16_t stateHigh = state >> 16;

    uint16_t targetLow = target & 0xFFFF;
    uint16_t targetHigh = target >> 16;
    uint16_t unusedCarry;

    uint16_t word1 = stateLow - targetLow + 0xDAAC;

    uint16_t word2 = MemoryOperations::rolCarry(targetHigh, targetLow, &unusedCarry) ^ stateHigh;

    return (uint32_t(word1) << 16) | word2;
}

EDC15Checksum::EDC15Checksum(EDC15File& file) : _file(file)
{}
