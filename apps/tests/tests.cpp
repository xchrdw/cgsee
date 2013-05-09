#include <QtTest/QtTest>
#include <QString>

class DataCoreTest: public QObject
{
    Q_OBJECT
public:

private slots:
    void firstTest();
};

void DataCoreTest::firstTest()
{
    QString str ="Hello!";
    QVERIFY(str.toUpper() == "HELLO!");
};

QTEST_MAIN(DataCoreTest);
#include "tests.moc"