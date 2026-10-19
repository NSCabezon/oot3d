#include "non_matchings.hpp"
extern "C" {
#include "global.hpp"
}


GLOBAL_ASM("asm/FUN_0018041c.s")


















GLOBAL_ASM("asm/FUN_00180d0c.s")






GLOBAL_ASM("asm/FUN_001810fc.s")








GLOBAL_ASM("asm/FUN_00181770.s")



GLOBAL_ASM("asm/FUN_00181b60.s")


GLOBAL_ASM("asm/FUN_00181c84.s")


GLOBAL_ASM("asm/FUN_00181e38.s")

GLOBAL_ASM("asm/FUN_00181f38.s")

GLOBAL_ASM("asm/FUN_00181fd0.s")


GLOBAL_ASM("../split/asm/FUN_001821fc.s")





GLOBAL_ASM("asm/FUN_00182500.s")

GLOBAL_ASM("asm/FUN_00182938.s")

GLOBAL_ASM("asm/FUN_00182b3c.s")



GLOBAL_ASM("asm/FUN_00182c70.s")






GLOBAL_ASM("asm/FUN_001832ac.s")

GLOBAL_ASM("asm/FUN_001832ec.s")

GLOBAL_ASM("asm/FUN_00183518.s")

GLOBAL_ASM("asm/FUN_00183634.s")

GLOBAL_ASM("asm/FUN_0018397c.s")

GLOBAL_ASM("asm/FUN_00183a38.s")



GLOBAL_ASM("asm/FUN_00183af4.s")

GLOBAL_ASM("asm/FUN_00183c00.s")

GLOBAL_ASM("asm/FUN_00183c74.s")

GLOBAL_ASM("asm/FUN_00183e30.s")

GLOBAL_ASM("asm/FUN_00183f88.s")



GLOBAL_ASM("asm/FUN_0018414c.s")

GLOBAL_ASM("asm/FUN_00184168.s")

GLOBAL_ASM("asm/FUN_00184184.s")

GLOBAL_ASM("asm/FUN_001843e0.s")

GLOBAL_ASM("asm/FUN_00184630.s")



GLOBAL_ASM("asm/FUN_00184a70.s")


GLOBAL_ASM("asm/FUN_00184ec8.s")

GLOBAL_ASM("asm/FUN_00184ff8.s")




GLOBAL_ASM("asm/FUN_00185468.s")





GLOBAL_ASM("asm/FUN_00185a58.s")

GLOBAL_ASM("asm/FUN_00185a5c.s")

GLOBAL_ASM("asm/FUN_00185a60.s")



GLOBAL_ASM("asm/FUN_00185fe4.s")

GLOBAL_ASM("asm/FUN_00186090.s")





extern "C" {
u16 FUN_00186680(PlayState* play) {
    u16 faceReaction = Text_GetFaceReaction(play, 0xE);

    if (faceReaction == 0) {
        if (gSaveContext.infTable[13] & 0x200) {
            return 0x5019;
        }
        return 0x5018;
    }
    return faceReaction;
}
}

GLOBAL_ASM("asm/FUN_001866b8.s")

GLOBAL_ASM("asm/FUN_00186794.s")

GLOBAL_ASM("asm/FUN_001867ec.s")

GLOBAL_ASM("asm/FUN_00186974.s")

GLOBAL_ASM("asm/FUN_0018741c.s")


GLOBAL_ASM("asm/FUN_001883ac.s")

GLOBAL_ASM("asm/FUN_00188484.s")

GLOBAL_ASM("asm/FUN_00188650.s")

GLOBAL_ASM("asm/FUN_00188704.s")


GLOBAL_ASM("../split/asm/FUN_00188ad4.s")
GLOBAL_ASM("../split/asm/FUN_00188cc8.s")



extern "C" {
void FUN_003e1370(void);

s32 FUN_00189628(Actor* actor) {
    return *(void**)((u8*)actor + 0x1BC) == (void*)FUN_003e1370;
}
}




GLOBAL_ASM("asm/FUN_00189844.s")

GLOBAL_ASM("asm/FUN_0018f08c.s")



GLOBAL_ASM("../split/asm/FUN_00190730.s")

GLOBAL_ASM("asm/FUN_00190740.s")

GLOBAL_ASM("asm/FUN_00190888.s")

GLOBAL_ASM("../split/asm/FUN_00190990.s")


GLOBAL_ASM("asm/FUN_00190a20.s")

GLOBAL_ASM("asm/FUN_00190b48.s")

GLOBAL_ASM("asm/FUN_00190c2c.s")














GLOBAL_ASM("asm/IChain_Apply_f32.s")

GLOBAL_ASM("asm/IChain_Apply_u32.s")

GLOBAL_ASM("asm/IChain_Apply_Vec3s.s")

GLOBAL_ASM("asm/IChain_Apply_s32.s")

GLOBAL_ASM("asm/IChain_Apply_Vec3f.s")





GLOBAL_ASM("asm/FUN_00193da0.s")

GLOBAL_ASM("asm/FUN_00193f0c.s")











GLOBAL_ASM("asm/FUN_001959f8.s")


GLOBAL_ASM("asm/FUN_00195acc.s")

GLOBAL_ASM("asm/FUN_00195b08.s")

GLOBAL_ASM("asm/FUN_00195b90.s")


GLOBAL_ASM("../split/asm/FUN_00195e2c.s")


GLOBAL_ASM("asm/FUN_001960e8.s")



GLOBAL_ASM("asm/FUN_00196404.s")

GLOBAL_ASM("asm/FUN_00196554.s")

GLOBAL_ASM("asm/FUN_001966b8.s")



GLOBAL_ASM("asm/FUN_00196978.s")

GLOBAL_ASM("asm/FUN_001969cc.s")


GLOBAL_ASM("asm/FUN_00196afc.s")





GLOBAL_ASM("asm/FUN_00196f54.s")













GLOBAL_ASM("asm/FUN_0019846c.s")

GLOBAL_ASM("asm/FUN_00198c20.s")


GLOBAL_ASM("asm/FUN_001990c8.s")

GLOBAL_ASM("asm/FUN_00199308.s")


GLOBAL_ASM("../split/asm/FUN_00199688.s")
GLOBAL_ASM("../split/asm/FUN_00199700.s")



GLOBAL_ASM("asm/FUN_0019993c.s")


GLOBAL_ASM("asm/FUN_00199b8c.s")













GLOBAL_ASM("asm/FUN_0019ae14.s")

GLOBAL_ASM("asm/FUN_0019af78.s")

GLOBAL_ASM("asm/FUN_0019b0e4.s")


GLOBAL_ASM("asm/FUN_0019b63c.s")


GLOBAL_ASM("asm/FUN_0019b858.s")









GLOBAL_ASM("asm/FUN_0019c118.s")


GLOBAL_ASM("asm/FUN_0019c4ac.s")


GLOBAL_ASM("asm/FUN_0019c680.s")


GLOBAL_ASM("asm/FUN_0019c82c.s")




GLOBAL_ASM("asm/FUN_0019ce2c.s")

GLOBAL_ASM("asm/FUN_0019d028.s")

GLOBAL_ASM("asm/FUN_0019d0d4.s")


GLOBAL_ASM("asm/FUN_0019e118.s")





GLOBAL_ASM("asm/FUN_0019f010.s")






GLOBAL_ASM("asm/FUN_0019f66c.s")












GLOBAL_ASM("asm/FUN_0019fe84.s")




GLOBAL_ASM("asm/FUN_001a0c6c.s")


GLOBAL_ASM("asm/FUN_001a0f14.s")

GLOBAL_ASM("asm/FUN_001a0f90.s")


GLOBAL_ASM("asm/FUN_001a10a8.s")

GLOBAL_ASM("asm/FUN_001a11e4.s")


GLOBAL_ASM("asm/FUN_001a13a8.s")



GLOBAL_ASM("asm/FUN_001a167c.s")

GLOBAL_ASM("asm/FUN_001a172c.s")

GLOBAL_ASM("asm/FUN_001a1800.s")

GLOBAL_ASM("asm/FUN_001a1a48.s")

GLOBAL_ASM("asm/FUN_001a1c20.s")

GLOBAL_ASM("asm/FUN_001a23d8.s")


GLOBAL_ASM("asm/FUN_001a2418.s")

GLOBAL_ASM("../split/asm/FUN_001a250c.s")




GLOBAL_ASM("asm/FUN_001a2a70.s")

GLOBAL_ASM("asm/FUN_001a2d28.s")

GLOBAL_ASM("asm/FUN_001a2eb8.s")

GLOBAL_ASM("asm/FUN_001a2f98.s")

GLOBAL_ASM("asm/FUN_001a2f9c.s")



GLOBAL_ASM("asm/FUN_001a3210.s")

GLOBAL_ASM("asm/FUN_001a322c.s")



GLOBAL_ASM("asm/FUN_001a35cc.s")



GLOBAL_ASM("asm/FUN_001a36b0.s")


GLOBAL_ASM("asm/FUN_001a3870.s")

GLOBAL_ASM("asm/FUN_001a3c70.s")

GLOBAL_ASM("asm/FUN_001a3f24.s")

GLOBAL_ASM("asm/FUN_001a4324.s")




GLOBAL_ASM("asm/caseD_0_001a6260.s")

GLOBAL_ASM("asm/Collider_SetJntSphAllocType1.s")



GLOBAL_ASM("asm/FUN_001ac538.s")




GLOBAL_ASM("asm/FUN_001b09ac.s")

GLOBAL_ASM("asm/FUN_001b0ea0.s")

GLOBAL_ASM("asm/FUN_001b2dfc.s")




GLOBAL_ASM("asm/caseD_0_001bce9c.s")

GLOBAL_ASM("asm/caseD_0_001bcf6c.s")

GLOBAL_ASM("asm/FUN_001bf194.s")

GLOBAL_ASM("asm/FUN_001bfb74.s")
