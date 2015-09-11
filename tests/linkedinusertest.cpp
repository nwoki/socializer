#include "linkedinusertest.h"

#include <LinkedInUser>

#include <QtCore/QDebug>


LinkedInUserTest::LinkedInUserTest(QObject *parent)
    : m_linkedInUser(new Socializer::LinkedInUser(nullptr))
{
}

LinkedInUserTest::~LinkedInUserTest()
{
    delete m_linkedInUser;
    m_linkedInUser = nullptr;
}

void LinkedInUserTest::cleanupTestCase()
{
    // handled in the destructor
}

void LinkedInUserTest::initTestCase()
{
}


QTEST_MAIN(LinkedInUserTest)

