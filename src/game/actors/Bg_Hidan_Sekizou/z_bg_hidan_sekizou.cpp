extern "C" {
#include "z_bg_hidan_sekizou.hpp"

#define FLAGS 0x80400000

void BgHidanSekizou_Init(Actor* actor, GameState* state);
void BgHidanSekizou_Destroy(Actor* actor, GameState* state);
void BgHidanSekizou_Update(Actor* actor, GameState* state);
void BgHidanSekizou_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Bg_Hidan_Sekizou.data.s")

ActorInit Bg_Hidan_Sekizou_InitVars = {
    ACTOR_BG_HIDAN_SEKIZOU,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_HIDAN_OBJECTS,
    sizeof(BgHidanSekizou),
    (ActorFunc)BgHidanSekizou_Init,
    (ActorFunc)BgHidanSekizou_Destroy,
    (ActorFunc)BgHidanSekizou_Update,
    (ActorFunc)BgHidanSekizou_Draw,
};

GLOBAL_ASM("asm/BgHidanSekizou_Init.s")

void Collider_DestroyJntSph(PlayState* play, BgHidanSekizouColliderJntSph* collider);

void BgHidanSekizou_Destroy(Actor* actor, GameState* state) {
    BgHidanSekizou* self = (BgHidanSekizou*)actor;
    PlayState* play = (PlayState*)state;

    DynaPoly_DeleteBgActor(play, &play->colCtx.dyna, self->dyna.bgId);
    Collider_DestroyJntSph(play, &self->collider);
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], &self->skelAnimModel[2],
                 &self->skelAnimModel[3], &self->skelAnimModel[4], &self->skelAnimModel[5],
                 &self->skelAnimModel[6], &self->skelAnimModel[7], &self->skelAnimModel[8],
                 &self->skelAnimModel[9], &self->skelAnimModel[10], &self->skelAnimModel[11],
                 &self->skelAnimModel[12], &self->skelAnimModel[13], &self->skelAnimModel[14],
                 &self->skelAnimModel[15], &self->skelAnimModel[16], &self->skelAnimModel[17], 0);
}

// N64: func_8088D720 (updateFunc while params == 0). 3DS timer: 36 * 3 / 2 = 54.
void FUN_003dc540(BgHidanSekizou* self, PlayState* play) {
    self->unk_168[0]--;
    if (self->unk_168[0] <= -54) {
        self->unk_168[0] = 54;
    }
}

// BgHidanSekizou_Update draft (NOT matched yet, ROM 456 B vs 444 B, bic r0 vs r1 on atFlags clear).
#if 0
// N64: func_8088D750 (inlined into the ROM Update)
static void BgHidanSekizou_Knockback(BgHidanSekizou* self, PlayState* play) {
    s16 phi_a3;

    if (self->dyna.actor.xzDistToPlayer > 200.0f) {
        phi_a3 = self->dyna.actor.yawTowardsPlayer;
    } else if (self->dyna.actor.params == 0) {
        phi_a3 = self->dyna.actor.yawTowardsPlayer - self->dyna.actor.shape.rot.y;
        if (phi_a3 > 0x2000) {
            phi_a3 = self->dyna.actor.shape.rot.y + 0x6000;
        } else if (phi_a3 < -0x2000) {
            phi_a3 = self->dyna.actor.shape.rot.y - 0x6000;
        } else if (phi_a3 > 0) {
            phi_a3 = self->dyna.actor.shape.rot.y - 0x2000;
        } else {
            phi_a3 = self->dyna.actor.shape.rot.y + 0x2000;
        }
    } else {
        phi_a3 = self->dyna.actor.yawTowardsPlayer;
        if (phi_a3 > 0x6000) {
            phi_a3 = 0x4000;
        } else if (phi_a3 > 0x4000) {
            phi_a3 = -0x8000;
        } else if (phi_a3 > 0x2000) {
            phi_a3 = 0;
        } else if (phi_a3 > 0) {
            phi_a3 = 0x4000;
        } else if (phi_a3 < -0x6000) {
            phi_a3 = -0x4000;
        } else if (phi_a3 < -0x4000) {
            phi_a3 = -0x8000;
        } else if (phi_a3 < -0x2000) {
            phi_a3 = 0;
        } else {
            phi_a3 = -0x4000;
        }
    }
    FUN_00374bb8(play, &self->dyna.actor, 5.0f, phi_a3, 1.0f);
}

void BgHidanSekizou_Update(Actor* actor, GameState* state) {
    BgHidanSekizou* self = (BgHidanSekizou*)actor;
    PlayState* play = (PlayState*)state;

    if (self->unk_170 != 0) {
        self->unk_170--;
    }
    if (self->unk_170 == 0) {
        self->unk_170 = 4;
    }

    if (self->collider.base.atFlags & AT_HIT) {
        BgHidanSekizou* s2 = self;

        s2->collider.base.atFlags &= ~AT_HIT;
        {
            s16 phi_a3;

            if (self->dyna.actor.xzDistToPlayer > 200.0f) {
                phi_a3 = self->dyna.actor.yawTowardsPlayer;
            } else if (self->dyna.actor.params == 0) {
                phi_a3 = self->dyna.actor.yawTowardsPlayer - self->dyna.actor.shape.rot.y;
                if (phi_a3 > 0x2000) {
                    phi_a3 = self->dyna.actor.shape.rot.y + 0x6000;
                } else if (phi_a3 < -0x2000) {
                    phi_a3 = self->dyna.actor.shape.rot.y - 0x6000;
                } else if (phi_a3 > 0) {
                    phi_a3 = self->dyna.actor.shape.rot.y - 0x2000;
                } else {
                    phi_a3 = self->dyna.actor.shape.rot.y + 0x2000;
                }
            } else {
                phi_a3 = self->dyna.actor.yawTowardsPlayer;
                if (phi_a3 > 0x6000) {
                    phi_a3 = 0x4000;
                } else if (phi_a3 > 0x4000) {
                    phi_a3 = -0x8000;
                } else if (phi_a3 > 0x2000) {
                    phi_a3 = 0;
                } else if (phi_a3 > 0) {
                    phi_a3 = 0x4000;
                } else if (phi_a3 < -0x6000) {
                    phi_a3 = -0x4000;
                } else if (phi_a3 < -0x4000) {
                    phi_a3 = -0x8000;
                } else if (phi_a3 < -0x2000) {
                    phi_a3 = 0;
                } else {
                    phi_a3 = -0x4000;
                }
            }
            FUN_00374bb8(play, &self->dyna.actor, 5.0f, phi_a3, 1.0f);
        }
    }

    self->updateFunc(self, play);

    if (self->dyna.actor.params == 0) {
        if (self->unk_168[0] > 0) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
            FUN_00373264(&self->dyna.actor, 0x0100018A);
        }
    } else {
        if ((self->unk_168[0] > 0) || (self->unk_168[1] > 0) || (self->unk_168[2] > 0) || (self->unk_168[3] > 0)) {
            CollisionCheck_SetAT(play, &play->colChkCtx, &self->collider.base);
            CollisionCheck_SetOC(play, &play->colChkCtx, &self->collider.base);
            FUN_00373264(&self->dyna.actor, 0x0100018A);
        }
    }
}

#endif

GLOBAL_ASM("asm/BgHidanSekizou_Update.s")

GLOBAL_ASM("asm/BgHidanSekizou_Draw.s")

GLOBAL_ASM("asm/FUN_0031f978.s")

}
