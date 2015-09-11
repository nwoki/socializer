#include "facebookusertest.h"

#include <FacebookUser>

#include <QtCore/QDebug>


FacebookUserTest::FacebookUserTest(QObject *parent)
    : m_facebookUser(new Socializer::FacebookUser(nullptr))
{
}

FacebookUserTest::~FacebookUserTest()
{
    delete m_facebookUser;
    m_facebookUser = nullptr;
}

void FacebookUserTest::cleanupTestCase()
{
    // handled in the destructor
}

void FacebookUserTest::initTestCase()
{
}


QTEST_MAIN(FacebookUserTest)

