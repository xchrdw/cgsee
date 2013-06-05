#include <QObject>
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
    void sillyTest();
    void writeReadTest();
    void iteratorTest();
    void constIteratorTest();
    void cleanupTestCase();

    void init();
    void cleanup();
};