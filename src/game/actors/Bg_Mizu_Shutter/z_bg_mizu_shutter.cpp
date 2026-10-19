extern "C" {
#include "z_bg_mizu_shutter.hpp"

#define FLAGS 0x80000010

void BgMizuShutter_Init(Actor* actor, GameState* state);
void BgMizuShutter_Destroy(Actor* actor, GameState* state);
void BgMizuShutter_Update(Actor* actor, GameState* state);
void BgMizuShutter_Draw(Actor* actor, GameState* state);

void BgMizuShutter_Move(BgMizuShutter* self, PlayState* play);
void BgMizuShutter_WaitForSwitch(BgMizuShutter* self, PlayState* play);
void BgMizuShutter_WaitForCutscene(BgMizuShutter* self, PlayState* play);
void BgMizuShutter_WaitForTimer(BgMizuShutter* self, PlayState* play);
void FUN_0034a80c(MTX34* dst, MTX34* src);
f32 sinf(f32 x);
f32 cosf(f32 x);
s32 FUN_0032d870(Vec3f* pos, f32 cx, f32 cy, f32 cz, f32 hx, f32 hy, f32 hz);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);
s32 OnePointCutscene_Attention(PlayState* play, Actor* actor, s32 arg2, s32 arg3);
void FUN_0036d940(Actor* actor, s32 timer);

GLOBAL_ASM("data/z_Bg_Mizu_Shutter.data.s")

GLOBAL_ASM("../src/game/actors/Bg_Mizu_Shutter/data_syms.s")

// sModelIdx / sCollIdx / sDisplacements / sInitChain live in the data dump above (EQU symbols, no scatter symbols).
extern s32 BgMizuShutter_sModelIdx[];
extern s32 BgMizuShutter_sCollIdx[];
extern Vec3f BgMizuShutter_sDisplacements[];
extern InitChainEntry BgMizuShutter_sInitChain[];
#define sModelIdx BgMizuShutter_sModelIdx
#define sCollIdx BgMizuShutter_sCollIdx
#define sDisplacements BgMizuShutter_sDisplacements
#define sInitChain BgMizuShutter_sInitChain

// 3DS Matrix_RotateY/X/Z (mode 1 = apply to mtx, otherwise replace), inlined at the call site.
// N64 Matrix_MultVec3f(src, dest) argument order; gives the ROM argument setup order.
static inline void BgMizuShutter_MultVec3f(Vector3f* src, Vector3f* dest, MTX34* mtx) {
    VEC3TransformAsm(dest, mtx, src);
}

static inline void BgMizuShutter_RotY(MTX34* m, s32 mode, f32 angle) {
    f32 sinA;
    f32 cosA;

    if (mode == 1) {
        if (angle != 0.0f) {
            sinA = sinf(angle);
            cosA = cosf(angle);
            for (s32 r = 0; r < 3; r++) {
                f32 a = m->data[r][0];
                f32 b = m->data[r][2];

                m->data[r][0] = a * cosA - b * sinA;
                m->data[r][2] = a * sinA + b * cosA;
            }
        }
    } else {
        if (angle == 0.0f) {
            sinA = 0.0f;
            cosA = 1.0f;
        } else {
            sinA = sinf(angle);
            cosA = cosf(angle);
        }
        m->data[0][0] = cosA;
        m->data[0][1] = 0.0f;
        m->data[0][2] = sinA;
        m->data[0][3] = 0.0f;
        m->data[1][1] = 1.0f;
        m->data[1][0] = 0.0f;
        m->data[1][2] = 0.0f;
        m->data[1][3] = 0.0f;
        m->data[2][0] = -sinA;
        m->data[2][1] = 0.0f;
        m->data[2][2] = cosA;
        m->data[2][3] = 0.0f;
    }
}

static inline void BgMizuShutter_RotX(MTX34* m, s32 mode, f32 angle) {
    f32 sinA;
    f32 cosA;

    if (mode == 1) {
        if (angle != 0.0f) {
            sinA = sinf(angle);
            cosA = cosf(angle);
            for (s32 r = 0; r < 3; r++) {
                f32 a = m->data[r][1];
                f32 b = m->data[r][2];

                m->data[r][1] = a * cosA + b * sinA;
                m->data[r][2] = b * cosA - a * sinA;
            }
        }
    }
}

static inline void BgMizuShutter_RotZ(MTX34* m, s32 mode, f32 angle) {
    f32 sinA;
    f32 cosA;

    if (mode == 1) {
        if (angle != 0.0f) {
            sinA = sinf(angle);
            cosA = cosf(angle);
            for (s32 r = 0; r < 3; r++) {
                f32 a = m->data[r][0];
                f32 b = m->data[r][1];

                m->data[r][0] = a * cosA + b * sinA;
                m->data[r][1] = b * cosA - a * sinA;
            }
        }
    }
}

ActorInit Bg_Mizu_Shutter_InitVars = {
    ACTOR_BG_MIZU_SHUTTER,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_MIZU_OBJECTS,
    sizeof(BgMizuShutter),
    (ActorFunc)BgMizuShutter_Init,
    (ActorFunc)BgMizuShutter_Destroy,
    (ActorFunc)BgMizuShutter_Update,
    (ActorFunc)BgMizuShutter_Draw,
};

void BgMizuShutter_Init(Actor* actor, GameState* state) {
    BgMizuShutter* self = (BgMizuShutter*)actor;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;

    Actor_ProcessInitChain(actor, sInitChain);
    FUN_00372f38(actor, play, &self->skelAnimModel, sModelIdx[(u16)actor->params >> 12], 0);
    DynaPolyActor_Init(&self->dyna, 1);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, sCollIdx[(u16)actor->params >> 12]), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);

    s32 size = (u16)actor->params >> 12;
    if (size == 0 || size == 1) {
        MTX34 mtx;

        self->closedPos = actor->world.pos;
        self->timer = 0;
        self->timerMax = ((actor->params >> 6) & 0x3F) * 30;

        MTX34CopyAsm(&mtx, &actor->modelMtx);
        BgMizuShutter_RotY(&mtx, 0, actor->world.rot.y * (2.0f * 3.14159274f / 65536.0f));
        BgMizuShutter_RotX(&mtx, 1, actor->world.rot.x * (2.0f * 3.14159274f / 65536.0f));
        BgMizuShutter_RotZ(&mtx, 1, actor->world.rot.z * (2.0f * 3.14159274f / 65536.0f));
        BgMizuShutter_MultVec3f((Vector3f*)&sDisplacements[(u16)actor->params >> 12], (Vector3f*)&self->openPos, &mtx);
        self->openPos.x += actor->world.pos.x;
        self->openPos.y += actor->world.pos.y;
        self->openPos.z += actor->world.pos.z;

        if (self->timerMax != 0x3F * 30) {
            Flags_UnsetSwitch(play, actor->params & 0x3F);
            actor->world.pos = self->closedPos;
        }
        if (Flags_GetSwitch(play, actor->params & 0x3F)) {
            actor->world.pos = self->openPos;
            self->actionFunc = BgMizuShutter_WaitForTimer;
        } else {
            self->actionFunc = BgMizuShutter_WaitForSwitch;
        }
    }
}

void BgMizuShutter_Destroy(Actor* actor, GameState* state) {
    BgMizuShutter* self = (BgMizuShutter*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    FUN_00350f34(actor, &self->skelAnimModel, 0);
}

void BgMizuShutter_Update(Actor* actor, GameState* state) {
    BgMizuShutter* self = (BgMizuShutter*)actor;

    self->actionFunc(self, (PlayState*)state);
}

void BgMizuShutter_Draw(Actor* actor, GameState* state) {
    BgMizuShutter* self = (BgMizuShutter*)actor;
    MTX34 modelMtx;

    MTX34CopyAsm(&modelMtx, &actor->modelMtx);
    if (self->skelAnimModel != NULL) {
        self->skelAnimModel->SetUnkAC(1);
        self->skelAnimModel->FUN_003721e0(&modelMtx);
        self->unk_1E8 = 1;
        self->skelAnimModel->FUN_00372170(0);
    }
}

// BgMizuShutter_Move: draft, 30 words off (size matches; the pos==target compare chains get if-converted (vldreq/vcmpeq) where the ROM
// uses plain bne branches; `Vector3f lim(...)` + `-lim.x` fixed the player-push block, see WaitForSwitch). See STATUS.md.
#if 0
void BgMizuShutter_Move(BgMizuShutter* self, PlayState* play) {
    f32 minStep;
    f32 step;
    f32 one;

    if (Flags_GetSwitch(play, self->dyna.actor.params & 0x3F)) {
        minStep = 0.1f;
        one = 1.0f;
        step = 4.0f;
        Math_SmoothStepToF(&self->dyna.actor.world.pos.x, self->openPos.x, one, step, minStep);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->openPos.y, one, step, minStep);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.z, self->openPos.z, one, step, minStep);
        {
            Vector3f* p = (Vector3f*)&self->dyna.actor.world.pos;
            Vector3f* o = (Vector3f*)&self->openPos;

            if (p->x != o->x || p->y != o->y || p->z != o->z) {
                return;
            }
        }
        self->timer = self->timerMax;
        self->actionFunc = BgMizuShutter_WaitForTimer;
    } else {
        Math_SmoothStepToF(&self->maxSpeed, 20.0f, 1.0f, 3.0f, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.x, self->closedPos.x, 1.0f, self->maxSpeed, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->closedPos.y, 1.0f, self->maxSpeed, 0.1f);
        Math_SmoothStepToF(&self->dyna.actor.world.pos.z, self->closedPos.z, 1.0f, self->maxSpeed, 0.1f);
        {
            Vector3f* p = (Vector3f*)&self->dyna.actor.world.pos;
            Vector3f* c = (Vector3f*)&self->closedPos;

            if (p->x == c->x && p->y == c->y && p->z == c->z) {
                Audio_PlayActorSound2(&self->dyna.actor, 0x01000174); // NA_SE_EV_STONE_BOUND
                self->actionFunc = BgMizuShutter_WaitForSwitch;
            }
        }
        {
            Player* player = GET_PLAYER(play);

            if ((self->unk_1E8 != 0) && (self->dyna.actor.shape.rot.x == 0)) {
                MTX34 mtx;
                Vec3f local;
                Vector3f lim(500.0f, 800.0f, 100.0f);

                self->unk_1E8 = 0;
                FUN_0034a80c(&mtx, &self->dyna.actor.modelMtx);
                VEC3TransformAsm((Vector3f*)&local, &mtx, (Vector3f*)&player->actor.world.pos);
                if (local.x < lim.x && local.x > -lim.x && local.y < lim.y && local.y >= 0.0f && local.z < lim.z &&
                    local.z > -lim.z) {
                    player->pushedYaw = self->dyna.actor.shape.rot.y;
                    if (local.z >= 0.0f) {
                        player->pushedSpeed = 1.0f;
                    } else {
                        player->pushedSpeed = -1.0f;
                    }
                }
            }
        }
    }
}
#endif

GLOBAL_ASM("asm/BgMizuShutter_Move.s")

void BgMizuShutter_WaitForCutscene(BgMizuShutter* self, PlayState* play) {
    if (self->timer-- == 0) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x010001B0); // NA_SE_EV_METALDOOR_OPEN
        self->actionFunc = BgMizuShutter_Move;
    }
}

void BgMizuShutter_WaitForSwitch(BgMizuShutter* self, PlayState* play) {
    if (Flags_GetSwitch(play, self->dyna.actor.params & 0x3F)) {
        if (ABS(self->dyna.actor.world.rot.x) > 0x2C60) {
            if (gSaveContext.masterQuestFlag != 0) {
                s32 csId = 4510;
                if (play->sceneNum == 6) {
                    if (self->dyna.actor.params == 0xFFE) {
                        OnePointCutscene_Init(play, csId + 1, -99, &self->dyna.actor, 0);
                        goto done;
                    }
                } else if ((play->sceneNum == 5) && (((u8*)play)[0x4C30] == 7)) {
                    Vec3f* pos = &GET_PLAYER(play)->actor.world.pos;

                    if (FUN_0032d870(pos, -3120.0f, 1060.0f, -4290.0f, 280.0f, 10.0f, 20.0f) ||
                        FUN_0032d870(pos, -3125.0f, 1060.0f, 4210.0f, 85.0f, 10.0f, 70.0f)) {
                        pos->x = -3200.0f;
                        pos->y = 1060.0f;
                        pos->z = -4220.0f;
                    }
                }
            }
            OnePointCutscene_Init(play, 4510, -99, &self->dyna.actor, 0);
        } else {
            OnePointCutscene_Attention(play, &self->dyna.actor, 0, 0);
        }
    done:
        self->actionFunc = BgMizuShutter_WaitForCutscene;
        self->timer = 45;
    }
    {
        Player* player = GET_PLAYER(play);

        if ((self->unk_1E8 != 0) && (self->dyna.actor.shape.rot.x == 0)) {
            MTX34 mtx;
            Vec3f local;
            Vector3f lim(500.0f, 800.0f, 100.0f);

            self->unk_1E8 = 0;
            FUN_0034a80c(&mtx, &self->dyna.actor.modelMtx);
            VEC3TransformAsm((Vector3f*)&local, &mtx, (Vector3f*)&player->actor.world.pos);
            if (local.x < lim.x && local.x > -lim.x && local.y < lim.y && local.y >= 0.0f && local.z < lim.z &&
                local.z > -lim.z) {
                player->pushedYaw = self->dyna.actor.shape.rot.y;
                if (local.z >= 0.0f) {
                    player->pushedSpeed = 1.0f;
                } else {
                    player->pushedSpeed = -1.0f;
                }
            }
        }
    }
}


void BgMizuShutter_WaitForTimer(BgMizuShutter* self, PlayState* play) {
    if (self->timerMax != 0x3F * 30) {
        self->timer--;
        FUN_0036d940(&self->dyna.actor, self->timer);
        if (self->timer == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x010001B1); // NA_SE_EV_METALDOOR_CLOSE
            Flags_UnsetSwitch(play, self->dyna.actor.params & 0x3F);
            self->actionFunc = BgMizuShutter_Move;
        }
    }
}
}
