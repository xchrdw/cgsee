#include <glm/glm.hpp>

#include <QtTest/QtTest>
#include <QString>
#include <QMetaType>

#include "tests.h"

void DataCoreTest::initVertexList()
{
    qRegisterMetaType<glm::vec3> ("vec3");
    QList<AttributeSpec> attributeSpec;
    attributeSpec.append(AttributeSpec("position", "vec3"));
    attributeSpec.append(AttributeSpec("normal", "vec3"));
    vertexList->initialize(attributeSpec);
}

void DataCoreTest::initTestCase()
{
    vertexList = DataBlock::createDataBlockWithName<VertexList> ("vertex", registry);
    QVERIFY(vertexList);
    QVERIFY(vertexList->objectName() == "vertex");
    QVERIFY(registry.getDataBlockByName("vertex") == vertexList);
    qDebug(tr("%1").arg((int)this).toLocal8Bit());

    initVertexList();
}

void DataCoreTest::firstTest()
{
    glm::vec3 * pos1 = vertexList->getVertexAttribute<glm::vec3>(1, "vec3", "position");
    QVERIFY(pos1 != nullptr);
    (*pos1) = glm::vec3(1.0f, 2.0f, 3.0f); // you can now safely access the vertex attribute, but thats not easy
};

void DataCoreTest::cleanupTestCase()
{
}

QTEST_MAIN(DataCoreTest);