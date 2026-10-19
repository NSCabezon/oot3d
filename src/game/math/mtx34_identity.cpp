extern "C" {
#include "mtx34_identity.hpp"

// Out-of-line copy of the shared identity accessor (include/mtx34_identity.hpp); ~70 other ROM functions
// inline the same guard + init.
MTX34* MTX34Identity(MTX34* pOut) {
    MTX34CopyAsm(pOut, (MTX34*)&MTX34_Identity());
    return pOut;
}
}
