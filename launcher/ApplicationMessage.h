#pragma once

#include <QByteArray>
#include <QHash>
#include <QString>

struct ApplicationMessage {
    QString command;
    QHash<QString, QString> args;

    QByteArray serialize();
    [[nodiscard]] bool parse(const QByteArray& input);
};
