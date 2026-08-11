#pragma once

#include <QString>

namespace MCEditTool {
    void setPath(QString& path);
    QString path();
    bool check(const QString& toolPath, QString& error);
    QString getProgramPath();
};
