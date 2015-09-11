#include "facebooktest.h"

void FacebookTest::toUpper()
{
    QString str = "Hello";
    QVERIFY(str.toUpper() == "HELLO");
}

QTEST_MAIN(FacebookTest)

