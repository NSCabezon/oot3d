extern "C" {
#include "z_en_fire_rock.hpp"
#include "actors/En_Encount2/z_en_encount2.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000030

void EnFireRock_Init(Actor* thisx, PlayState* play);
void EnFireRock_Destroy(Actor* thisx, PlayState* play);
void EnFireRock_Update(Actor* thisx, PlayState* play);
void EnFireRock_Draw(Actor* thisx, PlayState* play);
void FUN_003eda98(EnFireRock* self, PlayState* play);
void FUN_0010cb10(EnFireRock* self, PlayState* play);
void FUN_003a30b8(PlayState* play, Actor* actor, s16 a2, u8 a3, f32 f0, f32 f1); // z_en_fire_rock_knockback.cpp

GLOBAL_ASM("data/z_En_Fire_Rock.data.s")

GLOBAL_ASM("../src/game/actors/En_Fire_Rock/data_syms.s")

ActorInit En_Fire_Rock_InitVars = {
    ACTOR_EN_FIRE_ROCK,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_EFC_STAR_FIELD,
    sizeof(EnFireRock),
    (ActorFunc)EnFireRock_Init,
    (ActorFunc)EnFireRock_Destroy,
    (ActorFunc)EnFireRock_Update,
    (ActorFunc)EnFireRock_Draw,
};

void EffectSsEnFire_SpawnVec3f(PlayState* play, Actor* actor, Vec3f* pos, s16 scale, s16 params, s16 flags,
                               s16 bodyPart, s32 unk);
void FUN_0036f00c(PlayState* play, Actor* actor, Vec3f* pos, f32 a, s32 b, f32 c, s32 d, s32 e, s32 f); // Actor_SpawnFloorDustRing
void FUN_0036fca8(Actor* actor, PlayState* play, s32 quakeIndex, s32 rumbleIndex); // Actor_RequestQuakeAndRumble

#define FIRE_ROCK_FLAME_EXTRA(play) \
    ((((play)->sceneNum == 0xE) || ((play)->sceneNum == 0xF) || ((play)->sceneNum == 0x1A)) ? 2 : 1)

// Moved from src/functions/functions_3E0000s.cpp. N64 EnFireRock_Fall.
void FUN_003eda98(EnFireRock* self, PlayState* play) {
    Player* player;
    Vec3f flamePos;
    s32 i;
    PlayState* play2 = play;

    player = *(Player**)((u8*)play2 + 0x20AC); // GET_PLAYER
    if ((self->actor.floorHeight == -10000.0f) || ((player->actor.world.pos.y - 200.0f) > self->actor.world.pos.y)) {
        Actor_Kill(&self->actor);
        return;
    }
    switch (self->type) {
        case 0:
            if (player->actor.world.pos.y < self->actor.world.pos.y) {
                if ((player->actor.world.pos.x > -700.0f) || (player->actor.world.pos.x < 100.0f) ||
                    (player->actor.world.pos.z > -1290.0f) || (player->actor.world.pos.z < -3880.0f)) {
                    Math_ApproachF(&self->actor.world.pos.x, player->actor.world.pos.x, 1.0f, 10.0f);
                    Math_ApproachF(&self->actor.world.pos.z, player->actor.world.pos.z, 1.0f, 10.0f);
                }
            }
            /* fallthrough */
        case 3:
            if (DECR(self->flameTimer) == 0) {
                flamePos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                flamePos.y = Rand_CenteredFloat(20.0f) + self->actor.world.pos.y;
                flamePos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                EffectSsEnFire_SpawnVec3f(play, &self->actor, &flamePos, 100, 0, 0, -1, FIRE_ROCK_FLAME_EXTRA(play));
                self->flameTimer = 10;
            }
            break;
        case 1:
            break;
    }
    if ((self->actor.bgCheckFlags & 1) && (self->timer == 0)) {
        switch (self->type) {
            case 0:
            case 3:
                FUN_0036fca8(&self->actor, play, 5, 2);
                /* fallthrough */
            case 1:
                FUN_0036f00c(play, &self->actor, &self->actor.world.pos, self->actor.shape.shadowScale, 1, 8.0f, 500, 10,
                             0);
                for (i = 0; i < 2; i++) {
                    flamePos.x = Rand_CenteredFloat(20.0f) + self->actor.world.pos.x;
                    flamePos.y = self->actor.floorHeight;
                    flamePos.z = Rand_CenteredFloat(20.0f) + self->actor.world.pos.z;
                    EffectSsEnFire_SpawnVec3f(play, &self->actor, &flamePos, 300, 0, 0, -1,
                                              FIRE_ROCK_FLAME_EXTRA(play));
                }
                self->actionFunc = FUN_0010cb10;
                break;
            default:
                FUN_0036f00c(play, &self->actor, &self->actor.world.pos, self->actor.shape.shadowScale, 3, 8.0f, 200, 10,
                             0);
                Audio_PlaySoundAtPosition(play, &self->actor.world.pos, 40, 0x0100024E);
                Actor_Kill(&self->actor);
                break;
        }
    }
}

// Moved from src/functions/functions_100000s.cpp (called from EnFireRock_Update). N64 EnFireRock_SpawnMoreBrokenPieces.
void FUN_0010cb10(EnFireRock* self, PlayState* play) {
    EnFireRock* spawnedFireRock;
    s32 nextRockType;
    s32 i;

    if (self->type == 0) {
        nextRockType = 1;
    } else if (self->type == 1) {
        nextRockType = 2;
    } else if (self->type == 3) {
        nextRockType = 1;
    } else {
        Actor_Kill(&self->actor);
        return;
    }

    for (i = 0; i < 2; i++) {
        spawnedFireRock = (EnFireRock*)Actor_Spawn(
            &play->actorCtx, play, ACTOR_EN_FIRE_ROCK, Rand_CenteredFloat(3.0f) + self->actor.world.pos.x,
            Rand_CenteredFloat(3.0f) + (self->actor.world.pos.y + 10.0f),
            Rand_CenteredFloat(3.0f) + self->actor.world.pos.z, 0, 0, 0, (s16)nextRockType, 1);
        if (spawnedFireRock != NULL) {
            spawnedFireRock->actor.world.rot.y = self->actor.world.rot.y;
            if (i == 0) {
                spawnedFireRock->actor.shape.rot.y = self->actor.shape.rot.y;
            }
            spawnedFireRock->scale = self->scale - 0.01f;
        }
    }
    if (spawnedFireRock != NULL) {
        Audio_PlayActorSound2(&spawnedFireRock->actor, 0x01000347);
    }
    Actor_Kill(&self->actor);
}

void FUN_001e7cd8(EnFireRock* self, PlayState* play); // N64 FireRock_WaitSpawnRocksFromCeiling (still asm)
void FUN_0019af78(EnFireRock* self, PlayState* play); // N64 FireRock_WaitOnFloor (still asm)

// Init: 36/238 words off (structure and jump table match; only the hoisted-constant VFP numbering differs: ROM
// s18=0.01 s19=0.02 s20=65535 s21=3 s22=4 s23=1 s24=2, ours rotated). Draft kept as asm; needs the EQUs in data_syms.s.
#if 0
extern ColliderCylinderInit EnFireRock_sColInitA[]; // 0x00523710 (EQU in data_syms.s): D_80A12CA0 (falling rocks)
extern ColliderCylinderInit EnFireRock_sColInitB[]; // 0x00523748 (EQU in data_syms.s): D_80A12CCC (floor rock)

void EnFireRock_Init(Actor* thisx, PlayState* play) {
    Player* player = *(Player**)((u8*)play + 0x20AC); // GET_PLAYER
    EnFireRock* self = (EnFireRock*)thisx;
    EnFireRock* s2 = self;
    PlayState* play2 = play;

    self->type = self->actor.params;
    if (self->type != 5) {
        ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 15.0f);
        if (self->type != 6) {
            self->angularVelocity.x = (Rand_ZeroFloat(10.0f) + 15.0f);
            self->angularVelocity.y = (Rand_ZeroFloat(10.0f) + 15.0f);
            self->angularVelocity.z = (Rand_ZeroFloat(10.0f) + 15.0f);
        }
    }
    FUN_00372f38(thisx, play, &self->skelAnimModel, (Rand_ZeroOne() < 0.5f) + 2, 0);
    self->actor.unk_19B = 3;
    switch (self->type) {
        case 5:
            self->actor.draw = NULL;
            self->actionFunc = FUN_001e7cd8;
            break;
        case 6:
            Actor_SetScale(&self->actor, 0.03f);
            Collider_InitCylinder(play2, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->actor, EnFireRock_sColInitB);
            self->collider.dim.radius = 23;
            self->collider.dim.height = 37;
            self->collider.dim.yShift = -10;
            Actor_ChangeType(play, &play->actorCtx, &self->actor, 6);
            self->actor.colChkInfo.mass = MASS_IMMOVABLE;
            self->actionFunc = FUN_0019af78;
            break;
        case 0:
            self->unk_1D4.x = (f32)(Rand_CenteredFloat(50.0f) + player->actor.world.pos.x);
            self->unk_1D4.z = (f32)(Rand_CenteredFloat(50.0f) + player->actor.world.pos.z);
            /* fallthrough */
        case 3:
            self->scale = (Rand_ZeroFloat(2.0f) / 100.0f) + 0.02f;
            Actor_SetScale(&self->actor, self->scale);
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->actor, EnFireRock_sColInitA);
            self->actor.world.rot.y = self->actor.shape.rot.y = Rand_CenteredFloat(65535.0f);
            self->actor.shape.shadowScale = 15.0f;
            self->actionFunc = FUN_003eda98;
            break;
        case 1:
            self->actor.velocity.y = Rand_ZeroFloat(3.0f) + 4.0f;
            self->actor.speedXZ = Rand_ZeroFloat(3.0f) + 3.0f;
            self->scale = (Rand_ZeroFloat(1.0f) / 100.0f) + 0.02f;
            Actor_SetScale(&self->actor, self->scale);
            self->actor.gravity = -1.5f;
            Collider_InitCylinder(play, &self->collider);
            Collider_SetCylinder(play, &self->collider, &self->actor, EnFireRock_sColInitA);
            self->actor.shape.shadowScale = 10.0f;
            s2->actor.world.rot.y = self->actor.shape.rot.y = Rand_CenteredFloat(65535.0f);
            self->actionFunc = FUN_003eda98;
            break;
        case 2:
            self->actor.velocity.y = Rand_ZeroFloat(3.0f) + 4.0f;
            self->actor.speedXZ = Rand_ZeroFloat(3.0f) + 2.0f;
            self->scale = (Rand_ZeroFloat(1.0f) / 500.0f) + 0.01f;
            Actor_SetScale(&self->actor, self->scale);
            self->actor.gravity = -1.2f;
            self->actor.shape.shadowScale = 5.0f;
            self->actor.world.rot.y = self->actor.shape.rot.y = Rand_CenteredFloat(65535.0f);
            self->actionFunc = FUN_003eda98;
            break;
        default:
            Actor_Kill(&self->actor);
            break;
    }
}
#endif
GLOBAL_ASM("asm/EnFireRock_Init.s")

void EnFireRock_Destroy(Actor* thisx, PlayState* play) {
    EnFireRock* self = (EnFireRock*)thisx;

    if ((self->actor.parent != NULL) && (self->actor.parent == &self->spawner->actor)) {
        EnEncount2* spawner = (EnEncount2*)self->actor.parent;
        if ((spawner->actor.update != NULL) && (*(s16*)&spawner->numSpawnedRocks > 0)) {
            *(s16*)&spawner->numSpawnedRocks -= 1; // N64 type is s16 (header says u16)
        }
    }
    FUN_003508b8(thisx, self->skelAnimModel, 0);
    Collider_DestroyCylinder(play, &self->collider);
}

void EnFireRock_Update(Actor* thisx, PlayState* play) {
    EnFireRock* self = (EnFireRock*)thisx;
    s16 setCollision;
    Player* player = *(Player**)((u8*)play + 0x20AC); // GET_PLAYER
    Actor* playerActor = &(*(Player**)((u8*)play + 0x20AC))->actor;
    f32 temp;

    if (self->timer2 != 0) {
        self->timer2--;
    }
    if (self->timer != 0) {
        self->timer--;
    }
    self->actionFunc(self, play);

    if (self->type != 5) {
        self->rockRotation.x += self->angularVelocity.x;
        self->rockRotation.y += self->angularVelocity.y;
        self->rockRotation.z += self->angularVelocity.z;
        self->relativePos.y = 3.0f;

        temp = 10.0f + (self->scale * 300.0f);
        self->actor.shape.shadowScale = temp;
        if (thisx->shape.shadowScale < 10.0f) {
            thisx->shape.shadowScale = 10.0f;
        }
        if (thisx->shape.shadowScale > 20.0f) {
            thisx->shape.shadowScale = 20.0f;
        }

        if ((self->type == 0) || (self->type == 3)) {
            self->actor.gravity = -0.3f - (self->scale * 7.0f);
        }
        if (self->type != 6) {
            PlayState* play2 = play;

            Actor_MoveForward(thisx);
            Actor_UpdateBgCheckInfo(play2, thisx, 50.0f, 50.0f, 100.0f, 0x1C);
        }

        setCollision = false;
        if (self->actionFunc != FUN_0010cb10) {
            if ((self->type == 0) || (self->type == 3) || (self->type == 1)) {
                if (self->collider.base.atFlags & AT_BOUNCED) {
                    Collider* col = &self->collider.base;

                    col->atFlags &= ~AT_BOUNCED;
                    Audio_PlayActorSound2(thisx, 0x01000165);
                    thisx->speedXZ = 0.0f;
                    thisx->velocity.y = 0.0f;
                    self->actionFunc = FUN_0010cb10;
                    return;
                }
                setCollision = true;
            }
        }

        if (self->type == 6) {
            if (self->collider.base.atFlags & AT_HIT) {
                self->collider.base.atFlags &= ~AT_HIT;
                if (self->collider.base.at == playerActor) {
                    if (!(player->stateFlags1 & 0x4000000)) {
                        FUN_003a30b8(play, thisx, -player->actor.world.rot.y, 4, 2.0f, 3.0f);
                    }
                    return;
                }
            }
            setCollision = true;
        }
        if (setCollision) {
            Collider_UpdateCylinder(thisx, &self->collider);
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
            CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
        }
    }
}

extern "C++" { struct FRV { f32& operator[](s32 i) { return (&x)[i]; } f32 x, y, z; }; }

f32 __hardfp_sinf(f32 x);
f32 __hardfp_cosf(f32 x);

static inline void FR_Trans(MTX34* m, Actor* a, Vec3f* r) {
    FRV t;
    f32 y;
    f32 z;
    f32 x;

    x = a->world.pos.x + r->x;
    y = a->world.pos.y + r->y;
    z = a->world.pos.z + r->z;
    t[0] = x;
    t[1] = y;
    t[2] = z;
    MTX34_MakeTrans(m, (Vec3f*)&t);
}

void EnFireRock_Draw(Actor* thisx, PlayState* play) {
    EnFireRock* self = (EnFireRock*)thisx;
    MTX34 mtx;
    f32 angle;
    f32 s;
    f32 c;
    f32 a;
    f32 b;
    f32 zero;
    f32 deg;

    zero = 0.0f;
    deg = 0.017453292f;
    FR_Trans(&mtx, thisx, &self->relativePos);

    angle = self->rockRotation.x * deg;
    if (angle != zero) {
        s = __hardfp_sinf(angle);
        c = __hardfp_cosf(angle);
        a = mtx.data[0][1];
        b = mtx.data[0][2];
        mtx.data[0][1] = a * c + b * s;
        mtx.data[0][2] = b * c - a * s;
        a = mtx.data[1][1];
        b = mtx.data[1][2];
        mtx.data[1][1] = a * c + b * s;
        mtx.data[1][2] = b * c - a * s;
        a = mtx.data[2][1];
        b = mtx.data[2][2];
        mtx.data[2][1] = a * c + b * s;
        mtx.data[2][2] = b * c - a * s;
    }
    angle = self->rockRotation.y * deg;
    if (angle != zero) {
        s = __hardfp_sinf(angle);
        c = __hardfp_cosf(angle);
        a = mtx.data[0][0];
        b = mtx.data[0][2];
        mtx.data[0][0] = a * c - b * s;
        mtx.data[0][2] = a * s + b * c;
        a = mtx.data[1][0];
        b = mtx.data[1][2];
        mtx.data[1][0] = a * c - b * s;
        mtx.data[1][2] = a * s + b * c;
        a = mtx.data[2][0];
        b = mtx.data[2][2];
        mtx.data[2][0] = a * c - b * s;
        mtx.data[2][2] = a * s + b * c;
    }
    angle = self->rockRotation.z * deg;
    if (angle != zero) {
        s = __hardfp_sinf(angle);
        c = __hardfp_cosf(angle);
        a = mtx.data[0][0];
        b = mtx.data[0][1];
        mtx.data[0][0] = a * c + b * s;
        mtx.data[0][1] = b * c - a * s;
        a = mtx.data[1][0];
        b = mtx.data[1][1];
        mtx.data[1][0] = a * c + b * s;
        mtx.data[1][1] = b * c - a * s;
        a = mtx.data[2][0];
        b = mtx.data[2][1];
        mtx.data[2][0] = a * c + b * s;
        mtx.data[2][1] = b * c - a * s;
    }
    {
        f32 sx = thisx->scale.x;
        f32 sy = thisx->scale.y;
        f32 sz = thisx->scale.z;

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

    self->skelAnimModel->SetUnkAC(1);
    self->skelAnimModel->FUN_003721e0(&mtx);
    self->skelAnimModel->FUN_00372170(0);
}
}
