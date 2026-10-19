extern "C" {
#include <math.h>
#include "z_en_wood02.hpp"
#include "model_manager.hpp"

#define FLAGS 0x80000000

void EnWood02_Init(Actor* thisx, PlayState* play);
void EnWood02_Destroy(Actor* thisx, PlayState* play);
void EnWood02_Update(Actor* thisx, PlayState* play);
void EnWood02_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Wood02.data.s")



void* ZAR_GetCMBByIndex(ZARInfo* zar, s32 index);
f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor, Vec3f* pos);

void FUN_00368cc0(PlayState* play, Vec3f* pos, Vec3f* out, f32* w);
void FUN_0034e6d0(Actor* thisx, PlayState* play);
void Item_DropCollectibleRandom(PlayState* play, Actor* actor, Vec3f* pos, s32 params);

ActorInit En_Wood02_InitVars = {
    ACTOR_EN_WOOD02,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_WOOD02,
    sizeof(EnWood02),
    (ActorFunc)EnWood02_Init,
    (ActorFunc)EnWood02_Destroy,
    (ActorFunc)EnWood02_Update,
    (ActorFunc)EnWood02_Draw,
};

GLOBAL_ASM("asm/EnWood02_Init.s")

void EnWood02_Destroy(Actor* thisx, PlayState* play) {
    EnWood02* self = (EnWood02*)thisx;

    if (self->actor.params < 0xB) {
        Collider_DestroyCylinder(play, &self->collider);
    }
    if (self->model != NULL) {
        self->model->Destroy();
    }
    self->model = NULL;
    if (self->model2 != NULL) {
        self->model2->Destroy();
    }
    self->model2 = NULL;
}

GLOBAL_ASM("asm/EnWood02_Update.s")

void EnWood02_Draw(Actor* thisx, PlayState* play) {
    EnWood02* self = (EnWood02*)thisx;
    MTX34 mtx;
    s16 type;
    u8 red;
    u8 green;
    u8 blue;

    s32 x;
    s32 y;
    s32 z;

    MTX34CopyAsm(&mtx, &self->actor.modelMtx);
    if (FUN_0033caf8(GET_PLAYER(play))) {
        x = self->actor.home.pos.x;
        y = self->actor.home.pos.y;
        z = self->actor.home.pos.z;
        if ((x == 0xF5F) && (y == -0x74) && (z == 0x1887)) {
            return;
        }
    }

    type = self->actor.params;
    if ((type == 8) || (type == 9) || (type == 5) || (type == 0x17)) {
        red = 50;
        green = 170;
        blue = 70;
    } else if ((type == 6) || (type == 7) || (type == 0x18)) {
        red = 180;
        green = 155;
        blue = 0;
    } else {
        red = green = blue = 255;
    }

    if ((type == 0x17) || (type == 0x18)) {
        FUN_003695cc((SkeletonAnimationModel*)self->model, 0, 4, 0, red * (1.0f / 255.0f), green * (1.0f / 255.0f),
                     blue * (1.0f / 255.0f), 127.0f / 255.0f);
        ((SkeletonAnimationModel*)self->model)->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->model)->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)self->model)->FUN_00372170(0);
    } else {
        ((SkeletonAnimationModel*)self->model2)->SetUnkAC(1);
        ((SkeletonAnimationModel*)self->model2)->FUN_003721e0(&mtx);
        ((SkeletonAnimationModel*)self->model2)->FUN_00372170(0);
    }
}

GLOBAL_ASM("asm/FUN_0034e6d0.s")

}

// ---------------------------------------------------------------------------------------------------------------
// Drafts (G65 Sonnet), kept out of the build. State: EnWood02_Init 3 words off, EnWood02_Update 7 words off,
// FUN_0034e6d0 far (~108 words). Details in ws/En_Wood02/STATUS.md.
// The draft C uses real statics for the whole data chunk 0x530424..0x5304AC in ROM order (armcc then derives the table
// bases from one literal in Init, `sub r5,r7,#0x7c`). To enable it: drop the data GLOBAL_ASM line, the
// `z_En_Wood02_ASM.data` ld region and the data_chunks.txt row (the chunk is fully replaced by these statics).
// The 6-entry (12-byte) s16 array keeps ROM order under --data-reorder; a 4-entry one is sorted first.
// ---------------------------------------------------------------------------------------------------------------
#if 0
typedef struct EnWood02Head {
    u32 initChain;
    f32 spawnCos;
    f32 spawnSin;
} EnWood02Head;

// Data of the original TU, in ROM order, right after InitVars (ROM derives the table bases from one literal).
static EnWood02Head EnWood02_sHead = { 0x15E00A0C, 0.0f, 0.0f };
static u32 EnWood02_sCmbIndices[8] = { 4, 7, 0xA, 0, 2, 2, 0xD, 8 };
static u32 EnWood02_sCylinderInit[14] = { 0x390D000D, 0x00000110, 0x00000005, 0x00000000, 0x00000000,
                                          0x0FC0074A, 0x00000000, 0x00010100, 0x41900000, 0x43340000,
                                          0x00000000, 0x00000000, 0x00000000, 0x00000000 };
static f32 EnWood02_sSpawnDistance[6] = { 707.0f, 525.0f, 510.0f, 500.0f, 566.0f, 141.0f };
// 6 entries in ROM; the last two (0xD6C3, 0xA000) live in the asm tail.
static s16 EnWood02_sSpawnAngle[6] = { 0x1FFF, 0x4C9E, 0x77F5, 0xA5C9, (s16)0xD6C3, (s16)0xA000 };
#define EnWood02_sSpawnCos EnWood02_sHead.spawnCos
#define EnWood02_sSpawnSin EnWood02_sHead.spawnSin
#define EnWood02_sInitChain ((InitChainEntry*)&EnWood02_sHead)
#define EnWood02_sCylinderInitPtr ((ColliderCylinderInit*)EnWood02_sCylinderInit)


void EnWood02_Init(Actor* thisx, PlayState* play2) {
    s16 spawnType;
    f32 actorScale;
    PlayState* play = play2;
    EnWood02* self = (EnWood02*)thisx;
    CollisionPoly* poly;
    s32 bgId;
    f32 floorY;
    s16 extraRot;
    ZARInfo* zar;
    ModelAllocator* alloc;
    s32 drawIdx;

    spawnType = 0;
    actorScale = 1.0f;
    self->unk_14C = (self->actor.params >> 8) & 0xFF;

    if (self->actor.home.rot.z != 0) {
        self->actor.home.rot.z = (self->actor.home.rot.z << 8) | self->unk_14C;
        self->unk_14C = -1;
        self->actor.world.rot.z = self->actor.shape.rot.z = 0;
    } else if (self->unk_14C & 0x80) {
        self->unk_14C = -1;
    }

    if (play->sceneNum == 0x52) {
        self->actor.flags &= ~0x80000000;
    }

    {
        s16 params = self->actor.params;
        self->unk_208 = params & 0x80;
        self->actor.params = params & 0x7F;
    }
    Actor_ProcessInitChain(&self->actor, EnWood02_sInitChain);

    if (self->actor.params < 0xB) {
        Collider_InitCylinder(play, &self->collider);
        Collider_SetCylinder(play, &self->collider, &self->actor, EnWood02_sCylinderInitPtr);
    }

    switch (self->actor.params) {
        case 15:
        case 21:
            spawnType = 1;
        case 16:
        case 22:
            spawnType++;
        case 0:
        case 12:
        case 18:
            actorScale = 1.5f;
            self->actor.uncullZoneForward = 4400.0f;
            self->actor.uncullZoneScale = 2000.0f;
            self->actor.uncullZoneDownward = 2400.0f;
            break;
        case 3:
        case 6:
        case 8:
        case 13:
        case 19:
            spawnType = 1;
        case 4:
        case 7:
        case 9:
        case 14:
        case 20:
            spawnType++;
        case 1:
        case 5:
        case 10:
        case 11:
        case 17:
            self->actor.uncullZoneForward = 4400.0f;
            self->actor.uncullZoneScale = 800.0f;
            self->actor.uncullZoneDownward = 1800.0f;
            break;
        case 2:
            actorScale = 0.6f;
            self->actor.uncullZoneForward = 4400.0f;
            self->actor.uncullZoneScale = 400.0f;
            self->actor.uncullZoneDownward = 1000.0f;
            break;
        case 23:
        case 24:
            self->unk_14E[0] = 0x4B;
            actorScale = 0.02f;
            self->actor.velocity.x = Rand_CenteredFloat(6.0f);
            self->actor.velocity.z = Rand_CenteredFloat(6.0f);
            self->actor.velocity.y = (Rand_ZeroOne() * 1.25f) + -3.1f;
            break;
    }

    if (self->actor.params < 5) {
        self->drawType = 0;
    } else if (self->actor.params < 10) {
        self->drawType = 1;
        if (self->unk_208 != 0) {
            self->drawType = 7;
        }
    } else {
        u8 t;

        if (self->actor.params < 11) {
            t = 2;
        } else if (self->actor.params < 17) {
            t = 3;
        } else if (self->actor.params < 24) {
            t = 4;
        } else {
            t = 5;
        }
        self->drawType = t;
    }

    Actor_SetScale(&self->actor, actorScale);

    if (self->actor.params == 2 && play->sceneNum >= 0x20 && play->sceneNum <= 0x22) {
        self->actor.scale.x = 0.3f;
        self->actor.scale.y = 0.65f;
        self->actor.scale.z = 0.3f;
    }

    self->spawnType = spawnType;

    if (spawnType != 0) {
        extraRot = 0;

        if (self->actor.params == 15) {
            extraRot = 0x4000;
        }

        if (spawnType == 2) {
            self->drawType |= self->unk_14C << 4;
            FUN_0034e6d0(&self->actor, play);
            EnWood02_sSpawnCos = Math_CosS(self->actor.world.rot.y + EnWood02_sSpawnAngle[5] + extraRot);
            EnWood02_sSpawnSin = Math_SinS(self->actor.world.rot.y + EnWood02_sSpawnAngle[5] + extraRot);
            self->actor.world.pos.x += EnWood02_sSpawnSin * EnWood02_sSpawnDistance[5];
            self->actor.world.pos.z += EnWood02_sSpawnCos * EnWood02_sSpawnDistance[5];
        } else {
            self->actor.flags |= 0x10;
        }

        thisx->world.pos.y += 200.0f;
        floorY = BgCheck_EntityRaycastFloor4(&play->colCtx, &poly, &bgId, &self->actor, &self->actor.world.pos);

        if (floorY > -32000.0f) {
            thisx->world.pos.y = floorY;
        } else {
            Actor_Kill(&self->actor);
            return;
        }
    }

    thisx->unk_19A = 1;

    alloc = GetModelManager().allocator;
    alloc->owner = self->actor.unk_178;
    zar = &play->GetObjectStatus(self->actor.objBankIndex)->zarInfo;
    self->model = NULL;
    self->model2 = NULL;
    drawIdx = self->drawType & 0xF;
    if ((self->actor.params == 0x17) || (self->actor.params == 0x18)) {
        self->model = (EnWood02Model*)alloc->Create(ZAR_GetCMBByIndex(zar, 0xD), 1);
    } else {
        self->model2 = (EnWood02Model*)alloc->Create(ZAR_GetCMBByIndex(zar, EnWood02_sCmbIndices[drawIdx]), 1);
    }
    alloc->owner = NULL;

    ActorShape_Init(&self->actor.shape, 0.0f, NULL, 0.0f);
    self->actor.home.rot.y = 0;
    self->actor.colChkInfo.mass = 0xFF;
}

void EnWood02_Update(Actor* thisx, PlayState* play2) {
    PlayState* play = play2;
    EnWood02* self = (EnWood02*)thisx;
    EnWood02* s2 = self;
    f32 wobbleAmplitude;

    if ((self->spawnType == 1) && (self->actor.parent != NULL)) {
        if (!(self->actor.flags & 0x40) &&
            !(self->actor.unk_EC.z < self->actor.uncullZoneForward * (1.0f / 3.0f))) {
            u8 idx = self->unk_14E[0];
            u8 val = 0;

            if (self->unk_14C < 0) {
                val = 0x80;
            }
            ((EnWood02*)self->actor.parent)->unk_14E[idx] = val;
            Actor_Kill(&self->actor);
            return;
        }
    } else if (self->spawnType == 2) {
        FUN_0034e6d0(&self->actor, play);
    }

    if (self->actor.params < 0xB) {
        if (self->collider.base.acFlags & AC_HIT) {
            self->collider.base.acFlags &= ~AC_HIT;
            Audio_PlayActorSound2(&self->actor, 0x01000129);
        }

        if (self->actor.home.rot.y != 0) {
            Vec3f dropsSpawnPt = self->actor.world.pos;

            dropsSpawnPt.y += 200.0f;

            if ((self->unk_14C >= 0) && (self->unk_14C < 0x64)) {
                Item_DropCollectibleRandom(play, &self->actor, &dropsSpawnPt, (s16)(self->unk_14C << 4));
            } else {
                if (self->actor.home.rot.z != 0) {
                    self->actor.home.rot.z |= 0xE000;
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_SW, dropsSpawnPt.x, dropsSpawnPt.y, dropsSpawnPt.z, 0,
                                self->actor.world.rot.y, 0, self->actor.home.rot.z, 1);
                    self->actor.home.rot.z = 0;
                }
            }

            if (self->unk_14C >= -1) {
                s32 i;
                s32 leavesParams = 0x17;

                if ((self->actor.params == 6) || (self->actor.params == 7)) {
                    leavesParams = 0x18;
                }
                Audio_PlayActorSound2(&self->actor, 0x010001CE);

                for (i = 3; i >= 0; i--) {
                    Actor_Spawn(&play->actorCtx, play, ACTOR_EN_WOOD02, dropsSpawnPt.x, dropsSpawnPt.y, dropsSpawnPt.z,
                                0, Rand_CenteredFloat(65535.0f), 0, leavesParams, 1);
                }
            }
            self->unk_14C = -0x15;
            thisx->home.rot.y = 0;
        }

        if (self->actor.xzDistToPlayer < 600.0f) {
            Collider_UpdateCylinder(&self->actor, &self->collider);
            CollisionCheck_SetAC(play, &play->colChkCtx, &s2->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
        }
    } else if (self->actor.params < 0x17) {
        Player* player = GET_PLAYER(play);

        if (self->unk_14C >= -1) {
            if (((player->rideActor == NULL) && (__sqrtf(thisx->xyzDistToPlayerSq) < 20.0f) &&
                 (player->xzSpeed != 0.0f)) ||
                ((player->rideActor != NULL) && (__sqrtf(self->actor.xyzDistToPlayerSq) < 60.0f) &&
                 (player->rideActor->speedXZ != 0.0f))) {
                if ((self->unk_14C >= 0) && (self->unk_14C < 0x64)) {
                    Item_DropCollectibleRandom(play, &self->actor, &self->actor.world.pos,
                                               (s16)(0x8000 | (self->unk_14C << 4)));
                }
                self->unk_14C = -0x15;
                Audio_PlayActorSound2(&self->actor, 0x010001CE);
            }
        }
    } else {
        self->unk_14C++;
        Math_ApproachF(&self->actor.velocity.x, 0.0f, 1.0f, 0.05f);
        Math_ApproachF(&self->actor.velocity.z, 0.0f, 1.0f, 0.05f);
        FUN_0036b96c(&self->actor);
        self->actor.shape.rot.z = Math_SinS(3000 * self->unk_14C) * 16384.0f;
        self->unk_14E[0]--;

        if (self->unk_14E[0] == 0) {
            Actor_Kill(&self->actor);
        }
    }

    if (self->unk_14C < -1) {
        self->unk_14C++;
        wobbleAmplitude = Math_SinS((self->unk_14C ^ 0xFFFF) * 0x3332) * 250.0f;
        self->actor.shape.rot.x = (Math_CosS(self->actor.yawTowardsPlayer - self->actor.shape.rot.y) * wobbleAmplitude);
        self->actor.shape.rot.z = (Math_SinS(self->actor.yawTowardsPlayer - self->actor.shape.rot.y) * wobbleAmplitude);
    }
}

static s32 EnWood02_SpawnZoneCheck(EnWood02* self, PlayState* play, Vec3f* pos) {
    f32 invW;

    FUN_00368cc0(play, pos, &self->actor.unk_EC, &self->actor.unk_F8);
    if (self->actor.unk_F8 == 0.0f) {
        invW = 1000.0f;
    } else {
        invW = fabsf(1.0f / self->actor.unk_F8);
    }
    if ((-self->actor.uncullZoneScale < self->actor.unk_EC.z) &&
        (self->actor.unk_EC.z < self->actor.uncullZoneForward + self->actor.uncullZoneScale) &&
        (((fabsf(self->actor.unk_EC.x) - self->actor.uncullZoneScale) * invW) < 1.0f) &&
        (((self->actor.unk_EC.y + self->actor.uncullZoneDownward) * invW) > -1.0f) &&
        (((self->actor.unk_EC.y - self->actor.uncullZoneScale) * invW) < 1.0f)) {
        return 1;
    }
    return 0;
}

void FUN_0034e6d0(Actor* thisx, PlayState* play) {
    EnWood02* self = (EnWood02*)thisx;
    Vec3f childPos;
    EnWood02* child;
    s16* childSpawnAngle;
    s16 extraRot;
    s16 childParams;
    f32* dist;
    f32 d;
    s32 i;

    for (i = 4; i >= 0; i--) {
        if ((self->unk_14E[i] & 0x7F) == 0) {
            extraRot = 0;
            if (self->actor.params == 0xF) {
                extraRot = 0x4000;
            }
            childSpawnAngle = &EnWood02_sSpawnAngle[i];
            EnWood02_sSpawnCos = Math_CosS(self->actor.world.rot.y + *childSpawnAngle + extraRot);
            EnWood02_sSpawnSin = Math_SinS(self->actor.world.rot.y + *childSpawnAngle + extraRot);
            dist = &EnWood02_sSpawnDistance[i];
            childPos.x = self->actor.home.pos.x + EnWood02_sSpawnSin * d;
            childPos.y = self->actor.home.pos.y;
            childPos.z = self->actor.home.pos.z + EnWood02_sSpawnCos * d;

            s32 inZone = EnWood02_SpawnZoneCheck(self, play, &childPos);
            if (!inZone) {
                if (fabsf(self->actor.unk_EC.z) + d < self->actor.uncullZoneForward * (1.0f / 3.0f)) {
                    inZone = 1;
                }
            }
            if (inZone || FUN_0033caf8(GET_PLAYER(play))) {
                if ((self->unk_14E[i] & 0x80) != 0) {
                    childParams = 0xFF00 | (self->actor.params + 1);
                } else {
                    childParams = ((self->drawType & 0xF0) << 4) | (self->actor.params + 1);
                }
                childParams |= self->unk_208;
                child = (EnWood02*)Actor_SpawnAsChild(&play->actorCtx, &self->actor, play, ACTOR_EN_WOOD02,
                                                      childPos.x, childPos.y, childPos.z, self->actor.world.rot.x,
                                                      *childSpawnAngle, 0, childParams);
                if (child != NULL) {
                    child->unk_14E[0] = i;
                    self->unk_14E[i] |= 1;
                    child->actor.unk_EC = self->actor.unk_EC;
                    if (self->actor.unk_EC.z + *dist < self->actor.uncullZoneForward * (1.0f / 3.0f)) {
                        child->actor.unk_19E[0] = 10;
                    }
                } else {
                    self->unk_14E[i] &= 0x80;
                }
            }
        }
    }
}
#endif
