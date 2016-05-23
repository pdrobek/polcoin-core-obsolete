// Copyright (c) 2011-2014 The Polcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef POLCOIN_QT_POLCOINADDRESSVALIDATOR_H
#define POLCOIN_QT_POLCOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class PolcoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit PolcoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Polcoin address widget validator, checks for a valid polcoin address.
 */
class PolcoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit PolcoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // POLCOIN_QT_POLCOINADDRESSVALIDATOR_H
