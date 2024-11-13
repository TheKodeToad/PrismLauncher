#include "NBTStudioTool.h"

#include "FastFileIconProvider.h"

#ifdef Q_OS_WIN32
#include "windows.h"
#endif

NBTStudioTool::NBTStudioTool(const SettingsObjectPtr& settings)
{
    settings->registerSetting("NBTStudioPath");
    m_settings = settings;
}

void NBTStudioTool::setPath(const QString& path)
{
    m_settings->set("NBTStudioPath", path);
}

QString NBTStudioTool::path() const
{
    return m_settings->get("NBTStudioPath").toString();
}

bool NBTStudioTool::check(const QString& toolPath, QString& error) const
{
    if (toolPath.isEmpty()) {
        error = QObject::tr("Path is empty");
        return false;
    }

    if (!QFile::exists(toolPath)) {
        error = QObject::tr("File does not exist");
        return false;
    }

#ifdef Q_OS_WIN32
    const std::wstring pathBytes = toolPath.toStdWString();

    DWORD binaryType;  // unused

    if (!GetBinaryTypeW(pathBytes.c_str(), &binaryType)) {
        error = QObject::tr("File is not a Windows executable");
        return false;
    }

#endif

    return true;
}

QString NBTStudioTool::getProgramPath() const
{
    QString result = path();

    if (QFile::exists(result))
        return result;

    return {};
}
