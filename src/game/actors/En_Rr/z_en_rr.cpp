extern "C" {
#include "z_en_rr.hpp"

#define FLAGS 0x00000435

void EnRr_Init(Actor* actor, GameState* state);
void EnRr_Destroy(Actor* thisx, PlayState* play);
void EnRr_Update(Actor* actor, GameState* state);
void EnRr_Draw(Actor* actor, GameState* state);

void FUN_003685a0(void* arg);

void EnRr_Stunned(EnRr* self, PlayState* play);
void FUN_00355b40(EnRr* self, PlayState* play);
void FUN_003cf768(EnRr* self, PlayState* play);
void FUN_003c3e54(EnRr* self, PlayState* play);
s32 Player_InCsMode(PlayState* play);
void EnRr_CollisionCheck(EnRr* self, PlayState* play);

GLOBAL_ASM("data/z_En_Rr.data.s")

ActorInit En_Rr_InitVars = {
    ACTOR_EN_RR,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_RR,
    sizeof(EnRr),
    (ActorFunc)EnRr_Init,
    (ActorFunc)EnRr_Destroy,
    (ActorFunc)EnRr_Update,
    (ActorFunc)EnRr_Draw,
};


void EnRr_Destroy(Actor* thisx, PlayState* play) {
    EnRr* self = (EnRr*)thisx;

    Collider_DestroyCylinder(play, &self->collider1);
    Collider_DestroyCylinder(play, &self->collider2);
    FUN_003685a0(&self->animModel);
    SkelAnime_Free2(&self->skelAnime);
}

// Draft EnRr_Init (~180w, far): needs data_syms.s EQUs EnRr_sCylinderInit1=0x52E424, EnRr_sCylinderInit2=0x52E45C,
// EnRr_sDamageTable=0x52E494, EnRr_sInitChain=0x52E4E4 (extern decls + GLOBAL_ASM of an actor-dir data_syms.s) and the
// header field `SkeletonAnimationModel_unk_0C animModel` (0x27B4, now char[0x98]). ZARInfo* FUN_0034f248(Actor*, PlayState*).
// Remaining diffs: damageTable/health store temp regs (r0 vs r1), skelAnime ptr kept in r6 via `mov r6,r2`, animModel ptr reg.
#if 0
void EnRr_Init(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnRr* self = (EnRr*)thisx;
    ZARInfo* zar;
    s32 i;
    s32 phase;

    Actor_ProcessInitChain(&self->actor, EnRr_sInitChain);
    self->actor.colChkInfo.damageTable = &EnRr_sDamageTable;
    self->actor.colChkInfo.health = 4;
    zar = FUN_0034f248(&self->actor, play);
    {
        SkelAnime* sa;
        SkeletonAnimationModel_unk_0C* am;
        AnimatedMaterial* mat;
        void* cmab;

        FUN_00353c9c(&self->actor, play, sa = &self->skelAnime, 0, -1, self->unk_24BC, self->unk_24BC + 0x104, 5);
        mat = sa->unk_28->unk_10;
        cmab = zar->GetCMABByIndex(0);
        am = &self->animModel;
        am->unk_00 = mat;
        am->FUN_00372d94(cmab);
        am->animType = 1;
        am->animSpeed = 1.0f;
    }
    Collider_InitCylinder(play, &self->collider1);
    Collider_SetCylinderType1(play, &self->collider1, &self->actor, &EnRr_sCylinderInit1);
    Collider_InitCylinder(play, &self->collider2);
    Collider_SetCylinderType1(play, &self->collider2, &self->actor, &EnRr_sCylinderInit2);
    Actor_SetFocus(&self->actor, 30.0f);
    self->actor.scale.y = 0.013f;
    self->actor.scale.x = self->actor.scale.z = 0.014f;
    self->actor.colChkInfo.mass = 0xFF;
    self->actor.velocity.y = self->actor.speedXZ = 0.0f;
    self->actor.gravity = -0.4f;
    self->actionTimer = 0;
    self->eatenShield = 0;
    self->eatenTunic = 0;
    self->retreat = false;
    self->grabTimer = 0;
    self->invincibilityTimer = 0;
    self->effectTimer = 0;
    self->unk_284C = 0;
    self->hasPlayer = false;
    self->stopScroll = false;
    self->unk_284D = 0;
    *((u8*)play + 0x7FD4) = 0;
    self->ocTimer = 0;
    self->reachState = self->isDead = false;
    self->actionFunc = FUN_003cf768;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].height = self->bodySegs[i].heightTarget = self->bodySegs[i].scaleMod.x =
            self->bodySegs[i].scaleMod.y = self->bodySegs[i].scaleMod.z = 0.0f;
    }
    self->segMovePhase = 0;
    self->segPhaseVel = 0.0f;
    self->segPhaseVelTarget = 2500.0f;
    self->segPulsePhaseDiff = 0.0f;
    self->segWobblePhaseDiffX = 0.0f;
    self->segWobbleXTarget = 3.0f;
    self->segWobblePhaseDiffZ = 0.0f;
    self->segWobbleZTarget = 1.0f;
    self->pulseSize = 0.0f;
    self->pulseSizeTarget = 0.15f;
    self->wobbleSize = 0.0f;
    self->wobbleSizeTarget = 2048.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].scaleMod.y = 0.0f;
        self->bodySegs[i].rotTarget.x = 0.0f;
        self->bodySegs[i].rotTarget.y = 0.0f;
        self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scale.x = self->bodySegs[i].scale.y = self->bodySegs[i].scale.z =
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.y = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    phase = 0;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].scaleMod.x = self->bodySegs[i].scaleMod.z =
            Math_CosS(phase + (s32)self->segPulsePhaseDiff * i * 0x1000) * self->pulseSize;
    }
    for (i = 1; i < 5; i++) {
        self->bodySegs[i].rotTarget.x = Math_CosS((s32)self->segWobblePhaseDiffX * i * 0x1000) * self->wobbleSize;
        self->bodySegs[i].rotTarget.z = Math_SinS((s32)self->segWobblePhaseDiffZ * i * 0x1000) * self->wobbleSize;
    }
}
#endif

GLOBAL_ASM("asm/EnRr_Init.s")

// Draft EnRr_Update (~290w, far): needs the guarded function-local statics sRadius/sRadiusSq (guard 0x52E418, +4 = 175.0f,
// +8 = r*r in z_En_Rr.data) pinned via pin_static, play/self+0x200/play+0x2000 land in different callee-saved regs.
#if 0
void EnRr_Update(Actor* thisx, PlayState* play) {
    EnRr* self = (EnRr*)thisx;
    s32 i;
    s16 phase;

    self->frameCount++;
    if (!self->stopScroll) {
        self->scrollTimer++;
    }
    if (self->actionTimer != 0) {
        self->actionTimer--;
    }
    if (self->grabTimer != 0) {
        self->grabTimer--;
    }
    if (self->ocTimer != 0) {
        self->ocTimer--;
    }
    if (self->invincibilityTimer != 0) {
        self->invincibilityTimer--;
    }
    if (self->effectTimer != 0) {
        self->effectTimer--;
    }
    if (self->hasPlayer == 1 && Player_InCsMode(play)) {
        self->unk_284D = 1;
    }
    Actor_SetFocus(&self->actor, 30.0f);
    phase = self->segMovePhase;
    if (!self->isDead) {
        for (i = 0; i < 5; i++) {
            self->bodySegs[i].scaleMod.x = self->bodySegs[i].scaleMod.z =
                Math_CosS(phase + (s32)self->segPulsePhaseDiff * i * 0x1000) * self->pulseSize;
        }
        phase = self->segMovePhase;
        if (!self->isDead && (self->reachState == 0)) {
            for (i = 1; i < 5; i++) {
                self->bodySegs[i].rotTarget.x =
                    Math_CosS(phase + (s32)self->segWobblePhaseDiffX * i * 0x1000) * self->wobbleSize;
                self->bodySegs[i].rotTarget.z =
                    Math_SinS(phase + (s32)self->segWobblePhaseDiffZ * i * 0x1000) * self->wobbleSize;
            }
        }
    }
    if (!self->stopScroll) {
        self->segMovePhase += (s16)self->segPhaseVel;
    }
    if (!self->isDead && ((*(u16*)((u8*)&self->actor + 0x11A) == 0) || !(*(u32*)((u8*)&self->actor + 0x11C) & 0x400000))) {
        EnRr_CollisionCheck(self, play);
    }

    self->actionFunc(self, play);

    Math_StepToF(&self->actor.speedXZ, 0.0f, 0.1f);
    Actor_MoveForward(&self->actor);
    Collider_UpdateCylinder(&self->actor, &self->collider1);
    self->collider2.dim.pos.x = self->mouthPos.x;
    self->collider2.dim.pos.y = self->mouthPos.y;
    self->collider2.dim.pos.z = self->mouthPos.z;
    if (!self->isDead && (self->invincibilityTimer == 0)) {
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider1.base);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider2.base);
        if (self->ocTimer == 0) {
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider1.base);
        }
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider2.base);
    } else {
        self->collider2.base.ocFlags1 &= ~2;
        self->collider2.base.acFlags &= ~2;
        self->collider1.base.ocFlags1 &= ~2;
        self->collider1.base.acFlags &= ~2;
    }
    {
        static f32 sRadius = 175.0f;
        static f32 sRadiusSq = sRadius * sRadius;
        Actor* other = play->actorCtx.actorLists[ACTORCAT_SWITCH].head;

        while (other != NULL) {
            if (other->id == 0xE6) {
                f32 dx = self->actor.world.pos.x - other->world.pos.x;
                f32 dz = self->actor.world.pos.z - other->world.pos.z;
                f32 d2 = dx * dx + dz * dz;

                if (d2 != 0.0f && d2 < sRadiusSq) {
                    f32 inv = 1.0f / sqrtf(d2);

                    self->actor.world.pos.x = other->world.pos.x + dx * inv * sRadius;
                    self->actor.world.pos.z = other->world.pos.z + dz * inv * sRadius;
                }
            }
            other = other->next;
        }
    }
    Actor_UpdateBgCheckInfo(play, &self->actor, 20.0f, 30.0f, 20.0f, 7);
    if (!self->stopScroll) {
        Math_ApproachF(&self->segPhaseVel, self->segPhaseVelTarget, 1.0f, 50.0f);
        Math_ApproachF(&self->segPulsePhaseDiff, 4.0f, 1.0f, 5.0f);
        Math_ApproachF(&self->segWobblePhaseDiffX, self->segWobbleXTarget, 1.0f, 0.04f);
        Math_ApproachF(&self->segWobblePhaseDiffZ, self->segWobbleZTarget, 1.0f, 0.01f);
        Math_ApproachF(&self->pulseSize, self->pulseSizeTarget, 1.0f, 0.0015f);
        Math_ApproachF(&self->wobbleSize, self->wobbleSizeTarget, 1.0f, 20.0f);
        for (i = 0; i < 5; i++) {
            Math_SmoothStepToS(&self->bodySegs[i].rot.x, self->bodySegs[i].rotTarget.x, 5, self->segMoveRate * 1000.0f,
                               0);
            Math_SmoothStepToS(&self->bodySegs[i].rot.z, self->bodySegs[i].rotTarget.z, 5, self->segMoveRate * 1000.0f,
                               0);
            Math_ApproachF(&self->bodySegs[i].scale.x, self->bodySegs[i].scaleTarget.x, 1.0f, self->segMoveRate * 0.2f);
            self->bodySegs[i].scale.z = self->bodySegs[i].scale.x;
            Math_ApproachF(&self->bodySegs[i].height, self->bodySegs[i].heightTarget, 1.0f, self->segMoveRate * 300.0f);
        }
        Math_ApproachF(&self->segMoveRate, 1.0f, 1.0f, 0.2f);
    }
}
#endif

GLOBAL_ASM("asm/EnRr_Update.s")

GLOBAL_ASM("asm/EnRr_Draw.s")

GLOBAL_ASM("asm/EnRr_CollisionCheck.s")

void EnRr_SetupStunned(EnRr* self) {
    s32 i;

    self->stopScroll = true;
    self->segMovePhase = 0;
    self->segPhaseVel = 0.0f;
    self->segPhaseVelTarget = 2500.0f;
    self->segPulsePhaseDiff = 0.0f;
    self->segWobblePhaseDiffX = 0.0f;
    self->segWobbleXTarget = 3.0f;
    self->segWobblePhaseDiffZ = 0.0f;
    self->segWobbleZTarget = 1.0f;
    self->pulseSize = 0.0f;
    self->pulseSizeTarget = 0.15f;
    self->wobbleSize = 0.0f;
    self->wobbleSizeTarget = 2048.0f;
    for (i = 0; i < 5; i++) {
        self->bodySegs[i].scaleMod.y = 0.0f;
        self->bodySegs[i].rotTarget.x = 0.0f;
        self->bodySegs[i].rotTarget.y = 0.0f;
        self->bodySegs[i].rotTarget.z = 0.0f;
        self->bodySegs[i].scale.x = self->bodySegs[i].scale.y = self->bodySegs[i].scale.z =
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.y = self->bodySegs[i].scaleTarget.z = 1.0f;
    }
    self->actionFunc = EnRr_Stunned;
}

// Best variant (60w): `u8* base = (u8*)play + 0x2000; player = *(Player**)(base + 0xAC);` gets the hoisted add #0x2000 and
// ldr =0x11a; ROM keeps play in r7 with r8 unused (push {r4-r8,lr}) and flag in r5, player in r6.
#if 0
void EnRr_Stunned(EnRr* self, PlayState* play) {
    u8* base = (u8*)play + 0x2000;
    Player* player;

    if (*(u16*)((u8*)&self->actor + 0x11A) == 0) {
        self->stopScroll = false;
        if (self->hasPlayer) {
            s32 ok = 1;
            player = *(Player**)(base + 0xAC);
            if (self->unk_284D != 0) {
                if (Player_InCsMode(play) != 0 || *((u8*)player + 0x2488) != 0) {
                    ok = 0;
                }
            }
            if (ok) {
                FUN_00355b40(self, play);
            }
        } else if (self->actor.colChkInfo.health != 0) {
            self->actionFunc = FUN_003cf768;
        } else {
            self->isDead = true;
            self->frameCount = 0;
            self->shrinkRate = 0.0f;
            self->segMoveRate = 0.0f;
            for (s32 i = 0; i < 5; i++) {
                self->bodySegs[i].heightTarget = 0.0f;
                self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
            }
            self->actionFunc = FUN_003c3e54;
            Audio_PlayActorSound2(&self->actor, 0x01000406);
            self->actor.flags &= ~1;
        }
    }
}
#endif

GLOBAL_ASM("asm/EnRr_Stunned.s")

void FUN_0015064c(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    EnRr* self = (EnRr*)arg;

    switch (limbIndex) {
        case 0:
            *mtx = self->limbMtx[0];
            break;
        case 1:
            *mtx = self->limbMtx[1];
            break;
        case 2:
            *mtx = self->limbMtx[2];
            break;
        case 3:
            *mtx = self->limbMtx[3];
            break;
        case 4:
            *mtx = self->limbMtx[4];
            break;
    }
}

void FUN_003cf5a4(EnRr* self, PlayState* play) {
    s32 i;

    if (*(u16*)((u8*)&self->actor + 0x11A) == 0) {
        self->segMoveRate = 0.0f;
        self->pulseSizeTarget = 0.15f;
        self->segPhaseVelTarget = 2500.0f;
        self->wobbleSizeTarget = 2048.0f;
        for (i = 0; i < 5; i++) {
            self->bodySegs[i].heightTarget = 0.0f;
            self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z = 1.0f;
        }
        self->actionFunc = FUN_003cf768;
    } else if ((*(u16*)((u8*)&self->actor + 0x11A) & 8) != 0) {
        for (i = 1; i < 5; i++) {
            self->bodySegs[i].rotTarget.z = 5000.0f;
        }
    } else {
        for (i = 1; i < 5; i++) {
            self->bodySegs[i].rotTarget.z = -5000.0f;
        }
    }
}


void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s32 arg14, s16 playSfx);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);

void FUN_003c3e54(EnRr* self, PlayState* play) {
    s32 i;

    if (self->frameCount < 40) {
        for (i = 0; i < 5; i++) {
            Math_ApproachF(&self->bodySegs[i].heightTarget, i + 59 - (self->frameCount * 25.0f), 1.0f, 50.0f);
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z =
                (SQ(4 - i) * (f32)self->frameCount * 0.003f) + 1.0f;
        }
    } else if (self->frameCount > 94) {
        Vec3f dropPos;

        dropPos.x = self->actor.world.pos.x;
        dropPos.y = self->actor.world.pos.y;
        dropPos.z = self->actor.world.pos.z;
        switch (self->eatenShield) {
            case 1:
                Item_DropCollectible(play, &dropPos, 0x15);
                break;
            case 2:
                Item_DropCollectible(play, &dropPos, 0x16);
                break;
        }
        switch (self->eatenTunic) {
            case 2:
                Item_DropCollectible(play, &dropPos, 0x18);
                break;
            case 3:
                Item_DropCollectible(play, &dropPos, 0x17);
                break;
        }
        switch (self->dropType) {
            case 1:
                Item_DropCollectible(play, &dropPos, 0xF);
                break;
            case 2:
                Item_DropCollectible(play, &dropPos, 5);
                break;
            case 3:
                Item_DropCollectible(play, &dropPos, 0x12);
                break;
            case 4:
                Item_DropCollectible(play, &dropPos, 0x14);
                break;
            case 5:
                Item_DropCollectible(play, &dropPos, 2);
                break;
            case 0:
            default:
                Item_DropCollectibleRandom(play, &self->actor, &dropPos, 0xC0);
                break;
        }
        Actor_Kill(&self->actor);
    } else if (self->frameCount == 88) {
        Vec3f pos;
        Vec3f vel;
        Vec3f accel;

        pos.x = self->actor.world.pos.x;
        pos.y = self->actor.world.pos.y + 20.0f;
        pos.z = self->actor.world.pos.z;
        accel.z = 0.0f;
        accel.y = 0.0f;
        accel.x = 0.0f;
        vel.z = 0.0f;
        vel.y = 0.0f;
        vel.x = 0.0f;

        EffectSsDeadDb_Spawn(play, &pos, &vel, &accel, 100, 0, 255, 255, 255, 255, 255, 0, 0, 1, 11, true);
    } else {
        Math_ApproachF(&self->actor.scale.x, 0.0f, 1.0f, self->shrinkRate);
        Math_ApproachF(&self->shrinkRate, 0.001f, 1.0f, 0.00001f);
        self->actor.scale.z = self->actor.scale.x;
    }
}

// FUN_00355b40 (EnRr_SetupReleasePlayer) 5w: only the register of the hoisted `ldr =0x284c` pointer (r1 vs r2) differs.
// GLOBAL_ASM stays in functions_350000s.cpp.
#if 0
u8 Inventory_DeleteEquipment(PlayState* play, s32 equipType);
void FUN_00368fc0(PlayState* play, Actor* actor, f32 speed, s16 yaw, f32 unk, s32 unk2);
void FUN_00367c7c(PlayState*, u16 textID, Actor* actor);
void FUN_003cf5a4(EnRr* self, PlayState* play);

static inline u8 EnRr_GetMessage(u8 shield, u8 tunic) {
    u8 messageIndex = 0;

    if ((shield == 1) || (shield == 2)) {
        messageIndex = 1;
    }
    if ((tunic == 2) || (tunic == 3)) {
        messageIndex |= 2;
    }
    return messageIndex;
}

void FUN_00355b40(EnRr* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    u8 tunic = 0;
    u8 shield = 0;
    s32 i;

    self->segMoveRate = 0.0f;
    self->hasPlayer = false;
    self->unk_284D = 0;
    self->actor.flags |= 1;
    u8* flag = &self->unk_284C;
    if (*flag != 0) {
        *(u32*)((u8*)player + 0x1714) &= ~0x20000000;
        self->unk_284C = 0;
    }
    self->ocTimer = 110;
    self->segPhaseVelTarget = 2500.0f;
    self->wobbleSizeTarget = 2048.0f;
    if (CUR_EQUIP_VALUE(1) != 3) {
        shield = Inventory_DeleteEquipment(play, 1);
        if (shield != 0) {
            self->eatenShield = shield;
            self->retreat = true;
        }
    }
    if (CUR_EQUIP_VALUE(2) != 1) {
        tunic = Inventory_DeleteEquipment(play, 2);
        if (tunic != 0) {
            self->eatenTunic = tunic;
            self->retreat = true;
        }
    }
    player->actor.parent = NULL;
    switch (EnRr_GetMessage(shield, tunic)) {
        case 1:
            FUN_00367c7c(play, 0x305F, NULL);
            break;
        case 2:
            FUN_00367c7c(play, 0x3060, NULL);
            break;
        case 3:
            FUN_00367c7c(play, 0x3061, NULL);
            break;
    }
    FUN_00368fc0(play, &self->actor, 4.0f, self->actor.shape.rot.y, 12.0f, 8);
    if (*(u16*)((u8*)&self->actor + 0x11A) == 0) {
        self->actionFunc = FUN_003cf768;
        Audio_PlayActorSound2(&self->actor, 0x01000404);
    } else if (self->actor.colChkInfo.health != 0) {
        self->reachState = 0;
        self->segMoveRate = 0.0f;
        self->actionTimer = 30;
        self->segPhaseVelTarget = 2500.0f;
        self->pulseSizeTarget = 0.0f;
        self->wobbleSizeTarget = 0.0f;
        for (i = 0; i < 5; i++) {
            self->bodySegs[i].heightTarget = 0.0f;
            self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
            self->bodySegs[i].scaleTarget.x = self->bodySegs[i].scaleTarget.z = 1.0f;
        }
        self->actionFunc = FUN_003cf5a4;
        Audio_PlayActorSound2(&self->actor, 0x01000405);
    } else {
        self->isDead = true;
        self->frameCount = 0;
        self->shrinkRate = 0.0f;
        self->segMoveRate = 0.0f;
        for (i = 0; i < 5; i++) {
            self->bodySegs[i].heightTarget = 0.0f;
            self->bodySegs[i].rotTarget.x = self->bodySegs[i].rotTarget.z = 0.0f;
        }
        self->actionFunc = FUN_003c3e54;
        Audio_PlayActorSound2(&self->actor, 0x01000406);
        self->actor.flags &= ~1;
    }
}
#endif

// FUN_003c3c9c (EnRr_GrabPlayer): GLOBAL_ASM stays in functions_3C0000s.cpp.
#if 0
void FUN_003c3c9c(EnRr* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    if ((self->frameCount & 7) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000403);
    }
    self->ocTimer = 8;
    if (self->unk_284C == 0) {
        f32 dx = player->actor.world.pos.x - self->mouthPos.x;
        f32 dy = player->actor.world.pos.y - (self->mouthPos.y - 55.0f);
        f32 dz = player->actor.world.pos.z - self->mouthPos.z;

        if (dx * dx + dy * dy + dz * dz < 16.0f) {
            u8* p = &self->unk_284C; *p = 1;
            *(u32*)((u8*)player + 0x1714) |= 0x20000000;
        }
    }
    if (self->unk_284D != 0) {
        *((u8*)play + 0x7FD4) = 1;
    }
    {
        s32 release = 0;

        if (self->grabTimer == 0) {
            Player* p2 = GET_PLAYER(play);

            release = 1;
            if (self->unk_284D != 0) {
                if (Player_InCsMode(play) != 0 || *((u8*)p2 + 0x2488) != 0) {
                    release = 0;
                }
            }
        }
        if (release || (*(u32*)((u8*)player + 0x1714) & 0x80) == 0) {
            FUN_00355b40(self, play);
            return;
        }
    }
    Math_ApproachF(&player->actor.world.pos.x, self->mouthPos.x, 1.0f, 30.0f);
    Math_ApproachF(&player->actor.world.pos.y, self->mouthPos.y + self->swallowOffset, 1.0f, 30.0f);
    Math_ApproachF(&player->actor.world.pos.z, self->mouthPos.z, 1.0f, 30.0f);
    Math_ApproachF(&self->swallowOffset, -55.0f, 1.0f, 5.0f);
}
#endif

}
