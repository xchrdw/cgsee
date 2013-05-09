#include <glm/glm.hpp>

#include <QtTest/QtTest>
#include <QString>
#include <QMetaType>

#include "core/datacore/vertexlist.h"

class DataCoreTest: public QObject
{
    Q_OBJECT
public:
    DataCoreTest(QObject* parent = nullptr):
        QObject(parent)
    ,   registry("")
    {

    }
protected:
    t_VertexListP vertexList;
    DataBlockRegistry registry;

    void initVertexList();

private slots:
    void initTestCase();
    void firstTest();
    void cleanupTestCase();
};

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

    initVertexList();
}

void DataCoreTest::firstTest()
{
    initTestCase();
    glm::vec3 * pos1 = vertexList->getVertexAttribute<glm::vec3>(1, "position");
    QVERIFY(pos1 != nullptr);
    (*pos1) = glm::vec3(1.0f, 2.0f, 3.0f);
};

void DataCoreTest::cleanupTestCase()
{
}

QTEST_MAIN(DataCoreTest);
#include "tests.moc"