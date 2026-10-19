extern "C" {
#include "z_bg_hidan_rock.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x80400000

void BgHidanRock_Init(Actor* actor, GameState* state);
void BgHidanRock_Destroy(Actor* thisx, PlayState* play);
void FUN_0049fa58(Vec3f* pos); // Audio_StopSfxByPos
s32 FUN_0036adf4(DynaPolyActor* dyna); // DynaPolyActor_IsPlayerOnTop
s16 Camera_GetCamDirYaw(Camera* camera);
f32 __hardfp_sinf(f32);
f32 __hardfp_cosf(f32);
void Matrix_UpdatePosition(MTX34* dst, MTX34* src, Vec3f* translate);
void FUN_00176cdc(BgHidanRock* self, PlayState* play);
void FUN_003e0910(BgHidanRock* self, PlayState* play);
void FUN_003eb5ec(BgHidanRock* self, PlayState* play);
void FUN_003d9534(BgHidanRock* self, PlayState* play);
void FUN_00128704(BgHidanRock* self, PlayState* play);
void FUN_004c6d34(void);
void FUN_003e1f74(BgHidanRock* self, PlayState* play);
void Audio_PlaySoundGeneral(u32 sfxId, Vec3f* pos, u8 a2, f32* a3, f32* a4, s8* a5);
void FUN_003d0830(PlayState* play, BgHidanRock* self);
void FUN_003b9ec0(BgHidanRock* self, PlayState* play);
s32 Quake_Add(Camera* camera, s32 type);
void Quake_SetSpeed(s32 quakeIndex, s32 speed);
void Quake_SetQuakeValues(s32 quakeIndex, s32 a, s32 b, s32 c, s32 d);
void Quake_SetCountdown(s32 quakeIndex, s32 countdown);
void Camera_ChangeSetting(Camera* camera, s32 setting);
s32 FUN_00341df0(void* colCtx, CollisionPoly* poly, s32 bgId);
void FUN_00176c38(BgHidanRock* self, PlayState* play);
void FUN_003e1f74(BgHidanRock* self, PlayState* play);
void BgHidanRock_Update(Actor* actor, GameState* state);
DynaPolyActor* DynaPoly_GetActor(CollisionContext* colCtx, s32 bgId);
s32 FUN_0040cbe8(PlayState* play, s32 arg) __attribute__((noinline));
void FUN_0034a80c(MTX34* dst, MTX34* src);
s32 CollisionCheck_SetAT(PlayState* play, CollisionCheckContext* colChkCtx, Collider* collider);
void BgHidanRock_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_Bg_Hidan_Rock.data.s")
GLOBAL_ASM("../src/game/actors/Bg_Hidan_Rock/data_syms.s")

// D_8088BF60 (Vec3f {3310, 120, 0}) lives in the data dump at 0x005699D4; absolute symbol from data_syms.s.
extern Vec3f BgHidanRock_D_8088BF60;
#define D_8088BF60 BgHidanRock_D_8088BF60
// sCylinderInit lives in the data dump at 0x00511BD4 (EQU in data_syms.s).
extern ColliderCylinderInit BgHidanRock_sCylinderInit;
#define sCylinderInit BgHidanRock_sCylinderInit
extern InitChainEntry BgHidanRock_sInitChain[];
// D_8088BFC0 (static f32, 0.0f) is the first word of the data dump at 0x00511BC4.
extern f32 BgHidanRock_D_8088BFC0[2]; // [1] (500.0f) is the Update lens bound
#define D_8088BFC0 BgHidanRock_D_8088BFC0[0]

ActorInit Bg_Hidan_Rock_InitVars = {
    ACTOR_BG_HIDAN_ROCK,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanRock),
    (ActorFunc)BgHidanRock_Init,
    (ActorFunc)BgHidanRock_Destroy,
    (ActorFunc)BgHidanRock_Update,
    (ActorFunc)BgHidanRock_Draw,
};

void BgHidanRock_Init(Actor* thisx, GameState* state) {
    BgHidanRock* self = (BgHidanRock*)thisx;
    PlayState* play = (PlayState*)state;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zarInfo;

    Actor_ProcessInitChain(thisx, BgHidanRock_sInitChain);
    DynaPolyActor_Init(&self->dyna, 1);
    zarInfo = FUN_00372f38(thisx, (PlayState*)state, &self->skelAnimModel[0], 0x18, &self->skelAnimModel[1], 8,
                           &self->skelAnimModel[2], 0xB, 0);
    self->skelAnimModel[0]->unk_0C->FUN_00372d94(zarInfo->GetCMABByIndex(1));
    self->skelAnimModel[0]->unk_0C->SetAnimType(1);

    self->unk_1C1 = 0;
    self->type = thisx->params;
    thisx->params = (thisx->params >> 8) & 0xFF;

    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, thisx, &sCylinderInit);

    if (self->type == 0) {
        if (Flags_GetSwitch(play, thisx->params)) {
            Math_Vec3f_Copy(&thisx->home.pos, &D_8088BF60);
            Math_Vec3f_Copy(&thisx->world.pos, &D_8088BF60);
            self->timer = 90;
            self->actionFunc = FUN_00176c38;
        } else {
            self->actionFunc = FUN_003e0910;
        }
        thisx->flags |= 0x30;
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 4), &colHeader);
    } else {
        ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, state, 7), &colHeader);
        self->collider.dim.pos = thisx->home.pos;
        self->actionFunc = FUN_00176cdc;
    }

    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    self->timer = 0;
}

void BgHidanRock_Destroy(Actor* thisx, PlayState* play) {
    BgHidanRock* self = (BgHidanRock*)thisx;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyCylinder(play, &self->collider);
    FUN_0049fa58(&self->unk_1C8);
    FUN_00350f34(thisx, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2], 0);
}

void BgHidanRock_Update(Actor* thisx, GameState* state) {
    BgHidanRock* self = (BgHidanRock*)thisx;
    PlayState* play = (PlayState*)state;
    BgHidanRock* s2 = self;
    PlayState* play2 = play;

    self->actionFunc(self, play);
    if (self->actionFunc == FUN_003b9ec0) {
        if (play2->sceneNum == 4 && s2->dyna.actor.room == 0x15) {
            Player* player = GET_PLAYER(play);

            if (player != NULL && FUN_0040cbe8((PlayState*)player, 0)) {
                DynaPolyActor* dyna = DynaPoly_GetActor(&play->colCtx, player->actor.floorBgId);

                if (dyna == &self->dyna) {
                    MTX34 mtx;
                    Vec3f local;

                    FUN_0034a80c(&mtx, &dyna->actor.modelMtx);
                    VEC3TransformAsm((Vector3f*)&local, &mtx, (Vector3f*)&player->actor.world.pos);
                    if (local.x < BgHidanRock_D_8088BFC0[1] && local.x > -BgHidanRock_D_8088BFC0[1] &&
                        local.z < BgHidanRock_D_8088BFC0[1] && local.z > -BgHidanRock_D_8088BFC0[1]) {
                        player->actor.bgCheckFlags |= 0x100;
                    }
                }
            }
        }
    }
    if (self->actionFunc == FUN_003d9534) {
        Actor_MoveForward(&self->dyna.actor);
        Actor_UpdateBgCheckInfo(play, &self->dyna.actor, 0.0f, 0.0f, 0.0f, 4);
    }

    if (self->unk_1C4 > 0.0f) {
        self->collider.dim.height = sCylinderInit.dim.height * self->unk_1C4;
        CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
    }
}

void FUN_003e0910(BgHidanRock* self, PlayState* play) {
    Player* player = GET_PLAYER(play);
    f32 sp2C;
    s32 temp_v1;
    s32 frame;

    if (self->dyna.unk_1A8 != 0.0f) {
        if (self->timer == 0) {
            if (D_8088BFC0 == 0.0f) {
                if (self->dyna.unk_1A8 > 0.0f) {
                    D_8088BFC0 += 0.01f * 2.0f / 3.0f;
                } else {
                    D_8088BFC0 -= 0.01f * 2.0f / 3.0f;
                }
            }

            self->dyna.actor.speedXZ += 0.05f * 2.0f / 3.0f;
            self->dyna.actor.speedXZ = (self->dyna.actor.speedXZ > 2.0f) ? 2.0f : self->dyna.actor.speedXZ;

            if (D_8088BFC0 > 0.0f) {
                temp_v1 = Math_StepToF(&D_8088BFC0, 20.0f, self->dyna.actor.speedXZ);
            } else {
                temp_v1 = Math_StepToF(&D_8088BFC0, -20.0f, self->dyna.actor.speedXZ);
            }

            self->dyna.actor.world.pos.x = (D_8088BFC0 * Math_SinS(self->dyna.unk_1B0)) + self->dyna.actor.home.pos.x;
            self->dyna.actor.world.pos.z = (D_8088BFC0 * Math_CosS(self->dyna.unk_1B0)) + self->dyna.actor.home.pos.z;

            if (temp_v1) {
                *(u32*)((u8*)player + 0x1714) &= ~0x10;
                self->dyna.actor.home.pos.x = self->dyna.actor.world.pos.x;
                self->dyna.actor.home.pos.z = self->dyna.actor.world.pos.z;
                self->dyna.unk_1A8 = 0.0f;
                D_8088BFC0 = 0.0f;
                self->dyna.actor.speedXZ = 0.0f;
                self->timer = 5;
            }

            FUN_00373264(&self->dyna.actor, 0x01000161);
        } else {
            *(u32*)((u8*)player + 0x1714) &= ~0x10;
            self->dyna.unk_1A8 = 0.0f;
            if (self->timer != 0) {
                self->timer--;
            }
        }
    } else {
        self->timer = 0;
    }

    sp2C = self->dyna.actor.world.pos.z - D_8088BF60.z;
    if (sp2C < 0.5f) {
        Flags_SetSwitch(play, self->dyna.actor.params);
        Math_Vec3f_Copy(&self->dyna.actor.home.pos, &D_8088BF60);
        self->dyna.actor.world.pos.x = D_8088BF60.x;
        self->dyna.actor.world.pos.z = D_8088BF60.z;
        self->dyna.actor.speedXZ = 0.0f;
        D_8088BFC0 = 0.0f;
        *(u32*)((u8*)player + 0x1714) &= ~0x10;
        self->actionFunc = FUN_003d9534;
    }

    frame = *(u32*)((u8*)play + 0x5BF4) & 0xFF;
    if (*(u32*)((u8*)play + 0x5BF4) & 0x100) {
        self->unk_1C4 = 0.0f;
    } else if (frame < 128) {
        self->unk_1C4 = __hardfp_sinf(frame * (4 * 0.001f * 3.14159265f) * 2.0f / 3.0f) * 19.625f;
    } else if (frame < 230) {
        self->unk_1C4 = 19.625f;
    } else {
        self->unk_1C4 -= 1.0f * 2.0f / 3.0f;
        self->unk_1C4 = (self->unk_1C4 < 0.0f) ? 0.0f : self->unk_1C4;
    }

    if (sp2C < 100.0f) {
        self->unk_1C4 = (self->unk_1C4 > 6.125f) ? 6.125f : self->unk_1C4;
    }
}

void BgHidanRock_Draw(Actor* thisx, PlayState* play) {
    BgHidanRock* self = (BgHidanRock*)thisx;
    MTX34 mtx;

    MTX34CopyAsm(&mtx, &thisx->modelMtx);
    if (self->type == 0) {
        if (self->skelAnimModel[1] != NULL) {
            self->skelAnimModel[1]->SetUnkAC(1);
            self->skelAnimModel[1]->FUN_003721e0(&mtx);
            self->skelAnimModel[1]->FUN_00372170(0);
        }
    } else {
        if (self->skelAnimModel[2] != NULL) {
            self->skelAnimModel[2]->SetUnkAC(1);
            self->skelAnimModel[2]->FUN_003721e0(&mtx);
            self->skelAnimModel[2]->FUN_00372170(0);
        }
    }

    if (self->unk_1C4 > 0.0f) {
        if (self->type == 0) {
            self->unk_1C8 = D_8088BF60;
        } else {
            self->unk_1C8 = self->dyna.actor.home.pos;
        }
        Audio_PlaySoundGeneral(0x0100018A, &self->unk_1C8, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
        FUN_003d0830(play, self);
    }
}

// FUN_003d0830 (N64 func_8088BC40, flame draw): NOT matched (recipe R8, MTX34_MakeTrans). The integer-store
// identity+translate block now matches; ~21 insns off (difflib) / 3 insns short: ROM stores pos.x/pos.y inside each
// branch and only z at the join, keeps the 0.0f in s16 and copies it to s18 after Camera_GetCamDirYaw, and hoists the
// angle constant + `mov r5,#1` (SetUnkAC) above the ry != 0 test. Tried: Translate per branch, Vec3f/f32 locals, ternary,
// pointer select, RotateY inline helpers, Translate param/decl perms.
#if 0
static inline void BgHidanRock_Translate(MTX34* m, s32 mode, f32 y, f32 z, f32 x) {
    Vec3f t;

    t.x = x;
    t.y = y;
    t.z = z;
    if (mode == 1) {
        Matrix_UpdatePosition(m, m, &t);
    } else {
        MTX34_MakeTrans(m, &t);
    }
}

static inline void BgHidanRock_Scale(MTX34* m, f32 x, f32 y, f32 z) {
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

#define BGHIDANROCK_ROTY(r) \
    a = mtx.data[r][0];     \
    b = mtx.data[r][2];     \
    mtx.data[r][0] = a * cs - b * sn; \
    mtx.data[r][2] = a * sn + b * cs;

static inline void BgHidanRock_RotateYF(MTX34* m, f32 ang) {
    f32 sn, cs, a, b;

    if (ang != 0.0f) {
        sn = __hardfp_sinf(ang);
        cs = __hardfp_cosf(ang);
        a = m->data[0][0];
        b = m->data[0][2];
        m->data[0][0] = a * cs - b * sn;
        m->data[0][2] = a * sn + b * cs;
        a = m->data[1][0];
        b = m->data[1][2];
        m->data[1][0] = a * cs - b * sn;
        m->data[1][2] = a * sn + b * cs;
        a = m->data[2][0];
        b = m->data[2][2];
        m->data[2][0] = a * cs - b * sn;
        m->data[2][2] = a * sn + b * cs;
    }
}

static inline void BgHidanRock_RotateY(MTX34* m, s16 binang) {
    if (binang != 0) {
        BgHidanRock_RotateYF(m, binang * 9.58738E-5f);
    }
}

void FUN_003d0830(PlayState* play, BgHidanRock* self) {
    MTX34 mtx;
    f32 animSpeed;
    s16 ry;
    f32 ang, sn, cs, a, b;

    MTX34CopyAsm(&mtx, &self->dyna.actor.modelMtx);
    animSpeed = 2.0f;
    if (PauseContext_GetState() != 0) {
        animSpeed = 0.0f;
    }

    {
        f32 y, x, z;
        if (self->type == 0) {
            x = D_8088BF60.x;
            y = D_8088BF60.y - 40.0f;
            z = D_8088BF60.z;
        } else {
            x = self->dyna.actor.home.pos.x;
            y = self->dyna.actor.home.pos.y - 40.0f;
            z = self->dyna.actor.home.pos.z;
        }
        BgHidanRock_Translate(&mtx, 0, y, z, x);
    }

    ry = Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) + 0x8000;
    if (ry != 0) {
        ang = ry * 9.58738E-5f;
        if (ang != 0.0f) {
            sn = __hardfp_sinf(ang);
            cs = __hardfp_cosf(ang);
            BGHIDANROCK_ROTY(0)
            BGHIDANROCK_ROTY(1)
            BGHIDANROCK_ROTY(2)
        }
    }
    BgHidanRock_Translate(&mtx, 1, 0.0f, 0.0f, -10.5f);
    BgHidanRock_Scale(&mtx, 6.0f, self->unk_1C4, 6.0f);

    if (self->skelAnimModel[0] != NULL) {
        self->skelAnimModel[0]->SetUnkAC(1);
        self->skelAnimModel[0]->FUN_003721e0(&mtx);
        self->skelAnimModel[0]->unk_0C->SetAnimSpeed(animSpeed);
        self->skelAnimModel[0]->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/FUN_003d0830.s")

void FUN_00140384(BgHidanRock* self) __attribute__((noinline));

void FUN_00176c38(BgHidanRock* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        FUN_00140384(self);
    }
}

void FUN_00140384(BgHidanRock* self) {
    self->dyna.actor.flags |= 0x30;
    self->actionFunc = FUN_003b9ec0;
}

void FUN_00128704(BgHidanRock* self, PlayState* play) {
    if (Math_StepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y, 1.0f)) {
        self->actionFunc = FUN_00176cdc;
    }
}

void FUN_00176cdc(BgHidanRock* self, PlayState* play) {
    if (FUN_0036adf4(&self->dyna)) {
        self->timer = 30;
        self->dyna.actor.world.rot.y = Camera_GetCamDirYaw(play->cameraPtrs[play->activeCamera]) + 0x4000;
        self->actionFunc = FUN_003eb5ec;
    }
}

void FUN_003b9ec0(BgHidanRock* self, PlayState* play) {
    Player* player = GET_PLAYER(play);

    self->timer++;
    if (self->dyna.unk_1A8 != 0.0f) {
        self->dyna.actor.speedXZ = 0.0f;
        *(u32*)((u8*)player + 0x1714) &= ~0x10;
    }

    if (((self->type == 0) &&
         (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 1820.0f, 0.25f, 20.0f,
                             0.5f) < 0.1f)) ||
        ((self->type != 0) &&
         (Math_SmoothStepToF(&self->dyna.actor.world.pos.y, self->dyna.actor.home.pos.y + 480.0f, 0.25f, 20.0f,
                             0.5f) < 0.1f))) {
        if (self->type == 0) {
            Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
        }
        self->timer = 30;
        self->actionFunc = FUN_003e1f74;
    }

    self->unk_1C4 = (self->dyna.actor.world.pos.y + 50.0f - self->dyna.actor.home.pos.y + 40.0f) / 80.0f;
    if (self->type == 0) {
        if (FUN_0036adf4(&self->dyna)) {
            if (self->unk_1C1 == 0) {
                self->unk_1C1 = 3;
            }
            Camera_ChangeSetting(play->cameraPtrs[0], 0x30);
        } else if (!FUN_0036adf4(&self->dyna)) {
            if (self->unk_1C1 != 0) {
                Camera_ChangeSetting(play->cameraPtrs[0], 3);
            }
            self->unk_1C1 = 0;
        }
    }
}

void FUN_003d9534(BgHidanRock* self, PlayState* play) {
    self->timer--;
    if (self->dyna.actor.bgCheckFlags & 2) {
        if (self->type == 0) {
            self->timer = 90;
            self->actionFunc = FUN_00176c38;
        } else {
            self->dyna.actor.world.pos.y = self->dyna.actor.home.pos.y - 15.0f;
            self->actionFunc = FUN_00128704;
            self->dyna.actor.flags &= ~0x30;
        }
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018C);
        Audio_PlayActorSound2(&self->dyna.actor,
                              FUN_00341df0(&play->colCtx, self->dyna.actor.floorPoly, self->dyna.actor.floorBgId) +
                                  0x01000001);

        f32 d = self->dyna.actor.xzDistToPlayer;
        if (d < 220.0f) {
            s32 five = 5;
            s32 z1 = 0, z2 = 0, z3 = 0;
            s32 three = 3;
            s32 speed = 30000;
            s32 quakeIndex = Quake_Add(play->cameraPtrs[play->activeCamera], three);

            Quake_SetSpeed(quakeIndex, speed);
            Quake_SetQuakeValues(quakeIndex, five, z1, z2, z3);
            Quake_SetCountdown(quakeIndex, three);
        }
    }

    self->unk_1C4 -= 0.33333334f;
    self->unk_1C4 = (self->unk_1C4 < 0.0f) ? 0.0f : self->unk_1C4;

    if (self->type == 0) {
        if (FUN_0036adf4(&self->dyna)) {
            if (self->unk_1C1 == 0) {
                self->unk_1C1 = 3;
            }
            Camera_ChangeSetting(play->cameraPtrs[0], 0x30);
        } else if (!FUN_0036adf4(&self->dyna)) {
            if (self->unk_1C1 != 0) {
                Camera_ChangeSetting(play->cameraPtrs[0], 3);
            }
            self->unk_1C1 = 0;
        }
    }
}

void FUN_003eb5ec(BgHidanRock* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer != 0) {
        s32 ang = self->dyna.actor.world.rot.y + self->timer * 0x4000;
        f32 two;
        f32 half;
        f32 third;
        two = 2.0f;
        third = (1.0f / 3.0f);
        half = 0.5f;
        s32 n = (s32)(ang > 0 ? half + (f32)ang * two * third : (f32)ang * two * third - half);
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x + 5.0f * Math_SinS(n);

        ang = self->dyna.actor.world.rot.y + self->timer * 0x4000;
        n = (s32)(ang > 0 ? half + (f32)ang * two * third : (f32)ang * two * third - half);
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z + 5.0f * Math_CosS(n);
    } else {
        self->dyna.actor.world.pos.x = self->dyna.actor.home.pos.x;
        self->dyna.actor.world.pos.z = self->dyna.actor.home.pos.z;
        self->dyna.actor.flags |= 0x30;
        self->actionFunc = FUN_003b9ec0;
    }
    if (!(self->timer % 6)) {
        Audio_PlayActorSound2(&self->dyna.actor, 0x0100018f);
    }
}

void FUN_003e1f74(BgHidanRock* self, PlayState* play) {
    if (self->timer != 0) {
        self->timer--;
    }
    if (self->timer == 0) {
        self->dyna.actor.velocity.y = 0.0f;
        self->actionFunc = FUN_003d9534;
    }
}

s32 FUN_0040cbe8(PlayState* play, s32 arg) {
    if (arg == 0) {
        if (*(void**)((u8*)play + 0x1708) == (void*)FUN_004c6d34) {
            return 1;
        }
    }
    return 0;
}


}
