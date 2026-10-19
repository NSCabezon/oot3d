extern "C" {
#include "z_en_skj.hpp"

typedef struct EnSkjPosBlock {
    char pad[0x40];
    f32 x;
    f32 y;
    f32 z;
} EnSkjPosBlock;

void FUN_00372a60(u8* arg) {
    EnSkjPosBlock* p = (EnSkjPosBlock*)arg;
    f32 tmp = p->y;

    p->y = p->z;
    p->z = tmp;
    p->x = -p->x;
}

}
