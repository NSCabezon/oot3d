// En_Po_Desert: SetNextPathPoint (FUN_00180fd4). Own TU: EnPoDesert_MoveToNextPoint tail-calls it out of line
// (armcc would inline it in the same TU); Init inlines its own copy of the same body.
extern "C" {
#include "z_en_po_desert.hpp"

void EnPoDesert_WaitForPlayer(EnPoDesert* self, PlayState* play);
void Animation_MorphToLoop(SkelAnime* skelAnime, s32 idx, f32 morph);
f32 Actor_WorldDistXZToPoint(Actor* actor, Vec3f* point);

void FUN_00180fd4(EnPoDesert* self, PlayState* play) {
    Path* path = &((Path*)(*(u8**)((u8*)play + 0x5C20)))[self->actor.params];
    Vec3s* pathPoint;

    Animation_MorphToLoop(&self->skelAnime, 0, -6.0f);
    pathPoint = &path->points[self->currentPathPoint];
    self->actor.home.pos.x = pathPoint->x;
    self->actor.home.pos.y = pathPoint->y;
    self->actor.home.pos.z = pathPoint->z;
    {
        f32 dist = Actor_WorldDistXZToPoint(&self->actor, &self->actor.home.pos);

        self->initDistToNextPoint = dist;
        if (!(dist >= 1.0f)) {
            dist = 1.0f;
        }
        self->initDistToNextPoint = dist;
    }
    self->yDiff = self->actor.home.pos.y - self->actor.world.pos.y;
    *(f32*)((u8*)self + 0x6C) = 0.0f;
    self->currentPathPoint++;
    if (path->count == self->currentPathPoint) {
        self->currentPathPoint = 0;
    }
    self->actionFunc = EnPoDesert_WaitForPlayer;
}

}
