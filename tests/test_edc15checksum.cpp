#include <gtest/gtest.h>
#include "EDC15/EDC15Checksum.h"
#include "EDC15/EDC15File.h"

TEST(EDC15ChecksumTest, TestFile012FK)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/038906012FK.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI41);
    EXPECT_EQ(ret.fixNeeded, 0);
}

TEST(EDC15ChecksumTest, TestFile038906019HK)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/038906019HK.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI41);
    EXPECT_EQ(ret.fixNeeded, 0);
}

TEST(EDC15ChecksumTest, TestFile038906019NJ)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/038906019NJ.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI2002);
    EXPECT_EQ(ret.fixNeeded, 0);
}

TEST(EDC15ChecksumTest, TestFileSharan038906019J)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/038906019J.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI41V2);
    EXPECT_EQ(ret.fixNeeded, 0);
}

TEST(EDC15ChecksumTest, TestFile367046)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/038906012HA.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI41);
    EXPECT_EQ(ret.fixNeeded, 0);
}

TEST(EDC15ChecksumTest, TestFile038906012A)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/038906012A.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI41V2);
    EXPECT_EQ(ret.fixNeeded, 0);
}

TEST(EDC15ChecksumTest, TestFile038906019GQ)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/038906019GQ.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI41);
    EXPECT_EQ(ret.fixNeeded, 0);
}

TEST(EDC15ChecksumTest, TestFile045906019BP)
{
    EDC15File edc15File{ std::string{ TEST_DATA_DIR } + "/" + "binaries/045906019BP.bin" };
    EDC15Checksum edc15Checksum{ edc15File };
    auto type = edc15Checksum.getChecksumType();
    auto ret = edc15Checksum.checksum();
    EXPECT_EQ(type, EDC15Checksum::ChecksumTYPE::TDI2002);
    EXPECT_EQ(ret.fixNeeded, 0);
}