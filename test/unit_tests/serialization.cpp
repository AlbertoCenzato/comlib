#include <gtest/gtest.h>
#include <com/serialization.h>
#include <com/stdx.h>
#include <com/msg/MovePath.h>

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

TEST(serialize, movePath) {
  msg::MovePath move_path;

  float count = 0;
  for (auto& point : move_path.path) {
    point = { count, count + 1, count + 2 };
    count++;
  }

  const auto size = move_path.getSize();
  stdx::vector<uint8_t> buffer{size};
  EXPECT_EQ(buffer.end() - buffer.begin(), size);
  auto buff_ptr = serialize(move_path, buffer.begin());
  EXPECT_EQ(buff_ptr, buffer.end());

  const uint8_t* new_buff_ptr = nullptr;
  auto message = msg::MovePath::deserialize(buffer.begin(), &new_buff_ptr);
  size_t diff1 = new_buff_ptr - buffer.begin();
  size_t diff2 = buffer.end() - buffer.begin();
  EXPECT_EQ(new_buff_ptr, buffer.end());

  const msg::MovePath* deserialized_message = reinterpret_cast<msg::MovePath*>(message.get());
  for (size_t i = 0; i < deserialized_message->path.size(); i++) {
    const auto& p1 = deserialized_message->path[i];
    const auto& p2 = move_path.path[i];
    EXPECT_EQ(p1.x, p2.x);
    EXPECT_EQ(p1.y, p2.y);
    EXPECT_EQ(p1.rot, p2.rot);
  }
}