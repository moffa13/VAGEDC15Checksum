#pragma once

#include "EDC15/EDC15File.h"
#include <filesystem>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>
#include <tuple>


class EDC15Checksum {
public:
	struct HookSite {
		size_t hookPosition;
		size_t nextHandlerAddr;
		size_t overWrittenCmpJmpAddr;
		size_t defaultTargetAddr;
	};

	struct ChecksumInfo {
		size_t fixNeeded;
		size_t computedBlocks;
		size_t checksumBlocksThatAreChecked;
	};

	enum class ChecksumTYPE {
		TDI41,
		TDI41V2,
		TDI2002
	};

	EDC15Checksum(EDC15File& file);
	ChecksumInfo checksum(const std::optional<std::filesystem::path>& path = std::nullopt);
	ChecksumTYPE getChecksumType();
private:
	bool checkAndFixChecksum(uint32_t seed, size_t start, size_t stop, uint32_t result);
	static constexpr std::string_view toString(EDC15Checksum::ChecksumTYPE t);
	uint32_t correction(uint32_t seed, size_t start, size_t stop, uint32_t result);
	uint32_t checksum(uint32_t seed, size_t start, size_t stop);
	ChecksumInfo checksumV2002();
	ChecksumInfo checksumTDI41();
	ChecksumInfo checksumTDI41V2();
	EDC15Checksum::ChecksumInfo staticChecksum(const std::vector<std::tuple<uint32_t, uint32_t, size_t, size_t>>& checksums);
	EDC15File& _file;
	
};


