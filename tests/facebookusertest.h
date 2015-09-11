#include <QtTest/QTest>


namespace Socializer {
    class FacebookUser;
}

class FacebookUserTest : public QObject
{
    Q_OBJECT

public:
    FacebookUserTest(QObject *parent = 0);
    ~FacebookUserTest();

private slots:
    void initTestCase();

    void cleanupTestCase();

private:
    Socializer::FacebookUser *m_facebookUser;
};
