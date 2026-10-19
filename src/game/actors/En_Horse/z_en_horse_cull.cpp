// FUN_0031d150 = Actor_CullingVolumeTest (N64). Kept in its own TU so armcc treats it as opaque in En_Horse callers
// (same-file callee knowledge changes the callers' VFP register choices).
extern "C" {
#include "z_en_horse.hpp"

extern __pure float _fabsf(float);
static __inline __pure float EnHorse_Fabsf(float f) {
    return _fabsf(f);
}

s32 FUN_0031d150(f32 projW, PlayState* play, Actor* actor, Vec3f* projPos) {
    if (actor->uncullZoneForward + actor->uncullZoneScale <= projPos->z) {
        ((u8*)actor)[0x120] = 2;
        return false;
    }
    if (-actor->uncullZoneScale < projPos->z) {
        f32 invW = (projW < 1.0f) ? 1.0f : 1.0f / projW;

        if ((EnHorse_Fabsf(projPos->x) - actor->uncullZoneScale) * invW < 1.0f) {
            f32 downward = projPos->y + actor->uncullZoneDownward;

            if (downward * invW > -1.0f && (projPos->y - actor->uncullZoneScale) * invW < 1.0f) {
                ((u8*)actor)[0x120] = 0;
                return true;
            }
        }
    }
    ((u8*)actor)[0x120] = 1;
    return false;
}
}
