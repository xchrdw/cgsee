#include <QList>

#include "stpatch.h"

class Mesh;
struct CGSEE_CORE_API AbstractUnwrapper
{
    virtual void generateTexturePatchesFrom(Mesh const* mesh, QList<StPatch> &patches) = 0;
};