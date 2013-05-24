
#include "geometrydata.h"
#include "aabb.h"

GeometryData::GeometryData( DataBlockRegistry & registry )
:   m_registry( registry )
{
    m_vertListHandle = "VERTICES";
    m_indicesHandle = "INDICES";
    t_VertexListP temp = DataBlock::createDataBlockWithName<VertexList>(m_vertListHandle, m_registry);
    DataBlock::createDataBlockWithName<VertexIndexList>(m_indicesHandle, m_registry, temp);

    QList<AttributeSpec> attrSpec;
    attrSpec.append(AttributeSpec("position", "glm::vec3"));
    attrSpec.append(AttributeSpec("normal", "glm::vec3"));
    attrSpec.append(AttributeSpec("texcoord", "glm::vec2"));
    temp->initialize(attrSpec);
}

GeometryData::~GeometryData()
{
}

t_vec3s GeometryData::vertices() const
{
    t_vec3s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "position", nullptr,
        [&temp](int i, const glm::vec3 & pos)
        {
            temp[i] = pos;
        });
    return temp;
}

//t_vec3s & PolygonalGeometry::vertices()
//{
//    invalidateBoundingBox();
//    return m_vertices;
//}

void GeometryData::setVertex(int i, const glm::vec3& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec3>(i, "position")) = data;
}

t_vec3s GeometryData::normals() const
{
    t_vec3s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec3>(0, myVList->size(), "normal", nullptr,
        [&temp](int i, const glm::vec3 & pos)
        {
            temp[i] = pos;
        });
    return temp;
}

//t_vec3s & PolygonalGeometry::normals()
//{
//    return m_normals;
//}

void GeometryData::setNormal(int i, const glm::vec3& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec3>(i, "normal")) = data;
}

t_vec2s GeometryData::texcs() const
{
    t_vec2s temp;
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    assert(myVList);
    temp.resize(myVList->size());
    myVList->foreachVertexAttribute<glm::vec2>(0, myVList->size(), "texcoord", nullptr,
        [&temp](int i, const glm::vec2 & pos)
        {
            temp[i] = pos;
        });
    return temp;
}
//t_vec2s & PolygonalGeometry::texcs()
//{
//    return m_texcs;
//}
void GeometryData::setTexC(int i, const glm::vec2& data)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    assert(myVList);
    *(myVList->getVertexAttribute<glm::vec2>(i, "texcoord")) = data;
}

t_uints GeometryData::indices() const
{
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesHandle));
    assert(inds);
    return inds->getIndices();
}

//t_uints & PolygonalGeometry::indices()
//{
//    return m_indices;
//}

void GeometryData::setIndex(int i, unsigned int data)
{
    VertexIndexList * inds = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesHandle));
    assert(inds);
    inds->setSingleIndex(i, data);
}

void GeometryData::retrieveNormals()
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesHandle));
    assert(myVList);
    assert(inds);
    if(! myVList->isAttributeUsed("position"))
        return;

    if(myVList->isAttributeUsed("normal"))
        qDebug("Normals will be replaced.");

    inds->foreachTriangle<glm::vec3>(0, inds->size(), "position", 
        [&](int i, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3)
        {
            glm::vec3 a = glm::normalize(v3-v1);
            glm::vec3 b = glm::normalize(v2-v1);
            glm::vec3 n = glm::cross(a, b);
            myVList->setVertexAttributes<glm::vec3>(i, i+3, "normal",
                [&](int i, glm::vec3& oldn)
                {
                    oldn = n;
                });
        });

    //assert(inds->size() % 3 == 0);

    //unsigned int i0, i1, i2;
    //glm::vec3 v[3], a, b, n;

    //for(int i = 0; i < inds->size(); i += 3)
    //{
    //    i0 = m_indices[i + 0];
    //    i1 = m_indices[i + 1];
    //    i2 = m_indices[i + 2];

    //    a = glm::normalize(m_vertices[i2] - m_vertices[i0]);
    //    b = glm::normalize(m_vertices[i1] - m_vertices[i0]);

    //    n = glm::cross(a, b);

    //    m_normals[i0] = n;
    //    m_normals[i1] = n;
    //    m_normals[i2] = n;
    //}
}

void GeometryData::resize(unsigned int size)
{
    t_VertexListP myVList = qobject_cast<t_VertexListP>(m_registry.getDataBlockByName(m_vertListHandle));
    t_VertexIndexListP inds = qobject_cast<VertexIndexList*>(m_registry.getDataBlockByName(m_indicesHandle));
    assert(myVList);
    assert(inds);

    myVList->resize(size);
    inds->resize(size);
}
