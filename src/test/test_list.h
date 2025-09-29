#include "isg_list.h"
#include "isl_list.h"

// unofficial sha64 hash algorithm
ist_u64 sha64(const void* data, size_t length) {
    const ist_u8* bytes = (const ist_u8*)data;
    ist_u64       hash  = 0xcbf29ce484222325ULL; // FNV-1a 64-bit offset basis

    for (size_t i = 0; i < length; ++i) {
        hash ^= bytes[i];
        hash *= 0x100000001b3ULL; // FNV-1a 64-bit prime
    }
    return hash;
}


void test_isg_list() {
    ist_usize_list* list = ist_usize_list_calloc(128);

    isg_list_foreach (item, *list, index) {
        *item = sha64(&index, sizeof(index));
    }

    isg_list_foreach (item, *list, index) {
        ist_u64 expected = sha64(&index, sizeof(index));
        ist_u64 actual   = *item;
        isl_assert(expected == actual);
    }

    ist_usize_list_delete(list);
}