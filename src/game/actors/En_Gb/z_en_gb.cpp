extern "C" {
#include "z_en_gb.hpp"

#define FLAGS 0x00000009

// highScores[HS_POE_POINTS] (3DS save struct has no highScores member yet: unk_ED0 is index 0)
#define EN_GB_POE_POINTS (((s32*)&gSaveContext.unk_ED0)[1])

void EnGb_Init(Actor* thisx, PlayState* play2);
void EnGb_Destroy(Actor* thisx, PlayState* play);
void EnGb_Update(Actor* actor, GameState* state);
void EnGb_Draw(Actor* actor, GameState* state);

void FUN_0016c874(EnGb* self, PlayState* play);
void FUN_00113768(EnGb* self, PlayState* play);
void FUN_001e11c0(EnGb* self, PlayState* play);
void FUN_001301a8(EnGb* self, PlayState* play);
void FUN_003b3bac(EnGb* self, PlayState* play);
void FUN_00130104(EnGb* self, PlayState* play);
void FUN_0019088c(EnGb* self, PlayState* play);
void FUN_003cf0c0(EnGb* self, PlayState* play);

void Player_UpdateBottleHeld(PlayState* play, Player* player, s32 item, s32 action);
void Rupees_ChangeBy(s16 value);
void LightContext_RemoveLight(PlayState* play, void* lightCtx, void* light);
void Actor_PlaySfx(Actor* actor, s32 sfxId);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void Animation_Change_00375c08(SkelAnime* skelAnime, s32 idx, f32 speed, f32 start, f32 end, s32 mode, f32 morph);
void FUN_0036bbd0(Actor* actor, PlayState* play, s32 exchangeItemId, f32 range);

GLOBAL_ASM("data/z_En_Gb.data.s")

ActorInit En_Gb_InitVars = {
    ACTOR_EN_GB,
    ACTORCAT_NPC,
    FLAGS,
    OBJECT_PS,
    sizeof(EnGb),
    (ActorFunc)EnGb_Init,
    (ActorFunc)EnGb_Destroy,
    (ActorFunc)EnGb_Update,
    (ActorFunc)EnGb_Draw,
};

#if 0 // draft: far (158 words; ROM spills thisx/play to the stack at entry (push {r0,r1,...}), self=r5/play=r7 reloaded copies)
// (draft; needs data_syms.s: EnGb_sInitChain 0x524A84, sFaceCmab 0x524A8C, sCylinderInit 0x524A98, sBottlesCylindersInit 0x524AD0, sCagedSoulPositions 0x524B78)
extern InitChainEntry EnGb_sInitChain[];
extern s32 EnGb_sFaceCmab[];
extern ColliderCylinderInit EnGb_sCylinderInit;
extern ColliderCylinderInit EnGb_sBottlesCylindersInit[];
extern Vec3f EnGb_sCagedSoulPositions[];
void Collider_SetCylinderType1(PlayState* play, ColliderCylinder* collider, Actor* actor, ColliderCylinderInit* init);
void* LightContext_InsertLight(PlayState* play, void* lightCtx, void* info);
void Lights_PointNoGlowSetInfo(void* info, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s32 a, s32 extra);

static inline void EnGb_SetTextIdFromInf(EnGb* self);

#define EN_GB_TIMER ((s16)Rand_ZeroFloat(100.0f) + 100)

void EnGb_Init(Actor* thisx, PlayState* play) {
    EnGb* self = (EnGb*)thisx;
    CollisionHeader* colHeader = NULL;
    ZARInfo* zar;
    f32 rand;
    s32 i;

    Actor_ProcessInitChain(thisx, EnGb_sInitChain);
    DynaPolyActor_Init(&self->dyna, 0);
    ReadCollisionHeaderFromZsi(DynaPolyInfo_Alloc(&self->dyna, (GameState*)play, 0), &colHeader);
    self->dyna.bgId = DynaPoly_SetBgActor(play, &play->colCtx.dyna, thisx, colHeader);
    zar = FUN_00372f38(thisx, play, &self->skelAnimModel[0], 0, 0);
    FUN_00353c9c(thisx, play, &self->skelAnime, 0, 0, (u8*)self + 0x240, (u8*)self + 0x448, 10);
    Collider_InitCylinder(play, &self->collider);
    Collider_SetCylinderType1(play, &self->collider, thisx, &EnGb_sCylinderInit);
    for (i = 0; i < 3; i++) {
        Collider_InitCylinder(play, &self->bottlesColliders[i]);
        Collider_SetCylinderType1(play, &self->bottlesColliders[i], thisx, &EnGb_sBottlesCylindersInit[i]);
    }
    self->light = LightContext_InsertLight(play, (u8*)play + 0xA70, (u8*)self + 0x7B8);
    Lights_PointNoGlowSetInfo((u8*)self + 0x7B8, (s16)thisx->home.pos.x, (s16)thisx->home.pos.y,
                              (s16)thisx->home.pos.z, 255, 255, 255, 200, 0);
    ActorShape_Init(&thisx->shape, 0.0f, ActorShadow_DrawCircle, 35.0f);
    Actor_SetScale(thisx, 0.01f);
    thisx->colChkInfo.mass = 255;
    thisx->speedXZ = 0.0f;
    thisx->velocity.y = 0.0f;
    thisx->gravity = -1.0f;
    self->actionTimer = (EN_GB_TIMER > 0) ? (s32)(0.5f + (EN_GB_TIMER * 3.0f) * 0.5f)
                                          : (s32)((EN_GB_TIMER * 3.0f) * 0.5f - 0.5f);
    for (i = 0; i < 4; i++) {
        self->cagedSouls[i].infoIdx = (s32)Rand_ZeroFloat(30.0f) % 3;
        self->cagedSouls[i].unk_14.x = self->cagedSouls[i].translation.x =
            EnGb_sCagedSoulPositions[i].x + thisx->world.pos.x;
        self->cagedSouls[i].unk_14.y = self->cagedSouls[i].translation.y =
            EnGb_sCagedSoulPositions[i].y + thisx->world.pos.y;
        self->cagedSouls[i].unk_14.z = self->cagedSouls[i].translation.z =
            EnGb_sCagedSoulPositions[i].z + thisx->world.pos.z;
        self->cagedSouls[i].unk_1 = 1;
        self->cagedSouls[i].unk_3 = self->cagedSouls[i].unk_2 = 0;
        self->cagedSouls[i].unk_20 = self->cagedSouls[i].unk_24 = 0.0f;
        self->cagedSouls[i].unk_6 = Rand_ZeroFloat(40.0f);
        self->cagedSouls[i].rotate180 = self->cagedSouls[i].unk_6 & 1;
        self->cagedSouls[i].unk_28 = 0.2f;
        FUN_00372f38(thisx, play, &self->skelAnimModel[i + 1], 1, 0);
        self->skelAnimModel[i + 1]->unk_0C->FUN_00372d94(zar->GetCMABByIndex(EnGb_sFaceCmab[self->cagedSouls[i].infoIdx]));
        self->skelAnimModel[i + 1]->unk_0C->animType = 1;
        self->skelAnimModel[i + 1]->unk_0C->animSpeed = 2.0f;
    }
    rand = Rand_ZeroOne();
    *((u8*)self + 0x7D6) = (s16)(rand * 30.0f) + 225;
    *((u8*)self + 0x7D7) = (s16)(rand * 100.0f) + 155;
    *((u8*)self + 0x7D8) = (s16)(rand * 160.0f) + 95;
    *((u8*)self + 0x7D9) = 200;
    EnGb_SetTextIdFromInf(self);
    self->actionFunc = FUN_00113768;
}
#endif

GLOBAL_ASM("asm/EnGb_Init.s")

void EnGb_Destroy(Actor* thisx, PlayState* play) {
    EnGb* self = (EnGb*)thisx;
    s32 i;

    Collider_DestroyCylinder(play, &self->collider);
    FUN_003508b8(thisx, self->skelAnimModel[0], 0);
    SkelAnime_Free2(&self->skelAnime);
    for (i = 0; i < 4; i++) {
        FUN_003508b8(thisx, self->skelAnimModel[i + 1], 0);
        self->skelAnimModel[i + 1] = NULL;
    }
    LightContext_RemoveLight(play, (u8*)play + 0xA70, self->light);
    DynaPoly_DeleteBgActor(play, (DynaCollisionContext*)((u8*)play + 0xAE8), self->dyna.bgId);
}

GLOBAL_ASM("asm/EnGb_Update.s")

#if 0 // draft: far (needs data_syms.s: EnGb_sDrawColorInit EQU 0x004D9614; #include "mtx34_trans.hpp") (~120 words: ROM keeps Vec3f at sp+4/mtx at sp+0x10, hoisted 0/1.0 in r7/r8, rotate angle via inline helper param s20)
typedef struct { f32 v[4]; } EnGbVec4;
extern EnGbVec4 EnGb_sDrawColorInit;
void GraphicsContext_UnknownStub(GraphicsContext* gfxCtx);
void FUN_00358778(SkeletonAnimationModel* model, s32 arg1, s32 arg2, f32* color, s32 arg4);
void Lights_PointNoGlowSetInfo(void* info, f32 x, f32 y, f32 z, u8 r, u8 g, u8 b, s32 a, s32 extra);
void FUN_00371fac(MTX34* mtx, void* billboard);
u32 PauseContext_GetState(void);
f32 sinf(f32 x);
f32 cosf(f32 x);

#define LC(n) (*((u8*)self + 0x7D6 + (n)))
void EnGb_Draw(Actor* actor, GameState* state) {
    EnGb* self = (EnGb*)actor;
    PlayState* play = (PlayState*)state;
    EnGbVec4 color;
    f32 mult;
    f32 flicker;
    f32 scale;
    f32 angle;
    s32 i;

    GraphicsContext_UnknownStub(play->state.gfxCtx);
    color = EnGb_sDrawColorInit;
    color.v[0] = LC(0) * (1.0f / 255.0f);
    color.v[1] = LC(1) * (1.0f / 255.0f);
    color.v[2] = LC(2) * (1.0f / 255.0f);
    FUN_00358778(self->skelAnime.unk_28, 1, 4, color.v, 1);
    Lights_PointNoGlowSetInfo((u8*)self + 0x7B8, self->dyna.actor.world.pos.x, self->dyna.actor.world.pos.y,
                              self->dyna.actor.world.pos.z, LC(0), LC(1), LC(2), LC(3), 0);
    SkelAnime_DrawOpa(&self->skelAnime, &self->dyna.actor.modelMtx, NULL, NULL, &self->dyna.actor, 0);

    mult = 2.0f;
    flicker = 0.0f;
    angle = -3.1415927f;
    scale = 0.007f;
    if (PauseContext_GetState() != 0) {
        mult = flicker;
    }
    for (i = 0; i < 4; i++) {
        MTX34 mtx;
        Vec3f t;

        t.x = self->cagedSouls[i].translation.x;
        t.y = self->cagedSouls[i].translation.y;
        t.z = self->cagedSouls[i].translation.z;
        MTX34_MakeTrans(&mtx, &t);
        FUN_00371fac(&mtx, (u8*)play + 0x2FC);
        if (self->cagedSouls[i].rotate180) {
            f32 sn = sinf(angle);
            f32 cs = cosf(angle);
            f32* m = (f32*)&mtx;
            f32 a, b;

            a = m[0]; b = m[2]; m[0] = a * cs - b * sn; m[2] = a * sn + b * cs;
            a = m[4]; b = m[6]; m[4] = a * cs - b * sn; m[6] = a * sn + b * cs;
            a = m[8]; b = m[10]; m[8] = a * cs - b * sn; m[10] = a * sn + b * cs;
        }
        {
            f32* m = (f32*)&mtx;

            m[0] *= scale;
            m[4] *= scale;
            m[8] *= scale;
            m[1] *= scale;
            m[5] *= scale;
            m[9] *= scale;
        }
        self->skelAnimModel[i]->SetUnkAC(1);
        self->skelAnimModel[i]->FUN_003721e0(&mtx);
        self->skelAnimModel[i]->unk_0C->animSpeed = mult;
        self->skelAnimModel[i]->FUN_00372170(0);
    }
}
#endif

GLOBAL_ASM("asm/EnGb_Draw.s")

static inline void EnGb_SetTextIdFromInf(EnGb* self) {
    if (gSaveContext.infTable[11] & 0x40) {
        self->textId = 0x70F5;
    } else {
        self->textId = 0x70F4;
    }
}

void FUN_001301a8(EnGb* self, PlayState* play) {
    if (Actor_HasParent(&self->dyna.actor, play)) {
        self->dyna.actor.parent = NULL;
        self->actionFunc = FUN_0016c874;
    } else {
        GiveItem(&self->dyna.actor, play, 0xF, 100.0f, 10.0f);
    }
}

void FUN_001e11c0(EnGb* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        GiveItem(&self->dyna.actor, play, 0xF, 100.0f, 10.0f);
        self->actionFunc = FUN_001301a8;
    }
}

void FUN_0016c874(EnGb* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        Actor_ProcessTalkRequest(&self->dyna.actor, play);
        EnGb_SetTextIdFromInf(self);
        self->actionFunc = FUN_00113768;
    }
}

void FUN_003b3bac(EnGb* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        if (!(gSaveContext.infTable[11] & 0x40)) {
            gSaveContext.infTable[11] |= 0x40;
        }
        EnGb_SetTextIdFromInf(self);
        self->actionFunc = FUN_00113768;
    }
}

void FUN_00130104(EnGb* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        if (!(gSaveContext.infTable[11] & 0x40)) {
            gSaveContext.infTable[11] |= 0x40;
        }
        EnGb_SetTextIdFromInf(self);
        Player_UpdateBottleHeld(play, GET_PLAYER(play), 0x14, 0x1E);
        Rupees_ChangeBy(10);
        self->actionFunc = FUN_00113768;
    }
}

void FUN_0019088c(EnGb* self, PlayState* play) {
    if (Message_GetState(&play->msgCtx) == 6 && Message_ShouldAdvance(play)) {
        if (!(gSaveContext.infTable[11] & 0x40)) {
            gSaveContext.infTable[11] |= 0x40;
        }
        EnGb_SetTextIdFromInf(self);
        Player_UpdateBottleHeld(play, GET_PLAYER(play), 0x14, 0x1E);
        Rupees_ChangeBy(50);
        EN_GB_POE_POINTS += 100;
        if (EN_GB_POE_POINTS != 1000) {
            s32 score = EN_GB_POE_POINTS;

            EN_GB_POE_POINTS = (score > 1100) ? 1100 : score;
            self->actionFunc = FUN_00113768;
        } else {
            Player* player = GET_PLAYER(play);

            player->exchangeItemId = 0;
            self->textId = 0x70F8;
            Message_ContinueTextbox(play, self->textId);
            self->actionFunc = FUN_001e11c0;
        }
    }
}

#if 0 // draft: 21 words off: ROM loads the address of FUN_00113768 itself from the literal pool (ours sub-pc) and has an armcc nop after the timer==0 actionFunc load
static inline s32 EnGb_AnyCageSoulActive(EnGb* self) {
    s32 i;

    for (i = 0; i < ARRAY_COUNT(self->cagedSouls); i++) {
        if (self->cagedSouls[i].unk_3) {
            return 1;
        }
    }
    return 0;
}

void FUN_00113768(EnGb* self, PlayState* play) {
    Player* player;
    s8 exchangeItemId;
    f32 zero = 0.0f;
    f32 one = 1.0f;

    if (!EnGb_AnyCageSoulActive(self)) {
        if (self->actionTimer != 0) {
            self->actionTimer--;
        } else {
            Animation_Change_00375c08(&self->skelAnime, 1, one, zero, Animation_GetLastFrame(&self->skelAnime, 1), 2,
                                      zero);
            self->actionFunc = FUN_003cf0c0;
            goto done;
        }
    }
    player = GET_PLAYER(play);
    if (Actor_ProcessTalkRequest(&self->dyna.actor, play)) {
        exchangeItemId = FUN_0036bc84(play);
        EnGbActionFunc cur = self->actionFunc;
        if (cur != FUN_00113768) {
            Animation_Change_00375c08(&self->skelAnime, 0, one, zero, Animation_GetLastFrame(&self->skelAnime, 0), 0,
                                      zero);
        }
        switch (exchangeItemId) {
            case 0:
                EnGb_SetTextIdFromInf(self);
                self->actionFunc = FUN_003b3bac;
                break;
            case 0x1B:
                player->actor.textId = 0x70F6;
                self->actionFunc = FUN_00130104;
                break;
            case 0x1C:
                player->actor.textId = 0x70F7;
                self->actionFunc = FUN_0019088c;
                break;
        }
    } else if (self->dyna.actor.xzDistToPlayer < 100.0f) {
        FUN_0036bbd0(&self->dyna.actor, play, 0x1B, 100.0f);
    }
done:;
}
#endif

}
