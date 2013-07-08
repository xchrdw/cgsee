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

    indList->foreachTriangle<glm::vec3>(0, indList->size(), m_posAttrName, 
        [&](int i, const glm::vec3 &, const glm::vec3 &, const glm::vec3 &)
        {
            s_Face *currFace = new s_Face;
            currFace->size = 3;
            currFace->m_verts = new unsigned int[3];
            currFace->m_verts[0] = inds[i];
            currFace->m_verts[1] = inds[i+1];
            currFace->m_verts[2] = inds[i+2];

            s_HalfEdge *currEdges[3] = {new s_HalfEdge, new s_HalfEdge, new s_HalfEdge};
            // make a ring around the face
            currEdges[0]->m_next = currEdges[1]; currEdges[1]->m_prev = currEdges[0];
            currEdges[1]->m_next = currEdges[2]; currEdges[2]->m_prev = currEdges[1];
            currEdges[2]->m_next = currEdges[0]; currEdges[0]->m_prev = currEdges[2];
            for (int j = 0; j < 3; ++j) 
            {
                currEdges[j]->m_vertIndex = inds[i+j];
                currEdges[j]->m_face = currFace;
                // Try to connect to other half-edges
                unsigned int v1, v2;
                v1 = currEdges[j]->m_vertIndex;
                v2 = currEdges[j]->m_prev->m_vertIndex;
                QPair<unsigned int, unsigned int> verts = qMakePair(qMin(v1, v2), qMax(v1, v2));
                if (edgeMap.contains(verts))
                {
                    currEdges[j]->m_pair = edgeMap[verts];
                    assert(edgeMap[verts]->m_pair == nullptr);
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

    assert(it == edgeMap.cend());
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

    m_faces.clear();
    m_connectData.clear();
}

void Mesh::sourceUpdated()
{
    initFrom(m_indsList, m_vertexList);
}

void Mesh::s_Face::getAdjacentFaces(QList<s_Face*> & out)
{
    out.clear();
    s_HalfEdge * start = m_halfEdge;
    do {
        out.append(start->m_pair->m_face);
        start = start->m_next;
    } while (start != m_halfEdge);
}