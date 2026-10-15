#pragma once

#include "EDC15/EDC15File.h"

#include <filesystem>
#include <cstdint>

class EDC15Checksum {
public:

	

	struct HookSite {
		size_t hookPosition;
		size_t nextHandlerAddr;
		size_t overWrittenCmpJmpAddr;
		size_t defaultTargetAddr;
	};

	struct ChecksumResult {
		size_t fixNeeded;
		size_t computedBlocks;
		size_t checksumBlocksThatAreChecked;
		uint32_t sum;
	};

	enum class ChecksumTYPE {
		TDI41,
		TDI41V2,
		TDI2002
	};

	EDC15Checksum(EDC15File& file);
	ChecksumResult checksum(std::optional<std::filesystem::path> path = std::nullopt);
	ChecksumTYPE getChecksumType();
private:
	ChecksumResult checkAndFixChecksum(uint32_t seed, size_t start, size_t stop, uint32_t result);
	uint32_t correction(uint32_t seed, size_t start, size_t stop, uint32_t result);
	uint32_t checksum(uint32_t seed, size_t start, size_t stop);
	ChecksumResult checksumV2002();
	ChecksumResult checksumTDI41();
	ChecksumResult checksumTDI41V2();
	EDC15File& _file;
	
};