extern "C" {
#include "non_matchings.hpp"
#include "z3D.hpp"
#include "z3Dactor.hpp"
#include "z3Dbgcheck.hpp"

GLOBAL_ASM("data/z_bgcheck.data.s")

void ReadCollisionHeaderFromZsi(void* arg1, CollisionHeader** col) {
    *col = (CollisionHeader*) arg1;
}

void* DynaPolyInfo_Alloc(DynaPolyActor* dyna, GameState* state, s32 index) {
    PlayState* play = (PlayState*)state;
    
    return play->GetObjectStatus(dyna->actor.objBankIndex)->zarInfo.FUN_003532c0(index);
}

s32 BgCheck_LineTestImpl(CollisionContext* colCtx, s32 type, s32 unk2, Vec3f* posA, Vec3f* posB, Vec3f* posResult,
                         f32 unk6, CollisionPoly** poly, s32* bgId, Actor* actor, s32 flags);

// N64 BgCheck_GetBccFlags (inlined)
static inline s32 BgCheck_GetBccFlags(s32 chkWall, s32 chkFloor, s32 chkCeil, s32 chkOneFace, s32 chkDyna) {
    s32 result = 0;

    if (chkWall) {
        result |= 1;
    }
    if (chkFloor) {
        result |= 2;
    }
    if (chkCeil) {
        result |= 4;
    }
    if (chkOneFace) {
        result |= 8;
    }
    if (chkDyna) {
        result |= 0x10;
    }
    return result;
}

// N64 BgCheck_EntityLineTest2 (moved here from Obj_Blockstop)
s32 FUN_003aa608(CollisionContext* colCtx, Vec3f* posA, Vec3f* posB, Vec3f* posResult, CollisionPoly** poly,
                 s32 chkWall, s32 chkFloor, s32 chkCeiling, s32 chkOneFace, s32* bgId, Actor* actor) {
    return BgCheck_LineTestImpl(colCtx, 2, 0, posA, posB, posResult, 1.0f, poly, bgId, actor,
                                BgCheck_GetBccFlags(chkWall, chkFloor, chkCeiling, chkOneFace, 1));
}


}
