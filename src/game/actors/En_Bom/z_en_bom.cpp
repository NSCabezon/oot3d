extern "C" {
#include "z_en_bom.hpp"

#define FLAGS 0x00000030

#define BOMB_BODY 0
#define BOMB_EXPLOSION 1

typedef struct {
    f32 v[4];
} EnBomEnvColor;

void EnBom_Init(Actor* thisx, PlayState* play);
void EnBom_Destroy(Actor* thisx, PlayState* play);
void EnBom_Update(Actor* thisx, PlayState* play);
void EnBom_Draw(Actor* thisx, PlayState* play);

void EnBom_Move(EnBom* self, PlayState* play);
void EnBom_WaitForRelease(EnBom* self, PlayState* play);
void EnBom_Explode(EnBom* self, PlayState* play);

GLOBAL_ASM("data/z_En_Bom.data.s")

GLOBAL_ASM("../src/game/actors/En_Bom/data_syms.s")

extern ColliderCylinderInit EnBom_sCylinderInit;
extern InitChainEntry EnBom_sInitChain[];
extern char EnBom_sJntSphInit[];
extern EnBomEnvColor EnBom_sEnvColor;

ActorInit En_Bom_InitVars = {
    ACTOR_EN_BOM,
    ACTORCAT_EXPLOSIVES,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnBom),
    (ActorFunc)EnBom_Init,
    (ActorFunc)EnBom_Destroy,
    (ActorFunc)EnBom_Update,
    (ActorFunc)EnBom_Draw,
};

void Collider_InitJntSph(PlayState* play, void* collider);
void Collider_SetJntSph(PlayState* play, void* collider, Actor* actor, void* init, void* elements);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void Collider_UpdateSpheres(s32 index, void* collider, MTX34* mtx);
s32 Actor_HasNoParent(Actor* actor, PlayState* play);
void* FUN_003687a8(void* model);
void FUN_003589cc(void* p, s32 n);
void FUN_00358964(void* p, s32 n, f32* vec);
void FUN_00314fe4(PlayState* play, Actor* actor);
void FUN_0034df30(Actor* actor, PlayState* play);

void EnBom_Init(Actor* thisx, PlayState* play) {
    EnBom* self = (EnBom*)thisx;

    Actor_ProcessInitChain(thisx, EnBom_sInitChain);
    self->unk_284 = 0;
    self->unk_28C = *(s8*)((u8*)play + 0x4C30);
    self->unk_280 = 0;
    self->unk_27C = 0;
    if (thisx->params == 2) {
        self->unk_280 = 1;
        thisx->params = 0;
    } else if (thisx->params == 3) {
        self->unk_280 = 2;
        thisx->params = 0;
    } else if (thisx->params == 4) {
        self->unk_27C = 1;
        thisx->params = 0;
    }
    FUN_00372f38(thisx, play, &self->model, 1, 0);
    ActorShape_Init(&thisx->shape, 700.0f, ActorShadow_DrawCircle, 16.0f);
    thisx->colChkInfo.mass = 200;
    thisx->colChkInfo.cylRadius = 5;
    thisx->colChkInfo.cylHeight = 10;
    self->timer = 105;
    self->flashSpeedScale = 7;
    Collider_InitCylinder(play, &self->bombCollider);
    Collider_InitJntSph(play, &self->explosionCollider);
    Collider_SetCylinder(play, &self->bombCollider, thisx, &EnBom_sCylinderInit);
    Collider_SetJntSph(play, &self->explosionCollider, thisx, EnBom_sJntSphInit, &self->explosionColliderElements[0]);
    ((u8*)self->explosionColliderElements)[5] += (u8)((u16)thisx->shape.rot.z >> 8);
    thisx->shape.rot.z &= 0xFF;
    if (thisx->shape.rot.z & 0x80) {
        thisx->shape.rot.z |= 0xFF00;
    }
    self->actionFunc = EnBom_Move;
}

void EnBom_Destroy(Actor* thisx, PlayState* play) {
    EnBom* self = (EnBom*)thisx;

    Collider_DestroyJntSph(play, &self->explosionCollider);
    Collider_DestroyCylinder(play, &self->bombCollider);
    FUN_003508b8(&self->actor, self->model, 0);
}

void EnBom_WaitForRelease(EnBom* self, PlayState* play) {
    if (Actor_HasNoParent(&self->actor, play)) {
        if (self->unk_285 != 0) {
            self->unk_285 = 0;
            self->unk_288 = 6;
        }
        self->actionFunc = EnBom_Move;
        EnBom_Move(self, play);
    }
}

void EnBom_Draw(Actor* thisx, PlayState* play) {
    EnBom* self = (EnBom*)thisx;
    MTX34 mtx;
    EnBomEnvColor col;
    void* p;

    if (self->actor.params == BOMB_BODY) {
        MTX34CopyAsm(&mtx, &self->actor.modelMtx);
        col = EnBom_sEnvColor;
        col.v[0] = self->flashIntensity * (1.0f / 255.0f);
        p = FUN_003687a8(self->model);
        FUN_003589cc(p, 4);
        FUN_00358964(p, 4, col.v);
        self->model->SetUnkAC(1);
        self->model->FUN_003721e0(&mtx);
        self->model->FUN_00372170(0);
        Collider_UpdateSpheres(0, &self->explosionCollider, &self->actor.modelMtx);
    }
}

GLOBAL_ASM("asm/EnBom_Update.s")

#if 0
#define EL(s) (*(u8**)((u8*)(s) + 0x218))

void EnBom_Explode(EnBom* self, PlayState* play) {
    Player* player;

    if (*(f32*)(EL(self) + 0x34) == 0.0f) {
        Actor* a = &self->actor;
        a->flags = a->flags | 0x20;
    }
    if (self->unk_280 == 0) {
        f32 r = (f32)self->actor.shape.rot.z + 8.0f;
        f32 t = r * 2.0f;
        f32 w = *(f32*)(EL(self) + 0x44);
        *(f32*)(EL(self) + 0x44) = w + t * (1.0f / 3.0f);
    } else {
        f32 r = (f32)self->actor.shape.rot.z + 8.0f;
        f32 w = *(f32*)(EL(self) + 0x44);
        w = w + r * 1.5f;
        if (w < 0.0f) {
            w = 0.0f;
        } else if (w > 80.0f) {
            w = 80.0f;
        }
        *(f32*)(EL(self) + 0x44) = w;
    }
    if (self->actor.params == BOMB_EXPLOSION) {
        CollisionCheck_SetAT(play, (CollisionCheckContext*)((u8*)play + 0x5C78), (Collider*)&self->explosionCollider);
    }

    if (*(s16*)((u8*)play + 0x3202) != 0) {
        *(s16*)((u8*)play + 0x3202) -= 25;
    }
    if (*(s16*)((u8*)play + 0x3204) != 0) {
        *(s16*)((u8*)play + 0x3204) -= 25;
    }
    if (*(s16*)((u8*)play + 0x3206) != 0) {
        *(s16*)((u8*)play + 0x3206) -= 25;
    }
    if (*(s16*)((u8*)play + 0x31FC) != 0) {
        *(s16*)((u8*)play + 0x31FC) -= 25;
    }
    if (*(s16*)((u8*)play + 0x31FE) != 0) {
        *(s16*)((u8*)play + 0x31FE) -= 25;
    }
    if (*(s16*)((u8*)play + 0x3200) != 0) {
        *(s16*)((u8*)play + 0x3200) -= 25;
    }

    if (self->timer <= 0) {
        player = GET_PLAYER(play);

        if ((*(u32*)((u8*)player + 0x1710) & 0x800) && (*(Actor**)((u8*)player + 0x1224) == &self->actor)) {
            player->actor.child = NULL;
            *(void**)((u8*)player + 0x1224) = NULL;
            *(void**)((u8*)player + 0x12B0) = NULL;
            *(u32*)((u8*)player + 0x1710) &= ~0x800;
        }

        Actor_Kill(&self->actor);
    }
}
#endif
GLOBAL_ASM("asm/EnBom_Explode.s")

void EnBom_Move(EnBom* self, PlayState* play) {
    if (Actor_HasParent(&self->actor, play)) {
        self->actionFunc = EnBom_WaitForRelease;
        self->actor.room = -1;
        return;
    }

    if ((self->actor.velocity.y > 0.0f) && (self->actor.bgCheckFlags & 0x10)) {
        self->actor.velocity.y = -self->actor.velocity.y;
    }

    if ((self->actor.speedXZ != 0.0f) && (self->actor.bgCheckFlags & 8) && !(self->actor.bgCheckFlags & 1)) {
        s16 yawDiff = self->actor.wallYaw - self->actor.world.rot.y;
        if (ABS(yawDiff) > 0x4000) {
            self->actor.world.rot.y = ((self->actor.wallYaw - self->actor.world.rot.y) + self->actor.wallYaw) - 0x8000;
        }
        Actor* thisx = &self->actor;
        if ((s16)thisx->unk_19C <= 0) {
            Audio_PlayActorSound2(thisx, 0x01000186);
            thisx->unk_19C = 6;
        }
        Actor_MoveForward(&self->actor);
        self->actor.speedXZ *= 0.7f;
        self->actor.bgCheckFlags &= ~8;
    }

    if (!(self->actor.bgCheckFlags & 1)) {
        Math_StepToF(&self->actor.speedXZ, 0.0f, 0.08f);
    } else {
        Math_StepToF(&self->actor.speedXZ, 0.0f, 1.0f);
        if ((self->actor.bgCheckFlags & 2) && (self->actor.velocity.y < -2.0f)) {
            FUN_00314fe4(play, &self->actor);
            self->actor.velocity.y *= -0.3f;
            self->actor.bgCheckFlags &= ~2;
        } else if (self->timer > 5) {
            FUN_0034df30(&self->actor, play);
        }

        if (self->actor.velocity.y < 2.0f) {
            self->actor.velocity.y = 0.0f;
        } else if (self->unk_288 <= 0) {
            self->actor.bgCheckFlags &= ~1;
        } else {
            self->actor.velocity.y = 0.0f;
        }
    }

    Actor_MoveForward(&self->actor);
}

}
