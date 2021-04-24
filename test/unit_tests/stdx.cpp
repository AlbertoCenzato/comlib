#include <com/stdx.h>
#include <gtest/gtest.h>

using namespace com::stdx;

TEST(swap, intswap)
{
  int a = 8;
  int b = 19;
  swap(a, b);
  EXPECT_EQ(a, 19);
  EXPECT_EQ(b, 8);
}

TEST(UPtr, defaultConstructor)
{
  EXPECT_NO_THROW(
    UPtr<int> ptr;
  );
}

TEST(UPtr, ptrMoveConstructor)
{
  EXPECT_NO_THROW(
    UPtr<int> ptr{ new int(5) };
  );
}

TEST(UPtr, nullptrConstructor)
{
  EXPECT_NO_THROW(
    UPtr<int> ptr{ nullptr };
  );
}

void func(UPtr<int> ptr) {}

TEST(UPtr, implicitConversionFromNullptr)
{
  EXPECT_NO_THROW(
    func(nullptr);
  );
}

TEST(UPtr, implicitConversionToBool)
{
  EXPECT_TRUE(UPtr<int>{ new int(6) });
  EXPECT_FALSE(UPtr<int>{ nullptr });
}

TEST(UPtr, operatorEQ)
{
  UPtr<int> ptr1{ new int(6) };
  UPtr<int> ptr{ nullptr };
  UPtr<int> ptr3{ nullptr };

  EXPECT_EQ(ptr, ptr3);
  EXPECT_EQ(ptr, nullptr);
  EXPECT_EQ(nullptr, ptr3);
  EXPECT_NE(ptr1, ptr3);
  EXPECT_NE(ptr1, nullptr);
}


/*
*
* NOTE: this should not compile
*
TEST(UPtr, deletedPtrConstructor)
{
  EXPECT_NO_THROW(
    auto int_ptr = new int(5);
    UPtr<int> ptr{ int_ptr };
  );
}

TEST(UPtr, deletedCopyConstructor)
{
  EXPECT_NO_THROW(
    UPtr<int> ptr1{ new int(5) };
    UPtr<int> ptr2{ ptr1 };
  );
}
*/

TEST(UPtr, moveConstructor)
{
  EXPECT_NO_THROW(
    UPtr<int> ptr{ UPtr<int>{ new int(5) } };
  );
}

TEST(UPtr, moveAssignmentOperator)
{
  EXPECT_NO_THROW(
    UPtr<int> ptr = UPtr<int>{ new int(5) };
  );
}

TEST(UPtr, operatorStar)
{
  UPtr<int> ptr{ new int(5) };
  *ptr = 4;
  EXPECT_EQ(4, *ptr);
}


struct TinyStruct {
  int a;
  int b;
};

TEST(UPtr, operatorArrow)
{
  UPtr<TinyStruct> ptr{ new TinyStruct{1,2} };
  ptr->a = 3;
  EXPECT_EQ(ptr->a, 3);
  EXPECT_EQ(ptr->b, 2);
}

TEST(UPtr, swap)
{
  UPtr<TinyStruct> ptr1{ new TinyStruct{1,2} };
  UPtr<TinyStruct> ptr2{ new TinyStruct{3,4} };
  swap(ptr1, ptr2);
  EXPECT_EQ(ptr1->a, 3);
  EXPECT_EQ(ptr1->b, 4);
  EXPECT_EQ(ptr2->a, 1);
  EXPECT_EQ(ptr2->b, 2);
}

TEST(vector, defaultConstructor)
{
  vector<int> array;
  EXPECT_EQ(array.size(), 0);
  EXPECT_EQ(array.begin(), nullptr);
  EXPECT_EQ(array.end(), static_cast<int*>(nullptr) + array.size());
}

TEST(vector, constructor0)
{
  vector<int> array{ 0 };
  EXPECT_EQ(array.size(), 0);
  EXPECT_EQ(array.begin(), nullptr);
  EXPECT_EQ(array.end(), static_cast<int*>(nullptr) + array.size());
}

TEST(vector, constructor)
{
  vector<int> array{ 256 };
  EXPECT_EQ(array.size(), 256);
  EXPECT_EQ(array.end() - array.begin(), 256);
}

TEST(vector, copyConstructor)
{
  vector<int> array_a{ 256 };
  for (auto& v : array_a)
    v = rand();

  vector<int> array_b{ array_a };
  EXPECT_NE(array_a.begin(), array_b.begin());
  EXPECT_EQ(array_a.size(), array_b.size());
  for (size_t i = 0; i < array_a.size(); i++)
    EXPECT_EQ(array_a[i], array_b[i]);
}

TEST(vector, moveConstructor)
{
  vector<int> array_a{ 256 };
  for (auto& v : array_a)
    v = rand();

  vector<int> array_b{ array_a };
  vector<int> array_c{ std::move(array_b) };

  EXPECT_EQ(array_a.size(), array_c.size());
  for (size_t i = 0; i < array_a.size(); i++)
    EXPECT_EQ(array_a[i], array_c[i]);
  EXPECT_EQ(array_b.size(), 0);
  EXPECT_EQ(array_b.begin(), nullptr);
}

TEST(vector, copyAssignment)
{
  vector<int> array_a{ 256 };
  for (auto& v : array_a)
    v = rand();
  
  auto array_b = array_a;
  EXPECT_NE(array_a.begin(), array_b.begin());
  EXPECT_EQ(array_a.size(), array_b.size());
  for (size_t i = 0; i < array_a.size(); i++)
    EXPECT_EQ(array_a[i], array_b[i]);
}
