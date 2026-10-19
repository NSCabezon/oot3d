extern "C" {
#include "z_actor_1d8.hpp"
#include "mtx34_trans.hpp"

#define FLAGS 0x00000035

void FUN_00367c48(void* obj);
void FUN_0037884c(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg);
void FUN_00367c54(void* obj);
void FUN_00367c60(void* obj, f32 arg);
void FUN_0036932c(SkeletonAnimationModel* model, s32 idx);
void FUN_0034fe20(Actor* actor, PlayState* play, SkelAnime* skelAnime, s32 cmbIndex, s32 csabIndex, void* jointTable,
                  void* morphTable, s32 limbCount);
void Animation_MorphToPlayOnce(SkelAnime* skelAnime, s32 idx, f32 morph);
s32 Animation_GetLastFrame(SkelAnime* skelAnime, s32 idx);
void FUN_00139b10(Actor1D8* self, PlayState* play);
extern Actor1D8* Actor1D8_sSelf;
extern InitChainEntry Actor1D8_sInitChain[];
extern ColliderCylinderInit* Actor1D8_sCylInits[];

void Actor1D8_Init(Actor* actor, GameState* state);
void Actor1D8_Destroy(Actor* actor, GameState* state);
void Actor1D8_Update(Actor* actor, GameState* state);
void Actor1D8_Draw(Actor* actor, GameState* state);

GLOBAL_ASM("data/z_Actor_1D8.data.s")

ActorInit Actor_1D8_InitVars = {
    ACTOR_1D8,
    ACTORCAT_BOSS,
    FLAGS,
    OBJECT_GANON_DOWN,
    sizeof(Actor1D8),
    (ActorFunc)Actor1D8_Init,
    (ActorFunc)Actor1D8_Destroy,
    (ActorFunc)Actor1D8_Update,
    (ActorFunc)Actor1D8_Draw,
};

GLOBAL_ASM("asm/Actor1D8_Init.s")

#if 0 // Actor1D8_Init draft (~75/157 words off): callee-saved numbering only (ROM player=sb zar=r6 unk_0C=r7 collider2=sl; ours player=r6 zar=r7 unk_0C=r8 collider2=sb), ROM ldm {r3,r7,r8}+ldr r6 for the 4 init ptrs and kill-arm placed after the else-arm
void Actor1D8_Init(Actor* actor, GameState* state) {
    Actor1D8* self = (Actor1D8*)actor;
    PlayState* play = (PlayState*)state;
    Player* player = (Player*)GET_PLAYER(play);
    ZARInfo* zar;

    zar = FUN_00372f38(actor, play, &self->skelAnimModel[0], 1, &self->skelAnimModel[1], 1, 0);
    SkeletonAnimationModel_unk_0C* u0 = self->skelAnimModel[0]->unk_0C;
    u0->FUN_00372d94(zar->GetCMABByIndex(1));
    SkeletonAnimationModel_unk_0C* u1 = self->skelAnimModel[1]->unk_0C;
    u1->FUN_00372d94(zar->GetCMABByIndex(1));
    Flags_SetSwitch(play, 0x14);
    Actor1D8_sSelf = self;
    actor->colChkInfo.health = 10;
    Actor_ProcessInitChain(actor, Actor1D8_sInitChain);
    ActorShape_Init(&actor->shape, 0.0f, NULL, 0.0f);
    Actor_SetScale(actor, 0.01f);
    FUN_0034fe20(actor, play, &self->skelAnime, 0, 0, 0, 0, 0);
    FUN_0036932c(self->skelAnime.unk_28, 4);
    FUN_0036932c(self->skelAnime.unk_28, 6);
    Collider_InitCylinder(play, &self->collider0);
    Collider_InitCylinder(play, &self->collider1);
    Collider_InitCylinder(play, &self->collider2);
    Collider_InitCylinder(play, &self->collider3);
    {
        ColliderCylinderInit* i0 = Actor1D8_sCylInits[0];
        ColliderCylinderInit* i1 = Actor1D8_sCylInits[1];
        ColliderCylinderInit* i2 = Actor1D8_sCylInits[2];
        ColliderCylinderInit* i3 = Actor1D8_sCylInits[3];
        Collider_SetCylinder(play, &self->collider0, actor, i0);
        Collider_SetCylinder(play, &self->collider1, actor, i1);
        Collider_SetCylinder(play, &self->collider2, actor, i2);
        Collider_SetCylinder(play, &self->collider3, actor, i3);
    }
    if (Flags_GetSwitch(play, 0x37) &&
        (play->sceneNum == 0x4F || play->sceneNum == 0x1A || play->sceneNum == 0xE || play->sceneNum == 0xF)) {
        Actor_Kill(actor);
    } else {
        Animation_MorphToPlayOnce(&self->skelAnime, 0, 0.0f);
        self->unk_264 = Animation_GetLastFrame(&self->skelAnime, 0);
        self->actionFunc = FUN_00139b10;
        self->unk_510 = 0;
        self->unk_514 = 100;
        self->unk_23C = 1;
        gSaveContext.magic = gSaveContext.magicMeterSize;
        gSaveContext.health = gSaveContext.healthCapacity;
    }
    FUN_0035af04(player, 1);
}
#endif

void Actor1D8_Destroy(Actor* actor, GameState* state) {
    Actor1D8* self = (Actor1D8*)actor;
    PlayState* play = (PlayState*)state;

    Collider_DestroyCylinder(play, &self->collider0);
    Collider_DestroyCylinder(play, &self->collider1);
    Collider_DestroyCylinder(play, &self->collider2);
    Collider_DestroyCylinder(play, &self->collider3);
    SkelAnime_Free2(&self->skelAnime);
    FUN_00350f34(actor, &self->skelAnimModel[0], &self->skelAnimModel[1], 0);
}

void FUN_0037884c(PlayState* play, s32 limbIndex, MTX34* mtx, void* arg) {
    Actor1D8* self = (Actor1D8*)arg;
    Vector3f d(3000.0f, 0.0f, 0.0f);
    Vector3f c(0.0f, 0.0f, 0.0f);
    Vector3f b(-3000.0f, 0.0f, -500.0f);
    Vector3f a(-6000.0f, 0.0f, -1000.0f);

    if (limbIndex == 1) {
        Vector3f* p0 = (Vector3f*)&self->limbPoints[0];
        VEC3TransformAsm(p0, mtx, &d);
        Vector3f* p1 = (Vector3f*)&self->limbPoints[1];
        VEC3TransformAsm(p1, mtx, &c);
        Vector3f* p2 = (Vector3f*)&self->limbPoints[2];
        VEC3TransformAsm(p2, mtx, &b);
        Vector3f* p3 = (Vector3f*)&self->limbPoints[3];
        VEC3TransformAsm(p3, mtx, &a);
    }
}

void FUN_0035b3b0(Actor* actor, PlayState* play, s32 mode, f32 target0, f32 target1) {
    Camera* camera = Gameplay_GetCamera(play, -1);

    if (mode < 0) {
        FUN_00367c48(camera);
        *(f32*)((u8*)camera + 0x144) = 52.0f;
        return;
    }
    if (mode == 0) {
        FUN_00367c54(camera);
        ((Actor1D8*)actor)->unk_5A8 = target0;
        ((Actor1D8*)actor)->unk_5AC = target1;
        FUN_00367c60(camera, ((Actor1D8*)actor)->unk_5A8);
        *(f32*)((u8*)camera + 0x144) = ((Actor1D8*)actor)->unk_5AC;
        return;
    }
    FUN_00367c54(camera);
    Math_ApproachF(&((Actor1D8*)actor)->unk_5A8, target0, 1.0f / mode, target0);
    Math_ApproachF(&((Actor1D8*)actor)->unk_5AC, target1, 1.0f / mode, target1);
    FUN_00367c60(camera, ((Actor1D8*)actor)->unk_5A8);
    *(f32*)((u8*)camera + 0x144) = ((Actor1D8*)actor)->unk_5AC;
}

GLOBAL_ASM("asm/Actor1D8_Update.s")

GLOBAL_ASM("../src/game/actors/Actor_1D8/data_syms.s")

extern Actor1D8Entry Actor1D8_sEntries[];
GLOBAL_ASM("asm/Actor1D8_Draw.s")

#if 0 // Actor1D8_Draw draft, 105/119 words off (MakeTrans+column scale: ROM re-materialises 0/0x3f800000 per iteration via strd r0,r1 and keeps only sb=1 hoisted; ours hoists them into sb/sl/fp -> spills)
void Actor1D8_Draw(Actor* actor, GameState* state) {
    Actor1D8* self = (Actor1D8*)actor;
    Actor1D8Entry* entry = Actor1D8_sEntries;
    MTX34 mtx;
    s16 i;
    s32 j = 0;

    SkelAnime_DrawOpa(&self->skelAnime, &actor->modelMtx, NULL, FUN_0037884c, actor, 0);
    self->head.x = self->pos.x;
    self->head.y = self->pos.y + 30.0f;
    self->head.z = self->pos.z;
    for (i = 0; i < 0x96; i++, entry++) {
        if (entry->type == 5) {
            FUN_003695cc(self->skelAnimModel[j], 0, 4, 2, 0.0f, 0.0f, 0.0f, (f32)entry->unk_2C * (1.0f / 255.0f));
            Vec3f t;
            t.x = entry->pos.x;
            t.y = entry->pos.y;
            t.z = entry->pos.z;
            MTX34_MakeTrans(&mtx, &t);
            {
                f32 a = entry->scaleX;
                f32 b = a * entry->scaleY;

                mtx.data[0][0] *= a;
                mtx.data[1][0] *= a;
                mtx.data[2][0] *= a;
                mtx.data[0][1] *= b;
                mtx.data[1][1] *= b;
                mtx.data[2][1] *= b;
                mtx.data[0][2] *= a;
                mtx.data[1][2] *= a;
                mtx.data[2][2] *= a;
            }
            self->skelAnimModel[j]->SetUnkAC(1);
            self->skelAnimModel[j]->FUN_003721e0(&mtx);
            self->skelAnimModel[j]->FUN_00372170(0);
            j++;
        }
        if (j == 2) {
            break;
        }
    }
}
#endif

}
