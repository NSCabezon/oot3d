extern "C" {
#include "z_en_river_sound.hpp"

#define FLAGS 0x20000030

void EnRiverSound_Init(Actor* actor, GameState* state);
void EnRiverSound_Destroy(Actor* actor, GameState* state);
void EnRiverSound_Update(Actor* actor, GameState* state);
void EnRiverSound_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_En_River_Sound.data.s")
GLOBAL_ASM("../src/game/actors/En_River_Sound/data_syms.s")

ActorInit En_River_Sound_InitVars = {
    ACTOR_EN_RIVER_SOUND,
    ACTORCAT_BG,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnRiverSound),
    (ActorFunc)EnRiverSound_Init,
    (ActorFunc)EnRiverSound_Destroy,
    (ActorFunc)EnRiverSound_Update,
    (ActorFunc)EnRiverSound_Draw,
};

// State helpers live in z_en_river_sound_state.cpp (separate TU: armcc would inline them into Destroy).
void FUN_00214848(void);
void FUN_0021485c(u8 arg);
void FUN_00285f34(void* arg);


void FUN_00285f50(u8 level);
void FUN_0033c7a4(s32 natureId);

void EnRiverSound_Init(Actor* thisx, GameState* state) {
    EnRiverSound* self = (EnRiverSound*)thisx;

    self->playSfx = false;
    self->pathIndex = (u8)(thisx->params >> 8);
    thisx->params = thisx->params & 0xFF;

    if (thisx->params >= 0xF8) {
        FUN_00285f50((u8)(thisx->params - 0xF8));
        Actor_Kill(thisx);
    } else if (thisx->params == 0xF7) {
        FUN_0033c7a4(4);
        Actor_Kill(thisx);
    } else if (thisx->params == 0xC) {
        if (!CHECK_QUEST_ITEM(QUEST_SONG_LULLABY) || CHECK_QUEST_ITEM(QUEST_SONG_SARIA)) {
            Actor_Kill(thisx);
        }
    }
}

void EnRiverSound_Destroy(Actor* thisx, GameState* state) {
    EnRiverSound* self = (EnRiverSound*)thisx;

    if (self->actor.params == 0xC) {
        FUN_00285f34(&self->unk_1A4);
    } else if (self->actor.params == 0xD) {
        FUN_00214848();
    }
}

f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* outBgId, Actor* actor, Vec3f* pos);
s32 FUN_003232bc(Vec3f* pointA, Vec3f* pointB, Vec3f* hearPos, Vec3f* newSoundPos);
s32 FUN_003232a4(void* colCtx, u32 poly, s32 bgId);

// Writes the position along the river path closest to hearPos into sfxPos (N64 EnRiverSound_GetSfxPos).
static s32 EnRiverSound_GetSfxPos(Vec3s* points, s32 numPoints, Vec3f* hearPos, Vec3f* sfxPos) {
    s32 i;
    s32 closestPointIdx = 0;
    s32 useAdjacentPoints[2] = { false, false };
    Vec3f closestPointPos;
    Vec3f lineSegClosest[2];
    Vec3f point;
    f32 closestPointDist = 10000.0f;
    Vec3f* h2 = hearPos;
    Vec3f* sp2 = sfxPos;
    Vec3s* pts2 = points;
    Vec3s* closestPoint;

    for (i = 0; i < numPoints; i++) {
        f32 dist;

        point.x = points[i].x;
        point.y = points[i].y;
        point.z = points[i].z;
        dist = Math_Vec3f_DistXYZ(hearPos, &point);

        if (closestPointDist > dist) {
            closestPointDist = dist;
            closestPointIdx = i;
        }
    }

    if (closestPointDist >= 10000.0f) {
        return false;
    }

    closestPoint = &points[closestPointIdx];
    closestPointPos.x = closestPoint->x;
    closestPointPos.y = closestPoint->y;
    closestPointPos.z = closestPoint->z;

    if (closestPointIdx != 0) {
        point.x = closestPoint[-1].x;
        point.y = closestPoint[-1].y;
        point.z = closestPoint[-1].z;
        useAdjacentPoints[0] = FUN_003232bc(&point, &closestPointPos, hearPos, &lineSegClosest[0]);
    }

    if (closestPointIdx + 1 != numPoints) {
        point.x = closestPoint[1].x;
        point.y = closestPoint[1].y;
        point.z = closestPoint[1].z;
        useAdjacentPoints[1] = FUN_003232bc(&closestPointPos, &point, h2, &lineSegClosest[1]);
    }

    if (useAdjacentPoints[0] && useAdjacentPoints[1]) {
        if (!FUN_003232bc(&lineSegClosest[0], &lineSegClosest[1], hearPos, sfxPos)) {
            sfxPos->x = (lineSegClosest[1].x + lineSegClosest[0].x) * 0.5f;
            sfxPos->y = (lineSegClosest[1].y + lineSegClosest[0].y) * 0.5f;
            sfxPos->z = (lineSegClosest[1].z + lineSegClosest[0].z) * 0.5f;
        }
    } else if (useAdjacentPoints[0]) {
        sfxPos->x = lineSegClosest[0].x;
        sfxPos->y = lineSegClosest[0].y;
        sfxPos->z = lineSegClosest[0].z;
    } else if (useAdjacentPoints[1]) {
        sfxPos->x = lineSegClosest[1].x;
        sfxPos->y = lineSegClosest[1].y;
        sfxPos->z = lineSegClosest[1].z;
    } else {
        sfxPos->x = closestPointPos.x;
        sfxPos->y = closestPointPos.y;
        sfxPos->z = closestPointPos.z;
    }

    return true;
}

void EnRiverSound_Update(Actor* thisx, GameState* state) {
    PlayState* play = (PlayState*)state;
    Path* path;
    Vec3f* pos;
    Player* player = GET_PLAYER(play);
    EnRiverSound* self = (EnRiverSound*)thisx;
    s32 bgId;
    EnRiverSound* s2 = self;
    PlayState* play2 = play;

    if ((thisx->params == 0) || (thisx->params == 4) || (thisx->params == 5)) {
        pos = &thisx->world.pos;
        path = &play->setupPathList[self->pathIndex];

        if (EnRiverSound_GetSfxPos(path->points, path->count, &player->actor.world.pos, pos)) {
            if (BgCheck_EntityRaycastFloor4(&play->colCtx, &thisx->floorPoly, &bgId, thisx, &thisx->world.pos) != -32000.0f) {
                self->sfxFreqIndex = FUN_003232a4(&play->colCtx, (u32)thisx->floorPoly, bgId);
            } else {
                self->sfxFreqIndex = 0;
            }

            if (self->sfxFreqIndex == 0) {
                if (thisx->params == 4) {
                    self->sfxFreqIndex = 0;
                } else if (thisx->params == 0) {
                    self->sfxFreqIndex = 1;
                } else {
                    self->sfxFreqIndex = 2;
                }
            } else {
                self->sfxFreqIndex--;
                self->sfxFreqIndex = (self->sfxFreqIndex > 2) ? 2 : self->sfxFreqIndex;
            }
        }
    } else if ((thisx->params == 0xD) || (thisx->params == 0x13)) {
        FUN_0036c5d8(&player->actor, &thisx->home.pos, &thisx->world.pos);
    } else if (play->sceneNum == 0x12 && Flags_GetClear(play, thisx->room)) {
        Actor_Kill(thisx);
    }
}

void FUN_0014eef8(Vec3f* pos, f32 volume);
void FUN_00368cc0(PlayState* play, Vec3f* pos, Vec3f* out, f32* w);
void FUN_00323378(Vec3f* pos, u32 seqId, u32 maxDist);
void FUN_002549b4(s32 id, f32 dist);

// PlayState byte not in the repo header (frozen/paused gate for the river sfx).
static inline u8 EnRiverSound_PlayByte(PlayState* play, s32 off) {
    u8* q = (u8*)play + off;
    return *q;
}

// N64 Sfx_PlaySfxCentered2.
static inline void EnRiverSound_PlayCentered(u32 sfxId) {
    Audio_PlaySoundGeneral(sfxId, NULL, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
}

void EnRiverSound_Draw(Actor* thisx, GameState* state) {
    EnRiverSound* self = (EnRiverSound*)thisx;

    if (!self->playSfx) {
        self->playSfx = true;
    } else if ((thisx->params == 0) || (thisx->params == 4) || (thisx->params == 5)) {
        if (EnRiverSound_PlayByte((PlayState*)state, 0x3372) == 0) {
            FUN_0014eef8(&thisx->world.pos, EnRiverSound_sSfxFreqs[self->sfxFreqIndex]);
        }
    } else if (thisx->params == 0xB) {
        FUN_0021485c(90);
    } else if (thisx->params == 0xC) {
        f32 w;
        FUN_00368cc0((PlayState*)state, &thisx->world.pos, (Vec3f*)self->unk_1A4, &w);
        FUN_002549b4((s32)self->unk_1A4, self->actor.xzDistToPlayer);
    } else if (thisx->params == 0xD) {
        FUN_00323378(&thisx->home.pos, 0x010005AB, 1000);
    } else if (thisx->params == 0x13) {
        FUN_00323378(&thisx->home.pos, 0x01000596, 1000);
    } else if ((thisx->params == 0xE) || (thisx->params == 0x10) || (thisx->params == 0x11) ||
               (thisx->params == 0x12)) {
        EnRiverSound_PlayCentered(EnRiverSound_sSoundEffects[thisx->params]);
    } else {
        Audio_PlayActorSound2(thisx, EnRiverSound_sSoundEffects[thisx->params]);
    }
}

void* FUN_0030c550(void);
void* FUN_0030c20c(void* ctx, s32 kind);
void FUN_0030c1e8(void* ctx, void* entry);
void FUN_0030ee14(void** out, void** slot);
void FUN_0030ede0(void** out);

// recipe_near4 2026-10-15: still 3 words; tried all 120 store orders (raw and typed entry/ctx structs), frame/kind locals,
// pointer aliases, single-site mutations.
// Draft (FUN_00404b20 3 words off: ROM schedules `ldr r2,[ctx,#0x180]` before `mov r1,#0x1b`): #if 0, GLOBAL_ASM kept in
// functions_400000s.cpp. The dead sfxId alias fixes the r4/r5 numbering. FUN_003ff980 (its caller) is matched in
// z_en_river_sound_state.cpp (separate TU so armcc does not inline it).
#if 0
// Queues a river sfx entry on the sfx context (pos block lives at arg + 0xF4).
void FUN_00404b20(void* arg, s32 sfxId, f32 freq) {
    u8* ctx = (u8*)FUN_0030c550();
    u8* entry = (u8*)FUN_0030c20c(ctx, 7);
    s32 sfxId2 = sfxId;

    *(u32*)(entry + 0xC) = *(u32*)(ctx + 0x180);
    entry[4] = 0x1B;
    *(void**)(entry + 0x10) = (u8*)arg + 0xF4;
    *(s32*)(entry + 0x14) = sfxId2;
    *(f32*)(entry + 0x18) = freq;
    FUN_0030c1e8(ctx, entry);
}
#endif

void FUN_003560d8(s32 a, s32 b, u8 volume);
void FUN_003ff980(s32 index, s32 sfxId, s32 arg2);

// Draft (5 words off, recipe_near4 2026-10-15: `f32 base = 64.0f` local fixed the 64.0/0.64 pool order, 9->5). Left: r0/r1
// numbering of the unk_80 reload + 100.0f bit-compare literal (ROM ldr r1,[state]; ldr r0,=100.0). Tried: ptr/idx/Vec3f* forms of
// the reload, Vec3f* param, compare operand order/negation, cur re-read, single-site mutations.
#if 0
// River current sfx: the nearest source wins the shared state; distance drives volume and pan.
void FUN_002549b4(s32 id, f32 dist) {
    f32 v;
    f32 vol;
    s32 pan;

    if (EnRiverSound_sState.unk_80 == 0) {
        EnRiverSound_sState.unk_80 = id;
        EnRiverSound_sState.unk_84 = dist;
    } else if (EnRiverSound_sState.unk_80 == id) {
        EnRiverSound_sState.unk_84 = dist;
    } else {
        f32 cur = EnRiverSound_sState.unk_84;

        if (dist < cur) {
            EnRiverSound_sState.unk_80 = id;
            EnRiverSound_sState.unk_84 = dist;
        }
    }

    v = *(f32*)EnRiverSound_sState.unk_80;

    if (v > 100.0f) {
        pan = 127;
    } else if (v < -100.0f) {
        pan = 0;
    } else {
        f32 base = 64.0f;

        pan = (s8)(s32)(base + (v * 0.64f));
    }

    if (EnRiverSound_sState.unk_84 > 400.0f) {
        vol = 0.1f;
    } else if (EnRiverSound_sState.unk_84 < 120.0f) {
        vol = 1.0f;
    } else {
        vol = 0.1f + ((1.0f - ((EnRiverSound_sState.unk_84 - 120.0f) * (1.0f / 280.0f))) * 0.9f);
    }

    FUN_003560d8(0, 0xFDFF, (u8)(vol * 127.0f));
    FUN_003ff980(0, 0xFDFF, pan);
}
#endif

GLOBAL_ASM("asm/FUN_002549b4.s")


// Closest point to hearPos on segment A-B; returns false if it falls outside the segment.
s32 FUN_003232bc(Vec3f* pointA, Vec3f* pointB, Vec3f* hearPos, Vec3f* newSoundPos) {
    f32 temp;
    Vec3f l0;
    Vec3f l1;
    Vec3f l2;

    l0.x = pointA->x - hearPos->x;
    l0.y = pointA->y - hearPos->y;
    l0.z = pointA->z - hearPos->z;
    l1.x = pointB->x - hearPos->x;
    l1.y = pointB->y - hearPos->y;
    l1.z = pointB->z - hearPos->z;
    l2.x = l1.x - l0.x;
    l2.y = l1.y - l0.y;
    l2.z = l1.z - l0.z;

    temp = (l2.x * l0.x) + (l2.y * l0.y) + (l2.z * l0.z);

    if ((((l2.x * l1.x) + (l2.y * l1.y) + (l2.z * l1.z)) * temp) < 0.0f) {
        temp = -temp / ((l2.x * l2.x) + (l2.y * l2.y) + (l2.z * l2.z));
        newSoundPos->x = (l2.x * temp) + pointA->x;
        newSoundPos->y = (l2.y * temp) + pointA->y;
        newSoundPos->z = (l2.z * temp) + pointA->z;
        return true;
    }

    return false;
}

void FUN_00356018(s32 a, s32 b, s32 c);
void FUN_0033c9fc(s8 volume);
void FUN_00355fac(s32 a, s32 b, s32 c, s32 d);

// Draft (far: ROM keeps 0 (r7) and the BGM_SUB player id 3 (r8) in callee-saved regs, VFP numbering of the xz distance
// differs). #if 0, GLOBAL_ASM kept in functions_320000s.cpp.
#if 0
// Nearest river source drives the nature sequence volume (distance and height fade, left/right balance).
void FUN_00323378(Vec3f* pos, u32 seqId, u32 maxDist) {
    f32 dist;
    f32 other;
    f32 yAbs;
    u8 volume;
    s32 zero = 0;

    if (EnRiverSound_sState.unk_38 != 0) {
        EnRiverSound_sState.unk_38--;
        return;
    }

    dist = __sqrtf((pos->x * pos->x) + (pos->z * pos->z));

    if (EnRiverSound_sState.unk_80 == 0) {
        EnRiverSound_sState.unk_80 = (s32)pos;
        Audio_QueueSeqCmd(3, seqId, 0);
        FUN_00356018(3, 7, 2);
    } else {
        other = __sqrtf((((Vec3f*)EnRiverSound_sState.unk_80)->x * ((Vec3f*)EnRiverSound_sState.unk_80)->x) +
                        (((Vec3f*)EnRiverSound_sState.unk_80)->z * ((Vec3f*)EnRiverSound_sState.unk_80)->z));
        if (other <= dist) {
            dist = other;
        } else {
            EnRiverSound_sState.unk_80 = (s32)pos;
        }
    }

    yAbs = pos->y;
    if (yAbs < 0.0f) {
        yAbs = -yAbs;
    }

    if (yAbs > ((f32)maxDist * (1.0f / 15.0f)) || dist >= (f32)maxDist) {
        volume = 0;
    } else {
        volume = (u8)((1.0f - (dist / (f32)maxDist)) * 127.0f);
    }

    if (seqId != 0x01000596) {
        FUN_0033c9fc((s8)volume);
    }

    FUN_00355fac(3, 3, volume, zero);
    FUN_00355fac(0, 3, (u8)(127 - volume), zero);
}
#endif


}
