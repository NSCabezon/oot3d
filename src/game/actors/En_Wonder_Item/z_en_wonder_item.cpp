extern "C" {
#include "z_en_wonder_item.hpp"

#define FLAGS 0x00000000

void EnWonderItem_Init(Actor* actor, PlayState* play);
void EnWonderItem_Destroy(Actor* actor, PlayState* play);
void EnWonderItem_Update(Actor* actor, PlayState* play);

GLOBAL_ASM("data/z_En_Wonder_Item.data.s")

ActorInit En_Wonder_Item_InitVars = {
    ACTOR_EN_WONDER_ITEM,
    ACTORCAT_PROP,
    FLAGS,
    OBJECT_GAMEPLAY_KEEP,
    sizeof(EnWonderItem),
    (ActorFunc)EnWonderItem_Init,
    (ActorFunc)EnWonderItem_Destroy,
    (ActorFunc)EnWonderItem_Update,
    NULL,
};

GLOBAL_ASM("asm/EnWonderItem_Init.s")

void EnWonderItem_Destroy(Actor* thisx, PlayState* play) {
    EnWonderItem* self = (EnWonderItem*)thisx;

    if ((self->collider.dim.radius != 0.0f) || (self->collider.dim.height != 0.0f)) {
        Collider_DestroyCylinder(play, &self->collider);
    }
}

GLOBAL_ASM("asm/EnWonderItem_Update.s")

#if 0 // draft: 14/32 words differ, Actor_SetFocus block is if-converted (ROM uses a bne)
void EnWonderItem_Update(Actor* thisx, PlayState* play) {
    EnWonderItem* self = (EnWonderItem*)thisx;

    if (self->timer != 0) {
        self->timer--;
    }
    self->updateFunc(self, play);

    if (self->wonderMode == 4) {
        Actor_SetFocus(&self->actor, self->unkHeight);
    }
    if ((self->wonderMode == 3) || (self->wonderMode == 8)) {
        Collider_UpdateCylinder(&self->actor, &self->collider);
        CollisionCheck_SetAC(play, &play->colChkCtx, &self->collider.base);
    }
}
#endif
}
