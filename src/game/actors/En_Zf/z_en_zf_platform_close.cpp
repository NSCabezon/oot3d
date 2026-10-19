extern "C" {
#include "z_en_zf.hpp"

extern Vec3f EnZf_sDCPlatformPositions[];

s16 FUN_00373fa4(Vec3f* pos, s16 preferred);

s32 FUN_003262b8(Vec3f* thisPos, s16 curPlatform, s16 fallbackPlatform, PlayState* play) {
    f32 platformDistToPlayer;
    f32 closestPlatformToPlayerDist;
    f32 secondClosestPlatformToPlayerDist;
    f32 maxPlatformDist;
    s16 i;
    s16 closestPlatformToPlayer;
    s16 secondClosestPlatformToPlayer;
    s16 playerPlatform;
    s32 iMin;
    Player* player;

    i = 7;
    iMin = 0;
    player = GET_PLAYER(play);
    maxPlatformDist = 500.0f;
    playerPlatform = FUN_00373fa4(&player->actor.world.pos, -1);
    closestPlatformToPlayerDist = 99998.0f;
    secondClosestPlatformToPlayerDist = 99999.0f;
    closestPlatformToPlayer = curPlatform;
    secondClosestPlatformToPlayer = fallbackPlatform;
    if (thisPos->y > 200.0f) {
        maxPlatformDist = 290.0f;
        i = 23;
        iMin = 8;
    }
    for (; i >= iMin; i--) {
        if (Math_Vec3f_DistXYZ(thisPos, &EnZf_sDCPlatformPositions[i]) > maxPlatformDist) {
            continue;
        }
        if (i != playerPlatform) {
            platformDistToPlayer = Math_Vec3f_DistXYZ(&player->actor.world.pos, &EnZf_sDCPlatformPositions[i]);
            if (platformDistToPlayer < closestPlatformToPlayerDist) {
                secondClosestPlatformToPlayerDist = closestPlatformToPlayerDist;
                secondClosestPlatformToPlayer = closestPlatformToPlayer;
                closestPlatformToPlayerDist = platformDistToPlayer;
                closestPlatformToPlayer = i;
            } else if (platformDistToPlayer < secondClosestPlatformToPlayerDist) {
                secondClosestPlatformToPlayerDist = platformDistToPlayer;
                secondClosestPlatformToPlayer = i;
            }
        } else {
            closestPlatformToPlayer = playerPlatform;
            break;
        }
    }

    if (secondClosestPlatformToPlayer != playerPlatform) {
        playerPlatform = closestPlatformToPlayer;
    } else {
        playerPlatform = secondClosestPlatformToPlayer;
    }
    return playerPlatform;
}


}
