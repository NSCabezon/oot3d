// Runtime pieces for function-local statics (see include/model_manager.hpp).
// __aeabi_atexit (0x00303B10, pinned in oot3d.ld) and __cxa_guard_release are empty C functions in the ROM:
// armlink --inline turns every call into `mov r0, r0`. They MUST be C-compiled (an asm `bx lr` stub has no
// size info and is not inlined). __cxa_guard_release's own ROM copy is unknown; ours is unplaced and
// dropped by --remove once all calls are inlined.
#include "global.hpp"
#include "libcpp/lib_functions.hpp"

extern "C" void __aeabi_atexit(void* object, void (*destroyer)(void*), void* dso_handle) {
}

extern "C" void __cxa_guard_release(unsigned int* guard_object) {
}

// ctor/dtor/__dso_handle aliases: src/game/shared_syms.s
