#include <meta>

namespace rjk {

template <typename Tag = decltype([] {}), bool On = true>
struct flag {
    consteval std::meta::info operator()(bool b) const requires On {
        return substitute(^^rjk::flag, {^^Tag, std::meta::reflect_constant(b)});
    }
};

template <typename T>
concept flag_type = (has_template_arguments(^^T) && template_of(^^T) == ^^flag);

consteval bool is_flag_set(std::meta::info entity, flag_type auto flag) {
    for (const auto annotation : annotations_of(entity)) {
        if (type_of(annotation) == dealias(^^std::meta::info)) {
            if (extract<std::meta::info>(annotation) == type_of(^^flag)) {
                return true;
            }
        } else if (decay(type_of(annotation)) == type_of(^^flag)) {
            return true;
        }
    }
    return false;
}

}  // namespace rjk
