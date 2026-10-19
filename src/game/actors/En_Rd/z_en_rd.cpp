extern "C" {
#include "z_en_rd.hpp"

#define FLAGS 0x00000415

void EnRd_Init(Actor* actor, GameState* state);
void EnRd_Destroy(Actor* actor, GameState* state);
void EnRd_Update(Actor* actor, GameState* state);
void EnRd_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Rd.data.s")

ActorInit En_Rd_InitVars = {
    ACTOR_EN_RD,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_RD,
    sizeof(EnRd),
    (ActorFunc)EnRd_Init,
    (ActorFunc)EnRd_Destroy,
    (ActorFunc)EnRd_Update,
    (ActorFunc)EnRd_Draw,
};

GLOBAL_ASM("asm/EnRd_Init.s")


GLOBAL_ASM("asm/EnRd_Update.s")

GLOBAL_ASM("asm/EnRd_Draw.s")

// FUN_00131120 = Actor_SetColorFilter (engine fn; listed as En_Rd-owned by plan_batches). DEBT: raw offsets, void* actor.
void FUN_00131120(void* actor, int colorFlag, int colorIntensity, int bufferFlag, int duration) {
    unsigned char* p = (unsigned char*)actor + 0x100;
    short timer = duration > 0 ? (int)(0.5f + (duration * 3.0f) * 0.5f) : (int)((duration * 3.0f) * 0.5f - 0.5f);
    *(short*)(p + 0x1A) = timer;
    *(unsigned int*)((unsigned char*)actor + 0x11C) =
        colorFlag | (unsigned short)timer | (bufferFlag | ((colorIntensity & 0xF8) << 13));
}

void FUN_0034f280(EnRd* self);

void FUN_0024d998(EnRd* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        FUN_0034f280(self);
    }
}

void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void FUN_00294a7c(EnRd* self, PlayState* play);

void FUN_00329154(EnRd* self) {
    Animation_Change_00375c08(&self->skelAnime, 5, 1.0f, 4.0f, Animation_GetLastFrame(&self->skelAnime, 5), 1, -4.0f);
    self->actor.speedXZ = 0.4f;
    self->action = 4;
    self->actionFunc = FUN_00294a7c;
}

void FUN_0034df48(void* mtx, Vec3s* rot);

s32 FUN_0016f158(void* arg0, s32 limbIndex, void* dList, void* thisx) {
    u8* p = (u8*)thisx + 0x900;
    Vec3s rot;

    rot.y = 0;
    rot.z = 0;
    rot.x = 0;
    if (limbIndex != 2) {
        if (limbIndex == 9) {
            rot.y = *(s16*)(p + 0x56);
            FUN_0034df48(dList, &rot);
        }
    } else {
        rot.y = *(s16*)(p + 0x58);
        FUN_0034df48(dList, &rot);
    }
    return 0;
}
void FUN_002a57b0(EnRd* self, PlayState* play);
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);

void FUN_002a5598(EnRd* self, PlayState* play);
void FUN_0024d7b0(EnRd* self, PlayState* play);

void FUN_0026d630(EnRd* self, PlayState* play) {
    if (FUN_00370734(&self->skelAnime)) {
        if (self->actor.parent != NULL) {
            Animation_Change_00375c08(&self->skelAnime, 5, 0.5f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 5), 1, -4.0f);
            self->action = 3;
            self->unk_94D = 1;
            self->actionFunc = FUN_002a5598;
        } else {
            Animation_Change_00375c08(&self->skelAnime, 4, 0.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 4), 2, 0.0f);
            self->action = 7;
            self->actionFunc = FUN_0024d7b0;
        }
    }
}
void FUN_00374ab0(PlayState* play, EnRd* self);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);

GLOBAL_ASM("asm/EnRd_Destroy.s")


#if 0
// Draft (not matched yet, see STATUS.md): FUN_0034f280 is 56/72 words off (VFP constant numbering, timer store).
void FUN_0034f280(EnRd* self) {
    if (self->actor.params == 2) {
        Animation_PlayLoop(&self->skelAnime, 0);
    } else {
        Animation_MorphToLoop(&self->skelAnime, 3, -6.0f);
    }
    self->action = 0;
    if ((s16)(s32)(5.0f + Rand_ZeroOne() * 10.0f) > 0) {
        s16 n = (s16)(s32)(5.0f + Rand_ZeroOne() * 10.0f);
        self->unk_954 = (s32)(0.5f + (n * 3.0f) * 0.5f);
    } else {
        s16 n = (s16)(s32)(5.0f + Rand_ZeroOne() * 10.0f);
        self->unk_954 = (s32)((n * 3.0f) * 0.5f - 0.5f);
    }
    self->actor.speedXZ = 0.0f;
    self->actor.world.rot.y = self->actor.shape.rot.y;
    self->actionFunc = FUN_0024d9c0;
}

// Draft (3 words off, pool fold of 0x588E58+0xB0): EnRd_Destroy
void EnRd_Destroy_draft(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnRd* self = (EnRd*)thisx;
    u8* miscCtx = (u8*)0x00588E58;

    if (*(u16*)(miscCtx + 0xB0) != 0) {
        *(u16*)(miscCtx + 0xB0) = 0;
    }
    Collider_DestroyCylinder(play, &self->collider);
    if (self->actor.params > -2) {
        FUN_003508b8(thisx, self->unk_9C0, 0);
    } else {
        FUN_003508b8(thisx, self->unk_9C4, 0);
    }
    SkelAnime_Free2(&self->skelAnime);
}

// Draft (4 words off, register order of play/p and two float compares): FUN_0026d700
void FUN_0026d700(EnRd* self, PlayState* play) {
    if (self->actor.shape.rot.x != -0x4000) {
        Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 0x7D0, 0);
        if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y, 0.3f, 2.0f, 0.3f) == 0.0f) {
            self->actor.gravity = -3.5f;
            if (self->unk_9C9 == 0) {
                FUN_0034f280(self);
            } else {
                self->unk_9C9 = 0;
                FUN_00374ab0(play, self);
            }
        }
    } else {
        if (self->actor.world.pos.y == self->actor.home.pos.y) {
            Audio_PlayActorSound2(&self->actor, 0x01000354);
        }

        if (Math_SmoothStepToF(&self->actor.world.pos.y, self->actor.home.pos.y + 50.0f, 0.3f, 2.0f, 0.3f) == 0.0f) {
            if (self->unk_954 != 0) {
                self->unk_954--;
                Math_SmoothStepToF(&self->actor.speedXZ, 6.0f, 0.3f, 1.0f, 0.3f);
            } else if (Math_SmoothStepToF(&self->actor.speedXZ, 0.0f, 0.3f, 1.0f, 0.3f) == 0.0f) {
                Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 1, 0x7D0, 0);
            }
        }
    }
}
#endif
}
