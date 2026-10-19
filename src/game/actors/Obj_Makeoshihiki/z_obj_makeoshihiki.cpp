extern "C" {
#include "z_obj_makeoshihiki.hpp"

#define FLAGS 0x00000020

#define PARAMS_GET_U(params, start, count) (((params) >> (start)) & ((1 << (count)) - 1))

void ObjMakeoshihiki_Init(Actor* actor, GameState* state);
void ObjMakeoshihiki_Draw(Actor* actor, GameState* state);

ActorInit Obj_Makeoshihiki_InitVars = {
    ACTOR_OBJ_MAKEOSHIHIKI,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_DANGEON_KEEP,
    sizeof(ObjMakeoshihiki),
    (ActorFunc)ObjMakeoshihiki_Init,
    (ActorFunc)Actor_Noop,
    (ActorFunc)Actor_Noop,
    (ActorFunc)ObjMakeoshihiki_Draw,
};

typedef struct BlockConfig {
    /* 0x00 */ Vec3f posVecs[3];
    /* 0x24 */ u8 unk_24[3];
    /* 0x27 */ u8 color;
    /* 0x28 */ u8 type;
    /* 0x2A */ s16 rotY;
} BlockConfig; // size = 0x2C

typedef struct ObjOshihikiCantMove {
    /* 0x0000 */ Actor actor;
    /* 0x01A4 */ char unk_1A4[0x8A];
    /* 0x022E */ u8 cantMove;
} ObjOshihikiCantMove;

typedef void (*ObjMakeoshihikiFlagFunc)(PlayState* play, s32 flag);

__attribute__((used)) static ObjMakeoshihikiFlagFunc sFlagSwitchFuncs[] = { Flags_UnsetSwitch, Flags_SetSwitch };

__attribute__((used)) static BlockConfig sBlocks[] = {
    { { { 660.0f, 460.0f, 660.0f }, { 660.0f, 457.0f, 540.0f }, { 780.0f, 454.0f, 540.0f } }, { 0x00, 0x00, 0x03 },
      0xFF, 0x02, 0x0000 },
    { { { -605.0f, -820.0f, -290.0f }, { -365.0f, -905.0f, -290.0f }, { -365.0f, -905.0f, -290.0f } },
      { 0x00, 0x03, 0x00 }, 0xFF, 0x00, 0x0000 }
};

__attribute__((used)) static u32 sFlags[3][2] = { { 0, 0 }, { 1, 0 }, { 0, 1 } };

void ObjMakeoshihiki_Init(Actor* actor, GameState* state) {
    PlayState* play = (PlayState*)state;
    BlockConfig* block = &sBlocks[actor->home.rot.z & 1];
    s32 typeIdx;

    actor->unk_19A = 1;

    if (!PARAMS_GET_U(actor->params, 6, 1) && Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 0, 6))) {
        typeIdx = 1;
    } else if (!PARAMS_GET_U(actor->params, 14, 1) && Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 8, 6))) {
        typeIdx = 2;
    } else {
        typeIdx = 0;
    }

    if (Actor_SpawnAsChild(&play->actorCtx, actor, play, ACTOR_OBJ_OSHIHIKI, block->posVecs[typeIdx].x, block->posVecs[typeIdx].y, block->posVecs[typeIdx].z, 0,
                           block->rotY, 0, ((block->color << 6) & 0xC0) | (block->type & 0xF) | 0xFF00) == NULL) {
        Actor_Kill(actor);
        return;
    }
    if (block->unk_24[typeIdx] & 2) {
        ((ObjOshihikiCantMove*)actor->child)->cantMove = true;
    }
    actor->world.rot.z = actor->shape.rot.z = 0;
}

static inline f32 ObjMakeoshihiki_DistSq(Vec3f* a, Vec3f* b) {
    f32 dx = a->x - b->x;
    f32 dy = a->y - b->y;
    f32 dz = a->z - b->z;
    return dx * dx + dy * dy + dz * dz;
}

void ObjMakeoshihiki_Draw(Actor* actor, GameState* state) {
    BlockConfig* block = &sBlocks[actor->home.rot.z & 1];
    PlayState* play = (PlayState*)state;
    s32 i;
    s32 sfxCond1;
    s32 sfxCond2;
    s32 cond;
    s32 cond2;

    for (i = 0; i < 3; i++) {
        if (ObjMakeoshihiki_DistSq(&actor->child->world.pos, &block->posVecs[i]) < 0.001f) {
            if (block->unk_24[i] & 1) {
                if (PARAMS_GET_U(actor->params, 6, 1)) {
                    sfxCond1 = false;
                } else {
                    sfxCond1 = (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 0, 6)) != 0) ^ sFlags[i][0];
                }

                if (PARAMS_GET_U(actor->params, 14, 1)) {
                    sfxCond2 = false;
                } else {
                    if (Flags_GetSwitch(play, PARAMS_GET_U(actor->params, 8, 6))) {
                        cond2 = true;
                    } else {
                        cond2 = false;
                    }
                    sfxCond2 = cond2 ^ sFlags[i][1];
                }

                if (sfxCond1 || sfxCond2) {
                    FUN_00372244(&play->unk_5FCC, 0x0A, 0x100048c);
                }
            }

            sFlagSwitchFuncs[sFlags[i][0]](play, PARAMS_GET_U(actor->params, 0, 6));
            sFlagSwitchFuncs[sFlags[i][1]](play, PARAMS_GET_U(actor->params, 8, 6));

            if (block->unk_24[i] & 2) {
                ((ObjOshihikiCantMove*)actor->child)->cantMove = true;
            }

            break;
        }
    }
}

s32 FUN_003815f0(void) {
    return 0;
}
}
