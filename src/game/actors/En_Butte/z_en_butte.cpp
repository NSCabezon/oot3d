extern "C" {
#include "z_en_butte.hpp"

#include "model_manager.hpp"

#define FLAGS 0x00000000
#define R_UPDATE_RATE SREG(30)
#define EB_MTX(s) ((MTX34*)((u8*)(s) + 0x148))
#define EB_COLLIDER(s) ((EnButteColliderJntSph*)((u8*)(s) + 0x1A4))
#define EB_SKEL(s) ((SkelAnime*)((u8*)(s) + 0x214))
#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

void EnButte_Init(Actor* thisx, PlayState* play);
void EnButte_Destroy(Actor* thisx, PlayState* play);
void EnButte_Update(Actor* thisx, PlayState* play);
void EnButte_Draw(Actor* thisx, PlayState* play);
void EnButte_TransformIntoFairy(EnButte* self, PlayState* play);

GLOBAL_ASM("../src/game/actors/En_Butte/data_syms.s")

GLOBAL_ASM("data/z_En_Butte.data.s")

ActorInit En_Butte_InitVars = {
    ACTOR_EN_BUTTE,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_GAMEPLAY_FIELD_KEEP,
    sizeof(EnButte),
    (ActorFunc)EnButte_Init,
    (ActorFunc)EnButte_Destroy,
    (ActorFunc)EnButte_Update,
    (ActorFunc)EnButte_Draw,
};

extern "C++" {
class EffDustFreer {
public:
    virtual void vf00();
    virtual void vf04();
    virtual void vf08();
    virtual void* Alloc(u32 size, const char* file, s32 line);
    virtual void Free(void* model);
};
}
extern EffDustFreer* gUnk_0055A1F8; // src/game/shared_syms.s

void Collider_DestroyJntSph(PlayState* play, EnButteColliderJntSph* collider);
void Collider_UpdateSpheres(s32 index, EnButteColliderJntSph* collider, MTX34* mtx);
void FUN_00348904(void* mgr, void* model);
void* FUN_003488e4(void* model);
void FUN_00171808(EnButte* self, PlayState* play);

extern u8 EnButte_sInitChain;  // data_syms.s
extern u8 EnButte_sColliderJntSphInit;
extern u8 EnButte_sModelData;
void Collider_InitJntSph(PlayState* play, EnButteColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, EnButteColliderJntSph* collider, Actor* actor, void* init, void* elements);
void* FUN_00348f34(void* obj, void* data);
void FUN_00348be4(void* obj);
void* ZAR_GetCTXBByIndex(void* zar, s32 index);
void FUN_00348a64(void* obj, s32 index, void* ctxb, s32 a, s32 b, s32 c, s32 d);
void* FUN_0034897c(void* mgr, void* obj, void* p, s32 arg);
void FUN_003429c8(void* obj, s32 a, f32* color);
void Animation_Change_00375c08(SkelAnime* skel, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
f32 Rand_ZeroOne(void);
s16 Rand_S16Offset_003702c8(s16 base, s16 range);

typedef struct EnButteFlightParams {
    /* 0x00 */ s16 minTime;
    /* 0x02 */ s16 maxTime;
    /* 0x04 */ f32 speedXZTarget;
    /* 0x08 */ f32 speedXZScale;
    /* 0x0C */ f32 speedXZStep;
    /* 0x10 */ s16 rotYStep;
} EnButteFlightParams; // size = 0x14

extern EnButteFlightParams sFlyAroundParams[]; // data_syms.s (table in z_En_Butte.data.s)
void EnButte_FlyAround(EnButte* self, PlayState* play);

#if 0 // Init draft: structure matches (size 808 vs 812) but armcc keeps 1 in r8 and inlines 2; ROM hoists 0/1/2 into r7/sb/r8 and re-materialises self into r0 for the idx store (164 words, register cascade)
#define EN_BUTTE_FILE "d:\\home\\queen\\dailyBuild\\game_us\\sources\\z_en_choo.cpp"

void EnButte_Init(Actor* thisx, PlayState* play) {
    EnButte* self = (EnButte*)thisx;
    EnButteFlightParams* params;

    if (self->actor.params == -1) {
        self->actor.params = 0;
    }

    Actor_ProcessInitChain(&self->actor, (InitChainEntry*)&EnButte_sInitChain);

    if (PARAMS_GET_U(self->actor.params, 0, 1) == 1) {
        ZARInfo* zar;
        void* ctxb;
        struct { f32 c[4]; } color;

        self->actor.uncullZoneScale = 200.0f;
        zar = &play->GetObjectStatus(0)->zarInfo;
        ctxb = ZAR_GetCTXBByIndex(zar, 2);
        {
            const char* file = EN_BUTTE_FILE;
            void* mem = gUnk_0055A1F8->Alloc(0x1B8, file, 493);

            self->unk_3D0 = (mem != NULL) ? FUN_00348f34(mem, &EnButte_sModelData) : NULL;
        }
        FUN_00348be4(self->unk_3D0);
        FUN_00348a64(self->unk_3D0, 0, ctxb, 0x2601, 0x2601, 0x812F, 0x812F);
        color.c[0] = 200.0f / 255.0f;
        color.c[1] = 200.0f / 255.0f;
        color.c[2] = 210.0f / 255.0f;
        color.c[3] = 1.0f;
        self->unk_3D4 = FUN_0034897c(GetModelManager().unk_247C, self->unk_3D0, self->actor.unk_178, 0);
        FUN_003429c8(self->unk_3D4, 1, color.c);
        *(u32*)((u8*)self->unk_3D4 + 0x178) |= 2;
    }

    FUN_00353c9c(&self->actor, play, EB_SKEL(self), 1, 4, (u8*)self + 0x298, (u8*)self + 0x334, 3);
    self->actor.unk_19A = 1;
    Collider_InitJntSph(play, EB_COLLIDER(self));
    Collider_SetJntSph(play, EB_COLLIDER(self), &self->actor, &EnButte_sColliderJntSphInit, (u8*)self + 0x1C4);
    self->actor.colChkInfo.mass = 0;
    self->unk_25C = Rand_ZeroOne() * 65535.0f;
    self->unk_25E = Rand_ZeroOne() * 65535.0f;
    self->unk_260 = Rand_ZeroOne() * 65535.0f;
    Animation_Change_00375c08(EB_SKEL(self), 4, 1.5f, 0.0f, 0.0f, 1, 0.0f);
    *((u8*)self + 0x295) = 1;

    params = &sFlyAroundParams[self->flightParamsIdx];
    if (self->flightParamsIdx == 0) {
        if (Rand_ZeroOne() < 0.6f) {
            self->flightParamsIdx = 1;
            goto timer;
        }
        self->flightParamsIdx = 2;
    } else {
        self->flightParamsIdx = 0;
    }
timer:
    self->timer = Rand_S16Offset_003702c8(params->minTime, params->maxTime);
    self->actionFunc = EnButte_FlyAround;
    self->actor.shape.rot.x -= 0x2320;
    self->drawSkelAnime = true;
}
#endif
GLOBAL_ASM("asm/EnButte_Init.s")

void EnButte_Destroy(Actor* thisx, PlayState* play) {
    EnButte* self = (EnButte*)thisx;

    Collider_DestroyJntSph(play, EB_COLLIDER(self));
    SkelAnime_Free2((SkelAnime*)((u8*)thisx + 0x214));
    if (self->unk_3D0 != NULL) {
        void* model = FUN_003488e4(self->unk_3D0);

        gUnk_0055A1F8->Free(model);
        self->unk_3D0 = NULL;
    }
    if (self->unk_3D4 != NULL) {
        FUN_00348904(GetModelManager().unk_247C, self->unk_3D4);
    }
}

#if 0 // Update draft: 6 words off (movne r1 vs r0 on child=NULL; r2/r3 swap in the unk_25E block; EnButte struct tail typed in z_en_butte.hpp)
void EnButte_Update(Actor* thisx, PlayState* play) {
    EnButte* self = (EnButte*)thisx;

    if ((self->actor.child != NULL) && (self->actor.child->update == NULL) && (&self->actor != self->actor.child)) {
        { Actor* z = NULL; self->actor.child = z; }
    }

    if (self->timer > 0) {
        self->timer--;
    }

    self->unk_25C += 0.5f + R_UPDATE_RATE / 3.0f * 0x222;
    self->unk_25E += 0.5f + R_UPDATE_RATE / 3.0f * 0x1000;
    self->unk_260 += 0.5f + R_UPDATE_RATE / 3.0f * 0x600;

    if (PARAMS_GET_U(self->actor.params, 0, 1) == 1) {
        if (*(bool*)((u8*)GET_PLAYER(play) + 0x2227) == 0) {
            if (self->swordDownTimer > 0) {
                self->swordDownTimer--;
            }
        } else {
            self->swordDownTimer = 0.5f + 3.0f / R_UPDATE_RATE * 80;
        }
    }

    self->actionFunc(self, play);

    if (self->actor.update != NULL) {
        Actor_MoveForward(&self->actor);
        Math_StepToF(&self->actor.world.pos.y, self->posYTarget, R_UPDATE_RATE / 3.0f * 0.6f);
        if (self->actor.xyzDistToPlayerSq < 5000.0f) {
            CollisionCheck_SetOC(play, &play->colChkCtx, (Collider*)EB_COLLIDER(self));
        }
        Actor_SetFocus(&self->actor, self->actor.shape.yOffset * self->actor.scale.y);
    }
}
#endif
GLOBAL_ASM("asm/EnButte_Update.s")

void FUN_001849f8(EnButte* self) {
    EnButteFlightParams* params = &sFlyAroundParams[self->flightParamsIdx];

    if (self->flightParamsIdx == 0) {
        if (Rand_ZeroOne() < 0.6f) {
            self->flightParamsIdx = 1;
            goto timer;
        }
        self->flightParamsIdx = 2;
    } else {
        self->flightParamsIdx = 0;
    }
timer:
    self->timer = Rand_S16Offset_003702c8(params->minTime, params->maxTime);
    self->actionFunc = EnButte_FlyAround;
}

void FUN_0041507c(u16* dst, u8* src) {
    dst[0] = *(u16*)(src + 0x182);
    dst[1] = *(u16*)(src + 0x184);
    dst[2] = *(u16*)(src + 0x186);
}

// FUN_00171808 (EnButte_DrawTransformationEffect, still GLOBAL_ASM in functions_140000s.cpp): not drafted. 3DS inline 3x4 RotateY/X/Z + scale
// (per-multiply scale copies), guarded static Vec3f {0,0,-3} (guard 0x5217F4 / obj 0x598F90, needs pin_static) and model vtable stores; far.

void EnButte_Draw(Actor* thisx, PlayState* play) {
    EnButte* self = (EnButte*)thisx;

    if (self->drawSkelAnime) {
        SkelAnime_DrawOpa(EB_SKEL(self), EB_MTX(self), NULL, NULL, &self->actor, 0);
        Collider_UpdateSpheres(0, EB_COLLIDER(self), EB_MTX(self));
    }

    if ((PARAMS_GET_U(self->actor.params, 0, 1) == 1) && (self->actionFunc == EnButte_TransformIntoFairy)) {
        FUN_00171808(self, play);
    }
}

s32 Math_ScaledStepToS(s16* pValue, s16 target, s16 step);
extern "C++" {
float sinf(float);
}
extern EnButteFlightParams sFollowLinkParams[]; // 0x521858 (data_syms.s)
void EnButte_FollowLink(EnButte* self, PlayState* play);
static inline void EnButte_SelectFlightParams(EnButte* self, EnButteFlightParams* flightParams) {
    if (self->flightParamsIdx == 0) {
        if (Rand_ZeroOne() < 0.6f) {
            self->flightParamsIdx = 1;
        } else {
            self->flightParamsIdx = 2;
        }
    } else {
        self->flightParamsIdx = 0;
    }
    self->timer = Rand_S16Offset_003702c8(flightParams->minTime, flightParams->maxTime);
}
#define EB_SHAPE_YOFF(s) (*(f32*)((u8*)(s) + 0xC4))
#define EB_UNK257(s) (*((u8*)(s) + 0x3DF))

#if 0 // FlyAround draft: far (1408 vs 1552 B; two runtime-scaled guarded static f32[3] tables at 0x598F9C/0x598FA8 derived from one literal + inlined N64 helpers; needs pin_static/adjacent statics)
void EnButte_FlyAround(EnButte* self, PlayState* play) {
    EnButteFlightParams* flightParams = &sFlyAroundParams[self->flightParamsIdx];
    static f32 sYOffA[] = { R_UPDATE_RATE / 3.0f * 50.0f, R_UPDATE_RATE / 3.0f * 80.0f, R_UPDATE_RATE / 3.0f * 100.0f };
    static f32 sYOffB[] = { R_UPDATE_RATE / 3.0f * 30.0f, R_UPDATE_RATE / 3.0f * 40.0f, R_UPDATE_RATE / 3.0f * 50.0f };
    s16 yaw;
    Player* player = GET_PLAYER(play);
    f32 distSqFromHome;
    f32 maxDistSqFromHome;
    f32 minAnimSpeed;
    f32 animSpeed;
    s16 rotStep;

    distSqFromHome = SQ(self->actor.world.pos.x - self->actor.home.pos.x) + SQ(self->actor.world.pos.z - self->actor.home.pos.z);
    EB_SHAPE_YOFF(self) += Math_SinS(self->unk_25C) * sYOffA[self->flightParamsIdx] + Math_SinS(self->unk_25E) * sYOffB[self->flightParamsIdx];
    EB_SHAPE_YOFF(self) = CLAMP(EB_SHAPE_YOFF(self), -2000.0f, 2000.0f);
    Math_SmoothStepToF(&self->actor.speedXZ, flightParams->speedXZTarget, flightParams->speedXZScale, flightParams->speedXZStep, 0.0f);

    if (EB_UNK257(self) == 1) {
        maxDistSqFromHome = SQ(100.0f);
        rotStep = 1000.0f * R_UPDATE_RATE / 3.0f + 0.5f;
    } else {
        maxDistSqFromHome = SQ(35.0f);
        rotStep = 600.0f * R_UPDATE_RATE / 3.0f + 0.5f;
    }

    minAnimSpeed = 0.0f;
    self->posYTarget = self->actor.home.pos.y;

    if ((self->flightParamsIdx != 0) && ((distSqFromHome > maxDistSqFromHome) || (self->timer < 4))) {
        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.home.pos);
        if (Math_ScaledStepToS(&self->actor.world.rot.y, yaw, flightParams->rotYStep) == 0) {
            minAnimSpeed = 0.5f;
        }
    } else if ((EB_UNK257(self) == 0) && (self->actor.child != NULL) && (self->actor.child != &self->actor)) {
        yaw = Math_Vec3f_Yaw(&self->actor.world.pos, &self->actor.child->world.pos);
        if (Math_ScaledStepToS(&self->actor.world.rot.y, yaw, rotStep) == 0) {
            minAnimSpeed = 0.3f;
        }
    } else if (EB_UNK257(self) == 1) {
        yaw = self->actor.yawTowardsPlayer + 0x8000 + (s16)((Rand_ZeroOne() - 0.5f) * 0x6000);
        if (Math_ScaledStepToS(&self->actor.world.rot.y, yaw, rotStep) == 0) {
            minAnimSpeed = 0.4f;
        }
    } else {
        self->actor.world.rot.y += (s16)(sinf(self->unk_25C) * 100.0f);
    }
    {
        s16 target = self->actor.world.rot.y + 0x8000;
        s16 diff = target - self->actor.shape.rot.y;

        Math_ScaledStepToS(&self->actor.shape.rot.y, target, ABS(diff) >> 3);
        self->actor.shape.rot.x = (s16)(sinf(self->unk_260) * 600.0f) - 0x2320;
    }

    FUN_003731e0(EB_SKEL(self));

    if (self->timer <= 0) {
        EnButte_SelectFlightParams(self, &sFlyAroundParams[self->flightParamsIdx]);
    }

    if ((PARAMS_GET_U(self->actor.params, 0, 1) == 1) && (*((u8*)player + 0x1A9) == 6) && (self->swordDownTimer <= 0) &&
        ((SQ(player->actor.world.pos.x - self->actor.home.pos.x) + SQ(player->actor.world.pos.z - self->actor.home.pos.z) < SQ(120.0f)) ||
         (self->actor.xzDistToPlayer < 60.0f))) {
        EnButte_SelectFlightParams(self, &sFollowLinkParams[self->flightParamsIdx]);
        self->actionFunc = EnButte_FollowLink;
        EB_UNK257(self) = 2;
    } else if (self->actor.xzDistToPlayer < 120.0) {
        EB_UNK257(self) = 1;
    } else {
        EB_UNK257(self) = 0;
    }
}
#endif
GLOBAL_ASM("asm/EnButte_FlyAround.s")

GLOBAL_ASM("asm/EnButte_FollowLink.s")

#if 0 // TransformIntoFairy draft (25 words off: VFP numbering / literal-pool order of the effect update)
typedef struct {
    /* 0x00 */ s16 alpha;
    /* 0x02 */ s16 pad;
    /* 0x04 */ f32 scale;
} EnButteTransformEffect;

extern EnButteTransformEffect sTransformationEffect; // data_syms.s (0x5217EC, shared with the effect Draw)

void EnButte_TransformIntoFairy(EnButte* self, PlayState* play) {
    FUN_003731e0(EB_SKEL(self));
    f32 k = 0.003f;
    sTransformationEffect.scale += R_UPDATE_RATE * k / 3.0f;
    sTransformationEffect.alpha += R_UPDATE_RATE * k * 4000.0f + 0.5f;

    if (self->timer == 8) {
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 60, 0x0100023E);
    } else if (self->timer == 6) {
        Actor_Spawn(&play->actorCtx, play, 0x18, self->actor.focus.pos.x, self->actor.focus.pos.y,
                    self->actor.focus.pos.z, 0, self->actor.shape.rot.y, 0, 2, 1);
        self->drawSkelAnime = false;
    } else if (self->timer <= 0) {
        self->timer = 0.5f + (3.0f / R_UPDATE_RATE) * 64;
        self->actionFunc = EnButte_WaitToDie;
        self->actor.draw = NULL;
    }
}
#endif
GLOBAL_ASM("asm/EnButte_TransformIntoFairy.s")

void EnButte_WaitToDie(EnButte* self, PlayState* play) {
    if (self->timer <= 0) {
        Actor_Kill(&self->actor);
    }
}

}
