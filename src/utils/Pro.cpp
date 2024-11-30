#include "Pro.hpp"

#ifdef BETTEREDIT_PRO
#include <pro/Pro.hpp>
bool be::isProEnabled() {
    return HAS_PRO();
}
bool be::isProUIEnabled() {
    return pro::isNewUIEnabled();
}
#else
bool be::isProEnabled() {
    return false;
}
bool be::isProUIEnabled() {
    return false;
}
#endif
