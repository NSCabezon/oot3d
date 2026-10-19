extern "C" {
#include "z_bg_ydan_sp.hpp"
#include "mtx34_trans.hpp"
#include "model_manager.hpp"

void FUN_0036c174(MTX34*, MTX34*, MTX34*);

#define FLAGS 0x80000000

void BgYdanSp_Init(Actor* actor, GameState* state);
void BgYdanSp_Destroy(Actor* actor, GameState* state);
void BgYdanSp_Update(Actor* actor, GameState* state);
void BgYdanSp_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Ydan_Sp.data.s")

ActorInit Bg_Ydan_Sp_InitVars = {
    ACTOR_BG_YDAN_SP,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_YDAN_OBJECTS,
    sizeof(BgYdanSp),
    (ActorFunc)BgYdanSp_Init,
    (ActorFunc)BgYdanSp_Destroy,
    (ActorFunc)BgYdanSp_Update,
    (ActorFunc)BgYdanSp_Draw,
};

// These aliases name existing bytes; they do not add or replace data.
GLOBAL_ASM("../src/game/actors/Bg_Ydan_Sp/data_syms.s")
struct BgYdanSpTrisElementInit { char info[0x18]; Vec3f vtx[3]; };
static InitChainEntry sInitChain[] = { ICHAIN_VEC3F_DIV1000(scale, 100, ICHAIN_STOP) };
extern BgYdanSpTrisElementInit BgYdanSp_sFloorTris, BgYdanSp_sWallTris;
extern char BgYdanSp_sTrisInit;
void Collider_InitTris(PlayState*, void*);
void Collider_SetTris(PlayState*, void*, Actor*, void*, void*);
void Collider_SetTrisVertices(void*, s32, Vec3f*, Vec3f*, Vec3f*);
void BgYdanSp_FloorWebIdle(BgYdanSp*, PlayState*);
static inline void BgYdanSp_BurnWeb(BgYdanSp*, PlayState*);
void BgYdanSp_WallWebIdle(BgYdanSp*, PlayState*);

void BgYdanSp_Init(Actor* thisx, GameState* state) {
    BgYdanSp* self = (BgYdanSp*)thisx;
    PlayState* play = (PlayState*)state;
    Vec3f tri[3];
    s32 i;
    CollisionHeader* colHeader = NULL;
    f32 cossY;
    f32 sinsY;
    f32 cossX;
    f32 nSinsX;

    Actor_ProcessInitChain(&self->actor, sInitChain);
    self->isDestroyedSwitchFlag = (u16)thisx->params & 0x3F;
    self->burnSwitchFlag = ((u16)thisx->params >> 6) & 0x3F;
    self->playChime = 1 & ~((u32)thisx->params >> 15);
    self->enablePlayerPush = ((u16)thisx->params & 0x4000) >> 14;
    s32 type = self->actor.params = ((u16)thisx->params >> 12) & 3;
    self->actor.unk_19A = 1;
    ZARInfo* zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    u8 modelIndices[] = {6, 5, 0, 0};
    FUN_00372f38(thisx, play, &self->model, modelIndices[type], 0);
    if (self->actor.params != 1) {
        FUN_00372f38(thisx, play, &self->fragments[0], 2,
            &self->fragments[1], 2, &self->fragments[2], 2,
            &self->fragments[3], 2, &self->fragments[4], 2,
            &self->fragments[5], 2, &self->fragments[6], 2,
            &self->fragments[7], 2, 0);
        self->actor.flags |= 0x400000;
    }
    DynaPolyActor_Init((DynaPolyActor*)self, 1);
    Collider_InitTris(play, &self->collider);
    Collider_SetTris(play, &self->collider, &self->actor, &BgYdanSp_sTrisInit, self->elements);
    if (self->actor.params == 0) {
        ReadCollisionHeaderFromZsi(zar->FUN_003532c0(2), &colHeader);
        void (*floorAction)(BgYdanSp*, PlayState*) = BgYdanSp_FloorWebIdle;
        BgYdanSpTrisElementInit* triElementInit0 = &BgYdanSp_sFloorTris;
        self->actionFunc = floorAction;

        for (i = 0; i < 3; i++) {
            tri[i].x = self->actor.world.pos.x + triElementInit0->vtx[i].x;
            tri[i].y = self->actor.world.pos.y + triElementInit0->vtx[i].y;
            tri[i].z = self->actor.world.pos.z + triElementInit0->vtx[i].z;
        }

        Collider_SetTrisVertices(&self->collider, 0, &tri[0], &tri[1], &tri[2]);
        tri[1].x = tri[0].x;
        tri[1].z = tri[2].z;
        Collider_SetTrisVertices(&self->collider, 1, &tri[0], &tri[2], &tri[1]);
        self->amplitude = 0.0f;
    } else {
        ReadCollisionHeaderFromZsi(zar->FUN_003532c0(1), &colHeader);
        self->actionFunc = BgYdanSp_WallWebIdle;
        Actor_SetFocus(thisx, 30.0f);
        BgYdanSpTrisElementInit* triElementInit1 = &BgYdanSp_sWallTris;
        sinsY = Math_SinS(self->actor.shape.rot.y);
        cossY = Math_CosS(self->actor.shape.rot.y);
        nSinsX = -Math_SinS(self->actor.shape.rot.x);
        cossX = Math_CosS(self->actor.shape.rot.x);

        for (i = 0; i < 3; i++) {
            f32 pointX = self->actor.world.pos.x + (cossY * triElementInit1->vtx[i].x);
            pointX -= (triElementInit1->vtx[i].y * sinsY * nSinsX);
            tri[i].x = pointX;
            tri[i].y = self->actor.world.pos.y + (triElementInit1->vtx[i].y * cossX);
            f32 pointZ = self->actor.world.pos.z - (sinsY * triElementInit1->vtx[i].x);
            pointZ += (triElementInit1->vtx[i].y * cossY * nSinsX);
            tri[i].z = pointZ;
        }

        Collider_SetTrisVertices(&self->collider, 0, &tri[0], &tri[1], &tri[2]);

        f32 pointX = self->actor.world.pos.x + (cossY * triElementInit1->vtx[0].x);
        pointX -= (triElementInit1->vtx[2].y * sinsY * nSinsX);
        tri[1].x = pointX;
        tri[1].y = self->actor.world.pos.y + (triElementInit1->vtx[2].y * cossX);
        f32 pointZ = self->actor.world.pos.z - (sinsY * triElementInit1->vtx[0].x);
        pointZ += (triElementInit1->vtx[2].y * cossY * nSinsX);
        tri[1].z = pointZ;
        Collider_SetTrisVertices(&self->collider, 1, &tri[0], &tri[2], &tri[1]);
    }
    self->bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->actor, colHeader);
    self->timer = 0;
    if (Flags_GetSwitch(play, self->isDestroyedSwitchFlag)) {
        Actor_Kill(&self->actor);
    }
}

extern "C" void Collider_DestroyTris(PlayState*, void*);
void BgYdanSp_Destroy(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    DynaPolyActor* dyna = (DynaPolyActor*)actor;
    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, dyna->bgId);
    Collider_DestroyTris(play, &((BgYdanSp*)actor)->collider);
    FUN_00350f34(actor, &((BgYdanSp*)actor)->model, (void*)0);
    FUN_00350f34(actor,
                 &((BgYdanSp*)actor)->fragments[0], &((BgYdanSp*)actor)->fragments[1],
                 &((BgYdanSp*)actor)->fragments[2], &((BgYdanSp*)actor)->fragments[3],
                 &((BgYdanSp*)actor)->fragments[4], &((BgYdanSp*)actor)->fragments[5],
                 &((BgYdanSp*)actor)->fragments[6], &((BgYdanSp*)actor)->fragments[7], (void*)0);
}

void BgYdanSp_Update(Actor* actor, GameState* state) {
    BgYdanSp* self = (BgYdanSp*)actor;
    self->actionFunc(self, (PlayState*)state);
}

#if 0 // Draw best: 187/231 words; 928 B vs ROM 924 B; matrix helper allocation/scheduling.
void FUN_00330c34(Actor*, BgYdanSp*);
void BgYdanSp_FloorWebBroken(BgYdanSp*, PlayState*);
void FUN_0036c258(f32*, f32*, f32);
void FUN_00330b98(void*, SkeletonAnimationModel*, s32);
struct BgYdanSpManagerView { char unk_000[0x180]; char renderQueue; };
static inline void BgYdanSp_RotateAxis(MTX34* dst, MTX34* src, f32 angle, f32 x, f32 y, f32 z) {
    f32 sin, cos;
    FUN_0036c258(&sin, &cos, angle);
    f32 t = 1.0f - cos;
    MTX34 rotation;
    rotation.data[0][0] = cos + t * x * x;
    rotation.data[0][1] = t * x * y - sin * z;
    rotation.data[0][2] = t * x * z + sin * y;
    rotation.data[0][3] = 0.0f;
    rotation.data[1][0] = t * x * y + sin * z;
    rotation.data[1][1] = cos + t * y * y;
    rotation.data[1][2] = t * y * z - sin * x;
    rotation.data[1][3] = 0.0f;
    rotation.data[2][0] = t * x * z - sin * y;
    rotation.data[2][1] = t * y * z + sin * x;
    rotation.data[2][2] = cos + t * z * z;
    rotation.data[2][3] = 0.0f;
    FUN_0036c174(dst, src, &rotation);
}
void BgYdanSp_Draw(Actor* actor, GameState* state) {
    BgYdanSp* self = (BgYdanSp*)actor;
    if (self->model == NULL) return;
    if (actor->params == 1) {
        self->model->FUN_003721e0(&actor->modelMtx);
        self->model->SetUnkAC(1);
        self->model->FUN_00372170(0);
    } else if (self->actionFunc == BgYdanSp_FloorWebBroken) {
        MTX34 mtx;
        if (self->timer == 60) FUN_00330c34(actor, self);
        for (s32 i = 0; i < 8; i++) {
            if (self->fragments[i]) {
                f32 angle = (f32)(s16)(i * 0x2000) * 0.000095873801f * 40.7436638f * 0.0245436933f * 40.7436638f;
                BgYdanSp_RotateAxis(&mtx, &actor->modelMtx, angle, 0.0f, 1.0f, 0.0f);
                BgYdanSp_RotateAxis(&mtx, &mtx, -5.625f, 1.0f, 0.0f, 0.0f);
                Vector3f translate(0.0f, 700.0f, -900.0f);
                Matrix_UpdatePosition(&mtx, &mtx, (Vec3f*)&translate);
                Vector3f scale(3.5f, 5.0f, 1.0f);
                MTX34 scaleMtx;
                MTX34_MakeScale(&scaleMtx, (Vec3f*)&scale);
                FUN_0036c174(&mtx, &mtx, &scaleMtx);
                self->fragments[i]->FUN_003721e0(&mtx);
                self->fragments[i]->SetUnkAC(1);
                BgYdanSpManagerView* manager = (BgYdanSpManagerView*)&GetModelManager();
                FUN_00330b98(&manager->renderQueue, self->fragments[i], 0);
            }
        }
    } else {
        FUN_00330c34(actor, self);
    }
}

#endif
GLOBAL_ASM("asm/BgYdanSp_Draw.s")

void EffectSsDeadDb_Spawn(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, s16 scale, s16 scaleStep,
                          s16 primR, s16 primG, s16 primB, s16 primA, s16 envR, s16 envG, s16 envB, s16 unused,
                          s32 arg14, s16 playSfx);

void BgYdanSp_BurnFloorWeb(BgYdanSp* self, PlayState* play) {
    static Vector3f accel(0.0f, 0.0f, 0.0f);
    Vec3f velocity;
    Vec3f pos2;
    f32 distXZ;
    f32 sins;
    f32 coss;
    s16 rot;
    s16 rot2;
    s32 i;

    if (self->timer != 0) {
        self->timer--;
    }

    if (self->timer == 0) {
        Actor_Kill(&self->actor);
        return;
    }
    if ((self->timer % 5) == 0) {
        rot2 = Rand_ZeroOne() * 0x2AAA;
        velocity.y = 0.0f;
        pos2.y = self->actor.world.pos.y;

        for (i = 0; i < 6; i++) {
            rot = Rand_CenteredFloat(0x2800) + rot2;
            sins = Math_SinS(rot);
            coss = Math_CosS(rot);
            pos2.x = self->actor.world.pos.x + (120.0f * sins);
            pos2.z = self->actor.world.pos.z + (120.0f * coss);
            distXZ = Math_Vec3f_DistXZ(&self->actor.home.pos, &pos2) * (1.0f / 120.0f);
            if (distXZ < 0.7f) {
                sins = Math_SinS(rot + 0x8000);
                coss = Math_CosS(rot + 0x8000);
                pos2.x = self->actor.world.pos.x + (120.0f * sins);
                pos2.z = self->actor.world.pos.z + (120.0f * coss);
                distXZ = Math_Vec3f_DistXZ(&self->actor.home.pos, &pos2) * (1.0f / 120.0f);
            }
            velocity.x = (7.0f * sins) * distXZ;
            velocity.y = 0.0f;
            velocity.z = (7.0f * coss) * distXZ;
            EffectSsDeadDb_Spawn(play, &self->actor.home.pos, &velocity, (Vec3f*)&accel, 60, 6, 255, 255, 150, 170, 255, 0,
                                 0, 1, 0x10, 1);
            rot2 += 0x2AAA;
        }
    }
}



f32 sinf(f32);
void BgYdanSp_FloorWebBroken(BgYdanSp*, PlayState*);
void FUN_00366150(PlayState*, Vec3f*, Vec3f*, Vec3f*, Color_RGBA8*, Color_RGBA8*, s16, s32);
struct BgYdanSpCollisionHeader { char unk_00[0x18]; Vec3s* vtxList; };
static inline void BgYdanSp_UpdateFloorWebCollision(BgYdanSp* self, PlayState* play) {
    ObjectStatus* status = play->GetObjectStatus(self->actor.objBankIndex);
    BgYdanSpCollisionHeader* col = (BgYdanSpCollisionHeader*)status->zarInfo.FUN_003532c0(2);
    s16 y = (self->actor.home.pos.y - self->actor.world.pos.y) * 10.0f;
    col->vtxList[14].y = y;
    col->vtxList[12].y = y;
    col->vtxList[10].y = y;
    col->vtxList[9].y = y;
    col->vtxList[6].y = y;
    col->vtxList[5].y = y;
    col->vtxList[1].y = y;
    col->vtxList[0].y = y;
}

void BgYdanSp_FloorWebBreaking(BgYdanSp* self, PlayState* play2) {
    PlayState* play = play2;
    static Color_RGBA8 primColor = { 250, 250, 250, 255 };
    static Color_RGBA8 envColor = { 180, 180, 180, 255 };
    static Vector3f zeroVec(0.0f, 0.0f, 0.0f);
    s32 i;
    Vec3f pos;
    s16 rot;

    if (self->timer != 0) {
        self->timer--;
    }

    self->actor.world.pos.y = (self->amplitude * sinf((f32)self->timer * 0.10471976f)) + self->actor.home.pos.y;
    if (self->actor.home.pos.y - self->actor.world.pos.y > 190.0f) {
        FUN_0036b940(play, &play->colCtx.dyna, self->bgId);
        self->timer = 60;
        if (self->playChime) {
            FUN_00372244(&play->unk_5FCC, 30, 0x01000487);
        }
        Flags_SetSwitch(play, self->isDestroyedSwitchFlag);
        self->actionFunc = BgYdanSp_FloorWebBroken;
        pos.y = self->actor.world.pos.y - 60.0f;
        rot = 0;
        for (i = 0; i < 6; i++) {
            pos.x = Math_SinS(rot) * 60.0f + self->actor.world.pos.x;
            pos.z = Math_CosS(rot) * 60.0f + self->actor.world.pos.z;
            FUN_00366150(play, &pos, (Vec3f*)&zeroVec, (Vec3f*)&zeroVec, &primColor, &envColor, 1000, 10);

            rot += 0x2AAA;
        }
    }
    BgYdanSp_UpdateFloorWebCollision(self, play);
}

#if 0 // Wall best:13/186,744B equals ROM; native position ctor; zero ctor and Y/Z register choices.
void BgYdanSp_BurnWallWeb(BgYdanSp* self, PlayState* play) {
    static Vector3f accel(0.0f, 0.0f, 0.0f);
    Vec3f velocity;
    f32 distXYZ;
    f32 sins;
    f32 coss;
    f32 coss2;
    s16 rot;
    s16 rot2;
    s32 i;

    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        Actor_Kill(&self->actor);
        return;
    }
    if ((self->timer % 5) == 0) {
        rot2 = Rand_ZeroOne() * 0x2AAA;

        for (i = 0; i < 6; i++) {
            rot = Rand_CenteredFloat(0x2800) + rot2;
            sins = Math_SinS(rot);
            coss = Math_CosS(rot);
            coss2 = Math_CosS(self->actor.shape.rot.y) * sins;
            sins *= Math_SinS(self->actor.shape.rot.y);

            Vector3f spC8(self->actor.world.pos.x + (140.0f * coss2),
                          self->actor.world.pos.y + (140.0f * (1.0f + coss)),
                          self->actor.world.pos.z - (140.0f * sins));
            distXYZ = Math_Vec3f_DistXYZ(&self->actor.home.pos, (Vec3f*)&spC8) * (1.0f / 140.0f);
            if (distXYZ < 0.65f) {
                sins = Math_SinS(rot + 0x8000);
                coss = Math_CosS(rot + 0x8000);
                coss2 = Math_CosS(self->actor.shape.rot.y) * sins;
                sins *= Math_SinS(self->actor.shape.rot.y);
                spC8.x = self->actor.world.pos.x + (140.0f * coss2);
                spC8.y = self->actor.world.pos.y + (140.0f * (1.0f + coss));
                spC8.z = self->actor.world.pos.z - (140.0f * sins);
                distXYZ = Math_Vec3f_DistXYZ(&self->actor.home.pos, (Vec3f*)&spC8) * (1.0f / 140.0f);
            }
            velocity.x = 6.5f * coss2 * distXYZ;
            velocity.y = 6.5f * coss * distXYZ;
            velocity.z = -6.5f * sins * distXYZ;
            EffectSsDeadDb_Spawn(play, &self->actor.home.pos, &velocity, (Vec3f*)&accel, 80, 6, 255, 255, 150, 170, 255, 0,
                                 0, 1, 0x10, 1);
            rot2 += 0x2AAA;
        }
    }
}
#endif
GLOBAL_ASM("asm/BgYdanSp_BurnWallWeb.s")

void BgYdanSp_FloorWebBroken(BgYdanSp* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        Actor_Kill(&self->actor);
    }
}

#if 0 // Floor idle best:182/330,1304B vsROM1320; clamp forms; push arithmetic remains far.
void BgYdanSp_FloorWebBreaking(BgYdanSp*, PlayState*);
s32 FUN_003575e8(PlayState*, Vec3f*, f32);
s32 FUN_0036adf4(DynaPolyActor*);
void FUN_0048961c(u32);
f32 sinf(f32);
f32 sqrtf(f32);
struct BgYdanSpPushConstants { f32 range, height, maxSpeed, acceleration; };
extern BgYdanSpPushConstants BgYdanSp_sPushConstants;
extern s16 BgYdanSp_sYawRanges[6];

void BgYdanSp_FloorWebIdle(BgYdanSp* self, PlayState* play2) {
    PlayState* play = play2;
    BgYdanSpPlayer* player = (BgYdanSpPlayer*)GET_PLAYER(play);
    Vec3f webPos;
    webPos.x = self->actor.world.pos.x;
    webPos.y = self->actor.world.pos.y - 50.0f;
    webPos.z = self->actor.world.pos.z;
    if (FUN_003575e8(play, &webPos, 70.0f)) {
        self->actor.home.pos.x = player->stickTip.x;
        self->actor.home.pos.z = player->stickTip.z;
        BgYdanSp_BurnWeb(self, play);
        return;
    }
    if (self->collider.acFlags & 2) {
        BgYdanSp_BurnWeb(self, play);
        return;
    }
    if (FUN_0036adf4((DynaPolyActor*)self)) {
        f32 distance = player->fallDistance < 0 ? 0.0f : (f32)player->fallDistance;
        f32 amplitude = sqrtf(distance) * 2.0f;
        if (player->fallDistance > 750 && self->actor.xzDistToPlayer < 80.0f) {
            self->amplitude = 200.0f;
            self->actor.room = -1;
            self->actor.flags |= 0x10;
            self->timer = 60;
            Audio_PlayActorSound2(&self->actor, 0x010001B9);
            self->actionFunc = BgYdanSp_FloorWebBreaking;
            return;
        }
        if (self->amplitude < amplitude && amplitude > 2.0f) {
            self->amplitude = amplitude;
            self->timer = 21;
        }
        if (player->actor.speedXZ != 0.0f) {
            if (self->amplitude < 0.1f) self->timer = 21;
            self->amplitude = self->amplitude < 2.0f ? 2.0f : self->amplitude;
        }
    } else if (self->enablePlayerPush) {
        BgYdanSpPlayer* p = (BgYdanSpPlayer*)GET_PLAYER(play);
        if (p->action == 0x6B || p->action == 0x95) {
            if (self->actor.xzDistToPlayer < BgYdanSp_sPushConstants.range &&
                p->actor.world.pos.y > BgYdanSp_sPushConstants.height) {
                for (s32 i = 0; i < 3; i++) {
                    if (self->actor.yawTowardsPlayer > BgYdanSp_sYawRanges[i*2] &&
                        self->actor.yawTowardsPlayer < BgYdanSp_sYawRanges[i*2+1]) {
                        self->pushingPlayer = true;
                    }
                }
            }
            if (p->actor.velocity.y < 0.0f && self->pushingPlayer == true) {
                p->stateFlags2 |= 0x80000;
                f32 boost = self->actor.xzDistToPlayer * 0.01f;
                if (boost > 1.0f) boost = 1.0f;
                p->xzSpeed = 0.0f;
                p->actor.speedXZ = 0.0f;
                boost += 0.0f * BgYdanSp_sPushConstants.acceleration;
                p->pushedSpeed += boost;
                f32 maxSpeed = 0.0f + BgYdanSp_sPushConstants.maxSpeed;
                p->pushedSpeed = p->pushedSpeed > maxSpeed ? maxSpeed : p->pushedSpeed;
                p->pushedYaw = self->actor.yawTowardsPlayer + 0x8000;
            }
        } else {
            p->pushedSpeed = 0.0f;
            self->unk_1E8 = 0.0f;
            if (self->pushingPlayer) {
                p->stateFlags2 &= ~0x80000;
                self->pushingPlayer = false;
            }
        }
    }
    if (self->timer != 0) self->timer--;
    if (self->timer == 0) self->timer = 21;
    self->actor.world.pos.y = self->amplitude * sinf((f32)self->timer * 0.2991993f) + self->actor.home.pos.y;
    Math_ApproachZeroF(&self->amplitude, 1.0f, 0.8f);
    if (self->timer == 20) {
        if (self->amplitude > 3.0f) Audio_PlayActorSound2(&self->actor, 0x010001B8);
        else FUN_0048961c(0x010001B8);
    }
    BgYdanSp_UpdateFloorWebCollision(self, play);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider);
}
#endif
GLOBAL_ASM("asm/BgYdanSp_FloorWebIdle.s")

void BgYdanSp_BurnFloorWeb(BgYdanSp*, PlayState*);
void BgYdanSp_BurnWallWeb(BgYdanSp*, PlayState*);
s32 OnePointCutscene_Init(PlayState*, s32, s32, Actor*, s32);

static inline void BgYdanSp_BurnWeb(BgYdanSp* self, PlayState* play) {
    self->timer = 45;
    if (self->playChime) {
        FUN_00372244(&play->unk_5FCC, 30, 0x01000487);
    }
    Flags_SetSwitch(play, self->isDestroyedSwitchFlag);
    if (self->actor.params == 0) {
        self->actionFunc = BgYdanSp_BurnFloorWeb;
    } else {
        self->actionFunc = BgYdanSp_BurnWallWeb;
    }
}

void BgYdanSp_WallWebIdle(BgYdanSp* self, PlayState* play) {
    BgYdanSpPlayer* player = (BgYdanSpPlayer*)GET_PLAYER(play);
    Vec3f rel;
    if (Flags_GetSwitch(play, self->burnSwitchFlag) || (self->collider.acFlags & 2)) {
        self->actor.home.pos.y = self->actor.world.pos.y + 80.0f;
        BgYdanSp_BurnWeb(self, play);
    } else if (player->heldItemAction == 6 && player->stickBurnTimer != 0) {
        FUN_0036c5d8(&self->actor, &rel, &player->stickTip);
        if (__fabsf(rel.x) < 100.0f && rel.z < 1.0f && rel.y < 200.0f) {
            OnePointCutscene_Init(play, 3020, 40, &self->actor, 0);
            Math_Vec3f_Copy(&self->actor.home.pos, &player->stickTip);
            BgYdanSp_BurnWeb(self, play);
        }
    }
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider);
}


#if 0 // Best draft: 15/66 words; shared scale helper scheduling.
void FUN_00330c34(Actor* actor, BgYdanSp* self) {
    MTX34 mtx;
    MTX34* modelMtx = &self->actor.modelMtx;
    Vector3f translate(0.0f, (actor->home.pos.y - actor->world.pos.y) * 10.0f, 0.0f);
    Matrix_UpdatePosition(&mtx, modelMtx, (Vec3f*)&translate);
    Vector3f scale(1.0f, ((actor->home.pos.y - actor->world.pos.y) + 10.0f) * 0.1f, 1.0f);
    MTX34 scaleMtx;
    MTX34_MakeScale(&scaleMtx, (Vec3f*)&scale);
    FUN_0036c174(&mtx, &mtx, &scaleMtx);
    self->model->FUN_003721e0(&mtx);
    self->model->SetUnkAC(1);
    self->model->FUN_00372170(0);
}

#endif
GLOBAL_ASM("asm/FUN_00330c34.s")

}
