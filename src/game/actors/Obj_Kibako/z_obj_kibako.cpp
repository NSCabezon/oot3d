extern "C" {
#include "z_obj_kibako.hpp"

#define FLAGS 0x84000010

#ifndef NA_SE_EV_WOODBOX_BREAK
#define NA_SE_EV_WOODBOX_BREAK 0x01000201
#endif

void ObjKibako_Init(Actor* thisx, PlayState* play);
void ObjKibako_Destroy(Actor* thisx, PlayState* play2);
void ObjKibako_Update(Actor* thisx, PlayState* play);
void ObjKibako_Draw(Actor* thisx, PlayState* play);
void ObjKibako_Idle(ObjKibako* self, PlayState* play);
void ObjKibako_Held(ObjKibako* self, PlayState* play);
void ObjKibako_Thrown(ObjKibako* self, PlayState* play);
void ObjKibako_AirBreak(ObjKibako* self, PlayState* play);
void ObjKibako_WaterBreak(ObjKibako* self, PlayState* play);
void ObjKibako_SetupIdle(ObjKibako* self);
s32 FUN_003a7aac(u8* base, s32 id, s32 slot);
void FUN_003c06bc(ObjKibako* self, PlayState* play);

GLOBAL_ASM("data/z_Obj_Kibako.data.s")

GLOBAL_ASM("../src/game/actors/Obj_Kibako/data_syms.s")

extern u8 ObjKibako_sInfoTable[];
extern f32 ObjKibako_sFloatConst;
extern CollisionCheckInfoInit ObjKibako_sCCInfoInit;
extern ColliderCylinderInit ObjKibako_sCylinderInit;
extern InitChainEntry ObjKibako_sInitChain[];

ActorInit Obj_Kibako_InitVars = {
    ACTOR_OBJ_KIBAKO,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjKibako),
    (ActorFunc)ObjKibako_Init,
    (ActorFunc)ObjKibako_Destroy,
    (ActorFunc)ObjKibako_Update,
    (ActorFunc)ObjKibako_Draw,
};

static inline void ObjKibako_SetupIdleInl(ObjKibako* self) {
    self->actionFunc = ObjKibako_Idle;
    self->actor.colChkInfo.mass = MASS_HEAVY;
}

void FUN_003c06bc(ObjKibako* self, PlayState* play) {
    s32 idx = ((u32)self->actor.params << 16) >> 23;
    u8* info = *(u8**)(ObjKibako_sInfoTable + idx * 32 + 0x14);

    if (FUN_003a7aac((u8*)play + 0x208C, idx, info[2]) != 0) {
        self->actor.gravity = -1.2f;
        self->actor.minVelocityY = -13.0f;
        self->actionFunc = ObjKibako_Idle;
        self->actor.colChkInfo.mass = MASS_HEAVY;
    }
}

void ObjKibako_Init(Actor* thisx, PlayState* play) {
    ObjKibako* self = (ObjKibako*)thisx;
    s16 params;

    Actor_ProcessInitChain(thisx, ObjKibako_sInitChain);
    FUN_00372f38(thisx, play, &self->model, 2, 0);
    self->unk_204 = 0.0f;
    self->unk_208 = 0;
    self->actor.gravity = -1.2f;
    self->actor.minVelocityY = -13.0f;
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &ObjKibako_sCylinderInit);
    Collider_UpdateCylinder(thisx, &self->collider);
    CollisionCheck_SetInfo(&thisx->colChkInfo, NULL, &ObjKibako_sCCInfoInit);
    params = thisx->params;
    if (params != -1 && (params & 0x1F) == 0x1F) {
        self->actor.gravity = 0.0f;
        self->actor.minVelocityY = 0.0f;
        self->actionFunc = FUN_003c06bc;
    } else {
        ObjKibako_SetupIdleInl(self);
    }
    if (*(u8*)((u8*)&gSaveContext + 0x0E) == 0 && *(u16*)((u8*)play + 0x104) == 2 && thisx->room == 7 &&
        (*(u16*)((u8*)&gSaveContext + 0xEF2) & 0x80) == 0) {
        Actor_Kill(thisx);
    }
}

void ObjKibako_Destroy(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    ObjKibako* self = (ObjKibako*)thisx;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_00350f34(thisx, &self->model, 0);
}

void ObjKibako_Update(Actor* thisx, PlayState* play) {
    ObjKibako* self = (ObjKibako*)thisx;

    self->unk_208++;
    self->actionFunc(self, play);
}

// Unmatched: Draw (~49/69 words off, register numbering of play/1 across the model call). Kept as asm.
GLOBAL_ASM("asm/ObjKibako_Draw.s")

#if 0 // Draw draft
void ObjKibako_Draw(Actor* thisx, PlayState* play) {
    ObjKibako* self = (ObjKibako*)thisx;
    Player* player = GET_PLAYER(play);

    if (player != NULL && *(Actor**)((u8*)player + 0x1224) == thisx && *(s32*)((u8*)player + 0x284) == 0x70) {
        self->unk_204 = self->unk_204 + ObjKibako_sFloatConst;
        self->actor.world.pos.x += self->unk_204 * Math_SinS(*(s16*)((u8*)player + 0xBE));
        self->actor.world.pos.z += self->unk_204 * Math_CosS(*(s16*)((u8*)player + 0xBE));
        Actor_UpdateBgCheckInfo(play, thisx, 19.0f, 20.0f, 0.0f, 1);
        if (!(thisx->bgCheckFlags & 8)) {
            *(f32*)((u8*)self + 0x154) = thisx->world.pos.x;
            *(f32*)((u8*)self + 0x174) = thisx->world.pos.z;
        } else {
            self->unk_204 = 0.0f;
        }
    } else {
        self->unk_204 = 0.0f;
    }
    if (self->model != NULL) {
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0((MTX34*)((u8*)self + 0x148));
        self->model->FUN_00372170(0);
    }
}
#endif


void ObjKibako_AirBreak(ObjKibako* self, PlayState* play) {
    s16 angle;
    Vec3f* breakPos = &self->actor.world.pos;
    s32 i;
    Vec3f pos;
    Vec3f velocity;
    s16 phi_s0;

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.2f;
        velocity.y = (Rand_ZeroOne() * 6.0f) + 2.0f;
        velocity.z = pos.z * 0.2f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.1f) {
            phi_s0 = 0x60;
        } else if (temp_rand < 0.7f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -200, phi_s0 | 0x200, 10, 10, 0,
                             (Rand_ZeroOne() * 30.0f) + 10.0f, 0, 32, 60, -1, OBJECT_GAMEPLAY_DANGEON_KEEP, (void*)1);
    }
    FUN_0037378c(play, breakPos, 40.0f, 3, 50, 140, 1);
}

// Unmatched: Held (~52/59 words off, if-else shape of the shared Actor_UpdateBgCheckInfo tail). Kept as asm.
GLOBAL_ASM("asm/ObjKibako_Held.s")

#if 0 // Held draft
static __inline __pure float ObjKibako_Fabsf(float f) {
    return __fabsf(f);
}

static void ObjKibako_SetupThrownInl(ObjKibako* self) {
    f32 speed = self->actor.speedXZ;
    self->actor.velocity.x = Math_SinS(self->actor.world.rot.y) * speed;
    self->actor.velocity.z = Math_CosS(self->actor.world.rot.y) * speed;
    self->actor.colChkInfo.mass = 240;
    self->actionFunc = ObjKibako_Thrown;
}

static void ObjKibako_ApplyGravityInl(ObjKibako* self) {
    self->actor.velocity.y += self->actor.gravity;
    if (self->actor.velocity.y < self->actor.minVelocityY) {
        self->actor.velocity.y = self->actor.minVelocityY;
    }
}

void ObjKibako_Held(ObjKibako* self, PlayState* play) {
    if (Actor_HasNoParent(&self->actor, play)) {
        self->actor.room = *(s8*)((u8*)play + 0x4C30);
        if (ObjKibako_Fabsf(self->actor.speedXZ) < 0.1f) {
            ObjKibako_SetupIdle(self);
        } else {
            ObjKibako_SetupThrownInl(self);
            ObjKibako_ApplyGravityInl(self);
            FUN_0036b96c(&self->actor);
        }
        Actor_UpdateBgCheckInfo(play, &self->actor, 19.0f, 20.0f, 0.0f, 5);
    }
}
#endif

GLOBAL_ASM("asm/ObjKibako_Idle.s")

// Unmatched: Thrown (~59/86 words off, the shared break tail is laid out differently). Kept as asm.
GLOBAL_ASM("asm/ObjKibako_Thrown.s")

#if 0 // Thrown draft
static inline void ObjKibako_SpawnCollectibleInl(ObjKibako* self, PlayState* play) {
    PlayState* p = play;
    Actor* actor = &self->actor;
    s32 params = self->actor.params;
    s32 dropParams = params & 0x1F;

    if ((u32)dropParams < 0x1A) {
        Item_DropCollectible(p, &actor->world.pos, dropParams | (params & 0x3F00));
    }
}

void ObjKibako_Thrown(ObjKibako* self, PlayState* play) {
    if ((self->actor.bgCheckFlags & 0xB) || (self->collider.base.atFlags & AT_HIT)) {
        ObjKibako_AirBreak(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectibleInl(self, play);
        Actor_Kill(&self->actor);
    } else if (self->actor.bgCheckFlags & 0x40) {
        ObjKibako_WaterBreak(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectibleInl(self, play);
        Actor_Kill(&self->actor);
    } else {
        ObjKibako_ApplyGravityInl(self);
        FUN_0036b96c(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 19.0f, 20.0f, 0.0f, 5);
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetOC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), &self->collider.base);
        CollisionCheck_SetAT(play, (CollisionCheckContext*)((u8*)play + 0x5C78), &self->collider.base);
    }
}
#endif

void FUN_0036f59c(Actor* actor, u32 sfxId);
void FUN_0034df30(Actor* actor, PlayState* play);

#if 0 // Idle draft
void ObjKibako_Idle(ObjKibako* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actionFunc = ObjKibako_Held;
        self->actor.room = -1;
        FUN_0036f59c(&self->actor, 0x01000079);
    } else if ((self->actor.bgCheckFlags & 0x20) && (self->actor.yDistToWater > 19.0f)) {
        ObjKibako_WaterBreak(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectibleInl(self, play);
        Actor_Kill(&self->actor);
    } else if (self->collider.base.acFlags & AC_HIT) {
        ObjKibako_AirBreak(self, play);
        Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 20, NA_SE_EV_WOODBOX_BREAK);
        ObjKibako_SpawnCollectibleInl(self, play);
        Actor_Kill(&self->actor);
    } else {
        if (self->actor.bgCheckFlags & 2) {
            s16 sceneId = *(s16*)((u8*)play + 0x104);

            if (sceneId == 7) {
                if (self->actor.room != 2 || self->unk_208 > 50) {
                    FUN_0036f59c(&self->actor, 0x01000202);
                }
            } else if (sceneId != 5 || self->actor.room != 4 || self->unk_208 > 50) {
                FUN_0036f59c(&self->actor, 0x01000202);
            }
        }
        Actor_MoveForward(&self->actor);
        Actor_UpdateBgCheckInfo(play, &self->actor, 19.0f, 20.0f, 0.0f, 5);
        if (!(self->collider.base.ocFlags1 & 8) && (self->actor.xzDistToPlayer > 28.0f)) {
            self->collider.base.ocFlags1 |= 8;
        }
        if (self->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetAC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), &self->collider.base);
            if (self->actor.xzDistToPlayer > 4.0f && self->actor.xzDistToPlayer < 180.0f) {
                CollisionCheck_SetOC(play, (CollisionCheckContext*)((u8*)play + 0x5C78), &self->collider.base);
            }
        }
        if (self->actor.xzDistToPlayer < 100.0f) {
            FUN_0034df30(&self->actor, play);
        }
    }
}
#endif


void EffectSsGSplash_Spawn(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, s16 type, s16 scale);
void ObjKibako_WaterBreak(ObjKibako* self, PlayState* play) {
    s16 angle;
    s32 i;
    Vec3f* breakPos = &self->actor.world.pos;
    Vec3f pos;
    Vec3f velocity;
    s16 phi_s0;

    pos = *breakPos;
    pos.y += self->actor.yDistToWater;
    EffectSsGSplash_Spawn(play, &pos, NULL, NULL, 0, 500);

    for (i = 0, angle = 0; i < 12; i++, angle += 0x4E20) {
        f32 sn = Math_SinS(angle);
        f32 cs = Math_CosS(angle);
        f32 temp_rand;

        pos.x = sn * 16.0f;
        pos.y = (Rand_ZeroOne() * 5.0f) + 2.0f;
        pos.z = cs * 16.0f;
        velocity.x = pos.x * 0.18f;
        velocity.y = (Rand_ZeroOne() * 4.0f) + 2.0f;
        velocity.z = pos.z * 0.18f;
        pos.x += breakPos->x;
        pos.y += breakPos->y;
        pos.z += breakPos->z;
        temp_rand = Rand_ZeroOne();
        if (temp_rand < 0.2f) {
            phi_s0 = 0x40;
        } else {
            phi_s0 = 0x20;
        }
        EffectSsKakera_Spawn(play, &pos, &velocity, breakPos, -180, phi_s0 | 0x200, 30, 30, 0, (Rand_ZeroOne() * 30.0f) + 10.0f,
                             0, 32, 70, -1, OBJECT_GAMEPLAY_DANGEON_KEEP, (void*)1);
    }
}

}
