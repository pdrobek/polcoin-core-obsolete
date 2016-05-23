// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2016 The Polcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef POLCOIN_QT_TEST_URITESTS_H
#define POLCOIN_QT_TEST_URITESTS_H

#include <QObject>
#include <QTest>

class URITests : public QObject
{
    Q_OBJECT

private slots:
    void uriTests();
};

#endif // POLCOIN_QT_TEST_URITESTS_H
