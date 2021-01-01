#include <gtest/gtest.h>

#include <utils.h>
#include <numeric>

using namespace com;

TEST(endianness, isLittleEndian)
{
  std::uint16_t n = 1;
  bool runtime_check = *(reinterpret_cast<std::uint8_t*>(&n)) == 1;
  EXPECT_EQ(runtime_check, utils::isLittleEndian());
}

TEST(reverseMemcpy, reverseMemcpy)
{
  std::int8_t src[100];
  std::int8_t dest[100];

  std::iota(src, src + 100, 1);
  utils::reverseMemcpy(dest, src, 100);

  for (size_t i = 0; i < 100; i++)
  {
    EXPECT_EQ(dest[i], 100 - i);
  }
}

TEST(endianness, littleEndianUINT16)
{
  std::uint16_t x = 0x0123;
  std::uint16_t dest;

  utils::copyToLittleEndian(x, &dest);
  
  std::uint8_t* dest_ptr = reinterpret_cast<std::uint8_t*>(&dest);
  EXPECT_EQ(*(dest_ptr + 0), 0x23);
  EXPECT_EQ(*(dest_ptr + 1), 0x01);
}

TEST(endianness, bigEndianUINT16)
{
  std::uint16_t x = 0x0123;
  std::uint16_t dest;

  utils::copyToBigEndian(x, &dest);

  std::uint8_t* dest_ptr = reinterpret_cast<std::uint8_t*>(&dest);
  EXPECT_EQ(*(dest_ptr + 0), 0x01);
  EXPECT_EQ(*(dest_ptr + 1), 0x23);
}

TEST(endianness, littleEndianUINT32)
{
  std::uint32_t x = 0x01234567;
  std::uint32_t dest;

  utils::copyToLittleEndian(x, &dest);

  std::uint8_t* dest_ptr = reinterpret_cast<std::uint8_t*>(&dest);
  EXPECT_EQ(*(dest_ptr + 0), 0x67);
  EXPECT_EQ(*(dest_ptr + 1), 0x45);
  EXPECT_EQ(*(dest_ptr + 2), 0x23);
  EXPECT_EQ(*(dest_ptr + 3), 0x01);
}

TEST(endianness, bigEndianUINT32)
{
  std::uint32_t x = 0x01234567;
  std::uint32_t dest;

  utils::copyToBigEndian(x, &dest);
  
  std::uint8_t* dest_ptr = reinterpret_cast<std::uint8_t*>(&dest);
  EXPECT_EQ(*(dest_ptr + 0), 0x01);
  EXPECT_EQ(*(dest_ptr + 1), 0x23);
  EXPECT_EQ(*(dest_ptr + 2), 0x45);
  EXPECT_EQ(*(dest_ptr + 3), 0x67);
}

TEST(endianness, littleEndianUINT64)
{
  std::uint64_t x = 0x0123456789ABCDEF; 
  std::uint64_t dest;

  utils::copyToLittleEndian(x, &dest);

  std::uint8_t* dest_ptr = reinterpret_cast<std::uint8_t*>(&dest);
  EXPECT_EQ(*(dest_ptr + 0), 0xEF);
  EXPECT_EQ(*(dest_ptr + 1), 0xCD);
  EXPECT_EQ(*(dest_ptr + 2), 0xAB);
  EXPECT_EQ(*(dest_ptr + 3), 0x89);
  EXPECT_EQ(*(dest_ptr + 4), 0x67);
  EXPECT_EQ(*(dest_ptr + 5), 0x45);
  EXPECT_EQ(*(dest_ptr + 6), 0x23);
  EXPECT_EQ(*(dest_ptr + 7), 0x01);
}

TEST(endianness, bigEndianUINT64)
{
  std::uint64_t x = 0x0123456789ABCDEF;
  std::uint64_t dest;

  utils::copyToBigEndian(x, &dest);

  std::uint8_t* dest_ptr = reinterpret_cast<std::uint8_t*>(&dest);
  EXPECT_EQ(*(dest_ptr + 0), 0x01);
  EXPECT_EQ(*(dest_ptr + 1), 0x23);
  EXPECT_EQ(*(dest_ptr + 2), 0x45);
  EXPECT_EQ(*(dest_ptr + 3), 0x67);
  EXPECT_EQ(*(dest_ptr + 4), 0x89);
  EXPECT_EQ(*(dest_ptr + 5), 0xAB);
  EXPECT_EQ(*(dest_ptr + 6), 0xCD);
  EXPECT_EQ(*(dest_ptr + 7), 0xEF);
}

TEST(endianness, fromLittleEndianUINT16)
{
  std::uint16_t x = 0x0123;
  std::uint16_t x1;

  utils::copyToLittleEndian(x, &x1);
  std::uint16_t dest = utils::copyFromLittleEndian<std::uint16_t>(&x1);

  EXPECT_EQ(x, dest);
}

TEST(endianness, fromBigEndianUINT16)
{
  std::uint16_t x = 0x0123;
  std::uint16_t x1;

  utils::copyToBigEndian(x, &x1);
  std::uint16_t dest = utils::copyFromBigEndian<std::uint16_t>(&x1);

  EXPECT_EQ(x, dest);
}

TEST(endianness, fromLittleEndianUINT32)
{
  std::uint32_t x = 0x01234567;
  std::uint32_t x1;

  utils::copyToLittleEndian(x, &x1);
  std::uint32_t dest = utils::copyFromLittleEndian<std::uint32_t>(&x1);

  EXPECT_EQ(x, dest);
}

TEST(endianness, fromBigEndianUINT32)
{
  std::uint32_t x = 0x01234567;
  std::uint32_t x1;

  utils::copyToBigEndian(x, &x1);
  std::uint32_t dest = utils::copyFromBigEndian<std::uint32_t>(&x1);

  EXPECT_EQ(x, dest);
}

TEST(endianness, fromLittleEndianUINT64)
{
  std::uint64_t x = 0x0123456789ABCDEF;
  std::uint64_t x1;

  utils::copyToLittleEndian(x, &x1);
  std::uint64_t dest = utils::copyFromLittleEndian<std::uint64_t>(&x1);

  EXPECT_EQ(x, dest);
}

TEST(endianness, fromBigEndianUINT64)
{
  std::uint64_t x = 0x0123456789ABCDEF;
  std::uint64_t x1;

  utils::copyToBigEndian(x, &x1);
  std::uint64_t dest = utils::copyFromBigEndian<std::uint64_t>(&x1);

  EXPECT_EQ(x, dest);
}