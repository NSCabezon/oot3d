// Draft (not matched): EnZf_PostLimbDraw is 195/339 words off. 9 function-local Vector3f statics (guards 0x53069C..0x5306BC, objects 0x59B8EC..0x59B958,
// the first {1100,-700,0} is unused but initialised in ROM; armcc drops it here) need pin_static (own TU + guard/bss pins) and the VFP numbering of the 9 inits is unsolved.
// GLOBAL_ASM stays in z_en_zf.cpp.
#if 0
extern "C" {
#include "z_en_zf.hpp"

void FUN_0035479c(void* quad, Vec3f* a, Vec3f* b, Vec3f* c, Vec3f* d);
void* FUN_00362384(s32 index);
void FUN_0035eb74(void* blure);
void FUN_003620f0(void* blure, Vec3f* p1, Vec3f* p2);
void FUN_0034cbb4(Actor* actor, MTX34* mtx, s32 limbIndex, s32 a, Vector3f* b, s32 c, Vector3f* d);

void EnZf_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    static Vector3f sUnused(1100.0f, -700.0f, 0.0f);
    static Vector3f sFootOffset(300.0f, 0.0f, 0.0f);
    static Vector3f sSwordBlureP1(300.0f, -1700.0f, 0.0f);
    static Vector3f sSwordBlureP2(-600.0f, 300.0f, 0.0f);
    static Vector3f sSwordQuad1(0.0f, 1500.0f, 0.0f);
    static Vector3f sSwordQuad0(-600.0f, -3000.0f, 1000.0f);
    static Vector3f sSwordQuad3(-600.0f, -3000.0f, -1000.0f);
    static Vector3f sSwordQuad2(1500.0f, -3000.0f, 0.0f);
    static Vector3f sBodyPartOffset(0.0f, 0.0f, 0.0f);
    Vec3f sp54;
    Vec3f sp48;
    s32 bodyPartIndex = -1;
    EnZf* self = (EnZf*)thisx;
    u8* quad = (u8*)self + 0xB1C;

    if (limbIndex == 13) {
        VEC3TransformAsm((Vector3f*)(quad + 0xC), mtx, &sSwordQuad1);
        VEC3TransformAsm((Vector3f*)(quad + 0x0), mtx, &sSwordQuad0);
        VEC3TransformAsm((Vector3f*)(quad + 0x24), mtx, &sSwordQuad3);
        VEC3TransformAsm((Vector3f*)(quad + 0x18), mtx, &sSwordQuad2);
        FUN_0035479c(&self->swordCollider, (Vec3f*)(quad + 0x0), (Vec3f*)(quad + 0xC), (Vec3f*)(quad + 0x18), (Vec3f*)(quad + 0x24));
        VEC3TransformAsm((Vector3f*)&sp54, mtx, &sSwordBlureP1);
        VEC3TransformAsm((Vector3f*)&sp48, mtx, &sSwordBlureP2);
        if (self->action == 9) {
            if (self->skelAnime.curFrame < 14.0f) {
                FUN_0035eb74(FUN_00362384(self->swordBlureEffectIndex));
            } else if (self->skelAnime.curFrame < 20.0f) {
                FUN_003620f0(FUN_00362384(self->swordBlureEffectIndex), &sp54, &sp48);
            }
        }
    } else {
        FUN_0034cbb4(&self->actor, mtx, limbIndex, 3, &sFootOffset, 6, &sFootOffset);
    }
    switch (limbIndex) {
        case 3:
            VEC3TransformAsm((Vector3f*)&self->leftFootPos, mtx, &sFootOffset);
            break;
        case 6:
            VEC3TransformAsm((Vector3f*)&self->rightFootPos, mtx, &sFootOffset);
            break;
    }
    if (self->frozenTimer != 0) {
        switch (limbIndex) {
            case 15:
                bodyPartIndex = 0;
                break;
            case 14:
                bodyPartIndex = 1;
                break;
            case 7:
                bodyPartIndex = 2;
                break;
            case 12:
                bodyPartIndex = 3;
                break;
            case 9:
                bodyPartIndex = 4;
                break;
            case 0:
                bodyPartIndex = 5;
                break;
            case 13:
                bodyPartIndex = 6;
                break;
            case 5:
                bodyPartIndex = 7;
                break;
            case 2:
                bodyPartIndex = 8;
                break;
        }
        if (bodyPartIndex >= 0) {
            VEC3TransformAsm((Vector3f*)&self->bodyPartsPos[bodyPartIndex], mtx, &sBodyPartOffset);
        }
    }
}

}
#endif
