# EDC15Checksum

Portable checksum calculation and correction for **Bosch EDC15 ECUs (VAG group)**.

This repository exclusively uses the standard cpp library

## Features

- **Checksum calculation** for EDC15 flash regions.
- **Checksum correction**: computes the correction words so the region's checksum matches the value expected by the ECU.

## Why this project

The **TDIv2002** checksum algorithm is implemented incorrectly in every existing checksum tool we have tested. Whenever map modifications fall within the **`0x14000` – `0x48000`** range, those tools produce an invalid checksum and the ECU rejects the file.

EDC15Checksum handles this case correctly and produces a valid checksum for modifications in that range.

Because of that, depending on the modified regions, the other tools might indicate that the checksums are **incorrect**, it is important not to fix them. The TDI2002 algorithm has been decompiled and reproduced in c++. It's been checked that the ECUs accept the checksums


## Supported checksum types

- TDI41
- TDI41 V2
- TDI2002

## Command line

```
VAGEDC15Checksum.exe checksum 038906019NJ.bin --out 038906019NJ.cs.bin
```

## Build

Go to the root directory and run:

Run build.sh or build_vsXXXX.bat

The executable will be in build/app, and the library will be inside build/src

## Library API

```cpp
enum class ChecksumTYPE {
    TDI41,
    TDI41V2,
    TDI2002
};

struct ChecksumResult {
    size_t fixNeeded;
    size_t computedBlocks;
    size_t checksumBlocksThatAreChecked;
    uint32_t sum;
};

EDC15Checksum(EDC15File& file);
ChecksumResult checksum(std::optional<std::filesystem::path> path = std::nullopt);
ChecksumTYPE getChecksumType();
```

`ChecksumResult` fields:

| Field | Description |
|---|---|
| `fixNeeded` | Number of checksum blocks whose stored value was wrong and had to be fixed |
| `computedBlocks` | Number of blocks that are only computed (no checksum stored in the file) |
| `checksumBlocksThatAreChecked` | Number of blocks whose checksum is stored in the file and verified/fixed |
| `sum` | Internal value (correct checksum at the end of a block); not meaningful in the result of `checksum()` |

Example:

```cpp
EDC15File file(/* ... */);
EDC15Checksum cs(file);

EDC15Checksum::ChecksumTYPE type = cs.getChecksumType();
ChecksumResult result = cs.checksum("038906019NJ.cs.bin");
```

## Disclaimer

Always keep a backup of the original flash before writing a modified file to the ECU. Use at your own risk.
