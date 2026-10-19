extern "C" {
#include "z_en_nwc.hpp"

#define FLAGS 0x00000030

#define CHICK_BG_FLOOR (1 << 0)
#define CHICK_BG_WALL (1 << 1)

#define CHICK_NONE 0
#define CHICK_NORMAL 1

typedef struct EnNwcJntSphElementInit {
    /* 0x00 */ u32 words[12];
} EnNwcJntSphElementInit; // size = 0x30

typedef struct EnNwcJntSphInit {
    /* 0x00 */ u8 colMaterial;
    /* 0x01 */ u8 atFlags;
    /* 0x02 */ u8 acFlags;
    /* 0x03 */ u8 ocFlags1;
    /* 0x04 */ u32 colShape;
    /* 0x08 */ s32 count;
    /* 0x0C */ EnNwcJntSphElementInit* elements;
} EnNwcJntSphInit; // size = 0x10

// Data lives in GLOBAL_ASM("data/z_En_Nwc.data.s"); no C symbols for it yet.
// Absolute (EQU) symbols from data_syms.s.
// The element init (0x52D1C8) directly precedes the JntSph init (0x52D1F8).
extern EnNwcJntSphInit EnNwc_sJntSphInit;
extern EnNwcChickFunc EnNwc_sChickActionFuncs[];
#define sJntSphInit (&EnNwc_sJntSphInit)
#define sJntSphElementInit (((EnNwcJntSphElementInit*)sJntSphInit) - 1)

void EnNwc_Init(Actor* thisx, PlayState* play);
void EnNwc_Destroy(Actor* thisx, PlayState* play);
void EnNwc_Update(Actor* thisx, PlayState* play);
void EnNwc_Draw(Actor* thisx, PlayState* play);

void EnNwc_ChickNoop(EnNwcChick* chick, EnNwc* self, PlayState* play);
void EnNwc_ChickFall(EnNwcChick* chick, EnNwc* self, PlayState* play);
void EnNwc_Idle(EnNwc* self, PlayState* play);

void Collider_InitJntSph(PlayState* play, EnNwcJntSph* collider);
void Collider_SetJntSphAllocType1(PlayState* play, EnNwcJntSph* collider, Actor* actor, EnNwcJntSphInit* init);
void FUN_00161654(PlayState* play, EnNwcJntSph* collider);
s32 BgCheck_EntitySphVsWall1(CollisionContext* colCtx, Vec3f* outPos, Vec3f* posA, Vec3f* posB, f32 radius,
                             CollisionPoly** wallPoly, f32 checkHeight);
f32 BgCheck_EntityRaycastFloor3(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Vec3f* pos);

GLOBAL_ASM("data/z_En_Nwc.data.s")

GLOBAL_ASM("../src/game/actors/En_Nwc/data_syms.s")

ActorInit En_Nwc_InitVars = {
    ACTOR_EN_NWC,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_NWC,
    sizeof(EnNwc),
    (ActorFunc)EnNwc_Init,
    (ActorFunc)EnNwc_Destroy,
    (ActorFunc)EnNwc_Update,
    (ActorFunc)EnNwc_Draw,
};

void EnNwc_ChickNoop(EnNwcChick* chick, EnNwc* self, PlayState* play) {
}

void EnNwc_Destroy(Actor* thisx, PlayState* play) {
    EnNwc* self = (EnNwc*)thisx;

    FUN_00161654(play, &self->collider);
}

void EnNwc_Update(Actor* thisx, PlayState* play) {
    EnNwc* self = (EnNwc*)thisx;

    self->updateFunc(self, play);
    CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
}

void EnNwc_Draw(Actor* thisx, PlayState* play) {
}


extern __pure float _fabsf(float);
static __inline __pure float EnNwc_Fabsf(float f) {
    return _fabsf(f);
}

void EnNwc_Init(Actor* thisx, PlayState* play) {
    EnNwc* self = (EnNwc*)thisx;
    EnNwcJntSphElementInit elementInits[16];
    EnNwcJntSphElementInit* element;
    EnNwcChick* chick;
    s32 i;

    element = sJntSphInit->elements = elementInits;
    for (i = 0; i < 16; i++, element++) {
        *element = *sJntSphElementInit;
    }

    Collider_InitJntSph(play, &self->collider);
    Collider_SetJntSphAllocType1(play, &self->collider, &self->actor, sJntSphInit);
    self->count = 16;
    chick = self->chicks;
    for (i = 0; i < self->count; i++, chick++) {
        chick->type = CHICK_NORMAL;
        chick->pos.x = thisx->world.pos.x + ((Rand_ZeroOne() * 100.0f) - 50.0f);
        chick->pos.y = thisx->world.pos.y + 20.0f;
        chick->pos.z = thisx->world.pos.z + ((Rand_ZeroOne() * 100.0f) - 50.0f);
        chick->height = 5;
    }
    self->updateFunc = EnNwc_Idle;
}

/*
 * ChickFall: closest C draft below (3 words off: int register choice only).
 * ChickFall: ROM compares the velY raw bits as ints with the constant in r1 and the bits in r2; ours
 *   swaps r1/r2 (constant in r2). Needs `f32 v = velY - 0.1f; velY = v; if (v < -10.0f) velY = -10.0f;`
 *   and bgFlags |= FLOOR before pos.y = floorY.
 * Idle: MATCHED (Opus) -- the dx/dz differences go through a Vec3f temp `d` and are then copied to
 *   dx/dz locals; that lets prevChickPos.x/.z share s0 like the ROM. Plus a local `s = sqrtf(test)`.
 */
#if 0
static void EnNwc_ChickBgCheck(EnNwcChick* chick, PlayState* play) {
    CollisionPoly* groundPoly;
    s32 bgId;
    Vec3f outPos;
    f32 dy;

    chick->bgFlags &= ~CHICK_BG_WALL & ~CHICK_BG_FLOOR;
    outPos.x = chick->pos.x;
    outPos.y = chick->pos.y;
    outPos.z = chick->pos.z;
    if (BgCheck_EntitySphVsWall1(&play->colCtx, &outPos, &chick->pos, &chick->lastPos, 10.0f, &chick->floorPoly,
                                 20.0f)) {
        chick->bgFlags |= CHICK_BG_WALL;
    }
    chick->floorY = BgCheck_EntityRaycastFloor3(&play->colCtx, &groundPoly, &bgId, &outPos);
    dy = chick->floorY - chick->pos.y;
    if ((0.0f <= dy) && (dy < 40.0f)) {
        chick->bgFlags |= CHICK_BG_FLOOR;
        chick->pos.y = chick->floorY;
    }
}

void EnNwc_ChickFall(EnNwcChick* chick, EnNwc* self, PlayState* play) {
    f32 v = chick->velY - 0.1f;
    chick->velY = v;
    if (v < -10.0f) {
        chick->velY = -10.0f;
    }
    chick->pos.y = chick->pos.y + chick->velY;
    EnNwc_ChickBgCheck(chick, play);
}

#endif

GLOBAL_ASM("asm/EnNwc_ChickFall.s")

void EnNwc_Idle(EnNwc* self, PlayState* play) {
    EnNwcChick* chick = self->chicks;
    EnNwcJntSphElement* element = &self->collider.elements[0];
    Vec3f prevChickPos;
    s32 i;
    f32 test;

    prevChickPos.y = 99999.9f;
    for (i = 0; i < self->count; i++, prevChickPos = chick->pos, chick++, element++) {
        Math_Vec3f_Copy(&chick->lastPos, &chick->pos);

        EnNwc_sChickActionFuncs[chick->type](chick, self, play);

        element->center = chick->pos;

        test = chick->pos.y - prevChickPos.y;
        if (EnNwc_Fabsf(test) < 10.0f) {
            Vec3f d;
            f32 dx, dz;
            d.x = chick->pos.x - prevChickPos.x;
            d.z = chick->pos.z - prevChickPos.z;
            dx = d.x;
            dz = d.z;
            test = SQ(dx) + SQ(dz);
            if (test < SQ(10.0f)) {
                if (test != 0.0f) {
                    f32 s = sqrtf(test);
                    chick->pos.x += dx / s;
                    chick->pos.z += dz / s;
                } else {
                    chick->pos.x += 1.0f;
                    chick->pos.z += 1.0f;
                }
            }
        }
    }
}

}
