// Draft (not matched): FUN_00373fa4 3 words off (literal-pool order of 150.0f vs table address). Kept under #if 0; GLOBAL_ASM stays in functions_370000s.cpp.
#if 0
extern "C" {
#include "z_en_zf.hpp"

extern Vec3f EnZf_sDCPlatformPositions[];

s16 FUN_00373fa4(Vec3f* pos, s16 preferred) {
    Vec3f* platformPos;
    Vec3f* p2 = pos;
    f32 range;
    s16 i;

    range = 210.0f;
    if (pos->y >= 420.0f) {
        range = 110.0f;
    }
    if (preferred != -1) {
        platformPos = &EnZf_sDCPlatformPositions[preferred];
        if ((pos->y >= (platformPos->y - 150.0f)) && (pos->y <= (platformPos->y + 150.0f)) &&
            (p2->x >= (platformPos->x - range)) && (p2->x <= (platformPos->x + range)) &&
            (pos->z >= (platformPos->z - range)) && (pos->z <= (platformPos->z + range))) {
            return preferred;
        }
    }
    for (i = 24 - 1; i >= 0; i--) {
        platformPos = &EnZf_sDCPlatformPositions[i];
        if ((pos->y >= (platformPos->y - 150.0f)) && (pos->y <= (platformPos->y + 150.0f)) &&
            (pos->x >= (platformPos->x - range)) && (pos->x <= (platformPos->x + range)) &&
            (pos->z >= (platformPos->z - range)) && (pos->z <= (platformPos->z + range))) {
            break;
        }
    }
    return i;
}


}
#endif
