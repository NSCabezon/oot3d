// EnWf_PostLimbDraw: matches (0 words in fastchk) but needs two guarded function-local statics pinned
// (guards 0x530278/0x53027C, objects 0x59B7FC/0x59B808, 12 B each); see STATUS.md for the pin_static commands.
// The function must live in this own TU so its 8-byte .data (the guards) can be placed by an ld region.
extern "C" {
#include "z_en_wf.hpp"

void Collider_UpdateSpheres(s32 limbIndex, void* collider, MTX34* mtx);

static inline void EnWf_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

void EnWf_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sTailColliderOffset(1200.0f, 0.0f, 0.0f);
    static Vector3f sZeroVec(0.0f, 0.0f, 0.0f);
    EnWf* self = (EnWf*)thisx;
    s32 bodyPartIndex;

    bodyPartIndex = -1;
    Collider_UpdateSpheres(limbIndex, &self->attackAndBlockCollider, mtx);
    if (limbIndex == 0x13) {
        Vec3f tailColliderPos;

        EnWf_MultVec3f(&sTailColliderOffset, (Vector3f*)&tailColliderPos, mtx);
        self->tailCollider.dim.pos = tailColliderPos;
    }
    if ((*(u16*)((u8*)self + 0xCC8) != 0) ||
        ((*(u16*)((u8*)self + 0x11A) != 0) && (*(u32*)((u8*)self + 0x11C) & 0x400000))) {
        switch (limbIndex) {
            case 11: bodyPartIndex = 0; break;
            case 17: bodyPartIndex = 1; break;
            case 14: bodyPartIndex = 2; break;
            case 9: bodyPartIndex = 3; break;
            case 20: bodyPartIndex = 4; break;
            case 19: bodyPartIndex = 5; break;
            case 6: bodyPartIndex = 6; break;
            case 7: bodyPartIndex = 8; break;
            case 4: bodyPartIndex = 9; break;
        }
        if (bodyPartIndex >= 0) {
            Vec3f bodyPartPos;

            EnWf_MultVec3f(&sZeroVec, (Vector3f*)&bodyPartPos, mtx);
            self->bodyPartsPos[bodyPartIndex].x = bodyPartPos.x;
            self->bodyPartsPos[bodyPartIndex].y = bodyPartPos.y;
            self->bodyPartsPos[bodyPartIndex].z = bodyPartPos.z;
        }
    }
}
}
