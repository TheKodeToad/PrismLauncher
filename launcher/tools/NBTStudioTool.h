#pragma once
#include "settings/SettingsObject.h"

class NBTStudioTool {
public:
    NBTStudioTool(const SettingsObjectPtr& settings);
    // TODO: why do we need all this?
    void setPath(const QString& path);
    QString path() const;
    bool check(const QString& toolPath, QString& error) const;
    QString getProgramPath() const;

private:
    SettingsObjectPtr m_settings;
};
