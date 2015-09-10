/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * 
 *
 * Author Francesco Nwokeka <francesco.nwokeka@gmail.com>
 */

#include <LinkedIn>
#include <../src/linkedinuser.h>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc == 2) {
        // use auth CODE
        Socializer::LinkedIn *link = new Socializer::LinkedIn(argv[1], "1fx5y8590q6r", "1tTwdCxMuiycme2R", "http://www.2s2h.com");
        Q_UNUSED(link)
    } else {
        Socializer::LinkedIn *link = new Socializer::LinkedIn("AQUSKGN0k3y0X1uBmwH122uoday_fXDucQYu_DfXCkk_1jmN1QqaX6tlEruTAukPVGLBtXXJo23p-JwqBH_dc9vpI7SvsZ1JIvezm43qqoKjWb82aoQcdwWkpJzkz_miulGR7tN5XwPnfi5XPshf9EMsw4II3XfeD8v4OcoBpSRqrK-fT8w"
                                                            , false);
        link->userInfo()->setLastUpdatedTime("1377879703802");
        link->update();

        Q_UNUSED(link)
    }

    return app.exec();
}
