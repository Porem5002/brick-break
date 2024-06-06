#pragma once

#include <cassert>
#include <cstring>
#include <memory>
#include <utility>

template<typename T>
class closure_state
{
    T* data;

public:
    template<typename ...Tparams>
    closure_state(Tparams... p) : data(new T(std::forward<Tparams>(p)...)) { }

    void* extract() { 
        T* p = data;
        data = nullptr;
        return p;
    }

    ~closure_state()
    {
        assert(data == nullptr);
    }

    static void* copy(void* src)
    { 
        const T& typed_src = *(const T*)src;
        return new T(typed_src);
    }

    static void cleanup(void* p)
    { 
        delete (T*)p;
    }
};

template<typename T>
class closure;

template<typename Tresult, typename ...Tparams>
class closure<Tresult (Tparams...)>
{
    using func_type = Tresult (*)(void*, Tparams...);
    
    template<typename T>
    using func_type_safe = Tresult (*)(T*, Tparams...);

    using copy_func = void* (*)(void* src);
    using cleanup_func = void (*)(void* state);

    void* state;
    func_type func;
    copy_func copy;
    cleanup_func cleanup;

    void* do_copy() const
    {
        return copy == nullptr ? state : copy(state);
    }

    void do_cleanup()
    {
        if(cleanup != nullptr) cleanup(state);
    }

    void after_move()
    {
        state = nullptr;
        func = nullptr;
        copy = nullptr;
        cleanup = nullptr;
    }

public:
    template<typename Func>
    closure(Func func) 
        : state(nullptr), func(func), copy(nullptr), cleanup(nullptr) { }

    template<typename T, typename Func>
    closure(T* state, Func func_, cleanup_func cleanup = nullptr, copy_func copy = nullptr) 
        : state(state), copy(copy), cleanup(cleanup)
    {
        func_type_safe<T> validate_func = func_;

        memcpy(&func, &validate_func, sizeof(func_type));
    }

    template<typename T, typename Func>
    closure(closure_state<T>& state, Func func_)
        : state(state.extract()), copy(closure_state<T>::copy), cleanup(closure_state<T>::cleanup)
    {
        func_type_safe<T> validate_func = func_;

        memcpy(&func, &validate_func, sizeof(func_type));
    }

    closure(closure&& v)
        : state(v.state), func(v.func), copy(v.copy), cleanup(v.cleanup)
    {
        v.after_move();
    }

    closure(const closure& v)
        : state(v.do_copy()), func(v.func), copy(v.copy), cleanup(v.cleanup) { }

    closure& operator=(closure&& v)
    {
        if(this != &v)
        {
            do_cleanup();

            state = v.state;
            func = v.func;
            copy = v.copy;
            cleanup = v.cleanup;

            v.after_move();
        }

        return *this;
    }

    closure& operator=(const closure& v)
    {
        if(this != &v)
        {
            do_cleanup();

            state = v.do_copy();
            func = v.func;
            copy = v.copy;
            cleanup = v.cleanup;
        }

        return *this;
    }

    Tresult operator()(Tparams... p)
    {
        assert(func != nullptr);
        return func(state, std::forward<Tparams>(p)...);
    }

    ~closure()
    {
        do_cleanup();
    }
};

template<typename T>
using ClosureState = closure_state<T>;

template<typename T>
using Closure = closure<T>;