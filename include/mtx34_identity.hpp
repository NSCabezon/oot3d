#ifndef _MTX34_IDENTITY_HPP_
#define _MTX34_IDENTITY_HPP_

#include "global.hpp"

// Shared identity matrix reached through a function-local static in an inline accessor (nn::math
// MTX34::Identity()-style). armcc inlines the guard check + ctor into every caller (~70 ROM functions,
// incl. the out-of-line MTX34Identity @0x003009F0):
//   object  MTX34_Identity()::identity  @ 0x005BE29C (.bss__ZZ14MTX34_IdentityvE8identity, pinned in oot3d.ld)
//   guard                               @ 0x0055A210 (.data__ZZ14MTX34_IdentityvE8identity, pinned in oot3d.ld;
//                                                     cut out of data_54AC28 by data_chunks.txt)
// No dtor (trivially destructible) -> no __aeabi_atexit. __cxa_guard_release is the empty C stub
// (src/game/model_manager/model_manager.cpp) -> linker-inlined `mov r0, r0`.

extern "C++" {

struct MTX34Init : MTX34 {
    MTX34Init(f32 x00, f32 x01, f32 x02, f32 x03, f32 x10, f32 x11, f32 x12, f32 x13, f32 x20, f32 x21, f32 x22,
              f32 x23) {
        data[0][0] = x00;
        data[0][1] = x01;
        data[0][2] = x02;
        data[0][3] = x03;
        data[1][0] = x10;
        data[1][1] = x11;
        data[1][2] = x12;
        data[1][3] = x13;
        data[2][0] = x20;
        data[2][1] = x21;
        data[2][2] = x22;
        data[2][3] = x23;
    }
};

inline const MTX34& MTX34_Identity() {
    static const MTX34Init identity(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    return identity;
}

} // extern "C++"

#endif
