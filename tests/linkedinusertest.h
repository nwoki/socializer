#include <QtTest/QTest>


namespace Socializer {
    class LinkedInUser;
}

class LinkedInUserTest : public QObject
{
    Q_OBJECT

public:
    LinkedInUserTest(QObject *parent = 0);
    ~LinkedInUserTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

private:
    Socializer::LinkedInUser *m_linkedInUser;
};
