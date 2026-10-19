extern "C" {
#include "z_demo_gt.hpp"

void DemoGt_CollisionHeaderGetVirtual(CollisionHeader* collision, CollisionHeader** out) {
    *out = collision;
}
}
