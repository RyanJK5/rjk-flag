#include <meta>

namespace rjk {

template <typename Tag = decltype([] {})>
struct flag {
    bool on = true;

    consteval flag operator()(bool b) const {
        return flag{.on = b};
    }
};

template <typename T>
concept flag_type = (has_template_arguments(^^T) && template_of(^^T) == ^^flag);

consteval bool is_flag_set(std::meta::info entity, flag_type auto f) {
    for (const auto annotation : annotations_of(entity)) {
        if (decay(type_of(annotation)) != type_of(^^f)) {
            continue;
        }

        const bool valid = extract<decltype(f)>(annotation).on;
        if (valid) {
            return true;
        }    
    }
    return false;
}

}  // namespace rjk
