#pragma once

#include <QString>
#include <cstdint>
#include <optional>

struct InstanceConfig {
    QString name;
    QString iconKey;
    QString notes;

    int64_t lastLaunchTime{};
    int64_t totalTimePlayed{};

    QStringList linkedInstances;
    enum class ShortcutTarget : std::uint8_t { Desktop, Applications, Other, Count };
    struct Shortcut {
        QString name;
        QString filePath;
        ShortcutTarget target;

        bool operator==(const Shortcut&) const = default;
    };
    QList<Shortcut> shortcuts;
    QString uuid;

    struct GameTimeOverrides {
        bool showGameTime;
        bool recordGameTime;

        bool operator==(const GameTimeOverrides&) const = default;
    };
    std::optional<GameTimeOverrides> gameTimeOverrides;
    bool countGameTime{};

    struct CommandOverrides {
        QString preLaunchCommand;
        QString wrapperCommand;
        QString postExitCommand;

        bool operator==(const CommandOverrides&) const = default;
    };
    std::optional<CommandOverrides> commandOverrides;

    struct ConsoleOverrides {
        bool showConsole;
        bool autoCloseConsole;
        bool showConsoleOnError;
        bool logPrePostOutput;

        bool operator==(const ConsoleOverrides&) const = default;
    };
    std::optional<ConsoleOverrides> consoleOverrides;

    struct ManagedPack {
        QString type;
        QString id;
        QString name;
        QString versionId;
        QString versionName;
        QString url;

        bool operator==(const ManagedPack&) const = default;
    };
    std::optional<ManagedPack> managedPack;

    QString profiler;

    bool load(const QString& path);

    bool save(const QString& path) const;

    bool operator==(const InstanceConfig&) const = default;
};
