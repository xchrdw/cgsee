#include <deque>
#include <algorithm>

#include "mesh.h"

Mesh::Mesh():
    m_posAttrName("position")
{

}

Mesh::~Mesh()
{
    clear();
}

void Mesh::initFrom(t_VertexIndexListP indList, t_VertexListP vertList)
{
    typedef decltype(indList->getIndices()) t_IndicesContainer;
    typedef QMap<QPair<unsigned int, unsigned int>, s_HalfEdge*> t_EdgeMap;

    t_EdgeMap edgeMap;

    clear();

    const t_IndicesContainer inds = indList->getIndices();
    m_vertexList = vertList ? vertList : indList->getVertexList();
    m_indsList = indList;

    m_vertexList->foreachVertexAttribute<glm::vec3>(0, m_vertexList->size(), m_posAttrName, nullptr,
        [&](int, const glm::vec3& vt){ m_vertices.push_back(new s_Vertex(vt)); });

    indList->foreachTriangle<glm::vec3>(0, indList->size(), m_posAttrName, 
        [&](int i, const glm::vec3 &p1, const glm::vec3 &p2, const glm::vec3 &p3)
        {
            s_Face *currFace = new s_Face;
            currFace->size = 3;
            currFace->m_data = nullptr;
            currFace->m_verts = new unsigned int[3];
            currFace->m_verts[0] = inds[i];
            currFace->m_verts[1] = inds[i+1];
            currFace->m_verts[2] = inds[i+2];
            currFace->m_normal = glm::normalize(glm::cross(p2 - p1, p3 - p2));

            s_HalfEdge *currEdges[3] = {new s_HalfEdge, new s_HalfEdge, new s_HalfEdge};
            // make a ring around the face
            currEdges[0]->m_next = currEdges[1]; currEdges[1]->m_prev = currEdges[0];
            currEdges[1]->m_next = currEdges[2]; currEdges[2]->m_prev = currEdges[1];
            currEdges[2]->m_next = currEdges[0]; currEdges[0]->m_prev = currEdges[2];
            for (int j = 0; j < 3; ++j)
            {
                currEdges[j]->m_vertIndex = inds[i+j];
                currEdges[j]->m_vertex = m_vertices[currEdges[j]->m_vertIndex];
                currEdges[j]->m_face = currFace;
            }
            for (int j = 0; j < 3; ++j) 
            {
                // Try to connect to other half-edges
                unsigned int v1, v2;
                v1 = currEdges[j]->m_vertIndex;
                v2 = currEdges[j]->m_prev->m_vertIndex;
                QPair<unsigned int, unsigned int> verts = qMakePair(qMin(v1, v2), qMax(v1, v2));
                if (edgeMap.contains(verts))
                {
                    currEdges[j]->m_pair = edgeMap[verts];
                    if (edgeMap[verts]->m_pair != nullptr)
                    {
                        qWarning("%s (%d, %d)", "edge with >2 owners detected, handling not implemented", v2, v1);
                    }
                    edgeMap[verts]->m_pair = currEdges[j];
                }
                else
                {
                    currEdges[j]->m_pair = nullptr;
                    edgeMap.insert(verts, currEdges[j]);
                }
            }
            currFace->m_halfEdge = currEdges[0];

            m_faces.push_back(currFace);
            for (int j = 0; j < 3; ++j)
                m_connectData.push_back(currEdges[j]);
        });
    
    // All edges should have their pairs
    t_EdgeMap::const_iterator it = std::find_if(edgeMap.cbegin(), edgeMap.cend(), 
        [](s_HalfEdge const *const edge)->bool
        {
            return edge->m_pair == nullptr;
        } );

    if (it != edgeMap.cend())
    {
        qWarning("%s", "open geometry or failure of vertex reuse");
        qWarning("%d %s", countPatches(), " patches in the mesh detected");
    }
}

void Mesh::initFromAndTrack(t_VertexIndexListP indList, t_VertexListP vertList)
{
    initFrom(indList, vertList);

    connect (indList, SIGNAL(updated(QObject*)), SLOT(sourceUpdated()));
    if (m_vertexList)
        connect (m_vertexList, SIGNAL(updated(QObject*)), SLOT(sourceUpdated()));
}

void Mesh::commitChanges()
{
    // TODO: build vertex list and index list from half-edge data
}

void Mesh::clear()
{
    for (s_Face* pface : m_faces)
        delete pface;
    for (s_HalfEdge* pedge : m_connectData)
        delete pedge;
    for (s_Vertex* pver : m_vertices)
        delete pver;

    m_faces.clear();
    m_connectData.clear();
    m_vertices.clear();
}

void Mesh::clearAdditionalData()const
{
    for (auto facePtr: m_faces)
    {
        facePtr->m_data = nullptr;
    }
}

unsigned int Mesh::countPatches()
{
    unsigned int count = 0;

    for (auto facePtr: m_faces)
    {
        std::deque<s_Face *> adjFaces;
        s_Face * myFace = facePtr; 
        
        if (myFace->m_data != nullptr) continue;
        ++count;
        do {
            if (myFace->m_data == nullptr)
            {
                myFace->m_data = &count;
                QList<s_Face *> nextFaces;
                myFace->getAdjacentFaces(nextFaces);
                std::copy_if(nextFaces.begin(), nextFaces.end(), std::back_inserter(adjFaces), 
                    [](s_Face const* f)->bool{ return f->m_data == nullptr; });
            }
            if (adjFaces.empty())
                break;
            myFace = adjFaces.front();
            adjFaces.pop_front();
        } while (true);
    }
    clearAdditionalData();
    return count;
}

void Mesh::sourceUpdated()
{
    initFrom(m_indsList, m_vertexList);
}

QVector<Mesh::s_Face*> Mesh::getFaces()const
{
    return m_faces;
}

Mesh::s_Vertex *Mesh::getVertex(int i)const
{
    return m_vertices[i];
}

void Mesh::s_Face::getAdjacentFaces(QList<s_Face*> & out)
{
    out.clear();
    s_HalfEdge * start = m_halfEdge;
    do {
        s_HalfEdge * myPair = start->m_pair;
        if (myPair) out.append(myPair->m_face);
        start = start->m_next;
    } while (start != m_halfEdge);
}

void Mesh::s_Face::getAdjacentFaces(QList<s_Face*> & out, QList<s_Vertex*> &newVerts)
{
    out.clear();
    s_HalfEdge * start = m_halfEdge;
    do {
        s_HalfEdge * myPair = start->m_pair;
        if (myPair)
        {
            out.append(myPair->m_face);
            myPair = myPair->m_next;
            if (std::find(m_verts, m_verts+3, myPair->m_vertIndex) == m_verts+3)
            {
                newVerts.append(myPair->m_vertex);
            }
            else
            {
                myPair = myPair->m_next;
                assert (std::find(m_verts, m_verts+3, myPair->m_vertIndex) == m_verts+3);
                newVerts.append(myPair->m_vertex);
            }
        }
        start = start->m_next;
    } while (start != m_halfEdge);
}