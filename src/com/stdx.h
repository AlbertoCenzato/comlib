#pragma once

#include "utils.h"

namespace com::stdx
{

template <class T>
void swap(T& a, T& b) {
  T tmp = a;
  a = b;
  b = tmp;
}


template <class T>
class UPtr;

template <class T>
void swap(UPtr<T>& a, UPtr<T>& b);

template <class T>
class UPtr
{
public:
  UPtr() : owned_ptr(nullptr) {}
  UPtr(T*&& ptr) : owned_ptr(ptr) {}

  UPtr(const UPtr<T>& ptr) = delete;
  UPtr(UPtr<T>&& ptr) : UPtr<T>() {
    swap(*this, ptr);
  }

  UPtr<T>& operator=(const UPtr<T>& ptr) = delete;
  UPtr<T>& operator=(UPtr<T> ptr) {
    swap(*this, ptr);
    return *this;
  }

  ~UPtr() {
    if (owned_ptr)
      delete owned_ptr;
  }

  friend void swap<T>(UPtr<T>& ptr1, UPtr<T>& ptr2);

  T& operator*() { return *this->owned_ptr; }
  const T& operator*() const { return *this->owned_ptr; }
  T* operator->() { return this->owned_ptr; }
  const T* operator->() const { return this->owned_ptr; }

private:
  T* owned_ptr;
};

template <class T>
void swap(UPtr<T>& ptr1, UPtr<T>& ptr2) {
  swap(ptr1.owned_ptr, ptr2.owned_ptr);
}

}
