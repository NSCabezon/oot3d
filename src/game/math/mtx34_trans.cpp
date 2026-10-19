extern "C" {
#include "mtx34_trans.hpp"

// 3DS Matrix_Translate: mode 1 = apply translation to m, otherwise m = identity + translation.
void FUN_003713fc(MTX34* m, s32 mode, f32 x, f32 y, f32 z) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    if (mode == 1) {
        Matrix_UpdatePosition(m, m, &t);
    } else {
        MTX34_MakeTrans(m, &t);
    }
}
}
