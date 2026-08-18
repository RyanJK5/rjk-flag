#include <meta>

namespace rjk {

template <typename Tag, bool On>
struct flag {
    consteval auto operator()(bool b) const {
        return substitute(^^rjk::flag, {^^Tag, std::meta::reflect_constant(b)});
    }
};

template <typename Tag = decltype([] {})>
consteval auto make_flag() {
    return flag<Tag, true>{};
}

consteval bool is_flag_set(std::meta::info entity, auto flag) {
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
