/*
 * main.cpp
 *
 * This file is part of the Socializer library
 * Copyright (C) 2012 Ispirata <info@ispirata.com>
 *
 * Author Francesco Nwokeka <francesco@ispirata.com>
 */

#include <Facebook>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    if (argc == 2) {
        Socializer::Facebook *fb = new Socializer::Facebook(argv[1]);
        Q_UNUSED(fb)
    } else {
        Socializer::Facebook *fb = new Socializer::Facebook("CAACEdEose0cBAIW5kPJCF2r2hWFTOKM91vGqCFzf2IMKg7nCC4zx6OPVQRZCoKqq8IUqkwsx1qlaOGm3TrCaZB1FxoAKCWZAdMgNgniuQslLKdxtZA9pmlSk61gv3tGcZBh2NJdZCAvw8Weh4FZA3yqZC4s6eRQAXAkZD");
        Q_UNUSED(fb)
    }

    return app.exec();
}
