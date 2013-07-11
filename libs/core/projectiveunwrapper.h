#include "abstractunwrapper.h"
#include "mesh.h"
#include <glm\core\type.hpp>

class StPatch;

class CGSEE_CORE_API ProjectiveUnwrapper : public AbstractUnwrapper
{
public:
    virtual void generateTexturePatchesFrom(Mesh const* mesh, QList<StPatch> &patches);

    void addTriangleToPatch(Mesh const* mesh, Mesh::s_Face * myFace, StPatch &currPatch, glm::mat4 transformMatrix, int patchNum);

protected:
    glm::mat4 matrixForPlane(glm::vec3 normal);
    glm::mat4 matrixForPlane(glm::vec3 normal, glm::vec3 transl);
};