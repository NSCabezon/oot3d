#ifndef _MTX34_TRANS_HPP_
#define _MTX34_TRANS_HPP_

#include "global.hpp"

// Identity + translation MTX34 written WORD-WISE through integer registers (SDK-style raw MakeTrans).
// This is the "new matrix" branch of the 3DS Matrix_Translate (FUN_003713fc, src/game/math/mtx34_trans.cpp)
// and is inlined into ~80 Draw functions (DoorAna_Draw, OceffWipe*_Draw, BgHidanRock FUN_003d0830...).
// ACCEPTED EXCEPTION (user decision 2026-10-15) to the "no type-pun" rule: ~30 natural float shapes were
// ruled out; only this u32 copy reproduces the library function byte-exact (0/31 words). Use ONLY this
// helper; never hand-write the u32 stores in actor code.

#ifdef __cplusplus
extern "C" {
#endif

void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);

static inline void MTX34_MakeTrans(MTX34* m, const Vec3f* t) {
    u32* d = (u32*)m;
    const u32* s = (const u32*)t;

    d[0] = 0x3F800000;
    d[1] = 0;
    d[2] = 0;
    d[3] = s[0];
    d[4] = 0;
    d[5] = 0x3F800000;
    d[6] = 0;
    d[7] = s[1];
    d[8] = 0;
    d[9] = 0;
    d[10] = 0x3F800000;
    d[11] = s[2];
}

// Scale MTX34 (diagonal = scale, rest 0) written WORD-WISE through integer registers: the inlined
// "new matrix" branch of the 3DS Matrix_Scale, same SDK-style raw shape as MTX34_MakeTrans above.
// Users: EnPart_Update, FUN_0038639c (En_Dnt_Nomal DrawTargetScrub), En_Horse_Normal FUN_003a5778,
// EnDntJiji_Draw. ACCEPTED EXCEPTION (user decision 2026-10-15) to the "no type-pun" rule, like
// MakeTrans; revisit if a clean float/SDK source is found. Use ONLY this helper; never hand-write the
// u32 stores in actor code.
static inline void MTX34_MakeScale(MTX34* m, const Vec3f* s) {
    u32* d = (u32*)m;
    const u32* v = (const u32*)s;

    d[0] = v[0];
    d[1] = 0;
    d[2] = 0;
    d[3] = 0;
    d[4] = 0;
    d[5] = v[1];
    d[6] = 0;
    d[7] = 0;
    d[8] = 0;
    d[9] = 0;
    d[10] = v[2];
    d[11] = 0;
}

#ifdef __cplusplus
}
#endif

#endif
