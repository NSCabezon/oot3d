extern "C" {
#include "z_en_dodojr.hpp"

#define FLAGS 0x00000005

void EnDodojr_Init(Actor* actor, GameState* state);
void EnDodojr_Destroy(Actor* actor, GameState* state);
void EnDodojr_Update(Actor* actor, GameState* state);
void EnDodojr_Draw(Actor* actor, GameState* state);

// Helpers without a repo prototype yet (3DS addresses).
#define ColorFilterTimer(self) (*(u16*)((u8*)(self) + 0x11A))
void Actor_SetColorFilter(Actor* actor, s32 colorFlag, s32 alpha, s32 buffFlag, s32 duration);
s32 Collider_DestroyCylinder(PlayState* play, ColliderCylinder* collider);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Audio_PlayActorSound2(Actor* actor, s32 arg1);
void Item_DropCollectibleRandom(PlayState* play, Actor* parent, Vec3f* pos, s32 params);
void FUN_0035fb14(Actor* actor);
s32 FUN_0035d0bc(EnDodojr* self, PlayState* play);
void FUN_003179d0(EnDodojr* self, PlayState* play, s32 count);
u64 FUN_00368d94(s32 a, s32 b);
void FUN_00365d20(PlayState* play, Vec3f* pos, Vec3f* velocity, Vec3f* accel, Color_RGBA8* prim, Color_RGBA8* env, s32 life, s32 scale, s32 unk);
void FUN_003b7c64(EnDodojr* self, PlayState* play);
void FUN_003b7d80(EnDodojr* self, PlayState* play);
void FUN_001c429c(EnDodojr* self, PlayState* play);
void FUN_00194634(EnDodojr* self, PlayState* play);
void FUN_0019454c(EnDodojr* self, PlayState* play);
void FUN_00133434(EnDodojr* self, PlayState* play);
void FUN_0013336c(EnDodojr* self, PlayState* play);
void FUN_00219450(EnDodojr* self, PlayState* play);
void FUN_003d05e4(EnDodojr* self, PlayState* play);
void FUN_003d0744(EnDodojr* self, PlayState* play);

GLOBAL_ASM("data/z_En_Dodojr.data.s")

ActorInit En_Dodojr_InitVars = {
    ACTOR_EN_DODOJR,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_DODOJR,
    sizeof(EnDodojr),
    (ActorFunc)EnDodojr_Init,
    (ActorFunc)EnDodojr_Destroy,
    (ActorFunc)EnDodojr_Update,
    (ActorFunc)EnDodojr_Draw,
};

GLOBAL_ASM("../src/game/actors/En_Dodojr/data_syms.s")

f32 sinf(f32 x);
f32 cosf(f32 x);
s32 EnDodojr_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx);
DamageTable* DamageTable_Get(s32 index);

extern ColliderCylinderInit EnDodojr_sCylinderInit;
extern CollisionCheckInfoInit2 EnDodojr_sColChkInit;
#define sCylinderInit EnDodojr_sCylinderInit
#define sColChkInit EnDodojr_sColChkInit

void EnDodojr_Init(Actor* thisx, GameState* state) {
    EnDodojr* self = (EnDodojr*)thisx;
    PlayState* play = (PlayState*)state;

    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 18.0f);
    FUN_00372f38(&self->actor, play, NULL);
    FUN_00353c9c(&self->actor, play, &self->skelAnime, 0, 0, self->jointTable, self->morphTable, 6);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinder(play, &self->collider, &self->actor, &sCylinderInit);
    CollisionCheck_SetInfo2(&self->actor.colChkInfo, DamageTable_Get(4), &sColChkInit);

    self->actor.naviEnemyId = 0x0E;
    thisx->flags &= ~1;

    Actor_SetScale(&self->actor, 0.02f);

    self->actionFunc = FUN_00219450;
}

void EnDodojr_Destroy(Actor* thisx, GameState* state) {
    EnDodojr* self = (EnDodojr*)thisx;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider);
    SkelAnime_Free2(&self->skelAnime);
    FUN_003508b8(thisx, NULL);
}

void EnDodojr_Draw(Actor* thisx, GameState* state) {
    EnDodojr* self = (EnDodojr*)thisx;

    if ((self->actionFunc != FUN_00219450) && (self->actionFunc != FUN_003b7c64)) {
        FUN_00357fd0(GET_PLAYER((PlayState*)state), self->actor.unk_178, &self->actor.world.pos);
        SkelAnime_DrawOpa(&self->skelAnime, (MTX34*)&self->actor.modelMtx, EnDodojr_OverrideLimbDraw, NULL, &self->actor, 0);
    }
}

#if 0 // draft, 54 words off (ROM adds a vmov copy of the x/z scale before each of the 3 column-0 products; same blocker as En_Poh PostLimbDraw): kept as asm
// Matrix_Scale(x, y, z, MTXMODE_APPLY): scales each matrix column.
static inline void EnDodojr_ScaleMtx(MTX34* mtx, f32 x, f32 y, f32 z) {
    f32* m = (f32*)mtx;

    m[0] *= x;
    m[4] *= x;
    m[8] *= x;
    m[1] *= y;
    m[5] *= y;
    m[9] *= y;
    m[2] *= z;
    m[6] *= z;
    m[10] *= z;
}

s32 EnDodojr_OverrideLimbDraw(PlayState* play, s32 limbIndex, MTX34* mtx, void* thisx) {
    EnDodojr* self = (EnDodojr*)thisx;
    Vector3f D_809F7F64(480.0f, 620.0f, 0.0f);

    if (limbIndex == 0) {
        if (self->actionFunc == FUN_00133434) {
            f32 angle = 3.1415927f;
            f32 s = sinf(angle);
            f32 c = cosf(angle);
            f32* m = (f32*)mtx;
            f32 x;
            f32 z;

            x = m[0];
            z = m[2];
            m[0] = (x * c) - (z * s);
            m[2] = (x * s) + (z * c);

            x = m[4];
            z = m[6];
            m[4] = (x * c) - (z * s);
            m[6] = (x * s) + (z * c);

            x = m[8];
            z = m[10];
            m[8] = (x * c) - (z * s);
            m[10] = (x * s) + (z * c);
        }
    } else if (limbIndex == 1) {
        f32 rs = self->rootScale;
        f32* m = (f32*)mtx;
        f32 sz = (rs * 0.5f) + 1.0f;
        f32 sy = rs + 1.0f;
        f32 sx = sz;

        m[0] *= sx;
        m[4] *= sx;
        m[8] *= sx;
        m[1] *= sy;
        m[5] *= sy;
        m[9] *= sy;
        m[2] *= sz;
        m[6] *= sz;
        m[10] *= sz;
    } else if (limbIndex == 4) {
        VEC3TransformAsm((Vector3f*)&self->headPos, mtx, (Vector3f*)&D_809F7F64);
    }

    return false;
}
#endif

GLOBAL_ASM("asm/EnDodojr_OverrideLimbDraw.s")

void FUN_00375b70(PlayState* play, Actor* actor);
void FUN_003b7cb4(EnDodojr* self, PlayState* play);
void FUN_0016e964(EnDodojr* self, PlayState* play);
void FUN_003d0744(EnDodojr* self, PlayState* play);
void FUN_00104648(EnDodojr* self, PlayState* play);
void FUN_003d069c(EnDodojr* self, PlayState* play);

static inline void EnDodojr_SetupStandardDeathBounce(EnDodojr* self) {
    Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 3), 2, -10.0f);
    self->actor.speedXZ = 0.0f;
    self->actor.velocity.x = 0.0f;
    self->actor.velocity.z = 0.0f;
    self->actor.gravity = -0.8f;

    if (self->counter == 0) {
        self->actor.velocity.y = 10.0f;
        self->counter = 3;
    }
}

static inline s32 EnDodojr_CheckDamaged(EnDodojr* self, PlayState* play) {
    if ((self->actionFunc == FUN_001c429c) || (self->actionFunc == FUN_00194634) ||
        (self->actionFunc == FUN_0019454c) || (self->actionFunc == FUN_003b7cb4) ||
        (self->actionFunc == FUN_0013336c) || (self->actionFunc == FUN_00133434) ||
        (self->actionFunc == FUN_003b7c64)) {
        return false;
    }

    if (play->actorCtx.unk_02 != 0) {
        if (self->actionFunc != FUN_00219450) {
            if (self->actionFunc == FUN_003d05e4) {
                self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
            }

            Audio_PlayActorSound2(&self->actor, 0x01000291);
            self->actor.flags &= ~1;
            EnDodojr_SetupStandardDeathBounce(self);
            self->actionFunc = FUN_0013336c;
        }
        return false;
    }

    if (!(self->collider.base.acFlags & AC_HIT)) {
        return false;
    } else {
        self->collider.base.acFlags &= ~AC_HIT;

        if ((self->actionFunc == FUN_00219450) || (self->actionFunc == FUN_003d05e4)) {
            self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        }

        if ((self->actor.colChkInfo.damageEffect == 0) && (self->actor.colChkInfo.damage != 0)) {
            FUN_00375b70(play, &self->actor);
            self->freezeFrameTimer = 2;
            self->actionFunc = FUN_00104648;
            return true;
        }

        if ((self->actor.colChkInfo.damageEffect == 1) && (self->actionFunc != FUN_003d0744) &&
            (self->actionFunc != FUN_0016e964)) {
            Audio_PlayActorSound2(&self->actor, 0x0100030E);
            self->stunTimer = 180;
            Actor_SetColorFilter(&self->actor, 0, 200, 0, 120);
            EnDodojr_SetupStandardDeathBounce(self);
            self->actionFunc = FUN_0016e964;
        }

        return false;
    }
}

static inline void EnDodojr_UpdateCollider(EnDodojr* self, PlayState* play) {
    Collider_UpdateCylinder(&self->actor, &self->collider);

    if ((self->actionFunc != FUN_00219450) && (self->actionFunc != FUN_003b7c64)) {
        if ((self->actionFunc == FUN_003d05e4) || (self->actionFunc == FUN_003b7d80) ||
            (self->actionFunc == FUN_003d069c)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
        }

        if ((self->actionFunc == FUN_003d05e4) || (self->actionFunc == FUN_003b7d80) ||
            (self->actionFunc == FUN_0016e964) || (self->actionFunc == FUN_003d0744) ||
            (self->actionFunc == FUN_003d069c)) {
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }

        CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
    }
}

void EnDodojr_Update(Actor* thisx, GameState* state) {
    EnDodojr* self = (EnDodojr*)thisx;
    PlayState* play = (PlayState*)state;

    FUN_003731e0(&self->skelAnime);
    Actor_MoveForward(&self->actor);
    EnDodojr_CheckDamaged(self, play);

    if (self->actionFunc != FUN_00219450) {
        self->actor.gravity = -0.8f;
        Actor_UpdateBgCheckInfo(play, &self->actor, self->collider.dim.radius, self->collider.dim.height, 0.0f, 5);
    } else {
        self->actor.gravity = 0.0f;
    }

    self->actionFunc(self, play);
    Actor_SetFocus(&self->actor, 10.0f);
    EnDodojr_UpdateCollider(self, play);
}

void FUN_003b7c64(EnDodojr* self, PlayState* play) {
    if (DECR(self->timer) == 0) {
        Item_DropCollectibleRandom(play, NULL, &self->actor.world.pos, 0x40);
        Actor_Kill(&self->actor);
    }
}

static void EnDodojr_SetupCrawlTowardsTarget(EnDodojr* self) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);

    Animation_Change_00375c08(&self->skelAnime, 1, 1.8f, 0.0f, lastFrame, 1, -10.0f);
    self->actor.velocity.y = 0.0f;
    self->actor.speedXZ = 2.6f;
    self->actor.gravity = -0.8f;
}

void FUN_003d069c(EnDodojr* self, PlayState* play) {
    self->actor.flags |= 0x1000000;
    FUN_0035fb14(&self->actor);

    if (FUN_0035d0bc(self, play)) {
        EnDodojr_SetupCrawlTowardsTarget(self);
        self->actionFunc = FUN_003b7d80;
    }
}

void FUN_003d05e4(EnDodojr* self, PlayState* play) {
    f32 sp2C;

    Math_SmoothStepToS(&self->actor.shape.rot.x, 0, 4, 1000, 100);
    sp2C = self->actor.shape.rot.x;
    sp2C /= 16384.0f;
    self->actor.world.pos.y = self->actor.home.pos.y + (60.0f * sp2C);
    FUN_003179d0(self, play, 3);

    if (sp2C == 0.0f) {
        self->actor.shape.shadowDraw = ActorShadow_DrawCircle;
        self->actor.world.rot.x = self->actor.shape.rot.x;
        self->actor.speedXZ = 2.6f;
        self->actionFunc = FUN_003b7d80;
    }
}

void FUN_00104594(EnDodojr* self, PlayState* play) {
    Actor* bomb;

    if (((s16)self->skelAnime.curFrame - 8) <= 3) {
        bomb = self->bomb;
        if ((bomb->id != 0x10) && (bomb->id != 0x4C)) {
            if (bomb->id == 0xDA) {
                *((u8*)bomb + 0x1AB) = 1;
            }
        } else {
            *(s16*)((u8*)bomb + 0x26C) = *(s16*)((u8*)bomb + 0x26C) + 1;
        }
        self->bomb->world.pos = self->headPos;
    } else {
        Audio_PlayActorSound2(&self->actor, 0x010002C0);
        Actor_Kill(self->bomb);
        self->timer = 36;
        self->counter = 0;
        self->actionFunc = FUN_001c429c;
    }
}

s32 FUN_0035d0bc(EnDodojr* self, PlayState* play) {
    if (self->actor.velocity.y >= 0.0f) {
        return false;
    }

    if (self->counter == 0) {
        return false;
    }

    if (self->actor.bgCheckFlags & 1) {
        Audio_PlayActorSound2(&self->actor, 0x010002EB);
        self->dustPos = self->actor.world.pos;
        FUN_003179d0(self, play, 10);
        self->actor.velocity.y = 10.0f / (4 - self->counter);
        self->counter--;

        if (self->counter == 0) {
            self->actor.velocity.y = 0.0f;
            return true;
        }
    }

    return false;
}

void FUN_0013336c(EnDodojr* self, PlayState* play) {
    FUN_0035fb14(&self->actor);
    Math_SmoothStepToS(&self->actor.shape.rot.y, 0, 4, 1000, 10);
    self->actor.world.rot.x = self->actor.shape.rot.x;

    if (FUN_0035d0bc(self, play)) {
        self->timer = 90;
        Animation_Change_00375c08(&self->skelAnime, 4, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 4), 0, 0.0f);
        self->actor.velocity.y = 0.0f;
        self->actor.gravity = -0.8f;
        self->counter = 7;
        self->actionFunc = FUN_00133434;
    }
}

void FUN_0016e964(EnDodojr* self, PlayState* play) {
    FUN_0035fb14(&self->actor);

    if (FUN_0035d0bc(self, play)) {
        Animation_Change_00375c08(&self->skelAnime, 4, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 4), 0, 0.0f);
        self->actor.velocity.y = 0.0f;
        self->actor.gravity = -0.8f;
        self->actionFunc = FUN_003d0744;
    }

    Math_SmoothStepToS(&self->actor.shape.rot.y, 0, 4, 1000, 10);
    self->actor.world.rot.x = self->actor.shape.rot.x;
    ColorFilterTimer(self) = (u8)self->stunTimer;
}

void FUN_003d0744(EnDodojr* self, PlayState* play) {
    if (DECR(self->stunTimer) != 0) {
        if (self->stunTimer < 45) {
            if ((self->stunTimer & 1) != 0) {
                self->actor.world.pos.x += 1.5f;
                self->actor.world.pos.z += 1.5f;
            } else {
                self->actor.world.pos.x -= 1.5f;
                self->actor.world.pos.z -= 1.5f;
            }

            return;
        }
    } else {
        EnDodojr_SetupCrawlTowardsTarget(self);
        self->actionFunc = FUN_003b7d80;
    }
}

void FUN_00194634(EnDodojr* self, PlayState* play) {
    self->rootScale = 1.2f;
    self->rootScale *= ((f32)ColorFilterTimer(self) / 8);
    FUN_0035fb14(&self->actor);

    if (FUN_0035d0bc(self, play)) {
        self->timer = 90;
        Animation_Change_00375c08(&self->skelAnime, 4, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 4), 0, 0.0f);
        self->actor.velocity.y = 0.0f;
        self->actor.gravity = -0.8f;
        self->counter = 7;
        self->actionFunc = FUN_0019454c;
    }
}

void FUN_003b7cb4(EnDodojr* self, PlayState* play) {
    Math_SmoothStepToS(&self->actor.shape.rot.x, 0x4000, 4, 1000, 100);

    if (DECR(self->timer) != 0) {
        f32 burrowStep;
        s32 t;

        if (self->timer > 0) {
            t = (s32)((self->timer * 2.0f / 3.0f) + 0.5f);
        } else {
            t = (s32)((self->timer * 2.0f / 3.0f) - 0.5f);
        }
        burrowStep = (30 - t) / 30.0f;

        self->actor.world.pos.y = self->actor.home.pos.y - (60.0f * burrowStep);
    } else {
        Actor_Kill(&self->actor);
    }

    FUN_003179d0(self, play, 3);
}

void FUN_00104648(EnDodojr* self, PlayState* play) {
    if (DECR(self->freezeFrameTimer) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000291);
        self->actor.flags &= ~1;
        Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 3), 2, -10.0f);
        self->actor.speedXZ = 0.0f;
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.z = 0.0f;
        self->actor.gravity = -0.8f;

        if (self->counter == 0) {
            self->actor.velocity.y = 10.0f;
            self->counter = 3;
        }

        self->actionFunc = FUN_0013336c;
    }
}

void FUN_001c429c(EnDodojr* self, PlayState* play) {
    if (DECR(self->timer) == 0) {
        Audio_PlayActorSound2(&self->actor, 0x01000100);
        Actor_SetColorFilter(&self->actor, 0x400000, 200, 0, 8);
        self->actor.flags &= ~1;
        Animation_Change_00375c08(&self->skelAnime, 3, 1.0f, 0.0f, Animation_GetLastFrame(&self->skelAnime, 3), 2, -10.0f);
        self->actor.speedXZ = 0.0f;
        self->actor.velocity.x = 0.0f;
        self->actor.velocity.z = 0.0f;
        self->actor.gravity = -0.8f;

        if (self->counter == 0) {
            self->actor.velocity.y = 10.0f;
            self->counter = 3;
        }

        self->actionFunc = FUN_00194634;
    }
}

static void EnDodojr_DeathSequenceBody(EnDodojr* self, PlayState* play) {
    if (self->counter != 0) {
        if (ColorFilterTimer(self) == 0) {
            Actor_SetColorFilter(&self->actor, 0x400000, 200, 0, self->counter);
            self->counter--;
        }
    } else {
        Actor* bomb = Actor_Spawn(&play->actorCtx, play, 0x10, self->actor.world.pos.x, self->actor.world.pos.y,
                                  self->actor.world.pos.z, 0, 0, 0, 4, 1);

        if (bomb != NULL) {
            if ((bomb->id == 0x10) || (bomb->id == 0x4C)) {
                *(s16*)((u8*)bomb + 0x26C) = 0;
            } else if (bomb->id == 0xDA) {
                *(s16*)((u8*)bomb + 0x1A8) = 0;
            }
        }

        self->timer = 12;
        self->actionFunc = FUN_003b7c64;
    }
}

void FUN_00133434(EnDodojr* self, PlayState* play) {
    EnDodojr_DeathSequenceBody(self, play);
}

void FUN_0019454c(EnDodojr* self, PlayState* play) {
    EnDodojr_DeathSequenceBody(self, play);
}

void FUN_00219450(EnDodojr* self, PlayState* play) {
    f32 lastFrame = Animation_GetLastFrame(&self->skelAnime, 1);
    Player* player = GET_PLAYER(play);
    f32 dist;

    if (!(self->actor.xzDistToPlayer >= 320.0f)) {
        dist = self->actor.world.pos.y - player->actor.world.pos.y;

        if (!(dist >= 40.0f)) {
            Animation_Change_00375c08(&self->skelAnime, 1, 1.8f, 0.0f, lastFrame, 1, -10.0f);
            Audio_PlayActorSound2(&self->actor, 0x01000294);
            self->actor.flags |= 1;
            self->actor.world.pos.y -= 60.0f;
            self->actor.world.rot.x -= 0x4000;
            self->actor.shape.rot.x = self->actor.world.rot.x;
            self->dustPos = self->actor.world.pos;
            self->dustPos.y = self->actor.floorHeight;
            self->actionFunc = FUN_003d05e4;
        }
    }
}

// The color initializers live in the ROM's rodata (0x004D9380: {170,130,90,255}, {100,60,20,0}); referenced via an EQU
// in data_syms.s until the TU's constdata can be pinned there (debt).
typedef struct __attribute__((aligned(4))) EnDodojrDustColors {
    Color_RGBA8 prim;
    Color_RGBA8 env;
} EnDodojrDustColors;
extern const EnDodojrDustColors EnDodojr_sLargeDustColors;

void FUN_003179d0(EnDodojr* self, PlayState* play, s32 count) {
    Color_RGBA8 prim = EnDodojr_sLargeDustColors.prim;
    Color_RGBA8 env = EnDodojr_sLargeDustColors.env;
    Vector3f velocity(0.0f, 0.0f, 0.0f);
    Vector3f accel(0.0f, 0.3f, 0.0f);
    Vec3f pos;
    s16 angle = ((Rand_ZeroOne() - 0.5f) * 65536.0f);
    s32 i;

    pos.y = self->dustPos.y;

    for (i = count; i >= 0; i--, angle += (s16)(0x10000 / count)) {
        accel.x = (Rand_ZeroOne() - 0.5f) * 4.0f;
        accel.z = (Rand_ZeroOne() - 0.5f) * 4.0f;

        pos.x = (Math_SinS(angle) * 22.0f) + self->dustPos.x;
        pos.z = (Math_CosS(angle) * 22.0f) + self->dustPos.z;

        FUN_00365d20(play, &pos, (Vec3f*)&velocity, (Vec3f*)&accel, &prim, &env, 120, 40, 10);
    }
}

}
