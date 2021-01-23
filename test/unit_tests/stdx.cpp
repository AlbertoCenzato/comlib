#include <com/stdx.h>
#include <gtest/gtest.h>

using namespace com::stdx;

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