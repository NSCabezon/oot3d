// Pulls in the project-wide absolute symbol aliases (src/game/shared_syms.s) exactly once.
// Actor dirs must NOT re-define any of these names (L6200E): add new shared aliases there instead.
#include "global.hpp"

GLOBAL_ASM("../src/game/shared_syms.s")
