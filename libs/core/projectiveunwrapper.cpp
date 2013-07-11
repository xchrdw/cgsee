#include "projectiveunwrapper.h"
#include "mesh.h"
#include "glm\gtc\matrix_transform.hpp"
#include <cassert>
#include <random>
#include <deque>

struct ProjPointInfo
{
    QList<glm::vec2> sts;
    QList<int> ids;

    QList<int> patchNumbers;
};

struct ProjFaceInfo
{

};

struct ProjEdgeInfo
{

};

void ProjectiveUnwrapper::addTriangleToPatch(Mesh const* mesh, Mesh::s_Face * myFace, StPatch &currPatch, glm::mat4 transformMatrix, int patchNum)
{
    Mesh::s_Vertex *v1, *v2, *v3;
    v1 = mesh->getVertex(myFace->m_verts[0]);
    v2 = mesh->getVertex(myFace->m_verts[1]);
    v3 = mesh->getVertex(myFace->m_verts[2]);
    glm::ivec3 result = currPatch.addTriangle((transformMatrix * glm::vec4(v1->m_pos, 1)).xy,
        (transformMatrix * glm::vec4(v2->m_pos, 1)).xy,
        (transformMatrix * glm::vec4(v3->m_pos, 1)).xy);
    Mesh::s_Vertex *vn[] = {v1, v2, v3};
    for (int i = 0; i < 3; ++i)
    {
        Mesh::s_Vertex * myv = vn[i];
        if (myv->m_data == nullptr)
            myv->m_data = new ProjPointInfo;

        ProjPointInfo* info = reinterpret_cast<ProjPointInfo*>(myv->m_data);
        if (! info->patchNumbers.contains(patchNum))
        {
            info->ids.append(result[i]);
            info->sts.append(currPatch.getPoint(result[i]));
            info->patchNumbers.append(patchNum);
        }
    }
}

void ProjectiveUnwrapper::generateTexturePatchesFrom(Mesh const* mesh, QList<StPatch> &patches)
{
    mesh->clearAdditionalData();
    QVector<Mesh::s_Face*> facetsVector(mesh->getFaces());
    std::shuffle(facetsVector.begin(), facetsVector.end(), std::default_random_engine());

    if (facetsVector.empty())
    {
        qWarning("%s", "Unwrapping an empty mesh?");
        return;
    }

    // start with first facet
    int counter = 0;
    for (auto facePtr: facetsVector)
    {
        if (facePtr->m_data != nullptr) continue;

        std::deque<Mesh::s_Face *> adjFaces;
        std::deque<Mesh::s_Face *> greedyFaces;
        std::deque<Mesh::s_Vertex *> greedyVertex;
        Mesh::s_Face * myFace = facePtr; 
        StPatch currPatch;
        glm::vec3 centralNormal = facePtr->m_normal;
        glm::mat4 transformMatrix = matrixForPlane(centralNormal);

        do {
            addTriangleToPatch(mesh, myFace, currPatch, transformMatrix, counter);
            if (myFace->m_data == nullptr)
            {
                myFace->m_data = &patches;
                QList<Mesh::s_Face *> nextFaces;
                QList<Mesh::s_Vertex *> newVertices;
                myFace->getAdjacentFaces(nextFaces, newVertices);
                auto newVIter = newVertices.begin();
                for (Mesh::s_Face * face_iter: nextFaces)
                {
                    assert (newVIter != newVertices.end());
                    if (face_iter->m_data)
                    {
                        ++newVIter;
                        continue;
                    }
                    if (glm::dot(centralNormal, face_iter->m_normal) > 0.4)
                        adjFaces.push_back(face_iter);
                    else
                    {
                        greedyFaces.push_back(face_iter);
                        greedyVertex.push_back(*newVIter);
                    }
                    ++newVIter;
                }
            }
            if (adjFaces.empty())
                break;
            myFace = adjFaces.front();
            adjFaces.pop_front();
        } while (true);

        //"Greedy" is to try to add additional facets on the edge of the island
        Mesh::s_Vertex *myVertex = nullptr;
        for(;!greedyFaces.empty();myFace = greedyFaces.front(), myVertex = greedyVertex.front(), greedyFaces.pop_front(), greedyVertex.pop_front())
        {
            //...
        }
        ++counter;

        patches.push_back(currPatch);
    }

    qWarning("%d %s", patches.size(), "islands formed");
}

glm::mat4 ProjectiveUnwrapper::matrixForPlane(glm::vec3 normal)
{
    glm::mat4 m(1.0, 0.0, 0.0, 0.0, 
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 1.0);
    if (glm::dot(normal, glm::vec3(0, 0, 1)) > 0.999)
    {
        // no need for rotation
    }
    else if (glm::dot(normal, glm::vec3(0, 0, -1)) > 0.999)
    {
        m= glm::rotate(m, glm::degrees(std::acos(normal.z)), glm::vec3(0, 1, 0));
    }
    else
        m = glm::rotate(m, glm::degrees(std::acos(normal.z)), glm::cross(normal, glm::vec3(0, 0, 1)));

    assert(glm::length(m * glm::vec4(normal, 0)) < 0.001);
    return m;
}

glm::mat4 ProjectiveUnwrapper::matrixForPlane(glm::vec3 normal, glm::vec3 transl)
{
    glm::mat4 m(1.0, 0.0, 0.0, 0.0, 
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 0.0,
        0.0, 0.0, 0.0, 1.0);
    m = glm::translate(m, transl);
    if (glm::dot(normal, glm::vec3(0, 0, 1)) > 0.999)
    {
        // no need for rotation
    }
    else if (glm::dot(normal, glm::vec3(0, 0, -1)) > 0.999)
    {
        m= glm::rotate(m, glm::degrees(std::acos(normal.z)), glm::vec3(0, 1, 0));
    }
    else
        m = glm::rotate(m, glm::degrees(std::acos(normal.z)), glm::cross(normal, glm::vec3(0, 0, 1)));

    m = glm::translate(m, -transl);

    assert(glm::length(m * glm::vec4(normal, 0)) < 0.001);
    return m;
}