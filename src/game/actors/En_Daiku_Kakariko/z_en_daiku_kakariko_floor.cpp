extern "C" {
#include "z_en_daiku_kakariko.hpp"

f32 BgCheck_EntityRaycastFloor4(CollisionContext* colCtx, CollisionPoly** outPoly, s32* bgId, Actor* actor, Vec3f* pos);

f32 FUN_00299ec0(PlayState* play, Actor* actor) {
    s32 bgId = 50;

    return BgCheck_EntityRaycastFloor4(&play->colCtx, &actor->floorPoly, &bgId, actor, &actor->world.pos);
}

}
