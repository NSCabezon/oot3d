extern "C" {
#include "z_en_ko.hpp"
#include "z3Dscene.hpp"

#define FLAGS 0x00000019

void EnKo_Init(Actor* actor, GameState* state);
void EnKo_Destroy(Actor* actor, GameState* state);
void EnKo_Update(Actor* actor, GameState* state);
void EnKo_Draw(Actor* actor, GameState* state);
void FUN_0034e01c(MTX34* mtx, Vec3s* rot);
void FUN_003c3790(Actor* actor, GameState* state);
void FUN_00118960(Actor* actor, GameState* state);
void __ARM_common_memcpy2_6(void* dst, const void* src);
extern u16 gSaveContext_F00[];
extern struct_80034EC0_Entry EnKo_sAnimationInfo[];
void FUN_00134cec(Actor* actor, GameState* state);
void FUN_00146798(Actor* actor, GameState* state);
void FUN_001c51f8(Actor* actor, GameState* state);
void FUN_00134c8c(Actor* actor, GameState* state);
void Path_CopyLastPoint(Path* path, Vector3f* dst);

GLOBAL_ASM("data/z_En_Ko.data.s")

ActorInit En_Ko_InitVars = {
    ACTOR_EN_KO,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnKo),
    (ActorFunc)EnKo_Init,
    (ActorFunc)EnKo_Destroy,
    (ActorFunc)EnKo_Update,
    (ActorFunc)EnKo_Draw,
};

GLOBAL_ASM("asm/EnKo_Init.s")

void EnKo_Destroy(Actor* actor, GameState* state) {
    EnKo* self = (EnKo*)actor;
    PlayState* play = (PlayState*)state;
    ((FaceAnimations*)((u8*)self + 0xACC))->Destroy();
    SkelAnime_Free2((SkelAnime*)((u8*)self + 0x1A4));
    Collider_DestroyCylinder(play, (ColliderCylinder*)((u8*)self + 0x230));
}

#if 0
void FUN_00134cec(Actor* actor, GameState* state) {
    EnKo* self = (EnKo*)actor;
    if (*(f32*)((u8*)actor + 0x1E0) == 0.0f) {
        *(void**)((u8*)self + 0x228) = FUN_003c3790;
        FUN_003717ac((SkelAnime*)((u8*)actor + 0x1A4), EnKo_sAnimationInfo, 6);
    }
}
#endif

#if 0
void FUN_00146798(Actor* actor, GameState* state) {
    EnKo* self = (EnKo*)actor;
    if (*(f32*)((u8*)actor + 0x1E0) > *(f32*)((u8*)actor + 0x1EC) - 1.0f) {
        *(void**)((u8*)self + 0x228) = FUN_00118960;
        FUN_003717ac((SkelAnime*)((u8*)actor + 0x1A4), EnKo_sAnimationInfo, 7);
    }
}
#endif

GLOBAL_ASM("asm/EnKo_Update.s")

GLOBAL_ASM("asm/EnKo_Draw.s")

void FUN_001c51f8(Actor* actor, GameState* state) {
    if (Actor_HasParent(actor, (PlayState*)state)) {
        actor->parent = NULL;
        *(void**)((u8*)actor + 0x228) = FUN_00134c8c;
    } else {
        GiveItem(actor, (PlayState*)state, 0x21, 120.0f, 85.0f);
    }
}

#if 0
void FUN_00134c8c(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    u16* step = (u16*)((u8*)actor + 0x200) + 0x46;
    if (*step == 3) {
        *(u16*)((u8*)actor + 0x116) = 0x10B9;
        Message_ContinueTextbox(play, 0x10B9);
        *step = 1;
        gSaveContext_F00[7] |= 2;
        *(void**)((u8*)actor + 0x228) = FUN_003c3790;
    }
}
#endif

void Path_CopyLastPoint(Path* path, Vector3f* dst) {
    if (path != NULL) {
        Vec3s* point = path->points + path->count - 1;
        dst->x = point->x;
        dst->y = point->y;
        dst->z = point->z;
    }
}

#if 0 // Experimental draft: replace explicit compiler memcpy helper/raw word copy with native Vec3s copy before any future activation.
s32 EnKo_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    if (limbIndex == 9) {
        Vec3s rot;
        __ARM_common_memcpy2_6(&rot, (u8*)thisx + 0x29A);
        FUN_0034e01c(mtx, &rot);
    } else if (limbIndex == 10) {
        u32 rot[2];
        rot[0] = *(u32*)((u8*)thisx + 0x294);
        rot[1] = *(u32*)((u8*)thisx + 0x298);
        FUN_0034e01c(mtx, (Vec3s*)rot);
    }
    return 0;
}
#endif
GLOBAL_ASM("asm/EnKo_OverrideLimbDraw.s")

#if 0
void EnKo_PostLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    Actor* actor = (Actor*)thisx;
    Vector3f vec(0.0f, 0.0f, 0.0f);
    if (limbIndex == 10) {
        VEC3TransformAsm((Vector3f*)((u8*)actor + 0x3C), mtx, &vec);
    }
}
#endif
GLOBAL_ASM("asm/EnKo_PostLimbDraw.s")

GLOBAL_ASM("asm/FUN_00171ed4.s")

GLOBAL_ASM("asm/FUN_0034c92c.s")

}
