#include <gtest/gtest.h>
#include <com/serialization.h>

using namespace com;

TEST(serialize, arrayInt) {
  using array = stdx::array<int32_t, 15>;

  array a;
  for (int32_t i = 0; i < a.size(); i++)
    a[i] = i; 

  uint8_t buffer[array::size() * sizeof(int32_t)];
  auto end = serialize(a, buffer);

  array b;
  auto end2 = deserialize(buffer, b);

  for (size_t i = 0; i < a.size(); i++)
    EXPECT_EQ(a[i], b[i]);
}

TEST(serialize, arrayArrayInt) {
  using array = stdx::array<stdx::array<float, 6>, 56>;

  array a;
  for (auto& val : a)
    for (int32_t i = 0; i < array::value_type::size(); i++)
      val[i] = static_cast<float>(i);

  uint8_t buffer[array::size() * array::value_type::size() * sizeof(float)];
  auto end = serialize(a, buffer);

  array b;
  auto end2 = deserialize(buffer, b);

  for (size_t i = 0; i < a.size(); i++)
    for (size_t j = 0; j < array::value_type::size(); j++)
      EXPECT_EQ(a[i][j], b[i][j]);
}