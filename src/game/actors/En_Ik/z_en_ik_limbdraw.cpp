extern "C" {
#include "z_en_ik.hpp"

// FUN_0016ee30 (N64 EnIk_PostLimbDrawEnemy). Own TU: the four guarded function-local statics (N64 D_80A7847C[4])
// give this object a .data section holding only their guards (0x00528120..0x0052812C, reverse declaration order,
// ROM addresses them from 0x00528120) and a .bss with the four Vec3f (0x0059B014..0x0059B044). Pinned in
// data/oot3d.ld via `*z_en_ik_limbdraw.o(+DATA/+BSS)` (scripts/pin_static.py); the guards are cut from the data chunk.

void FUN_0034f184(void* bodyBreak, s32 limbIndex, s32 minLimb, s32 maxLimb, s32 count, MTX34* mtx); // BodyBreak_SetInfo
void FUN_0035479c(void* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d);                             // Collider_SetQuadVertices
void* FUN_00362384(s32 index);                                                                    // Effect_GetByIndex
void FUN_003620f0(void* blure, Vec3f* p1, Vec3f* p2);                                             // EffectBlure_AddVertex
void FUN_0035eb74(void* blure);                                                                   // EffectBlure_AddSpace
void Collider_SetTrisVertices(void* collider, s32 index, Vec3f* a, Vec3f* b, Vec3f* c);
// N64 D_80A784AC / D_80A784D0. Same-TU statics: ROM addresses the second as &first + 0x24. This TU's .constdata
// is pinned at 0x004D96E0 in data/oot3d.ld (cut out of the rodata_4CA000 chunk).
static const Vec3f sShieldVtxFront[3] = {
    { -3000.0f, -700.0f, -5000.0f },
    { -3000.0f, -700.0f, 2000.0f },
    { 4000.0f, -700.0f, 2000.0f },
};
static const Vec3f sShieldVtxBack[3] = {
    { 4000.0f, -700.0f, 2000.0f },
    { 4000.0f, -700.0f, -5000.0f },
    { -3000.0f, -700.0f, -5000.0f },
};

#define EN_IK_QUAD(self, i) ((Vec3f*)((u8*)(self) + 0xECC) + (i))
#define EN_IK_BLURE_IDX(self) (*(s32*)((u8*)(self) + 0xFE4))

// Vec3f with a Set()-based ctor (literal register choice in the guarded inits).
struct EnIkVec3f {
    f32 x, y, z;
    EnIkVec3f(f32 a, f32 b, f32 c) {
        Set(a, b, c);
    }
    void Set(f32 a, f32 b, f32 c) {
        x = a;
        y = b;
        z = c;
    }
};

// Ctor/Set() parameter orders below only steer armcc's literal register choice in the guarded inits (values unchanged).
struct EnIkVec3fA {
    f32 x, y, z;
    EnIkVec3fA(f32 x_, f32 y_, f32 z_) {
        Set(x_, z_, y_);
    }
    void Set(f32 x_, f32 z_, f32 y_) {
        x = x_;
        y = y_;
        z = z_;
    }
};
struct EnIkVec3fB {
    f32 x, y, z;
    EnIkVec3fB(f32 y_, f32 x_, f32 z_) {
        Set(y_, x_, z_);
    }
    void Set(f32 y_, f32 x_, f32 z_) {
        x = x_;
        y = y_;
        z = z_;
    }
};

// N64-order wrapper (Matrix_MultVec3f(src, dest)).
// mtx-first variant for the shield loop (fixes the arg setup order of the second call).
static inline void EnIk_MultVec3fM(MTX34* mtx, Vec3f* src, Vec3f* dest) {
    VEC3TransformAsm((Vector3f*)dest, mtx, (Vector3f*)src);
}

static inline void EnIk_MultVec3f(Vec3f* src, Vec3f* dest, MTX34* mtx) {
    VEC3TransformAsm((Vector3f*)dest, mtx, (Vector3f*)src);
}

void FUN_0016ee30(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static EnIkVec3fA sAxeVtx0(800.0f, -200.0f, -5200.0f);
    static EnIkVec3fB sAxeVtx1(0.0f, 0.0f, 0.0f);
    static Vector3f sAxeVtx2(-200.0f, -2200.0f, -200.0f);
    static EnIkVec3f sAxeVtx3(-6000.0f, 2000.0f, -3000.0f);
    Vec3f spF4;
    Vec3f spE8;
    EnIk* self = (EnIk*)thisx;

    if (self->unk_E0F & 1) {
        FUN_0034f184(self->unk_E20, limbIndex, 0x11, 0x12, 0x1C, mtx);
    }
    if (limbIndex == 14) {
        s32 i;
        Vec3f sp9C[3];
        Vec3f sp78[3];

        EnIk_MultVec3f((Vec3f*)&sAxeVtx0, EN_IK_QUAD(self, 1), mtx);
        EnIk_MultVec3f((Vec3f*)&sAxeVtx1, EN_IK_QUAD(self, 0), mtx);
        EnIk_MultVec3f((Vec3f*)&sAxeVtx2, EN_IK_QUAD(self, 3), mtx);
        EnIk_MultVec3f((Vec3f*)&sAxeVtx3, EN_IK_QUAD(self, 2), mtx);
        FUN_0035479c(self->axeCollider, EN_IK_QUAD(self, 0), EN_IK_QUAD(self, 1), EN_IK_QUAD(self, 2),
                     EN_IK_QUAD(self, 3));
        EnIk_MultVec3f((Vec3f*)&sAxeVtx0, &spF4, mtx);
        EnIk_MultVec3f((Vec3f*)&sAxeVtx1, &spE8, mtx);
        s8 blureState = self->unk_E12;
        if (blureState > 0) {
            FUN_003620f0(FUN_00362384(EN_IK_BLURE_IDX(self)), &spF4, &spE8);
        } else if (blureState == 0) {
            FUN_0035eb74(FUN_00362384(EN_IK_BLURE_IDX(self)));
            self->unk_E12 = 0xFF;
        }
        if (self->unk_E0C == 9) {
            for (i = 0; i < 3; i++) {
                EnIk_MultVec3fM(mtx, (Vec3f*)&sShieldVtxFront[i], &sp9C[i]);
                EnIk_MultVec3fM(mtx, (Vec3f*)&sShieldVtxBack[i], &sp78[i]);
            }
            Collider_SetTrisVertices(self->shieldCollider, 0, &sp9C[0], &sp9C[1], &sp9C[2]);
            Collider_SetTrisVertices(self->shieldCollider, 1, &sp78[0], &sp78[1], &sp78[2]);
        }
    }
}
}
