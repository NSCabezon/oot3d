static __inline __pure float EnHoll_Fabsf(float f) {
    return __fabsf(f);
}

namespace nn {
namespace svc {
int SleepThread(long long);
}
} // namespace nn

extern "C" {
#include "z_en_holl.hpp"

#define FLAGS 0x00000010

void EnHoll_Init(Actor* thisx, PlayState* play);
void EnHoll_Destroy(Actor* thisx, PlayState* play);
void EnHoll_Update(EnHoll* self, PlayState* play);
void EnHoll_Draw(Actor* actor, GameState* state);

s8 FUN_00339f0c(void);
void FUN_00342230(void);

// Action functions (N64 sActionFuncs order). They live here (moved from functions_*.cpp) unless noted.
void FUN_003f1d7c(EnHoll* self, PlayState* play); // N64: EnHoll_HorizontalVisibleNarrow
void FUN_00239934(EnHoll* self, PlayState* play); // N64: EnHoll_VerticalDownBgCoverLarge
void FUN_00275264(EnHoll* self, PlayState* play); // N64: EnHoll_VerticalInvisible
void FUN_001eaab0(EnHoll* self, PlayState* play); // N64: EnHoll_HorizontalBgCoverSwitchFlag
void FUN_001eac68(EnHoll* self, PlayState* play); // N64: EnHoll_HorizontalInvisible
void FUN_0021df78(EnHoll* self, PlayState* play); // N64: EnHoll_VerticalBgCover
void FUN_001ce6b8(EnHoll* self, PlayState* play); // N64: EnHoll_WaitRoomLoaded (still asm)

s32 FUN_0033b6bc(PlayState* play, void* roomCtx, s32 num); // N64: Room_RequestNewRoom
void FUN_0033b608(void);
void FUN_0036c520(PlayState* play, void* roomCtx); // N64: Room_FinishRoomChange

GLOBAL_ASM("data/z_En_Holl.data.s")

GLOBAL_ASM("../src/game/actors/En_Holl/data_syms.s")

// The table of action functions (0x00526488), init chain (0x005264A4), the narrow-holl trigger distances
// (0x004D9664) and a debug cam flag (0x00516EAC) have no symbols in the scatter file: absolute symbols (EQU).
extern EnHollActionFunc EnHoll_sActionFuncs[];
extern InitChainEntry EnHoll_sInitChain[];
extern f32 EnHoll_sHorizontalVisibleNarrowTriggerDists[2][4];
extern s32 gDbgCamEnabled;

ActorInit En_Holl_InitVars = {
    ACTOR_EN_HOLL,
    ACTORCAT_DOOR,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnHoll),
    (ActorFunc)EnHoll_Init,
    (ActorFunc)EnHoll_Destroy,
    (ActorFunc)EnHoll_Update,
    (ActorFunc)EnHoll_Draw,
};

// PlayState / RoomContext are not filled in the repo headers yet: raw offsets.
typedef struct {
    s8 room;
    s8 bgCamIndex;
} EnHollSide;
typedef struct {
    EnHollSide sides[2];
    s16 id;
    s16 pos[3];
    s16 rotY;
    s16 params;
} EnHollTransitionEntry; // size = 0x10
typedef struct {
    u8 data[0x3DC];
} EnHollRoom;

#define ENHOLL_TRANSITIONS(play) (*(EnHollTransitionEntry**)((u8*)(play) + 0x5B8C)) // play->transitionActors.list
#define ENHOLL_CUR_ROOM_NUM(play) (*(s8*)((u8*)(play) + 0x4C30))                    // play->roomCtx.curRoom.num
#define ENHOLL_PREV_ROOM_NUM(play) (*(s8*)((u8*)(play) + 0x500C))                   // play->roomCtx.prevRoom.num
#define ENHOLL_ROOMCTX(play) ((void*)((u8*)(play) + 0x4C30))
#define ENHOLL_CUR_ROOM(play) (*(EnHollRoom*)((u8*)(play) + 0x4C30))
#define ENHOLL_PREV_ROOM(play) (*(EnHollRoom*)((u8*)(play) + 0x500C))
#define ENHOLL_ROOM_STATUS(play) (*((u8*)(play) + 0x5401))                          // play->roomCtx.status
#define ENHOLL_BGCOVER_ALPHA(play) (*(s16*)((u8*)(play) + 0x5C30))                  // play->bgCoverAlpha
#define ENHOLL_PLAYER(play) (*(Actor**)((u8*)(play) + 0x20AC))
#define ENHOLL_TRANSITION_INDEX(actor) ((u16)(actor)->params >> 10)

static inline void EnHoll_SetupAction(EnHoll* self, EnHollActionFunc func) {
    self->actionFunc = func;
}

static inline void EnHoll_ChooseAction(EnHoll* self) {
    s32 type = ((u16)self->actor.params >> 6) & 7;

    EnHoll_SetupAction(self, EnHoll_sActionFuncs[type]);
    if (type != 0) {
        self->actor.draw = NULL;
    } else {
        self->planeAlpha = 255;
    }
}

void EnHoll_Init(Actor* thisx, PlayState* play) {
    EnHoll* self = (EnHoll*)thisx;

    self->actor.unk_19A = 1;
    FUN_00372f38(&self->actor, play, &self->skelAnimModel, 0x55, 0);
    self->unk_1B0 = 0xFF;
    Actor_ProcessInitChain(&self->actor, EnHoll_sInitChain);
    EnHoll_ChooseAction(self);
    self->resetBgCoverAlpha = 0;
}

void EnHoll_Destroy(Actor* thisx, PlayState* play) {
    EnHoll* self = (EnHoll*)thisx;
    s32 idx = (u16)thisx->params >> 10;
    u8* list = *(u8**)((u8*)play + 0x5B8C);
    s32 off = 4 + (idx << 4);
    s16 id;

    id = *(s16*)(list + off);
    *(s16*)(list + off) = -id;
    FUN_00350f34(thisx, &self->skelAnimModel, 0);
    if (self->unk_1B0 > 0) {
        FUN_00342230();
    }
}

void EnHoll_Update(EnHoll* self, PlayState* play) {
    if (FUN_00339f0c()) {
        if (*((u8*)play + 0x5401) == 0) {
            self->unk_1B0 = 1;
        }
    }
    if (self->unk_1B0 > 0) {
        self->unk_1B0--;
        if (self->unk_1B0 == 0) {
            FUN_00342230();
        }
    }
    self->actionFunc(self, play);
}

void FUN_00239934(EnHoll* self, PlayState* play) {
    Actor* player = ENHOLL_PLAYER(play);
    f32 absY = EnHoll_Fabsf(self->actor.yDistToPlayer);
    s32 idx;

    if (self->actor.xzDistToPlayer < 500.0f && absY < 700.0f) {
        idx = (u16)self->actor.params >> 10;
        if (absY < 520.0f) {
            ENHOLL_BGCOVER_ALPHA(play) = 255;
        } else if (absY > 180.0f) {
            ENHOLL_BGCOVER_ALPHA(play) = 0;
        } else {
            ENHOLL_BGCOVER_ALPHA(play) = (s16)(180.0f - absY) * 0.5f;
        }
        if (absY < 520.0f) {
            self->actor.room = (*(u8**)((u8*)(play) + 0x5B8C))[idx * 16 + 2];
            Math_SmoothStepToF(&player->world.pos.x, self->actor.world.pos.x, 0.25f, 3.476f, 0.39999998f);
            Math_SmoothStepToF(&player->world.pos.z, self->actor.world.pos.z, 0.25f, 3.476f, 0.39999998f);
            if (self->actor.room != ENHOLL_CUR_ROOM_NUM(play) && FUN_0033b6bc(play, ENHOLL_ROOMCTX(play), self->actor.room)) {
                FUN_0033b608();
                self->actionFunc = FUN_001ce6b8;
                self->resetBgCoverAlpha = 1;
                player->speedXZ = 0.0f;
            }
        }
    } else if (self->resetBgCoverAlpha) {
        ENHOLL_BGCOVER_ALPHA(play) = 0;
        self->resetBgCoverAlpha = 0;
    }
}

// ---- Unmatched drafts (kept under #if 0; their GLOBAL_ASM stays in functions_*.cpp / below) ----
// Raw offsets (see macros above): roomCtx.curRoom @0x4C30, prevRoom @0x500C, status @0x5401, transition list @0x5B8C,
// bgCoverAlpha @0x5C30, player @0x20AC.
#if 0
// FUN_00275264: 2 words differ (idx register: ROM lsr r1,r1 in place; build uses r2)
void FUN_00275264(EnHoll* self, PlayState* play) {
    f32 yd;
    f32 absY;
    u32 idx;
    s32 side;
    if (self->actor.xzDistToPlayer < 120.0f) {
        yd = self->actor.yDistToPlayer;
        absY = EnHoll_Fabsf(yd);
        if (absY < 200.0f) { if (absY > 50.0f) {
            idx = ((u16)self->actor.params >> 10) & 0x3F;
            side = (yd > 0.0f) ? 0 : 1;
            self->actor.room = (*(u8**)((u8*)(play) + 0x5B8C))[idx * 16 + side * 2];
            if (self->actor.room != ENHOLL_CUR_ROOM_NUM(play) && FUN_0033b6bc(play, ENHOLL_ROOMCTX(play), self->actor.room)) {
                FUN_0033b608();
                self->actionFunc = FUN_001ce6b8;
            }
        }}
    }
}

// FUN_0021df78: 2 words differ (same idx register issue)
void FUN_0021df78(EnHoll* self, PlayState* play) {
    f32 absY;
    s8 side;
    s32 idx;

    if (self->actor.xzDistToPlayer < 120.0f) {
        absY = EnHoll_Fabsf(self->actor.yDistToPlayer);
        if (absY < 200.0f) {
        if (absY < 50.0f) {
            ENHOLL_BGCOVER_ALPHA(play) = 255;
        } else {
            ENHOLL_BGCOVER_ALPHA(play) = (200.0f - absY) * 1.7f;
        }
        if (absY > 50.0f) {
            idx = (u16)self->actor.params >> 10;
            side = (self->actor.yDistToPlayer > 0.0f) ? 0 : 1;
            self->actor.room = (*(u8**)((u8*)(play) + 0x5B8C))[idx * 16 + side * 2];
            if (self->actor.room != ENHOLL_CUR_ROOM_NUM(play) && FUN_0033b6bc(play, ENHOLL_ROOMCTX(play), self->actor.room)) {
                FUN_0033b608();
                self->actionFunc = FUN_001ce6b8;
                self->resetBgCoverAlpha = 1;
            }
        }
        goto done;
        }
    }
    if (self->resetBgCoverAlpha) {
        self->resetBgCoverAlpha = 0;
        ENHOLL_BGCOVER_ALPHA(play) = 0;
    }
done:;
}

// FUN_001eaab0: 11 words differ, only play/play+0x5C00 callee-saved register swap (r5/r6); needs the goto to share the reset block
void FUN_001eaab0(EnHoll* self, PlayState* play) {
    Actor* player = ENHOLL_PLAYER(play);
    Vec3f rel;
    f32 orth;
    s32 idx;
    s32 side;

    if (!Flags_GetSwitch(play, (u16)self->actor.params & 0x3F)) {
        if (self->resetBgCoverAlpha) {
            goto reset;
        }
    } else {
        FUN_0036c5d8(&self->actor, &rel, &player->world.pos);
        orth = EnHoll_Fabsf(rel.z);
        if (-50.0f < rel.y && rel.y < 200.0f && EnHoll_Fabsf(rel.x) < 200.0f && orth < 100.0f) {
            idx = (u16)self->actor.params >> 10;
            self->resetBgCoverAlpha = 1;
            ENHOLL_BGCOVER_ALPHA(play) = 255 - (s32)((orth - 50.0f) * (255 / 50.0f + 0.8f));
            if (ENHOLL_BGCOVER_ALPHA(play) > 255) {
                ENHOLL_BGCOVER_ALPHA(play) = 255;
            } else if (ENHOLL_BGCOVER_ALPHA(play) < 0) {
                ENHOLL_BGCOVER_ALPHA(play) = 0;
            }
            if (orth < 50.0f) {
                side = (rel.z < 0.0f) ? 0 : 1;
                self->actor.room = (*(u8**)((u8*)(play) + 0x5B8C))[idx * 16 + side * 2];
                if (self->actor.room != ENHOLL_CUR_ROOM_NUM(play) && FUN_0033b6bc(play, ENHOLL_ROOMCTX(play), self->actor.room)) {
                    FUN_0033b608();
                    self->actionFunc = FUN_001ce6b8;
                }
            }
        } else if (self->resetBgCoverAlpha) {
        reset:
            ENHOLL_BGCOVER_ALPHA(play) = 0;
            self->resetBgCoverAlpha = 0;
        }
    }
}

// FUN_003f1d7c: 10 words differ: planeAlpha store schedules mov #0x1a4 early (ROM: right before usat/strh)
void FUN_003f1d7c(EnHoll* self, PlayState* play) {
    Actor* player = ENHOLL_PLAYER(play);
    s32 trigIdx = (u32)((play->sceneNum == 6) ? 1 : 0);
    Vec3f rel;
    f32 orth;
    EnHollRoom tmp;
    s32 idx;
    s8 room;
    f32* d = EnHoll_sHorizontalVisibleNarrowTriggerDists[trigIdx];

    FUN_0036c5d8(&self->actor, &rel, &player->world.pos);
    self->side = (rel.z < 0.0f) ? 0 : 1;
    orth = EnHoll_Fabsf(rel.z);
    if (rel.y > -50.0f && rel.y < 200.0f && EnHoll_Fabsf(rel.x) < 100.0f && orth < d[0]) {
        idx = (u16)self->actor.params >> 10;
        if (orth > d[1]) {
            if (ENHOLL_PREV_ROOM_NUM(play) >= 0 && *((u8*)play + 0x5401) == 0) {
                self->actor.room = ENHOLL_TRANSITIONS(play)[idx].sides[self->side].room;
                tmp = ENHOLL_CUR_ROOM(play);
                ENHOLL_CUR_ROOM(play) = ENHOLL_PREV_ROOM(play);
                ENHOLL_PREV_ROOM(play) = tmp;
                FUN_0036c520(play, ENHOLL_ROOMCTX(play));
            }
        } else {
            self->actor.room = ENHOLL_TRANSITIONS(play)[idx].sides[self->side ^ 1].room;
            if (ENHOLL_PREV_ROOM_NUM(play) < 0) {
                while (!FUN_0033b6bc(play, ENHOLL_ROOMCTX(play), self->actor.room)) {
                    nn::svc::SleepThread(0);
                }
                FUN_0033b608();
                self->unk_1B0 = 15;
            } else {
                s16 a = (255.0f / (d[2] - d[3])) * (orth - d[3]);
                a = CLAMP(a, 0, 255);
                self->planeAlpha = a;
                if (ENHOLL_CUR_ROOM_NUM(play) != self->actor.room) {
                    tmp = ENHOLL_CUR_ROOM(play);
                    ENHOLL_CUR_ROOM(play) = ENHOLL_PREV_ROOM(play);
                    ENHOLL_PREV_ROOM(play) = tmp;
                    self->unk_1B0 = 1;
                }
            }
        }
    }
}

// FUN_001eac68: ~60 words differ: play/actor in swapped callee-saved regs (r4/r5), ROM keeps isKokiri live in r0 through a dead predicated ldrsb of actor.room
void FUN_001eac68(Actor* thisx, PlayState* play) {
    EnHoll* self = (EnHoll*)thisx;
    Actor* player = ENHOLL_PLAYER(play);
    s32 useViewEye = gDbgCamEnabled || play->GetCutsceneState() != 0;
    Vec3f rel;
    s32 isKokiri;
    f32 halfWidth;
    f32 orth;
    s32 idx;
    s32 side;
    s8 room;

    FUN_0036c5d8(&self->actor, &rel, useViewEye ? &play->view.eye : &player->world.pos);
    halfWidth = (((u16)self->actor.params >> 6 & 7) == 6) ? 100.0f : 200.0f;
    isKokiri = gSaveContext.entranceIndex == 0xEE && *(s32*)((u8*)&gSaveContext + 0x14E8) == 8;
    if (isKokiri) goto body;
    if (rel.y > -50.0f && rel.y < 200.0f && EnHoll_Fabsf(rel.x) < halfWidth) {
        orth = EnHoll_Fabsf(rel.z);
        if (orth < 100.0f) {
            if (orth > 50.0f) goto body;
        }
    }
    return;
body:
    {
        idx = (u16)self->actor.params >> 10;
        side = (rel.z < 0.0f) ? 0 : 1;
        EnHollTransitionEntry* entry = &ENHOLL_TRANSITIONS(play)[idx];
        room = entry->sides[side].room;
        self->actor.room = room;
        if (isKokiri) {
        }
        if (self->actor.room != ENHOLL_CUR_ROOM_NUM(play)) {
            if (room) {
            }
            if (FUN_0033b6bc(play, ENHOLL_ROOMCTX(play), self->actor.room)) {
                FUN_0033b608();
                self->actionFunc = FUN_001ce6b8;
            }
        }
    }
}

#endif

#if 0
// EnHoll_Draw: 3DS-specific (model-based plane + per-room tint for scene 1). Structure is right, 792 B in ROM vs 768 B here:
//  - ROM keeps the constant 1 for SetUnkAC in r7 (hoisted above the `side == 0` rotation); here it is rematerialised.
//  - ROM shares one `bl FUN_003695cc` after case 10 (cases 0/7/1,8/9 jump to it) and keeps case 12 separate; armcc here
//    merges differently (tried all case orders / default placement).
//  - sinf/cosf(pi) of the Y-rotation are NOT folded (as in ROM). Per-frame statics live at 0x00526480 (EQU below).
typedef struct { s32 unk_00; SkeletonAnimationModel* model; } EHRoomObj;
typedef struct {
    s8 num;
    u8 unk_01[5];
    u8 unk_06;
    u8 unk_07;
    EHRoomObj unk_08;
} EHRoomHead;
typedef struct { u32 frame; f32 minDist; } EHDrawState;
extern EHDrawState EnHoll_sDrawState;

extern EHDrawState EnHoll_sDrawState;
void EnHoll_Draw(Actor* actor, GameState* state) {
    EnHoll* self = (EnHoll*)actor;
    PlayState* play = (PlayState*)state;
    f32 alpha;
    s32 i;

    if (self->planeAlpha != 0) {
        if (self->skelAnimModel != NULL) {
            f32 color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
            void* p;
            s32 one;

            color[3] = self->planeAlpha * (1.0f / 255.0f);
            p = FUN_003687a8(self->skelAnimModel);
            FUN_003589cc(p, 4);
            FUN_00358964(p, 4, color);
            one = 1;
            if (self->side == 0) {
                f32 s = sinf(3.1415927f);
                f32 c = cosf(3.1415927f);
                f32 x, z;
                x = self->actor.modelMtx.data[0][0]; z = self->actor.modelMtx.data[0][2];
                self->actor.modelMtx.data[0][0] = x * c - z * s; self->actor.modelMtx.data[0][2] = x * s + z * c;
                x = self->actor.modelMtx.data[1][0]; z = self->actor.modelMtx.data[1][2];
                self->actor.modelMtx.data[1][0] = x * c - z * s; self->actor.modelMtx.data[1][2] = x * s + z * c;
                x = self->actor.modelMtx.data[2][0]; z = self->actor.modelMtx.data[2][2];
                self->actor.modelMtx.data[2][0] = x * c - z * s; self->actor.modelMtx.data[2][2] = x * s + z * c;
            }
            self->skelAnimModel->FUN_003721e0(&self->actor.modelMtx);
            self->skelAnimModel->SetUnkAC(one);
            self->skelAnimModel->FUN_00372170(1);
        }
        alpha = self->planeAlpha * (1.0f / 255.0f);
        if (play->sceneNum == 1) {
            f32 absY = EnHoll_Fabsf(self->actor.yDistToPlayer);
            if (self->actor.xzDistToPlayer <= 500.0f && absY <= 700.0f) {
                if (play->state.frames != EnHoll_sDrawState.frame) {
                    EnHoll_sDrawState.minDist = 3.4028235e38f;
                    EnHoll_sDrawState.frame = play->state.frames;
                }
                if (self->actor.xzDistToPlayer < EnHoll_sDrawState.minDist) {
                    EnHoll_sDrawState.minDist = self->actor.xzDistToPlayer;
                    EHRoomHead* rooms[2];
                    rooms[0] = (EHRoomHead*)((u8*)play + 0x4C30);
                    rooms[1] = (EHRoomHead*)((u8*)play + 0x500C);
                    for (i = 0; i < 2; i++) {
                        EHRoomHead* room = rooms[i];
                        s32 num = room->num;
                        EHRoomObj* obj;
                        SkeletonAnimationModel* model;
                        if (room->unk_06 != 0 && (obj = &room->unk_08) != NULL && (model = obj->model) != NULL) {
                            switch (num) {
                                case 0:
                                    FUN_003695cc(model, 0x14, 4, 2, 0.0f, 0.0f, 0.0f, alpha);
                                    break;
                                case 7:
                                    FUN_003695cc(model, 7, 4, 2, 0.0f, 0.0f, 0.0f, alpha);
                                    break;
                                case 1:
                                case 8:
                                    FUN_003695cc(model, 4, 4, 2, 0.0f, 0.0f, 0.0f, alpha);
                                    break;
                                case 9:
                                    FUN_003695cc(model, 8, 4, 2, 0.0f, 0.0f, 0.0f, alpha);
                                    break;
                                case 10:
                                    FUN_003695cc(model, 5, 4, 2, 0.0f, 0.0f, 0.0f, alpha);
                                    break;
                                case 2: case 3: case 4: case 5: case 6: case 11:
                                    break;
                                case 12:
                                    FUN_003695cc(model, 5, 4, 2, 0.0f, 0.0f, 0.0f, alpha);
                                    break;
                            }
                        }
                    }
                }
            }
        }
    }
}
#endif

GLOBAL_ASM("asm/EnHoll_Draw.s")

}
