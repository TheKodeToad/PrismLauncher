#pragma once

template <typename T, void (*TDeleteFunc)(T*)>
struct FuncDeleter {
    void operator()(T* t) { TDeleteFunc(t); }
};

template <typename T, void (*TDeleteFunc)(T*)>
class Handle : public std::unique_ptr<T, FuncDeleter<T, TDeleteFunc>> {
   public:
    using std::unique_ptr<T, FuncDeleter<T, TDeleteFunc>>::unique_ptr;

    operator T*() const { return this->get(); }
};