extern "C" {
#include "z_bg_jya_megami.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x80400000

void BgJyaMegami_Init(Actor* actor, GameState* state);
void BgJyaMegami_Destroy(Actor* actor, GameState* state);
void BgJyaMegami_Update(Actor* actor, GameState* state);
void BgJyaMegami_Draw(Actor* actor, GameState* state);

void BgJyaMegami_DetectLight(BgJyaMegami* self, PlayState* play);

void Collider_InitJntSph(PlayState* play, BgJyaMegamiColliderJntSph* collider);
void Collider_SetJntSph(PlayState* play, BgJyaMegamiColliderJntSph* collider, Actor* actor,
                        BgJyaMegamiJntSphInit* init, BgJyaMegamiColliderElement* elements);
void Collider_DestroyJntSph(PlayState* play, BgJyaMegamiColliderJntSph* collider);

extern BgJyaMegamiJntSphInit BgJyaMegami_sJntSphInit;
extern InitChainEntry BgJyaMegami_sInitChain[];
typedef struct BgJyaMegamiPieceInit {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ f32 velX;
    /* 0x10 */ s16 rotVelX;
    /* 0x12 */ s16 rotVelY;
    /* 0x14 */ s16 delay;
} BgJyaMegamiPieceInit; // size = 0x18

void Collider_UpdateSpheres(s32 index, BgJyaMegamiColliderJntSph* collider, MTX34* mtx);
f32 sinf(f32 x);
f32 cosf(f32 x);
void BgJyaMegami_Explode(BgJyaMegami* self, PlayState* play);

// 3DS Matrix_RotateY/X (apply mode) and Matrix_Scale (apply mode), inlined at the call site.
static inline void BgJyaMegami_RotY(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        f32 sinA = sinf(angle);
        f32 cosA = cosf(angle);

        for (s32 r = 0; r < 3; r++) {
            f32 a = m->data[r][0];
            f32 b = m->data[r][2];

            m->data[r][0] = a * cosA - b * sinA;
            m->data[r][2] = a * sinA + b * cosA;
        }
    }
}

static inline void BgJyaMegami_RotX(MTX34* m, f32 angle) {
    if (angle != 0.0f) {
        f32 sinA = sinf(angle);
        f32 cosA = cosf(angle);

        for (s32 r = 0; r < 3; r++) {
            f32 a = m->data[r][1];
            f32 b = m->data[r][2];

            m->data[r][1] = a * cosA + b * sinA;
            m->data[r][2] = b * cosA - a * sinA;
        }
    }
}

static inline void BgJyaMegami_Scale(MTX34* m, f32 x, f32 y, f32 z) {
    for (s32 r = 0; r < 3; r++) {
        m->data[r][0] *= x;
    }
    for (s32 r = 0; r < 3; r++) {
        m->data[r][1] *= y;
    }
    for (s32 r = 0; r < 3; r++) {
        m->data[r][2] *= z;
    }
}

// Data 0x512AE4..0x512C9C lives in this TU as real statics (see below); only the init chain tail stays in asm.
GLOBAL_ASM("../src/game/actors/Bg_Jya_Megami/z_Bg_Jya_Megami_data_tail.s")
GLOBAL_ASM("../src/game/actors/Bg_Jya_Megami/data_syms.s")

ActorInit Bg_Jya_Megami_InitVars = {
    ACTOR_BG_JYA_MEGAMI,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_JYA_OBJ,
    sizeof(BgJyaMegami),
    (ActorFunc)BgJyaMegami_Init,
    (ActorFunc)BgJyaMegami_Destroy,
    (ActorFunc)BgJyaMegami_Update,
    (ActorFunc)BgJyaMegami_Draw,
};

void BgJyaMegami_Init(Actor* actor, GameState* state) {
    BgJyaMegami* self = (BgJyaMegami*)actor;
    PlayState* play = (PlayState*)state;
    FUN_00372f38(actor, play, &self->models[0], 0x11, &self->models[1], 0x12, &self->models[2], 0x13,
                 &self->models[3], 0x14, &self->models[4], 0x15, &self->models[5], 0x16, &self->models[6], 0x17,
                 &self->models[7], 0x18, &self->models[8], 0x19, &self->models[9], 0x1A, &self->models[10], 0x1B,
                 &self->models[11], 0x1C, &self->models[12], 0x1D, &self->models[13], 0x1E, 0);
    {
        CollisionHeader* colHeader;
        void* zsi = DynaPolyInfo_Alloc(&self->dyna, state, 4);

        colHeader = NULL;
        DynaPolyActor_Init(&self->dyna, 0);
        ReadCollisionHeaderFromZsi(zsi, &colHeader);
        self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, actor, colHeader);
    }
    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSph(play, &self->collider, actor, &BgJyaMegami_sJntSphInit, self->colliderElements);
    if (Flags_GetSwitch(play, actor->params & 0x3F)) {
        Actor_Kill(actor);
    } else {
        Actor_ProcessInitChain(actor, BgJyaMegami_sInitChain);
        Actor_SetFocus(actor, -50.0f);
        self->actionFunc = BgJyaMegami_DetectLight;
        self->lightTimer = 0;
        self->crumbleIndex = 0;
        actor->world.pos.x = 60.5f;
        actor->world.pos.y = 1159.0f;
        actor->world.pos.z = -1437.0f;
        actor->unk_19B = 2;
    }
}

void BgJyaMegami_Destroy(Actor* actor, GameState* state) {
    BgJyaMegami* self = (BgJyaMegami*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->models[0], &self->models[1], &self->models[2], &self->models[3], &self->models[4],
                 &self->models[5], &self->models[6], &self->models[7], &self->models[8], &self->models[9],
                 &self->models[10], &self->models[11], &self->models[12], &self->models[13], 0);
}

void BgJyaMegami_Update(Actor* actor, GameState* state) {
    BgJyaMegami* self = (BgJyaMegami*)actor;
    PlayState* play = (PlayState*)state;

    self->actionFunc(self, play);
}

void FUN_0037378c(PlayState* play, Vec3f* arg1, f32 arg2, s32 arg3, s16 arg4, s16 arg5, u8 arg6);
void FUN_00373264(Actor* actor, s32 sfxId);
s32 OnePointCutscene_Init(PlayState* play, s32 csId, s32 duration, Actor* actor, s32 camId);

// Real statics: ROM derives all table bases from one literal (mask), so they must share the TU .data.
// Collider element init (0x30 bytes, referenced by sJntSphInit): non-static so it lands in the InitVars .data
// section right behind InitVars. The last two words (0x512B14/0x512B18) are the guard words of the function-local
// statics of DetectLight and Explode (debt: once BOTH are C, shrink this to [12]; the guards are then emitted at
// the start of the statics' .data section, see the #if 0 drafts and STATUS.md).
u32 BgJyaMegami_sJntSphElementsInit[14] = { 0x00000000, 0x00000000, 0x00000000, 0x00200000, 0x00000000,
                                            0x00000100, 0x00000000, 0x00000000, 0xC4160000, 0xC3480000,
                                            0x42700000, 0x00000064, 0x00000000, 0x00000000 };
__attribute__((used)) static s16 sMask[3] = { 1, 3, 7 };
__attribute__((used)) static u32 sJntSphInitC[4] = { 0x0009000A, 0, 1, 0x00512AE4 };
__attribute__((used)) static BgJyaMegamiPieceInit sPiecesInitC[13] = {
    { { -50.0f, -21.28f, -38.92f }, -1.6f, 0xFED4, 0xFE70, 40 },
    { { -4.32f, -13.44f, -13.76f }, 0.0f, 0x04B0, 0x0190, 42 },
    { { 30.92f, -20.480001f, -28.84f }, 1.6f, 0xFCE0, 0x0320, 44 },
    { { -24.08f, -40.960003f, -21.359999f }, -1.0f, 0xFD44, 0x01F4, 36 },
    { { -44.8f, -73.92f, -49.76f }, -3.2f, 0x03E8, 0xFF38, 30 },
    { { -9.0f, -76.479996f, -13.24f }, -1.0f, 0xFC18, 0x0258, 26 },
    { { -10.240001f, -52.56f, -13.400001f }, 1.6f, 0x0258, 0xFE70, 34 },
    { { 34.04f, -61.72f, -37.04f }, 1.5f, 0x0258, 0x0258, 28 },
    { { 59.48f, -38.399998f, -49.4f }, 1.8f, 0x01F4, 0x0000, 38 },
    { { -19.04f, -112.24f, -35.120003f }, -1.6f, 0x012C, 0x0320, 22 },
    { { 12.24f, -99.04f, -31.64f }, 1.8f, 0xFC18, 0xFE70, 24 },
    { { 49.16f, -81.24f, -55.52f }, 2.4f, 0x02BC, 0x012C, 32 },
    { { 14.759999f, -125.8f, -44.16f }, 0.2f, 0x0320, 0x0258, 20 },
};
static s16 sDustC[8] = { 5, 8, 11, 14, 17, 20, 23, 26 };
static s16 sDelayC[8] = { 18, 26, 34, 42, 50, 60, 70, 80 };
static s16 sSizeC[8] = { 48, 42, 36, 32, 28, 24, 20, 16 };

void FUN_00315d54(PlayState* play, Vec3f* pos, Vec3f* velocity, s32 num, s32 arg4) {
    Vec3f spB4;
    s32 i;

    for (i = 0; i < num; i++) {
        s32 idx = ((s16)(Rand_ZeroOne() * 8.0f)) & sMask[arg4];
        s16 arg5;

        if (idx > 4) {
            arg5 = 0x20;
        } else {
            arg5 = (Rand_ZeroOne() < 0.7f) ? 0x40 : 0x20;
        }

        EffectSsKakera_Spawn(play, pos, velocity, pos, -90, arg5, sSizeC[idx], 4, 0, sDustC[idx], 0, 5, sDelayC[idx],
                             -1, OBJECT_JYA_OBJ, (void*)0x1F);
        if (Rand_ZeroOne() < 0.45f) {
            Math_Vec3f_Copy(&spB4, pos);
            spB4.z += 25.0f;
            FUN_0037378c(play, &spB4, 60.0f, 0, sDustC[idx] * 4 + 50, sDustC[idx] * 4 + 70, 1);
        }
    }
}

// DetectLight (0w in fastchk) + Explode (6w: sVec guarded-static init VFP numbering) + Draw (far: zero/one remat) drafts.
// DetectLight and Explode share the guard layout, so they can only be enabled together; then also apply
//   pin_static.py <tree> bss 0x00569A28 24 "z_bg_jya_megami.o(+BSS)" --label z_bg_jya_megami
// and shrink BgJyaMegami_sJntSphElementsInit to [12].
#if 0
#define PLAY_GAMEPLAY_FRAMES(play) (*(u32*)((u8*)(play) + 0x5BF4))

static inline void BgJyaMegami_SetupSpawnEffect(BgJyaMegami* self, PlayState* play, f32 arg2) {
    static Vector3f sVelocity(0.0f, 0.0f, 0.8f);
    s32 i;
    Vec3f pos;

    for (i = 0; i < 13; i++) {
        if (Rand_ZeroOne() < arg2) {
            PlayState* play2 = play;

            pos.x = self->dyna.actor.world.pos.x + sPiecesInitC[i].pos.x;
            pos.y = self->dyna.actor.world.pos.y + sPiecesInitC[i].pos.y;
            pos.z = self->dyna.actor.world.pos.z + sPiecesInitC[i].pos.z;
            pos.z += 15.0f;
            FUN_00315d54(play2, &pos, (Vec3f*)&sVelocity, 1, 0);
        }
    }
}

void BgJyaMegami_Explode(BgJyaMegami* self, PlayState* play);

static inline void BgJyaMegami_SetupExplode(BgJyaMegami* self) {
    s32 i;
    Vec3f* pos = &self->dyna.actor.world.pos;

    self->actionFunc = BgJyaMegami_Explode;
    for (i = 0; i < 13; i++) {
        Math_Vec3f_Copy(&self->pieces[i].pos, pos);
        self->pieces[i].vel.x = sPiecesInitC[i].velX;
    }
    self->explosionTimer = 0;
}

void BgJyaMegami_DetectLight(BgJyaMegami* self, PlayState* play) {
    PlayState* play2 = play;
    BgJyaMegami* s2 = self;
    if (self->collider.base.acFlags & 2) {
        self->lightTimer++;
        self->collider.base.acFlags = self->collider.base.acFlags & ~2;
        if ((PLAY_GAMEPLAY_FRAMES(play) & 3) == 0) {
            BgJyaMegami_SetupSpawnEffect(self, play2, (self->crumbleIndex * 0.04f) + 0.05f);
        }
        FUN_00373264(&self->dyna.actor, 0x01000228);
    } else if (self->lightTimer > 0) {
        self->lightTimer--;
    }
    if (self->lightTimer > 40) {
        Flags_SetSwitch(play, self->dyna.actor.params & 0x3F);
        BgJyaMegami_SetupExplode(self);
        Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 100, 0x01000227);
        OnePointCutscene_Init(play, 3440, -99, &self->dyna.actor, 0);
    } else {
        if (self->lightTimer < 8) {
            self->crumbleIndex = 0;
        } else if (self->lightTimer < 16) {
            self->crumbleIndex = 1;
        } else if (self->lightTimer < 24) {
            self->crumbleIndex = 2;
        } else if (self->lightTimer < 32) {
            self->crumbleIndex = 3;
        } else {
            self->crumbleIndex = 4;
        }
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}

void BgJyaMegami_Explode(BgJyaMegami* self, PlayState* play) {
    static Vector3f sVec(0.0f, 0.0f, 0.0f);
    BgJyaMegamiPiece* temp;
    s32 i;
    Vec3f sp8C;
    BgJyaMegamiPieceInit* temp2;

    self->explosionTimer++;
    if (self->explosionTimer == 30) {
        Audio_PlaySoundAtPosition(play, &self->dyna.actor.world.pos, 100, 0x01000226);
    }

    for (i = 0; i < 13; i++) {
        temp = &self->pieces[i];
        temp2 = &sPiecesInitC[i];
        if (self->explosionTimer > temp2->delay) {
            temp->vel.y -= 0.6f;
            if (temp->vel.y < -18.0f) {
                temp->vel.y = -18.0f;
            }
            temp->vel.x *= 0.995f;
            temp->pos.x += temp->vel.x;
            temp->pos.y += temp->vel.y;
            temp->rotVelX += temp2->rotVelX;
            temp->rotVelY += temp2->rotVelY;
            if (Rand_ZeroOne() < 0.067f) {
                sp8C.x = temp->pos.x + temp2->pos.x;
                sp8C.y = temp->pos.y + temp2->pos.y;
                sp8C.z = temp->pos.z + temp2->pos.z;
                sp8C.z += 10.0f;
                FUN_00315d54(play, &sp8C, &temp->vel, 2, 2);
            }
        } else if (self->explosionTimer == temp2->delay) {
            sp8C.x = temp->pos.x + temp2->pos.x;
            sp8C.y = temp->pos.y + temp2->pos.y;
            sp8C.z = temp->pos.z + temp2->pos.z;
            sp8C.z += 10.0f;
            FUN_00315d54(play, &sp8C, &temp->vel, 3, 2);
        }
    }

    if ((self->explosionTimer % 4 == 0) && (self->explosionTimer > 30) && (self->explosionTimer < 80) &&
        (self->explosionTimer > 40)) {
        sp8C.x = ((Rand_ZeroOne() - 0.5f) * 90.0f) + self->dyna.actor.world.pos.x;
        sp8C.y = (self->dyna.actor.world.pos.y - (Rand_ZeroOne() * 80.0f)) - 20.0f;
        sp8C.z = self->dyna.actor.world.pos.z - (Rand_ZeroOne() - 0.5f) * 50.0f;
        FUN_00315d54(play, &sp8C, (Vec3f*)&sVec, 1, 0);
    }
    if (self->explosionTimer <= 12) {
        sp8C.x = self->dyna.actor.world.pos.x;
        sp8C.y = self->dyna.actor.world.pos.y - 60.0f;
        sp8C.z = self->dyna.actor.world.pos.z;
        FUN_0037378c(play, &sp8C, 100.0f, 1, 150, 100, 1);
    }
    if (self->explosionTimer == 60) {
        FUN_00372244((u8*)play + 0x5FCC, 0x1E, 0x01000487);
    }
    if (self->explosionTimer >= 100) {
        Actor_Kill(&self->dyna.actor);
    }
}

void BgJyaMegami_Draw(Actor* actor, GameState* state) {
    BgJyaMegami* self = (BgJyaMegami*)actor;
    PlayState* play = (PlayState*)state;
    MTX34 baseMtx;
    MTX34 mtx;
    Vec3f pos;
    Vec3f t;
    s32 i;

    MTX34CopyAsm(&baseMtx, &actor->modelMtx);
    Collider_UpdateSpheres(0, &self->collider, &baseMtx);
    FUN_00357fd0(GET_PLAYER(play), actor->unk_178, &actor->world.pos);
    if (self->actionFunc == BgJyaMegami_Explode) {
        MTX34CopyAsm(&mtx, &actor->modelMtx);
        for (i = 0; i < 13; i++) {
            BgJyaMegamiPiece* piece = &self->pieces[i];
            BgJyaMegamiPieceInit* init = &sPiecesInitC[i];

            pos.x = init->pos.x + piece->pos.x;
            pos.y = init->pos.y + piece->pos.y;
            pos.z = init->pos.z + piece->pos.z;
            MTX34_MakeTrans(&mtx, &pos);
            BgJyaMegami_RotY(&mtx, piece->rotVelY * 9.58738019e-5f);
            BgJyaMegami_RotX(&mtx, piece->rotVelX * 9.58738019e-5f);
            BgJyaMegami_Scale(&mtx, 0.1f, 0.1f, 0.1f);
            t.x = init->pos.x * -10.0f;
            t.y = init->pos.y * -10.0f;
            t.z = init->pos.z * -10.0f;
            Matrix_UpdatePosition(&mtx, &mtx, &t);
            if (self->models[i + 1] != NULL) {
                self->models[i + 1]->SetUnkAC(1);
                self->models[i + 1]->FUN_003721e0(&mtx);
                self->models[i + 1]->FUN_00372170(0);
            }
        }
    } else {
        MTX34CopyAsm(&mtx, &actor->modelMtx);
        if (self->models[0] != NULL) {
            self->models[0]->SetUnkAC(1);
            self->models[0]->FUN_003721e0(&mtx);
            self->models[0]->FUN_00372170(0);
        }
    }
}
#endif

GLOBAL_ASM("asm/BgJyaMegami_DetectLight.s")

GLOBAL_ASM("asm/BgJyaMegami_Explode.s")

GLOBAL_ASM("asm/BgJyaMegami_Draw.s")

}
