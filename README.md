# rjk::flag

Compile-time boolean annotations for for C++26 reflection, analogous to `explicit(bool)` or `noexcept(bool)`.

Try it on [Compiler Explorer](https://godbolt.org/z/cx9nTsrKh)!

```c++
inline constexpr rjk::flag serializable{};
inline constexpr rjk::flag skip_field{};

template <typename T>
struct [[ =serializable ]] MyType {
    int x;
    int y;

    [[ =skip_field(std::is_pointer_v<T>) ]]
    T data;
};

// serializable is applied unconditionally
static_assert(rjk::is_flag_set(^^MyType<int>, serializable));

// skip_field is applied conditionally
static_assert(not rjk::is_flag_set(^^MyType<int>::data, skip_field));
static_assert(rjk::is_flag_set(^^MyType<int*>::data, skip_field));
```

## Motivation

[P3394: Annotations for Reflection](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2025/p3394r4.html) demonstrates creating flag annotations as such:

```c++
inline constexpr struct{} serializable{};
```

But, similarly to `explicit` or `noexcept`, it may be desirable for users to toggle whether or not an entity "opts-in" to a flag based on surrounding context.

For example, the alternative spelling for the example above would require a template specialization:

```c++
inline constexpr struct{} serializable{};
inline constexpr struct{} skip_field{};

template <typename T>
struct [[ =serializable ]] MyType {
    int x;
    int y;
    T data;
};

template <typename T> requires std::is_pointer_v<T>
struct [[ =serializable ]] MyType<T> {
    int x;
    int y;

    [[ =skip_field ]]
    T data;
};
```

Alongside additional boilerplate code for checking whether or not the annotation is present. A simple `rjk::flag` type makes annotations far more usable for generic programming.

## API

```c++
template <typename Tag = decltype([] {}), bool On = true>
struct flag {
    consteval std::meta::info operator(bool b) const requires On;
};

template <typename T>
concept flag_type = (has_template_arguments(^^T) && template_of(^^T) == ^^flag);

consteval bool is_flag_set(std::meta::info entity, flag_type auto f);
```

The template arguments on `flag` are defaulted to allow simple `rjk::flag` declaration syntax. It is ill-advised to explicitly provide template arguments to `flag`.

`flag<Tag>::operator()` accepts a boolean `b` and returns a reflection of `flag<Tag, b>`.

`flag_type<T>` is a concept that checks if `T` is an template specialization of `flag`.

`is_flag_set` queries whether an entity that can hold an annotation has the provided flag `f`. Formally, for each annotation on `entity`, it will check if the annotation
is either an expression of type `type_of(f)` or is a reflection of type `type_of(f)`.

## License

This project is licensed under the terms of the [Boost Software 1.0 License](LICENSE).
