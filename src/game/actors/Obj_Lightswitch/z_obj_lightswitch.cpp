extern "C" {
#include "z_obj_lightswitch.hpp"

#define FLAGS 0x00000010

#define PARAMS_GET_U(params, shift, count) ((((u16)(params)) >> (shift)) & ((1 << (count)) - 1))

void ObjLightswitch_Init(Actor* actor, GameState* state);
void ObjLightswitch_Destroy(Actor* thisx, PlayState* play2);
void ObjLightswitch_Update(Actor* thisx, PlayState* play2);
void ObjLightswitch_Draw(Actor* actor, GameState* state);

void ObjLightswitch_Off(ObjLightswitch* self, PlayState* play);
void ObjLightswitch_TurnOn(ObjLightswitch* self, PlayState* play);
void ObjLightswitch_On(ObjLightswitch* self, PlayState* play);
void ObjLightswitch_TurnOff(ObjLightswitch* self, PlayState* play);
void ObjLightswitch_DisappearDelay(ObjLightswitch* self, PlayState* play);
void ObjLightswitch_Disappear(ObjLightswitch* self, PlayState* play);
void FUN_001a24d4(ObjLightswitch* self);
void FUN_0036a84c(Actor* thisx, PlayState* play);

s32 FUN_0036ae04(void);
void FUN_0036a2dc(PlayState* play, Actor* actor, s32 sfxId, s32 arg3, s32 arg4);
void Collider_DestroyJntSph(PlayState* play, ObjLightswitchColliderJntSph* collider);
s32 Math_StepToS(s16* pValue, s16 target, s16 step);

GLOBAL_ASM("data/z_Obj_Lightswitch.data.s")

ActorInit Obj_Lightswitch_InitVars = {
    ACTOR_OBJ_LIGHTSWITCH,
    ACTORCAT_SWITCH,
    FLAGS,
    OBJECT_LIGHTSWITCH,
    sizeof(ObjLightswitch),
    (ActorFunc)ObjLightswitch_Init,
    (ActorFunc)ObjLightswitch_Destroy,
    (ActorFunc)ObjLightswitch_Update,
    (ActorFunc)ObjLightswitch_Draw,
};

#if 0 // (draft decls; data_syms.s with ObjLightswitch_sColChkInfoInit 0x5353B8, sJntSphInit 0x5353C0, sInitChain 0x5353D0, sVec 0x5A2EBC was removed)
extern InitChainEntry ObjLightswitch_sInitChain[];
extern CollisionCheckInfoInit2 ObjLightswitch_sColChkInfoInit;
extern u8 ObjLightswitch_sJntSphInit[];
extern Vec3f ObjLightswitch_sVec;

void Collider_InitJntSph(PlayState* play, ObjLightswitchColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, ObjLightswitchColliderJntSph* collider, Actor* actor, void* init,
                        ObjLightswitchColliderElement* elements);
void Collider_UpdateSpheres(s32 index, ObjLightswitchColliderJntSph* collider, MTX34* mtx);
void Matrix_Scale(MTX34* mtx, s32 mode, f32 x, f32 y, f32 z);
void FUN_003679d0(f32 x, f32 y, f32 z, MTX34* mtx, Vec3s* rot);
void FUN_0034e994(void* face, SkeletonAnimationModel* model, void* zar, s32 a, s32 b, s32 c);
#endif
#if 0 // draft: 18 words off (setup-block base reg r1/r2 swap after the params ldrsh; needs EQU syms in data_syms.s + the externs/decls below)
static inline u8* ObjLightswitch_GetObjStatus(PlayState* play, u8 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void ObjLightswitch_Init(Actor* actor, GameState* state) {
    ObjLightswitch* self = (ObjLightswitch*)actor;
    PlayState* play = (PlayState*)state;
    s32 removeSelf = 0;
    s32 switchFlagSet = Flags_GetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));

    Actor_ProcessInitChain(&self->actor, ObjLightswitch_sInitChain);
    FUN_00372f38(&self->actor, play, &self->skelAnimModel[0], 0, &self->skelAnimModel[1], 1, &self->skelAnimModel[2],
                 2, 0);
    FUN_0034e994(&self->unk_238, self->skelAnimModel[0],
                 ObjLightswitch_GetObjStatus(play, self->actor.objBankIndex) + 0x10, 0, -1, -1);
    Actor_SetFocus(&self->actor, 0.0f);
    if (switchFlagSet) {
        if (((actor->params >> 4) & 3) == OBJLIGHTSWITCH_TYPE_BURN) {
            removeSelf = 1;
        } else {
            { ObjLightswitchActionFunc* pf = &self->actionFunc; *pf = ObjLightswitch_On; }
            self->faceTextureIndex = 2;
            self->color[0] = 255 << 6;
            self->color[1] = 255 << 6;
            self->color[2] = 255 << 6;
            self->alpha = 255 << 6;
            self->flameRingRotSpeed = -0xAA;
            self->timer = 0;
        }
    } else {
        { ObjLightswitchActionFunc* pf = &self->actionFunc; *pf = ObjLightswitch_Off; }
        self->faceTextureIndex = 0;
        self->color[0] = 155 << 6;
        self->color[1] = 125 << 6;
        self->color[2] = 255 << 6;
        self->alpha = 255 << 6;
    }
    if (PARAMS_GET_U(self->actor.params, 0, 1) == 1) {
        if (switchFlagSet) {
            Math_Vec3f_Copy(&self->actor.world.pos, &ObjLightswitch_sVec);
            Math_Vec3f_Copy(&self->actor.home.pos, &ObjLightswitch_sVec);
        }
        self->actor.shape.rot.x = -0x4000;
        self->actor.home.rot.x = self->actor.shape.rot.x;
        self->actor.world.rot.x = self->actor.shape.rot.x;
        self->actor.shape.rot.z = 0;
        self->actor.home.rot.z = self->actor.shape.rot.z;
        self->actor.world.rot.z = self->actor.shape.rot.z;
        self->actor.flags |= 0x20;
        if (Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, 0xFF, self->actor.home.pos.x,
                               self->actor.home.pos.y, self->actor.home.pos.z, 0, self->actor.home.rot.y, 0,
                               -256) == NULL) {
            removeSelf = 1;
        }
    }
    {
        MTX34 mtx;

        Collider_InitJntSph(play, &self->collider);
        Collider_SetJntSph(play, &self->collider, &self->actor, ObjLightswitch_sJntSphInit, self->colliderElements);
        FUN_003679d0(self->actor.world.pos.x,
                     self->actor.world.pos.y + self->actor.shape.yOffset * self->actor.scale.y,
                     self->actor.world.pos.z, &mtx, &self->actor.shape.rot);
        Matrix_Scale(&mtx, 1, self->actor.scale.x, self->actor.scale.y, self->actor.scale.z);
        Collider_UpdateSpheres(0, &self->collider, &mtx);
    }
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, NULL, &ObjLightswitch_sColChkInfoInit);
    if (removeSelf) {
        Actor_Kill(&self->actor);
    }
}
#endif

GLOBAL_ASM("asm/ObjLightswitch_Init.s")

void ObjLightswitch_Destroy(Actor* thisx, PlayState* play2) {
    ObjLightswitch* self = (ObjLightswitch*)thisx;
    PlayState* play = play2;

    Collider_DestroyJntSph(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel[0], self->skelAnimModel[1], self->skelAnimModel[2], 0);
    ((FaceAnimations*)&self->unk_238)->Destroy();
}

void ObjLightswitch_DisappearDelay(ObjLightswitch* self, PlayState* play) {
    if (FUN_0036ae04() == self->actor.type || self->toggleDelay <= 0) {
        self->actionFunc = ObjLightswitch_Disappear;
        self->alpha = 255 << 6;
    }
}

GLOBAL_ASM("../split/asm/ObjLightswitch_Disappear.s")

void ObjLightswitch_Update(Actor* thisx, PlayState* play2) {
    ObjLightswitch* self = (ObjLightswitch*)thisx;
    PlayState* play = play2;

    if (self->toggleDelay > 0) {
        self->toggleDelay--;
    }

    {
    PlayState* play3 = play;
    self->actionFunc(self, play3);
    }

    if (self->actor.update != NULL) {
        if (PARAMS_GET_U(self->actor.params, 0, 1) == 1) {
            self->actor.world.pos.x = self->actor.child->world.pos.x;
            self->actor.world.pos.y = self->actor.child->world.pos.y + 60.0f;
            self->actor.world.pos.z = self->actor.child->world.pos.z;
            Actor_SetFocus(&self->actor, 0.0f);
        }

        self->prevFrameACflags = self->collider.base.acFlags;
        self->collider.base.acFlags &= ~AC_HIT;
        CollisionCheck_SetOC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), &self->collider.base);
        CollisionCheck_SetAC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), &self->collider.base);
    }
}


#if 0 // draft: far (241 words; ROM keeps pos in s17-s19 + r7=1 SetUnkAC family, SetColor duplicated per branch)
// (draft; needs data_syms.s: ObjLightswitch_sFaceAnims EQU 0x004DBB34)
extern u16 ObjLightswitch_sFaceAnims[];
void FUN_00331284(Player*, void*);
void* FUN_003687a8(SkeletonAnimationModel* model);
void FUN_003589cc(void* p, s32 n);
void FUN_00358964(void* p, s32 n, void* color);
void Collider_UpdateSpheres(s32 index, ObjLightswitchColliderJntSph* collider, MTX34* mtx);
void Matrix_Scale(MTX34* mtx, s32 mode, f32 x, f32 y, f32 z);
void FUN_003679d0(f32 x, f32 y, f32 z, MTX34* mtx, Vec3s* rot);

static inline void ObjLightswitch_DrawModel(ObjLightswitch* self, s32 i) {
    self->skelAnimModel[i]->SetUnkAC(1);
    self->skelAnimModel[i]->FUN_003721e0(&self->actor.modelMtx);
    self->skelAnimModel[i]->FUN_00372170(0);
}

static inline void ObjLightswitch_SetColor(ObjLightswitch* self) {
    f32 color[4];
    void* p;

    ((FaceAnimations*)&self->unk_238)->FUN_0035e3a4(0, ObjLightswitch_sFaceAnims[self->faceTextureIndex]);
    ((FaceAnimations*)&self->unk_238)->FUN_0035e330();
    color[0] = (u8)(self->color[0] >> 6) * (1.0f / 255.0f);
    color[1] = (u8)(self->color[1] >> 6) * (1.0f / 255.0f);
    color[2] = (u8)(self->color[2] >> 6) * (1.0f / 255.0f);
    color[3] = (u8)(self->alpha >> 6) * (1.0f / 255.0f);
    p = FUN_003687a8(self->skelAnimModel[0]);
    *((u8*)p + 0x1BA) = 0;
    FUN_003589cc(p, 4);
    FUN_00358964(p, 4, color);
}

void ObjLightswitch_Draw(Actor* actor, GameState* state) {
    ObjLightswitch* self = (ObjLightswitch*)actor;
    PlayState* play = (PlayState*)state;
    s32 alpha = (u16)self->alpha >> 6 & 0xFF;
    Vec3f pos;
    Vec3s rot;

    if (PARAMS_GET_U(actor->params, 0, 1) == 1) {
        Collider_UpdateSpheres(0, &self->collider, &actor->modelMtx);
    }
    FUN_00331284(GET_PLAYER(play), actor->unk_178);
    if (((actor->params >> 4) & 3) == OBJLIGHTSWITCH_TYPE_BURN && alpha < 255) {
        f32 px = actor->world.pos.x;
        f32 py = actor->world.pos.y + actor->shape.yOffset * actor->scale.y;
        f32 pz = actor->world.pos.z;
        ObjLightswitch_SetColor(self);
        ObjLightswitch_DrawModel(self, 0);
        rot.x = actor->shape.rot.x;
        rot.y = actor->shape.rot.y;
        rot.z = actor->shape.rot.z + self->flameRingRot;
        FUN_003679d0(px, py, pz, &actor->modelMtx, &rot);
        Matrix_Scale(&actor->modelMtx, 1, actor->scale.x, actor->scale.y, actor->scale.z);
        ObjLightswitch_DrawModel(self, 1);
        rot.z = actor->shape.rot.z - self->flameRingRot;
        FUN_003679d0(px, py, pz, &actor->modelMtx, &rot);
        Matrix_Scale(&actor->modelMtx, 1, actor->scale.x, actor->scale.y, actor->scale.z);
        ObjLightswitch_DrawModel(self, 2);
    } else {
        ObjLightswitch_SetColor(self);
        if (PARAMS_GET_U(actor->params, 0, 1) == 1) {
            actor->world.pos.x = actor->child->world.pos.x;
            actor->world.pos.y = actor->child->world.pos.y + 60.0f;
            actor->world.pos.z = actor->child->world.pos.z;
            Math_Vec3f_Copy(&pos, &actor->world.pos);
            FUN_003679d0(pos.x, pos.y, pos.z, &actor->modelMtx, &actor->shape.rot);
            Matrix_Scale(&actor->modelMtx, 1, actor->scale.x, actor->scale.y, actor->scale.z);
        } else {
            pos.x = actor->world.pos.x;
            pos.y = actor->world.pos.y + actor->shape.yOffset * actor->scale.y;
            pos.z = actor->world.pos.z;
        }
        ObjLightswitch_DrawModel(self, 0);
        rot.x = actor->shape.rot.x;
        rot.y = actor->shape.rot.y;
        rot.z = actor->shape.rot.z + self->flameRingRot;
        FUN_003679d0(pos.x, pos.y, pos.z, &actor->modelMtx, &rot);
        Matrix_Scale(&actor->modelMtx, 1, actor->scale.x, actor->scale.y, actor->scale.z);
        ObjLightswitch_DrawModel(self, 1);
        rot.z = actor->shape.rot.z - self->flameRingRot;
        FUN_003679d0(pos.x, pos.y, pos.z, &actor->modelMtx, &rot);
        Matrix_Scale(&actor->modelMtx, 1, actor->scale.x, actor->scale.y, actor->scale.z);
        ObjLightswitch_DrawModel(self, 2);
    }
}
#endif

GLOBAL_ASM("asm/ObjLightswitch_Draw.s")

static inline void ObjLightswitch_SetupTurnOn(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_TurnOn;
    self->toggleDelay = 150;
    self->timer = 0;
    self->faceTextureIndex = 0;
}

static inline void ObjLightswitch_SetupTurnOff(ObjLightswitch* self) {
    self->actionFunc = ObjLightswitch_TurnOff;
    self->toggleDelay = 150;
    self->timer = 30;
    self->faceTextureIndex = 1;
}

#if 0 // draft: 23 words off (reg numbering: ROM keeps thisx in r0, base r1=+0x200, switch val r2; ours r2/r1 swapped; needs Actor* signature -> casts at actionFunc sites)
void ObjLightswitch_Off(Actor* thisx, PlayState* play) {
    ObjLightswitch* self = (ObjLightswitch*)thisx;
    ObjLightswitch* s2 = (ObjLightswitch*)thisx;

    switch (PARAMS_GET_U(self->actor.params, 4, 2)) {
        case OBJLIGHTSWITCH_TYPE_STAY_ON:
        case OBJLIGHTSWITCH_TYPE_2:
            if (s2->collider.base.acFlags & AC_HIT) {
                ObjLightswitch_SetupTurnOn(self);
                FUN_0036a84c(thisx, play);
            }
            break;
        case OBJLIGHTSWITCH_TYPE_1:
            if ((s2->collider.base.acFlags & AC_HIT) && !(s2->prevFrameACflags & AC_HIT)) {
                ObjLightswitch_SetupTurnOn(self);
                FUN_0036a84c(thisx, play);
            }
            break;
        case OBJLIGHTSWITCH_TYPE_BURN:
            if (s2->collider.base.acFlags & AC_HIT) {
                self->actionFunc = ObjLightswitch_DisappearDelay;
                self->toggleDelay = 150;
                FUN_0036a84c(thisx, play);
                Flags_SetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));
            }
            break;
    }
}
#endif

GLOBAL_ASM("asm/ObjLightswitch_Off.s")


void ObjLightswitch_TurnOn(ObjLightswitch* self, PlayState* play) {
    if (FUN_0036ae04() == self->actor.type || self->toggleDelay <= 0) {
        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->actor, 0x010001C6);
        }
        self->timer++;
        Math_StepToS(&self->flameRingRotSpeed, -0xAA, 0xA);
        self->flameRingRot += self->flameRingRotSpeed;
        self->color[0] = self->timer * (((255 - 155) << 6) / 30) + (155 << 6);
        self->color[1] = self->timer * (((255 - 125) << 6) / 30) + (125 << 6);
        if (self->timer >= 30) {
            { ObjLightswitchActionFunc* pf = &self->actionFunc; *pf = ObjLightswitch_On; }
            self->faceTextureIndex = 2;
            self->color[0] = 255 << 6;
            self->color[1] = 255 << 6;
            self->color[2] = 255 << 6;
            self->alpha = 255 << 6;
            self->flameRingRotSpeed = -0xAA;
            self->timer = 0;
            Flags_SetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));
        } else if (self->timer == 23) {
            self->faceTextureIndex = 1;
            Audio_PlayActorSound2(&self->actor, 0x0100016C);
        }
    }
}


void ObjLightswitch_On(ObjLightswitch* self, PlayState* play) {
    s16 p = self->actor.params;
    switch (PARAMS_GET_U(p, 4, 2)) {
        case OBJLIGHTSWITCH_TYPE_STAY_ON:
            if (!Flags_GetSwitch(play, PARAMS_GET_U(p, 8, 6))) {
                ObjLightswitch_SetupTurnOff(self);
            }
            break;
        case OBJLIGHTSWITCH_TYPE_1:
            if (self->collider.base.acFlags & AC_HIT && !(self->prevFrameACflags & AC_HIT)) {
                ObjLightswitch_SetupTurnOff(self);
                if (Flags_GetSwitch(play, PARAMS_GET_U(p, 8, 6))) {
                    Flags_UnsetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));
                    if (PARAMS_GET_U(self->actor.params, 4, 2) == OBJLIGHTSWITCH_TYPE_1) {
                        FUN_0036a2dc(play, &self->actor, 0x0100048C, 0, 0);
                    }
                }
            }
            break;
        case OBJLIGHTSWITCH_TYPE_2:
            if (!(self->collider.base.acFlags & AC_HIT)) {
                if (self->timer > 9) {
                    ObjLightswitch_SetupTurnOff(self);
                    if (Flags_GetSwitch(play, PARAMS_GET_U(p, 8, 6))) {
                        Flags_UnsetSwitch(play, PARAMS_GET_U(self->actor.params, 8, 6));
                        if (PARAMS_GET_U(self->actor.params, 4, 2) == OBJLIGHTSWITCH_TYPE_1) {
                            FUN_0036a2dc(play, &self->actor, 0x0100048C, 0, 0);
                        }
                    }
                } else {
                    self->timer++;
                }
            } else {
                self->timer = 0;
            }
            break;
    }
    self->flameRingRot += self->flameRingRotSpeed;
}


void ObjLightswitch_TurnOff(ObjLightswitch* self, PlayState* play) {
    if (PARAMS_GET_U(self->actor.params, 4, 2) != OBJLIGHTSWITCH_TYPE_1 || FUN_0036ae04() == self->actor.type ||
        self->toggleDelay <= 0) {
        self->timer--;
        Math_StepToS(&self->flameRingRotSpeed, 0, 0xA);
        self->flameRingRot += self->flameRingRotSpeed;
        self->color[0] = self->timer * (((255 - 155) << 6) / 30) + (155 << 6);
        self->color[1] = self->timer * (((255 - 125) << 6) / 30) + (125 << 6);
        if (self->timer <= 0) {
            FUN_001a24d4(self);
        } else if (self->timer == 23) {
            self->faceTextureIndex = 0;
            Audio_PlayActorSound2(&self->actor, 0x0100016C);
        }
    }
}

}
