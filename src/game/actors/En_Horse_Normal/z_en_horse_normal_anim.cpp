extern "C" {
#include "z_en_horse_normal.hpp"

extern f32 EnHorseNormal_sPlaybackSpeeds[9];

f32 FUN_00350c68(EnHorseNormal* self) {
 f32 result;
 if(self->animationIdx==4) result=EnHorseNormal_sPlaybackSpeeds[self->animationIdx]*self->actor.speedXZ*(1.0f/2.0f);
 else if(self->animationIdx==5) result=EnHorseNormal_sPlaybackSpeeds[self->animationIdx]*self->actor.speedXZ*(1.0f/3.0f);
 else if(self->animationIdx==6) result=EnHorseNormal_sPlaybackSpeeds[self->animationIdx]*self->actor.speedXZ*(1.0f/5.0f);
 else result=EnHorseNormal_sPlaybackSpeeds[self->animationIdx];
 return result;
}

}
