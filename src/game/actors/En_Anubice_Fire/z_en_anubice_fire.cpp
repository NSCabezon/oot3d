extern "C" {
#include "z_en_anubice_fire.hpp"

#define FLAGS 0x00000010

void FUN_00178728(Actor* actor, GameState* state);
void FUN_00108774(Actor* actor, GameState* state);
void* SystemArena_Malloc_0035010c(s32 size);
void* FUN_003500c4(void* p);
void FUN_0034ff2c(void* obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void FUN_0034fea8(void* obj, s32 bank, GameState* state, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
void FUN_0034fc7c(void* p);
void SystemArena_Free(void* p);
s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);
s32 FUN_0034c3b8(void* p, Vec3f* pos, f32 radius);

void EnAnubiceFire_Init(Actor* actor, GameState* state);
void EnAnubiceFire_Destroy(Actor* actor, GameState* state);
void EnAnubiceFire_Update(Actor* actor, GameState* state);
void EnAnubiceFire_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_Anubice_Fire.data.s")

ActorInit En_Anubice_Fire_InitVars = {
    ACTOR_EN_ANUBICE_FIRE,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_ANUBICE,
    sizeof(EnAnubiceFire),
    (ActorFunc)EnAnubiceFire_Init,
    (ActorFunc)EnAnubiceFire_Destroy,
    (ActorFunc)EnAnubiceFire_Update,
    (ActorFunc)EnAnubiceFire_Draw,
};

void EnAnubiceFire_Init(Actor* actor, GameState* state) {
    EnAnubiceFire* self = (EnAnubiceFire*)actor;
    PlayState* play = (PlayState*)state;
    void* obj;
    u8* base;
    Vec3f* src;
    Vec3f* dst;

    Collider_InitCylinder(play, (ColliderCylinder*)&self->unk_200);
    Collider_SetCylinder(play, (ColliderCylinder*)&self->unk_200, actor, (ColliderCylinderInit*)0x00520680);
    self->unk_1AC = 2.0f;
    self->unk_1A8 = 0.0f;
    base = (u8*)self + 0x100;
    *(s16*)(base + 0xB2) = 30;
    src = (Vec3f*)((u8*)actor + 0x28);
    dst = (Vec3f*)((u8*)self + 0x1B8);
    *dst = *src;
    dst = (Vec3f*)((u8*)self + 0x1C4);
    *dst = *src;
    dst = (Vec3f*)((u8*)self + 0x1D0);
    *dst = *src;
    dst = (Vec3f*)((u8*)self + 0x1DC);
    *dst = *src;
    dst = (Vec3f*)((u8*)self + 0x1E8);
    *dst = *src;
    dst = (Vec3f*)((u8*)self + 0x1F4);
    *dst = *src;
    *(s16*)(base + 0xB6) = 0;
    self->actionFunc = (void*)FUN_00178728;
    obj = SystemArena_Malloc_0035010c(0x28);
    if (obj != NULL) {
        obj = FUN_003500c4(obj);
    }
    self->unk_258 = obj;
    FUN_0034ff2c(self->unk_258, 4, 4, 6, 1, 0);
    FUN_0034fea8(self->unk_258, 0, state, 0x10, 0x2601, 0x2601, 0x812F, 0x812F);
}


void FUN_0034fc7c(void* p);
void SystemArena_Free(void* p);
s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);

void EnAnubiceFire_Destroy(Actor* actor, GameState* state) {
    EnAnubiceFire* self = (EnAnubiceFire*)actor;
    Collider_DestroyCylinder((PlayState*)state, (ColliderCylinder*)&self->unk_200);
    if (self->unk_258 != NULL) {
        FUN_0034fc7c(self->unk_258);
        SystemArena_Free(self->unk_258);
        self->unk_258 = NULL;
    }
}


#if 0 /* FUN_003e04b8 draft: 12/177 words (0x24-0x7c: zero-init VFP reg/store order of velocity/sp84/sp78; an assignment-body ctor {y=b;x=a;z=c;} for sp84 gets 2) */
void EffectSsBomb2_SpawnLayered(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep);
void EffectSsKiraKira_SpawnDispersed(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* primColor, Color_RGBA8* envColor, s16 scale, s32 life);
s32 FUN_00351388(PlayState* play);

void FUN_003e04b8(EnAnubiceFire* self, PlayState* play) {
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    Vector3f accel(0.0f, 0.0f, 0.0f);
    Vec3f pos;
    Color_RGBA8 primColor = { 255, 255, 0, 255 };
    Color_RGBA8 envColor = { 255, 0, 0, 255 };
    Vector3f sp84(0.0f, 0.0f, 0.0f);
    Vector3f sp78(0.0f, 0.0f, 0.0f);

    self->actor.world.rot.z += 5000;
    if (self->unk_1B2 == 0) {
        self->unk_1AC = 0.0f;
    }

    Math_ApproachF(&self->unk_1A8, self->unk_1AC, 0.2f, 0.4f);
    if ((self->unk_1B2 == 0) && (self->unk_1A8 < 0.1f)) {
        Actor_Kill(&self->actor);
    } else if ((self->actor.params == 0) && (self->unk_200.base.atFlags & 4)) {
        if (FUN_00351388(play)) {
            Audio_PlayActorSound2(&self->actor, 0x010000FA);
            self->unk_200.base.atFlags &= ~0x16;
            self->unk_200.base.atFlags |= 8;
            self->unk_200.info.toucher.dmgFlags = 2;
            self->unk_1B2 = 30;
            self->actor.params = 1;
            self->actor.velocity.x *= -1.0f;
            self->actor.velocity.y *= -0.5f;
            self->actor.velocity.z *= -1.0f;
        } else {
            self->unk_1B2 = 0;
            EffectSsBomb2_SpawnLayered(play, &self->actor.world.pos, (Vec3f*)&sp78, (Vec3f*)&sp84, 10, 5);
            self->actor.velocity.x = self->actor.velocity.y = self->actor.velocity.z = 0.0f;
            Audio_PlayActorSound2(&self->actor, 0x0100040D);
            self->actionFunc = (void*)FUN_00108774;
        }
    } else if (!(self->unk_1A8 < 0.4f)) {
        f32 scale = 1000.0f;
        f32 life = 10.0f;
        s32 i;

        for (i = 0; i < 10; i++) {
            pos.x = self->actor.world.pos.x + (Rand_ZeroOne() - 0.5f) * (self->unk_1A8 * 20.0f);
            pos.y = self->actor.world.pos.y + (Rand_ZeroOne() - 0.5f) * (self->unk_1A8 * 20.0f);
            pos.z = self->actor.world.pos.z;
            EffectSsKiraKira_SpawnDispersed(play, &pos, (Vec3f*)&velocity, (Vec3f*)&accel, &primColor, &envColor, scale, life);
        }
        Audio_PlayActorSound2(&self->actor, 0x0100040C);
    }
}
#endif

void EnAnubiceFire_Update(Actor* thisx, GameState* state) {
    EnAnubiceFire* self = (EnAnubiceFire*)thisx;
    PlayState* play = (PlayState*)state;
    s32 i;

    Actor_SetScale(thisx, self->unk_1A8);
    ((void (*)(Actor*, GameState*))self->actionFunc)(thisx, state);
    FUN_0036b96c(thisx);

    self->unk_1B8[0] = thisx->world.pos;
    for (i = 4; i >= 0; i--) {
        self->unk_1B8[i + 1] = self->unk_1B8[i];
    }

    if (self->unk_1B2 != 0) {
        self->unk_1B2--;
    }
    if (self->unk_1B4 != 0) {
        self->unk_1B4--;
    }
    Actor_UpdateBgCheckInfo(play, thisx, 5.0f, 5.0f, 10.0f, 0x1D);

    if (!(self->unk_1A8 < 0.6f || self->actionFunc == (void*)FUN_00108774)) {
        self->unk_200.dim.radius = self->unk_1A8 * 15.0f + 5.0f;
        self->unk_200.dim.height = self->unk_1A8 * 15.0f + 5.0f;
        self->unk_200.dim.yShift = self->unk_1A8 * -0.75f + -15.0f;
        if (self->unk_1B2 != 0) {
            Collider_UpdateCylinder(thisx, &self->unk_200);
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->unk_200.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->unk_200.base);
        }
        if (FUN_0034c3b8((u8*)play + 0xA98, &thisx->world.pos, 30.0f) != 0) {
            thisx->velocity.x = thisx->velocity.y = thisx->velocity.z = 0.0f;
            Audio_PlayActorSound2(thisx, 0x0100040D);
            self->actionFunc = (void*)FUN_00108774;
        }
    }
}


GLOBAL_ASM("asm/EnAnubiceFire_Draw.s")
}
