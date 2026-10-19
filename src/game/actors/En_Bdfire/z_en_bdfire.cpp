extern "C" {
#include "z_en_bdfire.hpp"

#define FLAGS 0x00000030

void Lights_PointSetColorAndRadius(void* lightInfo, u8 r, u8 g, u8 b, s16 radius, s32 arg6);
void FUN_0035d8d8(void* player, PlayState* play, s32 arg);
void FUN_00368fc0(PlayState* play, Actor* actor, f32 speed, s16 yaw, f32 unk, s32 unk2);
void FUN_00348a64(void*, s32, void*, s32, s32, s32, s32);
void FUN_0034fc7c(void* p);
void SystemArena_Free(void* p);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* lightNode);

void FUN_0010a084(EnBdfire* self, PlayState* play) {
    u8* kd = (u8*)self->actor.parent;

    self->actor.world.pos.x = *(f32*)(kd + 0x9C8);
    self->actor.world.pos.y = *(f32*)(kd + 0x9CC);
    self->actor.world.pos.z = *(f32*)(kd + 0x9D0);
    if (kd[0x7B2] == 0) {
        Math_SmoothStepToF(&self->actor.scale.x, 0.0f, 1.0f, 0.6f, 0.0f);
        if (Math_SmoothStepToF(&self->unk_1E4, 0.0f, 1.0f, 20.0f, 0.0f) == 0.0f) {
            Actor_Kill(&self->actor);
        }
    } else {
        if (self->unk_1AC < (s32)(210.0f / SREG(30) + 0.5f)) {
            Math_SmoothStepToF(&self->unk_1E4, 255.0f, 0.1f, 3.0f, 0.0f);
            Math_SmoothStepToF(&self->unk_1E8, 255.0f, 1.0f, 3.8249998f, 0.0f);
            Math_SmoothStepToF(&self->unk_1EC, 100.0f, 1.0f, 1.5f, 0.0f);
        }
        if (DECR(self->unk_1AC) == 0) {
            Math_SmoothStepToF(&self->actor.scale.x, 0.0f, 1.0f, 0.3f, 0.0f);
            Math_SmoothStepToF(&self->unk_1E8, 0.0f, 1.0f, 25.5f, 0.0f);
            Math_SmoothStepToF(&self->unk_1EC, 0.0f, 1.0f, 10.0f, 0.0f);
            if (Math_SmoothStepToF(&self->unk_1E4, 0.0f, 1.0f, 10.0f, 0.0f) == 0.0f) {
                Actor_Kill(&self->actor);
            }
        }
        Actor_SetScale(&self->actor, self->actor.scale.x);
        Lights_PointSetColorAndRadius(&self->lightInfoNoGlow, (u8)self->unk_1E8, (u8)self->unk_1EC, 0, 300, 0);
        if ((self->unk_1B6 & 1) == 0) {
            s32 idx = (self->unk_1B6 % 60) / 2;

            self->trail[idx].pos = self->actor.world.pos;
            self->trail[idx].pos.x += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx].pos.y += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx].pos.z += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx].scale = self->unk_1E4 * (1.0f / 255.0f);
            self->trail[idx].active = 1;
        }
        self->unk_1B6++;
    }
}

void FUN_0033a61c(Actor* actor);
void EnBdfire_Init(Actor* thisx, PlayState* play);
void EnBdfire_Destroy(Actor* thisx, PlayState* play);
void EnBdfire_Update(Actor* thisx, PlayState* play);
void EnBdfire_Draw(Actor* thisx, PlayState* play);

GLOBAL_ASM("data/z_En_Bdfire.data.s")

ActorInit En_Bdfire_InitVars = {
    ACTOR_PLAYER,
    ACTORCAT_ENEMY,
    FLAGS,
    OBJECT_KINGDODONGO,
    sizeof(EnBdfire),
    (ActorFunc)EnBdfire_Init,
    (ActorFunc)EnBdfire_Destroy,
    (ActorFunc)EnBdfire_Update,
    (ActorFunc)EnBdfire_Draw,
};

void* SystemArena_Malloc_0035010c(s32 size);
void* FUN_003500c4(void* p);
void FUN_0034ff2c(void* obj, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void FUN_0034fea8(void* obj, s32 bank, PlayState* play, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7);
void* ZAR_GetCTXBByIndex(ZARInfo* zar, s32 index);
void Lights_PointNoGlowSetInfo(void* light, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s16 radius, s32 pad);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void FUN_0033a61c(Actor* actor);
void FUN_0010a084(EnBdfire* self, PlayState* play);
void FUN_003dc618(EnBdfire* self, PlayState* play);
void EnBdfire_DrawFire(EnBdfire* self, PlayState* play);
void FUN_003fd178(void** self, void* a1, s32 a2, s32 a3, s32 a4, s32 a5);
void __rt_memclr_w(void* p, s32 n);

#if 0
void EnBdfire_Init(Actor* thisx, PlayState* play) {
    EnBdfire* self = (EnBdfire*)thisx;
    ZARInfo* zar;
    void* obj;

    self->unk_244 = NULL;
    self->unk_248[0] = NULL;
    self->unk_248[1] = NULL;
    self->unk_1B6 = 0;
    self->unk_1B8 = 0;
    if (thisx->params <= 0) {
        zar = FUN_00372f38(thisx, play, &self->unk_244, 1, 0);
        self->unk_244->unk_0C->FUN_00372d94(zar->GetCMABByIndex(0));
        self->unk_244->unk_0C->SetAnimType(1);
        self->unk_244->unk_0C->SetAnimSpeed(2.0f);
        obj = SystemArena_Malloc_0035010c(0x28);
        if (obj != NULL) {
            obj = FUN_003500c4(obj);
        }
        self->unk_248[0] = obj;
        FUN_0034ff2c(obj, 4, 4, 0x1E, 3, 0);
        FUN_003fd178(&self->unk_248[0], ZAR_GetCTXBByIndex(zar, 10), 0x2601, 0x2601, 0x812F, 0x812F);
        *(u32*)(*(u8**)((u8*)self->unk_248[0] + 8) + 0x178) |= 1;
        __rt_memclr_w(&self->trail[0], 0x258);
        if (thisx->params == 0) {
            obj = SystemArena_Malloc_0035010c(0x28);
            if (obj != NULL) {
                obj = FUN_003500c4(obj);
            }
            self->unk_248[1] = obj;
            FUN_0034ff2c(obj, 4, 4, 0x1E, 1, 0);
            FUN_0034fea8(self->unk_248[1], 0, play, 0x10, 0x2601, 0x2601, 0x812F, 0x812F);
            *(u32*)(*(u8**)((u8*)self->unk_248[1] + 8) + 0x178) |= 1;
            __rt_memclr_w(&self->trail[30], 0x258);
        }
    }
    Actor_SetScale(thisx, 0.6f);
    self->drawFunc = (EnBdfireDrawFunc)EnBdfire_DrawFire;
    self->unk_1F0.x = self->unk_1F0.y = self->unk_1F0.z = 0.0f;
    self->unk_1FC.x = self->unk_1FC.y = self->unk_1FC.z = 1.0f;
    if (thisx->params < 0) {
        self->actionFunc = (EnBdfireActionFunc)FUN_0010a084;
        thisx->scale.x = 2.8f;
        self->unk_1AC = (s32)(270.0f / SREG(30) + 0.5f);
        Lights_PointNoGlowSetInfo(&self->lightInfoNoGlow, (f32)(s16)thisx->world.pos.x, (f32)(s16)thisx->world.pos.y,
                                  (f32)(s16)thisx->world.pos.z, 0, 0, 0, 300, 0);
        self->lightNode = LightContext_InsertLight(play, (u8*)play + 0xA70, &self->lightInfoNoGlow);
        self->unk_1FC.x = 0.0f;
        self->unk_1FC.z = self->unk_1FC.y = 0.0f;
    } else {
        self->actionFunc = (EnBdfireActionFunc)FUN_003dc618;
        ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 0.0f);
        thisx->speedXZ = 30.0f;
        self->unk_1AC = (s32)((25 - (s32)(thisx->params * 0.8f)) * 3.0f / SREG(30) + 0.5f);
        if (self->unk_1AC < 0) {
            self->unk_1AC = 0;
        }
        self->unk_1E0 = 4.2000003f - (thisx->params * 0.25f * 0.6f);
        if (self->unk_1E0 < 0.90000004f) {
            self->unk_1E0 = 0.90000004f;
        }
        self->unk_1E4 = 255.0f - (thisx->params * 10.0f);
        if (self->unk_1E4 < 20.0f) {
            self->unk_1E4 = 20.0f;
        }
        self->unk_1AE = (s16)(Rand_ZeroOne() * 8.0f);
    }
    FUN_0033a61c(thisx);
}
#endif
GLOBAL_ASM("asm/EnBdfire_Init.s")

void EnBdfire_Destroy(Actor* thisx, PlayState* play) {
    EnBdfire* self = (EnBdfire*)thisx;
    s32 i;

    if (thisx->params < 0) {
        LightContext_RemoveLight(play, (u8*)play + 0xA70, self->lightNode);
    }
    if (thisx->params <= 0) {
        FUN_00350f34(&self->actor, &self->unk_244, 0);
        for (i = 0; i < 2; i++) {
            if (self->unk_248[i] != NULL) {
                FUN_0034fc7c(self->unk_248[i]);
                SystemArena_Free(self->unk_248[i]);
                self->unk_248[i] = NULL;
            }
        }
    }
}

void EnBdfire_Update(Actor* thisx, PlayState* play) {
    EnBdfire* self = (EnBdfire*)thisx;

    self->unk_1AE++;
    self->actionFunc((EnBdfire*)thisx, play);
    Actor_MoveForward(&self->actor);
    FUN_0033a61c(&self->actor);
}

void EnBdfire_Draw(Actor* thisx, PlayState* play) {
    EnBdfire* self = (EnBdfire*)thisx;

    self->drawFunc(self, play);
}

GLOBAL_ASM("asm/EnBdfire_DrawFire.s")

#if 0
typedef struct {
    s32 key;
    f32 value;
    f32 tanA;
    f32 tanB;
} SplineKey;

f32 FUN_003694d4(SplineKey* table, s32 n, s32 time, s32 wrap) {
    s32 i;
    s32 lastKey;
    SplineKey* cur;
    SplineKey* prev;
    f32 fk, dk, u, a, d, b, c, e, g, result;
    lastKey = table[n - 1].key;
    if (time >= lastKey) {
        if (wrap != 0) {
            time = time % (lastKey + 1);
        } else {
            return table[n - 1].value;
        }
    }
    for (i = 0; i < n; i++) {
        s32 key = table[i].key;
        cur = &table[i];
        if (key == time) {
            return table[i].value;
        }
        if (key <= time) {
            continue;
        }
        prev = cur - 1;
        fk = (f32)(time - prev->key);
        dk = (f32)(key - prev->key);
        u = fk * (1.0f / dk);
        d = prev->value - cur->value;
        a = u * 2.0f - 3.0f;
        b = u - 1.0f;
        c = d * a;
        e = b * prev->tanB;
        g = c * u;
        e = e + u * cur->tanA;
        result = prev->value + g * u;
        result = result + (fk * b) * e;
        return result;
    }
}
#endif
GLOBAL_ASM("asm/FUN_003694d4.s")

void FUN_003dc618(EnBdfire* self, PlayState* play) {
    s32 quarterTurn = 0;
    Actor* kd = self->actor.parent;
    u8* player = (u8*)GET_PLAYER(play);

    self->unk_1B0 = *(u16*)((u8*)kd + 0x772);
    if (self->actor.params == 0) {
        if (self->unk_1E4 != 0.0f) {
            Audio_PlaySoundGeneral(0x01000279, &self->actor.world.pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20,
                                   &DAT_0054ac24.unk_0);
        }
        if ((self->unk_1B6 & 1) == 0) {
            s32 idx = (self->unk_1B6 % 60) / 2;

            self->trail[idx].pos = self->actor.world.pos;
            self->trail[idx].pos.x += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx].pos.y += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx].pos.z += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx].scale = self->unk_1E4 * (1.0f / 255.0f);
            self->trail[idx].active = 1;
            self->trail[idx + 30].pos = self->actor.world.pos;
            self->trail[idx + 30].pos.x += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx + 30].pos.y += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx + 30].pos.z += (Rand_ZeroOne() - 0.5f) * 24.0f;
            self->trail[idx + 30].scale = self->unk_1E4 * (1.0f / 255.0f);
            self->trail[idx + 30].active = 1;
        }
        self->unk_1B6++;
    }
    Math_SmoothStepToF(&self->actor.scale.x, self->unk_1E0, 0.3f, 0.5f, 0.0f);
    Actor_SetScale(&self->actor, self->actor.scale.x);
    if ((self->actor.world.pos.x < -1390.0f) && (self->actor.velocity.x < -10.0f)) {
        self->actor.world.pos.x = -1390.0f;
        quarterTurn = 1;
    }
    if ((self->actor.world.pos.x > -390.0f) && (self->actor.velocity.x > 10.0f)) {
        self->actor.world.pos.x = -390.0f;
        quarterTurn = 1;
    }
    if ((self->actor.world.pos.z > -2804.0f) && (self->actor.velocity.z > 10.0f)) {
        self->actor.world.pos.z = -2804.0f;
        quarterTurn = 1;
    }
    if ((self->actor.world.pos.z < -3804.0f) && (self->actor.velocity.z < -10.0f)) {
        self->actor.world.pos.z = -3804.0f;
        quarterTurn = 1;
    }
    if (quarterTurn) {
        if (self->unk_1B0 == 0) {
            self->actor.world.rot.y += 0x4000;
        } else {
            self->actor.world.rot.y -= 0x4000;
        }
    }
    if (DECR(self->unk_1AC) == 0) {
        Math_SmoothStepToF(&self->unk_1E4, 0.0f, 1.0f, 10.0f, 0.0f);
        if (self->actor.params == 0) {
            if (self->unk_1E4 >= 1.0f) {
                return;
            }
            if (++self->unk_1B8 <= 60) {
                return;
            }
        }
        Actor_Kill(&self->actor);
    } else if (player[0x246C] == 0) {
        f32 distToBurn = self->actor.scale.x * (130.0f / 4.2000003f);

        if (self->actor.xyzDistToPlayerSq < distToBurn * distToBurn) {
            FUN_0035d8d8(player, play, 0);
            FUN_00368fc0(play, &self->actor, 20.0f, self->actor.world.rot.y, 0.0f, 8);
        }
    }
}

void FUN_003fd178(void** self, void* a1, s32 a2, s32 a3, s32 a4, s32 a5) {
    FUN_00348a64(self[1], 0, a1, a2, a3, a4, a5);
    self[0] = a1;
}

}
