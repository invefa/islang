#include "isl_list.h"

inline ist_usize ceil_upon_powertwo(ist_usize _value) {
    if (!_value) return 1;
    /*
        If you want the result to be the nearest power of two but not more than that,
        you need to uncomment the following line. (and now it is)
    */
    --_value;
    _value |= _value >> 1;
    _value |= _value >> 2;
    _value |= _value >> 4;
    _value |= _value >> 8;
    _value |= _value >> 16;
    _value |= _value >> 32;
    // _value |= _value >> 64; // can we use 16 byte integer? unnecessary.
    return ++_value;
}
