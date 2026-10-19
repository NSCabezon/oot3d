extern "C" {
#include "z_demo_kekkai.hpp"

#define FLAGS 0x00000030

void DemoKekkai_Init(Actor* thisx, PlayState* play);
void DemoKekkai_Destroy(Actor* actor, PlayState* play);
void DemoKekkai_Update(Actor* actor, PlayState* play);
void DemoKekkai_TowerBarrier(Actor* thisx, PlayState* play);
void DemoKekkai_Draw(Actor* actor, PlayState* play);
void DemoKekkai_TrialBarrierIdle(Actor* thisx, PlayState* play);
void DemoKekkai_DrawTrialBarrier(Actor* thisx, PlayState* play);
void FUN_0047d548(SkeletonAnimationModel* model, s32 arg);
void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
extern ColliderCylinderInit DemoKekkai_sCylinderInit;
extern u32 DemoKekkai_sEventFlags[];

GLOBAL_ASM("data/z_Demo_Kekkai.data.s")
GLOBAL_ASM("../src/game/actors/Demo_Kekkai/data_syms.s")

ActorInit Demo_Kekkai_InitVars = {
    ACTOR_DEMO_KEKKAI,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_DEMO_KEKKAI,
    sizeof(DemoKekkai),
    (ActorFunc)DemoKekkai_Init,
    (ActorFunc)DemoKekkai_Destroy,
    (ActorFunc)DemoKekkai_Update,
    (ActorFunc)DemoKekkai_Draw,
};

void DemoKekkai_Init(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;
    ZARInfo* zar;
    s32 i;

    self->energyAlpha = 1.0f;
    self->sfxFlag = 0;
    Actor_SetScale(thisx, 0.1f);
    thisx->colChkInfo.mass = 0xFF;
    Collider_InitCylinder(play, &self->collider1);
    Collider_SetCylinder(play, &self->collider1, thisx, &DemoKekkai_sCylinderInit);
    Collider_InitCylinder(play, &self->collider2);
    Collider_SetCylinder(play, &self->collider2, thisx, &DemoKekkai_sCylinderInit);
    Collider_UpdateCylinder(thisx, &self->collider1);
    Collider_UpdateCylinder(thisx, &self->collider2);
    self->barrierScroll = 0.0f;
    self->barrierScrollRate = 1.0f;
    self->timer = 0;
    switch (thisx->params) {
        case 0:
            self->updateFunc = (DemoKekkaiUpdateFunc)DemoKekkai_TowerBarrier;
            self->collider2.dim.radius = (s16)(thisx->scale.x * 6100.0f);
            self->collider2.dim.height = (s16)(thisx->scale.y * 5000.0f);
            self->collider2.dim.yShift = 300;
            zar = FUN_00372f38(thisx, play, &self->skelAnimModel[0], 4, 0);
            self->skelAnimModel[0]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
            self->skelAnimModel[0]->unk_0C->SetAnimType(1);
            self->skelAnimModel[0]->unk_0C->SetAnimSpeed(2.0f);
            FUN_0047d548(self->skelAnimModel[0], 2);
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            self->energyAlpha = 1.0f;
            self->orbScale = 1.0f;
            Actor_SetScale(thisx, 0.1f);
            thisx->update = (ActorFunc)DemoKekkai_TrialBarrierIdle;
            thisx->draw = (ActorFunc)DemoKekkai_DrawTrialBarrier;
            self->collider1.dim.radius = (s16)(thisx->scale.x * 120.0f);
            self->collider1.dim.height = (s16)(thisx->scale.y * 2000.0f);
            self->collider1.dim.yShift = 0;
            self->collider2.dim.radius = (s16)(thisx->scale.x * 320.0f);
            self->collider2.dim.height = (s16)(thisx->scale.y * 510.0f);
            self->collider2.dim.yShift = 95;
            zar = FUN_00372f38(thisx, play, &self->skelAnimModel[0], 8, 0);
            FUN_00372f38(thisx, play, &self->skelAnimModel[1], 7, 0);
            FUN_00372f38(thisx, play, &self->skelAnimModel[2], 5, 0);
            { void* cmab = zar->GetCMABByIndex(thisx->params); SkeletonAnimationModel* m = self->skelAnimModel[0]; m->unk_0C->FUN_00372d94(cmab); }
            { void* cmab = zar->GetCMABByIndex(7); SkeletonAnimationModel* m = self->skelAnimModel[1]; m->unk_0C->FUN_00372d94(cmab); }
            { void* cmab = zar->GetCMABByIndex(8); SkeletonAnimationModel* m = self->skelAnimModel[2]; m->unk_0C->FUN_00372d94(cmab); }
            for (i = 0; i < 3; i++) {
                self->skelAnimModel[i]->unk_0C->animType = 1;
                self->skelAnimModel[i]->unk_0C->animSpeed = 2.0f;
                FUN_0047d548(self->skelAnimModel[i], 2);
            }
            self->cmbData = ZAR_GetCMBByIndex(zar, 8);
            break;
    }
    if ((u32)thisx->params >= 7 || Flags_GetEventChkInf(DemoKekkai_sEventFlags[thisx->params])) {
        if (thisx->params == 0) {
            ((u8*)play)[0x3237] = 1;
        }
        Actor_Kill(thisx);
    }
}

void DemoKekkai_Destroy(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;
    s32 i;

    Collider_DestroyCylinder(play, &self->collider1);
    Collider_DestroyCylinder(play, &self->collider2);
    for (i = 0; i < 3; i++) {
        if (self->skelAnimModel[i] != NULL) {
            FUN_003508b8(thisx, self->skelAnimModel[i], 0);
        }
        self->skelAnimModel[i] = NULL;
    }
}

#define R_UPDATE_RATE SREG(30)

void DemoKekkai_Update(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;

    if (self->energyAlpha > 0.99f) {
        if ((self->collider1.base.atFlags & AT_HIT) || (self->collider2.base.atFlags & AT_HIT)) {
            FUN_00374bb8(play, &self->actor, 6.0f, self->actor.yawTowardsPlayer, 6.0f);
        }
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider1.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider1.base);
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider2.base);
        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider2.base);
    }
    self->updateFunc(self, play);
    self->barrierScroll += self->barrierScrollRate * (R_UPDATE_RATE / 3.0f);
    if (self->barrierScroll > 65536.0f) {
        self->barrierScroll -= 65536.0f;
    }
}

void DemoKekkai_Draw(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;
    f32 speed = (PauseContext_GetState() != 0) ? 0.0f : 2.0f;

    self->skelAnimModel[0]->unk_0C->animSpeed = speed;
    self->skelAnimModel[0]->SetUnkAC(1);
    self->skelAnimModel[0]->FUN_003721e0(&self->actor.modelMtx);
    {
        SkeletonAnimationModel* m = self->skelAnimModel[0];
        f32 x = self->actor.modelMtx.data[0][3];
        f32 y = self->actor.modelMtx.data[1][3];
        f32 z = self->actor.modelMtx.data[2][3];
        m->unk_24 = x;
        m->unk_28 = y;
        m->unk_2C = z;
    }
    self->skelAnimModel[0]->unk_AD[0] = 1;
    self->skelAnimModel[0]->FUN_00372170(0);
}

// Draft, far (fastchk: layout/frame 0x3c matches, then diverges): ROM copies orbScale per multiply (vmov s1,s0; vmul s1,s2,s1) for
// the x/y columns only, keeps `1` in r7 (SetUnkAC family, see Arrow_Light / CLAUDE.md), and the VFP numbering of the two
// translate vectors differs. Pool: 0.0, 2.0, 1200.0, -1200.0, 1.0.
#if 0
#include "mtx34_trans.hpp"
void FUN_00333abc(void* mat, s32 idx, f32* color);
void FUN_00333a38(void* mat, s32 idx, f32* color);
struct KkVec {
    f32& operator[](s32 i) { return (&x)[i]; }
    f32 x, y, z;
};
static inline void KkTranslate(MTX34* mtx, f32 y, f32 x, f32 z) {
    KkVec p;
    p[0] = x;
    p[1] = y;
    p[2] = z;
    Matrix_UpdatePosition(mtx, mtx, (Vec3f*)&p);
}
static inline void KkScale(MTX34* m, f32 x, f32 y, f32 z) {
    m->data[0][0] *= x;
    m->data[1][0] *= x;
    m->data[2][0] *= x;
    m->data[0][1] *= y;
    m->data[1][1] *= y;
    m->data[2][1] *= y;
    m->data[0][2] *= z;
    m->data[1][2] *= z;
    m->data[2][2] *= z;
}

void DemoKekkai_DrawTrialBarrier(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;
    f32 speed;
    s32 i;

    if (self->orbScale == 0.0f) {
        return;
    }
    speed = 2.0f;
    if (PauseContext_GetState() != 0) {
        speed = 0.0f;
    }

    {
    MTX34 mtx;
    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    KkTranslate(&mtx, 1200.0f, 0.0f, 0.0f);
    { f32 sc = self->orbScale; KkScale(&mtx, sc, sc, sc); };
    KkTranslate(&mtx, -1200.0f, 0.0f, 0.0f);
    self->skelAnimModel[1]->unk_0C->animSpeed = speed;
    self->skelAnimModel[1]->SetUnkAC(1);
    self->skelAnimModel[1]->FUN_003721e0(&mtx);
    {
        SkeletonAnimationModel* m = self->skelAnimModel[1];
        m->unk_24 = mtx.data[0][3];
        m->unk_28 = mtx.data[1][3];
        m->unk_2C = mtx.data[2][3];
    }
    self->skelAnimModel[1]->unk_AD[0] = 1;
    self->skelAnimModel[1]->FUN_00372170(0);

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    mtx.data[1][3] += 1.0f;
    self->skelAnimModel[2]->unk_0C->animSpeed = speed;
    self->skelAnimModel[2]->SetUnkAC(1);
    self->skelAnimModel[2]->FUN_003721e0(&mtx);
    {
        SkeletonAnimationModel* m = self->skelAnimModel[2];
        m->unk_24 = mtx.data[0][3];
        m->unk_28 = mtx.data[1][3];
        m->unk_2C = mtx.data[2][3];
    }
    self->skelAnimModel[2]->unk_AD[0] = 1;
    self->skelAnimModel[2]->FUN_00372170(1);

    }
    self->skelAnimModel[0]->unk_0C->animSpeed = speed;
    for (i = 0; i < *(s32*)((u8*)*(void**)*(void**)((u8*)self->cmbData + 8) + 8); i++) {
        f32 color[4];
        AnimatedMaterial* mat = self->skelAnimModel[0]->unk_10;
        FUN_00333abc(mat, i, color);
        color[3] = self->energyAlpha;
        FUN_00333a38(mat, i, color);
        ((u8*)((void**)mat)[1])[i * 292] = 1;
    }
    self->skelAnimModel[0]->SetUnkAC(1);
    self->skelAnimModel[0]->FUN_003721e0(&thisx->modelMtx);
    {
        SkeletonAnimationModel* m = self->skelAnimModel[0];
        m->unk_24 = thisx->modelMtx.data[0][3];
        m->unk_28 = thisx->modelMtx.data[1][3];
        m->unk_2C = thisx->modelMtx.data[2][3];
    }
    self->skelAnimModel[0]->unk_AD[0] = 1;
    self->skelAnimModel[0]->FUN_00372170(0);
}
#endif
GLOBAL_ASM("asm/DemoKekkai_DrawTrialBarrier.s")

#define R_UPDATE_RATE SREG(30)
#define FRAMES(n) ((n) * 3.0f / R_UPDATE_RATE + 0.5f)

void FUN_003674e4(s32 seqId);

void DemoKekkai_TowerBarrier(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;

    if ((play->GetCutsceneState() != 0) && (play->csCtx.npcActions[0] != NULL) &&
        (*(u16*)play->csCtx.npcActions[0] != 1) && (*(u16*)play->csCtx.npcActions[0] == 2)) {
        if (!(self->sfxFlag & 1)) {
            FUN_003674e4(0xC);
            {
                u16 flags = self->sfxFlag | 1;
                self->sfxFlag = flags;
            }
        }
        if (self->barrierScrollRate < 7.0f) {
            self->barrierScrollRate += (R_UPDATE_RATE * 0.2f) / 3.0f;
        } else {
            self->timer++;
            if (self->timer > (s32)FRAMES(100)) {
                Flags_SetEventChkInf(0xC3);
                Actor_Kill(&self->actor);
                return;
            } else if (self->timer > (s32)FRAMES(40)) {
                self->actor.scale.z = self->actor.scale.x += (R_UPDATE_RATE * 0.003f) / 3.0f;
            }
        }
    }
    if (!(self->sfxFlag & 1)) {
        FUN_00373264(&self->actor, 0x01000245);
    }
}

void FUN_0028fa74(Actor* thisx, PlayState* play);

static inline u8* DemoKekkai_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

void DemoKekkai_TrialBarrierIdle(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;

    if (self->collider1.base.atFlags & AT_HIT) {
        FUN_00374bb8(play, &self->actor, 5.0f, self->actor.yawTowardsPlayer, 5.0f);
    }
    CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider1.base);
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider1.base);
    if (self->collider2.base.acFlags & AC_HIT) {
        Collider* hit = *(Collider**)((u8*)&self->collider2.info + 0x1C);
        if (hit != NULL && hit->actor != NULL && hit->actor->id == 0x16) {
            *(u8*)((u8*)hit->actor + 0x230) = 0;
        }
        FUN_00372244(&play->unk_5FCC, 0x1E, 0x01000487);
        thisx->update = (ActorFunc)FUN_0028fa74;
        self->timer = 0;
        {
            void* zar = (u8*)DemoKekkai_GetObjStatus(play, (u8)self->actor.objBankIndex) + 0x10;
            s16 p = self->actor.params;
            void** tbl = (void**)0x004D87B4;
            play->FUN_0037573c(((ZARInfo*)zar)->GetQDBByIndex((s32)tbl[p]));
        }
        gSaveContext.cutsceneTrigger = 1;
    }
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider2.base);
    FUN_00373264(&self->actor, 0x01000244);
}

// FUN_0028fa74 (N64 DemoKekkai_TrialBarrierDispel, lives in functions_280000s.cpp as GLOBAL_ASM): draft, far (~60+ insns of
// register numbering differ: ROM keeps self in r5 AND thisx in sl, play in fp, loads csCtx.frames via `add r1,play,#0x2200; ldrh [r1,#0xb8]`).
// Two guarded function-local statics (vel guard 0x51C3B8 obj 0x589018, accel guard 0x51C3B4 obj 0x589024, 12 B each); the
// colors/table are EQUs in data_syms.s (DemoKekkai_sLightYellow/sDarkRed/sCsFrames).
#if 0
void FUN_003674e4(s32 seqId);
void FUN_00373264(Actor* actor, s32 arg1);
void FUN_00359cf0(PlayState* play, Vec3f* pos, Vec3f* vel, Vec3f* accel, void* primColor, void* envColor, s32 scale, s32 life);
extern u16 DemoKekkai_sCsFrames[];
extern u32 DemoKekkai_sLightYellow;
extern u32 DemoKekkai_sDarkRed;
#define R_UPDATE_RATE SREG(30)
void FUN_0028fa74(Actor* thisx, PlayState* play) {
    DemoKekkai* self = (DemoKekkai*)thisx;
    Vec3f pos;
    s32 i;

    if (play->csCtx.frames == DemoKekkai_sCsFrames[self->actor.params]) {
        FUN_003674e4(0xA);
    }
    if (self->energyAlpha >= R_UPDATE_RATE * 0.05f / 3.0f) {
        self->energyAlpha -= R_UPDATE_RATE * 0.05f / 3.0f;
    } else {
        self->energyAlpha = 0.0f;
    }
    if (self->timer < (s32)(120.0f / R_UPDATE_RATE + 0.5f)) {
        self->orbScale = (f32)self->timer / (f32)(s32)(120.0f / R_UPDATE_RATE + 0.5f) + 1.0f;
    } else if (self->timer < (s32)(150.0f / R_UPDATE_RATE + 0.5f)) {
        self->orbScale = 2.0f;
    } else if (self->timer == (s32)(150.0f / R_UPDATE_RATE + 0.5f)) {
        static Vector3f vel(0.0f, 0.0f, 0.0f);
        static Vector3f accel(0.0f, 0.0f, 0.0f);

        Audio_PlayActorSound2(&self->actor, 0x01000134);
        for (i = 0; i < 85; i++) {
            s16 roll = Rand_ZeroFloat(65535.0f);
            s16 yaw = Rand_ZeroFloat(65535.0f);

            vel.x = Math_SinS(yaw) * Math_CosS(roll) * Rand_ZeroFloat(8.0f);
            vel.z = Math_CosS(yaw) * Math_CosS(roll) * Rand_ZeroFloat(8.0f);
            vel.y = Math_SinS(roll) * Rand_ZeroFloat(3.0f);

            pos.x = (vel.x * 7.0f) + self->actor.world.pos.x;
            pos.y = (vel.y * 20.0f) + self->actor.world.pos.y + 120.0f;
            pos.z = (vel.z * 7.0f) + self->actor.world.pos.z;

            FUN_00359cf0(play, &pos, (Vec3f*)&vel, (Vec3f*)&accel, &DemoKekkai_sLightYellow, &DemoKekkai_sDarkRed, 3000,
                         (s32)Rand_ZeroFloat(40.0f) + 45);
        }
    } else {
        self->orbScale = 0.0f;
    }
    if (self->orbScale != 0.0f) {
        FUN_00373264(&self->actor, 0x01000244);
    }
    self->timer++;
}
#endif


}
