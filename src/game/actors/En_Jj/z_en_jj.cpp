extern "C" {
#include "z_en_jj.hpp"

#define FLAGS 0x00000030
#define R_UPDATE_RATE SREG(30)

void EnJj_Init(Actor* actor, GameState* state);
void EnJj_Destroy(Actor* actor, PlayState* play);
void EnJj_Update(Actor* actor, GameState* state);
void EnJj_Draw(Actor* actor, GameState* state);
void EnJj_OpenMouth(EnJj* self, PlayState* play);
void EnJj_RemoveDust(EnJj* self, PlayState* play);
void EnJj_BeginCutscene(EnJj* self, PlayState* play);
void EnJj_UpdateStaticCollision(Actor* thisx, PlayState* play);
void EnJj_WaitToOpenMouth(EnJj* self, PlayState* play);
void EnJj_WaitForFish(EnJj* self, PlayState* play);
void FUN_0036ae48(Camera* camera);
void FUN_0034f6bc(PlayState* play, DynaCollisionContext* dyna, s32 bgId);
void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);
void Animation_PlayLoop(SkelAnime* skelAnime, s32 idx);

extern InitChainEntry EnJj_sInitChain[];
extern ColliderCylinderInit EnJj_sCylinderInit;

static inline void EnJj_SetupAction(EnJj* self, EnJjActionFunc actionFunc) {
    self->actionFunc = actionFunc;
}

// N64 setup helper taking Actor* (casts inside): needed for the Init action-func store shape.
static inline void EnJj_SetupActionT(Actor* thisx, EnJjActionFunc actionFunc) {
    ((EnJj*)thisx)->actionFunc = actionFunc;
}

static inline u8* EnJj_GetObjStatus(PlayState* play, s32 idx) {
    if ((u32)idx < 0x13 && *(s32*)((u8*)play + (idx << 7) + 0x3A64) != 0) {
        return (u8*)play + (idx << 7) + 0x3A5C;
    }
    return NULL;
}

GLOBAL_ASM("data/z_En_Jj.data.s")
GLOBAL_ASM("../src/game/actors/En_Jj/data_syms.s")

ActorInit En_Jj_InitVars = {
    ACTOR_EN_JJ,
    ACTORCAT_ITEMACTION,
    FLAGS,
    OBJECT_JJ,
    sizeof(EnJj),
    (ActorFunc)EnJj_Init,
    (ActorFunc)EnJj_Destroy,
    (ActorFunc)EnJj_Update,
    (ActorFunc)EnJj_Draw,
};

void EnJj_Init(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    EnJj* self = (EnJj*)thisx;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;

    Actor_ProcessInitChain(&self->dyna.actor, EnJj_sInitChain);
    ActorShape_Init(&self->dyna.actor.shape, 0.0f, NULL, 0.0f);
    zar = (ZARInfo*)((u8*)EnJj_GetObjStatus(play, (u8)self->dyna.actor.objBankIndex) + 0x10);

    switch (self->dyna.actor.params) {
        case JABUJABU_MAIN:
            FUN_0034fe20(&self->dyna.actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 12);
            Animation_PlayLoop(&self->skelAnime, 0);
            SkelAnime_SetFaceAnimations((FaceAnimations*)self->unk_240, &self->skelAnime, 0, -1, -1);
            self->flags = 0;
            self->eyeIndex = 0;
            self->blinkTimer = 0;
            self->extraBlinkCounter = 0;
            self->extraBlinkTotal = 0;

            if (gSaveContext.eventChkInf[3] & 0x400) {
                EnJj_SetupActionT(thisx, EnJj_WaitToOpenMouth);
            } else {
                EnJj_SetupActionT(thisx, EnJj_WaitForFish);
            }

            {
                PlayState* play2 = play;
                self->bodyCollisionActor = (DynaPolyActor*)Actor_SpawnAsChild(
                    &play2->actorCtx, &self->dyna.actor, play2, 0x5A, self->dyna.actor.world.pos.x - 10.0f,
                    self->dyna.actor.world.pos.y, self->dyna.actor.world.pos.z, 0, self->dyna.actor.world.rot.y, 0,
                    JABUJABU_COLLISION);
            }
            DynaPolyActor_Init(&self->dyna, 0);
            ReadCollisionHeaderFromZsi(zar->FUN_003532c0(0), &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->dyna.actor, &EnJj_sCylinderInit);
            self->dyna.actor.colChkInfo.mass = 0xFF;
            break;

        case JABUJABU_COLLISION:
            DynaPolyActor_Init(&self->dyna, 0);
            ReadCollisionHeaderFromZsi(zar->FUN_003532c0(1), &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
            FUN_0034f6bc(play, &play->colCtx.dyna, self->dyna.bgId);
            self->dyna.actor.update = (ActorFunc)EnJj_UpdateStaticCollision;
            self->dyna.actor.draw = NULL;
            Actor_SetScale(&self->dyna.actor, 0.087f);
            break;

        case JABUJABU_UNUSED_COLLISION:
            DynaPolyActor_Init(&self->dyna, 0);
            ReadCollisionHeaderFromZsi(zar->FUN_003532c0(2), &colHeader);
            self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, &self->dyna.actor, colHeader);
            self->dyna.actor.update = (ActorFunc)EnJj_UpdateStaticCollision;
            self->dyna.actor.draw = NULL;
            Actor_SetScale(&self->dyna.actor, 0.087f);
            break;
    }
}

void EnJj_Destroy(Actor* thisx, PlayState* play) {
    EnJj* self = (EnJj*)thisx;

    switch (self->dyna.actor.params) {
        case JABUJABU_MAIN:
            DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
            Collider_DestroyCylinder(play, &self->collider);
            SkelAnime_Free(&self->skelAnime, play);
            ((FaceAnimations*)self->unk_240)->Destroy();
            break;

        case JABUJABU_COLLISION:
        case JABUJABU_UNUSED_COLLISION:
            DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
            break;
    }
}

// 43 words off (fastchk): the cutscene/blink/jointTable body matches in shape; remaining diffs are (a) play/zero
// callee-saved numbering (ROM play=r6, zero=r7) + ROM hoists `mov r1,#1` to entry, and (b) the identity+translate
// MTX34 tail: ROM re-materialises 0 / 0x3F800000 in r0/r1 instead of reusing the function-wide zero in r7.
// Draft needs MTX34_MakeTrans (mtx34_trans.hpp), sinf/cosf protos, FUN_003731e0 and the real header (see ws STATUS).
#if 0
static void EnJj_Blink(EnJj* self) {
    if (self->blinkTimer > 0) {
        self->blinkTimer--;
    } else {
        self->eyeIndex++;
        if (self->eyeIndex >= 4) {
            self->eyeIndex = 0;
            if (self->extraBlinkCounter > 0) {
                self->extraBlinkCounter--;
            } else {
                self->blinkTimer = (s32)((u8)Rand_S16Offset_003702c8(20, 20) * 3.0f / R_UPDATE_RATE + 0.5f);
                self->extraBlinkCounter = self->extraBlinkTotal;
            }
        }
    }
}

static void EnJj_CutsceneUpdate(EnJj* self, PlayState* play) {
    switch (play->csCtx.npcActions[2]->action) {
        case 1:
            if (self->flags & 2) {
                self->eyeIndex = 0;
                self->blinkTimer = Rand_S16Offset_003702c8(20, 20);
                self->extraBlinkCounter = 0;
                self->extraBlinkTotal = 0;
                self->flags ^= 2;
            }
            break;

        case 2:
            self->flags |= 1;

            if (!(self->flags & 8)) {
                self->dust = Actor_SpawnAsChild(&play->actorCtx, &self->dyna.actor, play, 0x101, -1100.0f, 105.0f,
                                                -27.0f, 0, 0, 0, 0);
                self->flags |= 8;
            }
            break;

        case 3:
            if (!(self->flags & 2)) {
                self->eyeIndex = 0;
                self->blinkTimer = 0;
                self->extraBlinkCounter = 1;
                self->extraBlinkTotal = 0;
                self->flags |= 2;
            }
            break;
    }

    if (self->flags & 1) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001C4);

        if (self->mouthOpenAngle >= -5200) {
            self->mouthOpenAngle -= (s32)(((f32)R_UPDATE_RATE * 102.0f) / 3.0f + 0.5f);
        }
    }
}

void EnJj_Update(Actor* thisx, GameState* state) {
    EnJj* self = (EnJj*)thisx;
    PlayState* play = (PlayState*)state;
    MTX34* mtx;
    f32 angle;
    f32 s;
    f32 c;
    f32 a;
    f32 b;

    if ((play->GetCutsceneState() != 0) && (play->csCtx.npcActions[2] != NULL)) {
        EnJj_CutsceneUpdate(self, play);
    } else {
        self->actionFunc(self, play);

        if (self->skelAnime.curFrame == 41.0f) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100020D);
        }
    }

    EnJj_Blink(self);
    FUN_003731e0(&self->skelAnime);
    Actor_SetScale(&self->dyna.actor, 0.087f);

    mtx = (MTX34*)((u8*)self->skelAnime.jointTable + 0x68);
    {
        Vec3f t;
        t.x = mtx->data[0][3];
        t.y = mtx->data[1][3];
        t.z = mtx->data[2][3];
        MTX34_MakeTrans(mtx, &t);
    }
    angle = self->mouthOpenAngle * 9.58737992e-5f;
    if (angle != 0.0f) {
        s = __hardfp_sinf(angle);
        c = __hardfp_cosf(angle);
        a = mtx->data[0][0];
        b = mtx->data[0][1];
        mtx->data[0][0] = a * c + b * s;
        mtx->data[0][1] = b * c - a * s;
        a = mtx->data[1][0];
        b = mtx->data[1][1];
        mtx->data[1][0] = a * c + b * s;
        mtx->data[1][1] = b * c - a * s;
        a = mtx->data[2][0];
        b = mtx->data[2][1];
        mtx->data[2][0] = a * c + b * s;
        mtx->data[2][1] = b * c - a * s;
    }
}
#endif

GLOBAL_ASM("asm/EnJj_Update.s")

// 41 words off (fastchk): frame/layout match; remaining: callee-saved VFP numbering (ROM s16=0.0,s17=copy,s18=10.0),
// ROM copies the 4-word eyeTextures table through r0 -> r5 (ours r1 -> r0), no kept FaceAnimations alias, and the
// scale block uses 3 copies of s18. EnJj_sEyeTextures = rodata 0x004D9798 {0,1,2,1} (needs EQU in data_syms.s).
#if 0
f32 __hardfp_cosf(f32 x);
typedef struct { s32 v[4]; } __attribute__((aligned(4))) EnJjEyeTex;
extern EnJjEyeTex EnJj_sEyeTextures;
struct JjVec {
    f32& operator[](s32 i) { return (&x)[i]; }
    f32 x, y, z;
};
static inline void JjTranslate(MTX34* mtx, f32 x, f32 y, f32 z) {
    JjVec pos;
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    Matrix_UpdatePosition(mtx, mtx, (Vec3f*)&pos);
}
void EnJj_Draw(Actor* thisx, GameState* state) {
    EnJj* self = (EnJj*)thisx;
    EnJjEyeTex eyeTextures = EnJj_sEyeTextures;
    MTX34 mtx;
    FaceAnimations* fa = (FaceAnimations*)self->unk_240;

    fa->FUN_0035e3a4(0, eyeTextures.v[self->eyeIndex]);
    fa->FUN_0035e330();

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    JjTranslate(&mtx, 0.0f, 10.0f - __hardfp_cosf(self->skelAnime.curFrame * 0.076624215f) * 10.0f, 0.0f);
    {
        f32 sx = 10.0f;
        f32 sy = 10.0f;
        f32 sz = 10.0f;

        mtx.data[0][0] *= sx;
        mtx.data[1][0] *= sx;
        mtx.data[2][0] *= sx;
        mtx.data[0][1] *= sy;
        mtx.data[1][1] *= sy;
        mtx.data[2][1] *= sy;
        mtx.data[0][2] *= sz;
        mtx.data[1][2] *= sz;
        mtx.data[2][2] *= sz;
    }
    SkelAnime_DrawOpa(&self->skelAnime, &mtx, NULL, NULL, thisx, 0);
}
#endif

GLOBAL_ASM("asm/EnJj_Draw.s")

void EnJj_BeginCutscene(EnJj* self, PlayState* play) {
    EnJj* s2 = self;
    DynaPolyActor* bodyCollisionActor = self->bodyCollisionActor;

    if (self->cutsceneCountdownTimer > 0) {
        self->cutsceneCountdownTimer--;
    } else {
        EnJj_SetupAction(s2, EnJj_RemoveDust);
        {
            void* zar = (u8*)EnJj_GetObjStatus(play, (u8)s2->dyna.actor.objBankIndex) + 0x10;
            play->FUN_0037573c(((ZARInfo*)zar)->GetQDBByIndex(0));
        }
        gSaveContext.cutsceneTrigger = 1;
        FUN_0036b940(play, &play->colCtx.dyna, bodyCollisionActor->bgId);
        FUN_0036ae48(play->cameraPtrs[play->activeCamera]);
        gSaveContext.eventChkInf[3] |= 0x400;
        FUN_00372244((u8*)play + 0x5FCC, 0x1E, 0x01000487);
    }
}

void EnJj_OpenMouth(EnJj* self, PlayState* play) {
    DynaPolyActor* bodyCollisionActor = self->bodyCollisionActor;

    if (self->mouthOpenAngle >= -5200) {
        self->mouthOpenAngle -= (s32)(((f32)R_UPDATE_RATE * 102.0f) / 3.0f + 0.5f);

        if (self->mouthOpenAngle < -2600) {
            FUN_0036b940(play, &play->colCtx.dyna, bodyCollisionActor->bgId);
        }
    }
}

void EnJj_RemoveDust(EnJj* self, PlayState* play) {
    EnJj* s2 = self;

    if (!(self->flags & 4)) {
        self->flags |= 4;

        if (self->dust != NULL) {
            Actor_Kill(self->dust);
            s2->dyna.actor.child = NULL;
        }
    }
}

void EnJj_UpdateStaticCollision(Actor* thisx, PlayState* play) {
}

// Guarded function-local static Vector3f feedingSpot (guard 0x52891C = first word of z_En_Jj.data, object 0x59B05C,
// 12 bytes): body matches 83/87 words with the real function-local static; the 4 left words are VFP numbering of the
// ctor init (ROM s0,s1,s2 = x,y,z; ours s0,s2,s1). Orchestrator pins the guard/object at merge.
#if 0
void EnJj_WaitForFish(EnJj* self, PlayState* play) {
    static Vector3f feedingSpot(-1589.0f, 53.0f, -43.0f);
    Player* player = GET_PLAYER(play);

    if ((Math_Vec3f_DistXZ((Vec3f*)&feedingSpot, &player->actor.world.pos) < 300.0f) &&
        (*(s32 (**)(PlayState*))((u8*)play + 0x5B98))(play)) {
        self->cutsceneCountdownTimer = (s16)(300.0f / R_UPDATE_RATE + 0.5f);
        EnJj_SetupAction(self, EnJj_BeginCutscene);
    }

    self->collider.dim.pos.x = -1245.0f;
    self->collider.dim.pos.y = 20.0f;
    self->collider.dim.pos.z = -48.0f;
    CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
}
#endif

GLOBAL_ASM("asm/EnJj_WaitForFish.s")

void EnJj_WaitToOpenMouth(EnJj* self, PlayState* play) {
    if (self->dyna.actor.xzDistToPlayer < 300.0f) {
        EnJj_SetupAction(self, EnJj_OpenMouth);
    }
}

}
