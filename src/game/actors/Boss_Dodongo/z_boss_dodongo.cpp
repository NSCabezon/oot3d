extern "C" {
#include "z_boss_dodongo.hpp"

#define FLAGS 0x00000035

void BossDodongo_Init(Actor* actor, GameState* state);
void BossDodongo_Destroy(Actor* actor, GameState* state);
void BossDodongo_Update(Actor* actor, GameState* state);
void BossDodongo_Draw(Actor* actor, GameState* state);

void EffectSs_Spawn(PlayState* play, s32 type, s32 priority, void* initParams);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 animIndex);
s32 FUN_0036e5e0(SkelAnime* skelAnime, f32 frame, f32 updateRate);
void FUN_00353020(SkelAnime* skelAnime, s32* animIndex, f32 speed, f32 startFrame, f32 endFrame,
                 s32 mode, f32 morph);
void FUN_003508b8(Actor* actor, ...);
void FUN_0034f558(SkelAnime* skelAnime);
void FUN_0034fda8(Actor* actor, s32 count, void** modelPointers);
void Collider_DestroyJntSph(PlayState* play, void* collider);
void FUN_00345590(BossDodongo* self, PlayState* play);
void FUN_001ccc78(BossDodongo* self, PlayState* play);

extern s32 BossDodongo_AnimTable[];
extern s32 BossDodongo_Anim2;
extern s32 BossDodongo_Anim4;
extern s32 BossDodongo_Anim5;
extern s32 BossDodongo_Anim6;
extern s32 BossDodongo_Anim8;
extern s32 BossDodongo_Anim9;
extern s32 BossDodongo_Anim10;
extern s32 BossDodongo_Anim11;

GLOBAL_ASM("../src/game/actors/Boss_Dodongo/data_syms.s")
GLOBAL_ASM("data/z_Boss_Dodongo.data_005144C0.s")
GLOBAL_ASM("data/z_Boss_Dodongo.data_005145B4.s")

ActorInit Boss_Dodongo_InitVars = {
    ACTOR_EN_DODONGO,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_KINGDODONGO,
    sizeof(BossDodongo),
    (ActorFunc)BossDodongo_Init,
    (ActorFunc)BossDodongo_Destroy,
    (ActorFunc)BossDodongo_Update,
    (ActorFunc)BossDodongo_Draw,
};

GLOBAL_ASM("asm/BossDodongo_Init.s")

void BossDodongo_Destroy(Actor* actor, GameState* state) {
    BossDodongo* self = (BossDodongo*)actor;

    FUN_0034f558(&self->skelAnime);
    Collider_DestroyJntSph((PlayState*)state, self->collider);
    FUN_003508b8(actor, 0);
    FUN_0034fda8(actor, 80, self->modelPointers);
}

GLOBAL_ASM("asm/BossDodongo_Update.s")

GLOBAL_ASM("asm/BossDodongo_Draw.s")

GLOBAL_ASM("asm/FUN_0029e828.s")


/* Semantics-correct 368/376 B, 66w draft; original global ASM remains active. */
#if 0
void FUN_003dc268(BossDodongo* self, PlayState* play) {
    f32 one;
    f32 zero = 0.0f;
    FUN_00370734(&self->skelAnime);
    one=1.0f;
    if (FUN_0036e5e0(&self->skelAnime, 12.0f, one)) {
        Audio_PlayActorSound2(&self->actor, 0x01000275);
    }
    if (FUN_0036e5e0(&self->skelAnime, 17.0f, one)) {
        self->unk_798 = 0x1C;
    }
    if ((17.0f < self->skelAnime.curFrame) && (35.0f > self->skelAnime.curFrame)) {
        PlayState* play2=play;
        Actor_SpawnAsChild(&play2->actorCtx, &self->actor, play2, 0x30,
                           self->unk_9BC, self->unk_9C0 - 20.0f, self->unk_9C4,
                           0, self->actor.shape.rot.y, 0, self->unk_77E);
        self->unk_77E++;
        Math_SmoothStepToF(&self->unk_814, zero, one, 8.0f, zero);
    }
    if (self->unk_7AA == 0) {
        FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[6], one, zero, 59.0f, 2, -5.0f);
        self->actionFunc = FUN_001ccc78;
        self->unk_778 = 0;
        self->unk_7AA = 0x29;
    }
}
#endif

extern s32 BossDodongo_ResetDescriptor[4];
void FUN_0036932c(void* ptr, s32 setting);
void FUN_00358778(void* ptr, s32 index, s32 arg2, void* data, s32 count);



GLOBAL_ASM("asm/FUN_003309e0.s")

/* Generic room-material helper: actor argument is unused. */
struct BossDodongoMaterialState { char pad0[12]; f32 alpha; char pad10[136]; };
struct BossDodongoRoomMaterial { char pad0[4]; void* model; char pad8[4]; u16 count; char padE[2]; BossDodongoMaterialState* states; };
struct BossDodongoMaterialColor { f32 rgba[4]; };
#if 0
void FUN_003309e0(Actor* actor, PlayState* play, s32 reset) {
    BossDodongoRoomMaterial* room=(BossDodongoRoomMaterial*)((u8*)play+0x4C38);
    f32 current;
    BossDodongoMaterialColor color;
    s32 count;
    s32 i;
    if(*((u8*)play+0x4C36)==0) return;
    count=room->count;
    for(i=0;i<count;i++) {
        if(reset!=0) room->states[i].alpha=0.0f;
        else {
            current=room->states[i].alpha;
            Math_SmoothStepToF(&current,0.0f,0.1f,0.1f,0.0f);
            room->states[i].alpha=current;
        }
    }
    if(reset!=0) current=0.0f;
    color=*(BossDodongoMaterialColor*)BossDodongo_ResetDescriptor;
    color.rgba[3]=1.0f-current;
    if(color.rgba[3]<=0.0f) color.rgba[3]=0.0f;
    if(color.rgba[3]==1.0f) FUN_0036932c(room->model,3);
    FUN_00358778(room->model,5,0,&color,2);
    FUN_00358778(room->model,1,0,&color,2);
    FUN_00358778(room->model,3,0,&color,2);
    FUN_00358778(room->model,7,0,&color,2);
    FUN_00358778(room->model,8,0,&color,2);
    FUN_00358778(room->model,11,0,&color,2);
}
#endif

void FUN_0037100c(PlayState* play, Vec3f* pos) {
    Vec3f posCopy;

    Math_Vec3f_Copy(&posCopy, pos);
    EffectSs_Spawn(play, 0x0B, 0x80, &posCopy);
}

void FUN_00181770(BossDodongo* self, PlayState* play) {
    f32 frameCount;

    frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, BossDodongo_AnimTable[2]);
    FUN_00353020(&self->skelAnime, &BossDodongo_Anim2, 1.0f, 0.0f, frameCount, 2, -10.0f);
    self->unk_77A = 0;
    self->actionFunc = FUN_00345590;
    self->unk_7AA = 0;
    self->unk_7B4 = 0.0f;
    self->actor.flags |= 1;
}

void FUN_00109cfc(BossDodongo* self, PlayState* play) {
    s32 endFrame;
    f32 speed;

    FUN_00370734(&self->skelAnime);
    endFrame = Animation_GetLastFrame(&self->skelAnime, BossDodongo_AnimTable[10]);
    speed = 1.0f;
    if (FUN_0036e5e0(&self->skelAnime, (f32)endFrame, speed)) {
        FUN_00353020(&self->skelAnime, &BossDodongo_Anim6, speed, 0.0f, 59.0f, 2, -5.0f);
        self->actionFunc = FUN_001ccc78;
        self->unk_778 = 0;
        self->unk_7AA = 0x29;
    }
}

void FUN_003d944c(BossDodongo* self, PlayState* play) {
    f32 frameCount;

    Audio_PlayActorSound2(&self->actor, 0x010002C1);
    self->unk_78E = 10;
    Math_SmoothStepToF(&self->unk_7C8, 1.0f, 0.5f, 0.02f, 0.001f);
    Math_SmoothStepToF(&self->unk_7D8, 0.05f, 1.0f, 0.005f, 0.0f);
    FUN_00370734(&self->skelAnime);
    frameCount = (f32)Animation_GetLastFrame(&self->skelAnime, BossDodongo_AnimTable[10]);
    if (self->unk_7AA == 0) {
        FUN_00353020(&self->skelAnime, &BossDodongo_Anim10, 1.0f, 0.0f, frameCount, 2, -5.0f);
        self->actionFunc = FUN_00109cfc;
    }
}

#if 0
void FUN_0013cb28(BossDodongo* self, PlayState* play) {



    f32 firstFrame;
    f32 secondFrame;
    self->unk_78E = 10;
    Math_SmoothStepToF(&self->unk_7C8, 1.3f, 1.0f, 0.1f, 0.001f);
    FUN_00370734(&self->skelAnime);
    s32 anim11=Animation_GetLastFrame(&self->skelAnime, BossDodongo_AnimTable[11]);
    firstFrame=(f32)anim11;
    s32 anim9=Animation_GetLastFrame(&self->skelAnime, BossDodongo_AnimTable[9]);
    secondFrame=(f32)anim9;
    if (FUN_0036e5e0(&self->skelAnime, firstFrame, 1.0f)) {
        FUN_00353020(&self->skelAnime, &BossDodongo_AnimTable[9], 1.0f, 0.0f,
                     secondFrame, 0, -5.0f);
        self->actionFunc = FUN_003d944c;
        self->unk_7AA = 150;
    }
}
#endif

void FUN_00121ba0(BossDodongo* self, PlayState* play) {

    FUN_00370734(&self->skelAnime);
    Math_SmoothStepToF(&self->unk_7C8, 1.0f, 0.5f, 0.02f, 0.001f);
    Math_SmoothStepToF(&self->unk_7D8, 0.05f, 1.0f, 0.005f, 0.0f);
    if (FUN_0036e5e0(&self->skelAnime,
                     (f32)Animation_GetLastFrame(&self->skelAnime, BossDodongo_AnimTable[8]), 1.0f)) {
        FUN_00353020(&self->skelAnime, &BossDodongo_Anim6, 1.0f, 0.0f, 59.0f, 2, -5.0f);
        self->actionFunc = FUN_001ccc78;
        self->unk_778 = 0;
        self->unk_7AA = 0x29;
    }
}

/* Full 232 B candidate: target X/Z in s0/s1, threshold in r2. Provisional MATCH. */
void FUN_00315350(BossDodongo* self, PlayState* play, s32 threshold, f32 targetX, f32 targetZ) {
    Player* player = GET_PLAYER(play);
    f32 dx;
    f32 dz;
    f32 distance;
    if (self->unk_766 != 0) {
        self->unk_766--;
    } else if (self->unk_768 == 0) {
        *((f32*)((u8*)player + 0x12C8)) = self->actor.world.pos.x + self->unk_818;
        *((f32*)((u8*)player + 0x12CC)) = player->actor.world.pos.y;
        *((f32*)((u8*)player + 0x12D0)) = self->actor.world.pos.z + self->unk_81C;
        FUN_0036e980(play, &self->actor, 110);
        self->unk_768++;
    } else {
        dx = player->actor.world.pos.x - targetX;
        dz = player->actor.world.pos.z - targetZ;
        distance = dx*dx + dz*dz;
        if (*((f32*)((u8*)player + 0x221C)) == 0.0f || self->unk_768 > threshold || distance < 100.0f) {
            FUN_0036e980(play, &self->actor, 1);
        }
    }
}

}
