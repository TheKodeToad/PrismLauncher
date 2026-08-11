#include "InstanceConfig.h"
#include "settings/INIFile.h"

#include <algorithm>

bool InstanceConfig::load(const QString& path)
{
    qDebug() << u"Loading instance config from" << path;

    INIFile file;
    if (!file.loadFile(path)) {
        return false;
    }
    const auto value = [&file]<typename T>(const QString& key, T def) -> T {
        const QVariant val = file.value(key);
        if (!val.isValid() || !val.canConvert<T>()) {
            return def;
        }

        if (!val.canConvert<T>()) {
            const auto expected = QMetaType::fromType<T>();
            const auto got = val.metaType();
            qWarning() << u"Instance config value under" << key << u"was not of the correct type - expected" << expected.name()
                       << u"but got" << got.name();
            return def;
        }

        return val.value<T>();
    };

    QString type = value("type", QString(""));
    if (!type.isEmpty() && type != "OneSix") {
        qWarning() << "Bad instance type:" << type;
        return false;
    }

    // NOTE: all new keys should be added with UpperCamelCase for consistency
    // The lowerCamelCase keys are retained purely so that older launcher versions can understand the file format

    name = value("name", QString("Unnamed Instance"));
    iconKey = value("iconKey", QString("default"));
    notes = value("notes", QString());

    lastLaunchTime = static_cast<int64_t>(value("lastLaunchTime", qlonglong{ 0 }));
    totalTimePlayed = static_cast<int64_t>(value("totalTimePlayed", qlonglong{ 0 }));
    totalTimePlayed = std::max<int64_t>(totalTimePlayed, 0);

    linkedInstances = Json::toStringList(value("linkedInstances", QString("[]")));

    shortcuts.clear();

    const auto shortcutsJson = value("shortcuts", QByteArray("[]"));
    const auto shortcutsDoc = QJsonDocument::fromJson(shortcutsJson);
    if (shortcutsDoc.isArray()) {
        for (const auto shortcut : shortcutsDoc.array()) {
            if (!shortcut.isObject()) {
                qWarning() << u"Non-object value in instance shortcuts";
                continue;
            }

            const auto shortcutObj = shortcut.toObject();
            const auto name = shortcutObj["name"];
            const auto filePath = shortcutObj["filePath"];
            const auto target = shortcutObj["target"];

            if (!name.isString() || !filePath.isString() || !target.isDouble()) {
                qWarning() << u"Expected shape { name: string, filePath: string, target: number } for instance shortcut";
                continue;
            }

            const auto targetVal = target.toInt();
            if (targetVal < 0 || targetVal >= int(ShortcutTarget::Count)) {
                qWarning() << u"Found invalid instance shortcut target type";
                continue;
            }

            shortcuts.append(Shortcut{
                .name = name.toString(),
                .filePath = filePath.toString(),
                .target = ShortcutTarget(target.toInt()),
            });
        }
    } else {
        qWarning() << "Expected JSON array under instance shortcuts";
    }

    uuid = value("uuid", QString());

    bool overrideGameTime = value("OverrideGameTime", false);
    if (overrideGameTime) {
        gameTimeOverrides = GameTimeOverrides{
            .showGameTime = value("ShowGameTime", true),
            .recordGameTime = value("RecordGameTime", true),
        };
    } else {
        gameTimeOverrides = std::nullopt;
    }

    countGameTime = value("CountGameTime", true);

    bool overrideCommands = value("OverrideCommands", false);
    if (overrideCommands) {
        commandOverrides = CommandOverrides{
            .preLaunchCommand = value("PreLaunchCommand", QString()),
            .wrapperCommand = value("WrapperCommand", QString()),
            .postExitCommand = value("PostExitCommand", QString()),
        };
    } else {
        commandOverrides = std::nullopt;
    }

    bool overrideConsole = value("OverrideConsole", false);
    if (overrideConsole) {
        consoleOverrides = ConsoleOverrides{
            .showConsole = value("ShowConsole", false),
            .autoCloseConsole = value("AutoCloseConsole", false),
            .showConsoleOnError = value("ShowConsoleOnError", true),
        };
    } else {
        consoleOverrides = std::nullopt;
    }

    bool isManagedPack = value("ManagedPack", false);
    if (isManagedPack) {
        managedPack = ManagedPack{
            .type = value("ManagedPackType", QString()),
            .id = value("ManagedPackID", QString()),
            .name = value("ManagedPackName", QString()),
            .versionId = value("ManagedPackVersionID", QString()),
            .versionName = value("ManagedPackVersionID", QString()),
            .url = value("ManagedPackURL", QString()),
        };
    } else {
        managedPack = std::nullopt;
    }

    profiler = value("Profiler", QString());

    return true;
}

bool InstanceConfig::save(const QString& path) const
{
    INIFile file;
    file["type"] = "OneSix";

    file["name"] = name;
    file["iconKey"] = iconKey;
    file["notes"] = notes;

    file["lastLaunchTime"] = static_cast<qlonglong>(lastLaunchTime);
    file["totalTimePlayed"] = static_cast<qlonglong>(totalTimePlayed);

    file["linkedInstance"] = Json::fromStringList(linkedInstances);

    QJsonArray shortcutsArray;
    for (const auto& shortcut : shortcuts) {
        shortcutsArray.append(QJsonObject{
            { "name", shortcut.name },
            { "filePath", shortcut.filePath },
            { "target", static_cast<int>(shortcut.target) },
        });
    }

    const QJsonDocument shortcutsDoc{ shortcutsArray };
    file["shortcuts"] = QString::fromUtf8(shortcutsDoc.toJson(QJsonDocument::Compact));
    file["uuid"] = uuid;

    file["OverrideGameTime"] = gameTimeOverrides.has_value();
    if (gameTimeOverrides.has_value()) {
        file["ShowGameTime"] = gameTimeOverrides->showGameTime;
        file["RecordGameTime"] = gameTimeOverrides->recordGameTime;
    }

    file["CountGameTime"] = countGameTime;

    file["CommandOverrides"] = commandOverrides.has_value();
    if (commandOverrides.has_value()) {
        file["PreLaunchCommand"] = commandOverrides->preLaunchCommand;
        file["WrapperCommand"] = commandOverrides->wrapperCommand;
        file["PostExitCommand"] = commandOverrides->postExitCommand;
    }

    file["ConsoleOverrides"] = consoleOverrides.has_value();
    if (consoleOverrides.has_value()) {
        file["ShowConsole"] = consoleOverrides->showConsole;
        file["AutoCloseConsole"] = consoleOverrides->autoCloseConsole;
        file["ShowConsoleOnError"] = consoleOverrides->showConsoleOnError;
        file["LogPrePostOutput"] = consoleOverrides->logPrePostOutput;
    }

    file["ManagedPack"] = managedPack.has_value();
    if (managedPack.has_value()) {
        file["ManagedPackType"] = managedPack->type;
        file["ManagedPackID"] = managedPack->id;
        file["ManagedPackName"] = managedPack->name;
        file["ManagedPackVersionID"] = managedPack->versionId;
        file["ManagedPackVersionName"] = managedPack->versionName;
        file["ManagedPackURL"] = managedPack->url;
    }

    file["Profiler"] = profiler;

    return file.saveFile(path);
}
