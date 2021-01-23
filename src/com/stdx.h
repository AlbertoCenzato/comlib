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
T&& move(T& a) {
  return static_cast<T&&>(a);
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
  UPtr(T*&& ptr) : UPtr() { swap(owned_ptr, ptr); }
  UPtr(nullptr_t ptr) : UPtr() {}

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

  operator bool() const { return owned_ptr; }

  friend void swap<T>(UPtr<T>& ptr1, UPtr<T>& ptr2);

  T& operator*() { return *this->owned_ptr; }
  const T& operator*() const { return *this->owned_ptr; }
  T* operator->() { return this->owned_ptr; }
  const T* operator->() const { return this->owned_ptr; }

  const T* get() const { return owned_ptr; }
  T* get() { return owned_ptr; }

private:
  T* owned_ptr;
};

template <class T>
bool operator==(const UPtr<T>& a, const UPtr<T>& b) { return a.get() == b.get(); }
template <class T>
bool operator!=(const UPtr<T>& a, const UPtr<T>& b) { return !(a == b); }

template <class T>
bool operator==(nullptr_t a, const UPtr<T>& b) { return UPtr<T>(a) == b; }
template <class T>
bool operator!=(nullptr_t a, const UPtr<T>& b) { return !(a == b); }

template <class T>
bool operator==(const UPtr<T>& a, nullptr_t b) { return a == UPtr<T>(b); }
template <class T>
bool operator!=(const UPtr<T>& a, nullptr_t b) { return !(a == b); }


template <class T>
void swap(UPtr<T>& ptr1, UPtr<T>& ptr2) {
  swap(ptr1.owned_ptr, ptr2.owned_ptr);
}

}
