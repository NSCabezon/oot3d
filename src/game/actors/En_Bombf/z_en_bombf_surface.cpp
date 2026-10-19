// FUN_00357680 (Actor_PlaySfx_SurfaceBomb, 3DS). Own TU so that EnBombf_Move (z_en_bombf.cpp) keeps calling it
// as an opaque function.
extern "C" {
#include "z_en_bombf.hpp"

s32 FUN_00341df0(CollisionContext* colCtx, CollisionPoly* poly, s32 bgId); // SurfaceType_GetSfxOffset

static inline void EnBombf_PlaySfx(Vec3f* pos, u32 sfxId) {
    Audio_PlaySoundGeneral(sfxId, pos, 4, &FLOAT_0054ac20, &FLOAT_0054ac20, &DAT_0054ac24.unk_0);
}

void FUN_00357680(PlayState* play, Actor* actor) {
    s32 surfaceSfxOffset;

    if (actor->bgCheckFlags & 0x20) {
        if (actor->yDistToWater < 20.0f) {
            surfaceSfxOffset = 4;
        } else {
            surfaceSfxOffset = 5;
        }
    } else {
        surfaceSfxOffset = FUN_00341df0(&play->colCtx, actor->floorPoly, actor->floorBgId);
    }

    if ((s16)actor->unk_19C <= 0) {
        EnBombf_PlaySfx(&actor->world.pos, 0x01000186);
        EnBombf_PlaySfx(&actor->world.pos, 0x01000001 + surfaceSfxOffset);
        actor->unk_19C = 20;
    }
}
}
