// Obj_Kibako: actor-list slot count. Separate TU on purpose: the ROM code calls FUN_003a7aac from
// FUN_003c06bc as an opaque extern (bl), armcc would inline it in the same TU.
extern "C" {
#include "z_obj_kibako.hpp"

void ObjKibako_Idle(ObjKibako* self, PlayState* play);

// Separate TU: ROM Held/Thrown call SetupIdle with bl while Init inlines its own copy.
void ObjKibako_SetupIdle(ObjKibako* self) {
    self->actionFunc = ObjKibako_Idle;
    self->actor.colChkInfo.mass = MASS_HEAVY;
}

s32 FUN_003a7aac(u8* base, s32 id, s32 slot) {
    s32 count;
    u8* node;

    base += slot * 8;
    node = *(u8**)(base + 0x10);
    count = 0;
    while (node != NULL) {
        if (*(s16*)node == id && *(s32*)(node + 0x134) == 0) {
            count++;
        }
        node = *(u8**)(node + 0x130);
    }
    return count;
}

}
