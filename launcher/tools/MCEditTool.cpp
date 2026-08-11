#include "MCEditTool.h"

#include <QDir>
#include <QProcess>
#include <QUrl>

#include "Application.h"
#include "BaseInstance.h"
#include "config/GlobalConfig.h"

void MCEditTool::setPath(QString& path)
{
    APPLICATION->updateConfig().mcEditPath = path;
}

QString MCEditTool::path()
{
    return APPLICATION->config().mcEditPath;
}

bool MCEditTool::check(const QString& toolPath, QString& error)
{
    if (toolPath.isEmpty()) {
        error = QObject::tr("Path is empty");
        return false;
    }
    const QDir dir(toolPath);
    if (!dir.exists()) {
        error = QObject::tr("Path does not exist");
        return false;
    }
    if (!dir.exists("mcedit.sh") && !dir.exists("mcedit.py") && !dir.exists("mcedit.exe") && !dir.exists("Contents") &&
        !dir.exists("mcedit2.exe")) {
        error = QObject::tr("Path does not seem to be a MCEdit path");
        return false;
    }
    return true;
}

QString MCEditTool::getProgramPath()
{
#ifdef Q_OS_MACOS
    return path();
#else
    const QString mceditPath = path();
    QDir mceditDir(mceditPath);
#if defined(Q_OS_LINUX) || defined(Q_OS_FREEBSD) || defined(Q_OS_OPENBSD)
    if (mceditDir.exists("mcedit.sh")) {
        return mceditDir.absoluteFilePath("mcedit.sh");
    } else if (mceditDir.exists("mcedit.py")) {
        return mceditDir.absoluteFilePath("mcedit.py");
    }
    return QString();
#elif defined(Q_OS_WIN32)
    if (mceditDir.exists("mcedit.exe")) {
        return mceditDir.absoluteFilePath("mcedit.exe");
    } else if (mceditDir.exists("mcedit2.exe")) {
        return mceditDir.absoluteFilePath("mcedit2.exe");
    }
    return QString();
#endif
#endif
}
