#pragma once

#include "datacore/vertexlist.h"
#include "datacore/vertexindexlist.h"

class Mesh : public QObject
{
    Q_OBJECT
public:

    struct s_HalfEdge;

    struct s_Face
    {
        unsigned int *m_verts;
        unsigned int size;
        s_HalfEdge* m_halfEdge;
    };

    struct s_HalfEdge
    {
        unsigned int m_vertIndex;
        s_HalfEdge* m_next, *m_prev;
        s_HalfEdge* m_pair;
        s_Face* m_face;
    };

    Mesh();
    ~Mesh();

    void initFrom(t_VertexIndexListP indList, t_VertexListP vertList = nullptr);
    void initFromAndTrack(t_VertexIndexListP indList, t_VertexListP vertList = nullptr);
    void commitChanges();
    void clear();

protected slots:
    void sourceUpdated();

protected:
    t_VertexListP m_vertexList;
    t_VertexIndexListP m_indsList;
    QString m_posAttrName;

    QVector<s_HalfEdge*> m_connectData;
    QVector<s_Face*> m_faces;
};