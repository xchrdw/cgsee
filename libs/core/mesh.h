#pragma once

#include "datacore/vertexlist.h"
#include "datacore/vertexindexlist.h"

class CGSEE_CORE_API Mesh : public QObject
{
    Q_OBJECT
public:

    struct s_HalfEdge;
    struct s_Vertex;

    struct s_Face
    {
        unsigned int *m_verts;
        unsigned int size;
        s_HalfEdge* m_halfEdge;
        glm::vec3 m_normal;

        void * m_data;

        void getAdjacentFaces(QList<s_Face*> &out);
        void getAdjacentFaces(QList<s_Face*> &out, QList<s_Vertex*> &newVertices);
        ~s_Face()
        {
            if (m_verts) delete[] m_verts;
        }
    };

    struct s_HalfEdge
    {
        unsigned int m_vertIndex;
        s_HalfEdge* m_next, *m_prev;
        s_HalfEdge* m_pair;
        s_Face* m_face;
        s_Vertex* m_vertex;

        void * m_data;
    };

    struct s_Vertex
    {
        glm::vec3 m_pos;

        void * m_data;

        s_Vertex(const glm::vec3 &p): m_pos(p), m_data(nullptr){}
    };

    Mesh();
    ~Mesh();

    void initFrom(t_VertexIndexListP indList, t_VertexListP vertList = nullptr);
    void initFromAndTrack(t_VertexIndexListP indList, t_VertexListP vertList = nullptr);
    void commitChanges();
    void clear();

    void clearAdditionalData()const;
    unsigned int countPatches();

    QVector<s_Face*> getFaces()const;
    s_Vertex* getVertex(int i)const;

protected slots:
    void sourceUpdated();

protected:
    t_VertexListP m_vertexList;
    t_VertexIndexListP m_indsList;
    QString m_posAttrName;

    QVector<s_HalfEdge*> m_connectData;
    QVector<s_Face*> m_faces;
    QVector<s_Vertex *> m_vertices;
};