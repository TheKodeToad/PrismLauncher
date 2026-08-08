#pragma once

#include "net/PasteUpload.h"

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <cstdint>

struct GlobalConfig {
    QString iconTheme;
    QString applicationTheme;
    QString backgroundCat;
    QString lastUsedGroupForNewInstance;
    bool menuBarInsteadOfToolBar{};

    int numberOfConcurrentTasks{};
    int numberOfConcurrentDownloads{};
    int numberOfManualRetries{};
    int requestTimeout{};

    QString consoleFont;
    int consoleFontSize{};
    int consoleMaxLines{};
    bool consoleOverflowStop{};

    QString instanceDir;
    QStringList additionalInstanceDirs;
    QString lastUsedInstDirForNewInstance;
    QString centralModsDir;
    QString iconsDir;
    QString downloadsDir;
    bool downloadsDirWatchRecursive{};
    bool moveModsFromDownloadsDir{};
    QString skinsDir;
    QString javaDir;

    // TODO: readd security-scoped bookmarks

    QString language;
    bool useSystemLocale{};

    bool showConsole{};
    bool autoCloseConsole{};
    bool showConsoleOnError{};
    bool logPrePostOutput{};

    bool launchMaximized{};
    int minecraftWinWidth{};
    int minecraftWinHeight{};

    // TODO: switch with enum
    QString proxyType;
    QString proxyAddr;
    int proxyPort{};
    QString proxyUser;
    QString proxyPass;

    int minMemAlloc{};
    int maxMemAlloc{};
    int permGen{};
    bool lowMemWarning{};

    QString javaPath;
    QString javaSignature;
    QString javaArchitecture;
    QString javaRealArchitecture;
    QString javaVersion;
    QString javaVendor;
    QString lastHostname;
    QString jvmArgs;
    bool ignoreJavaCompatibility{};
    bool ignoreJavaWizard{};
    bool automaticJavaSwitch{};
    bool automaticJavaDownload{};
    bool userAskedAboutAutomaticJavaDownload{};

    bool onlineFixes{};

    bool useNativeOpenAL{};
    QString customOpenALPath;
    bool useNativeGLFW{};
    QString customGLFWPath;
    bool useNativeSDL{};
    QString customSDLPath;

    bool enableFeralGamemode{};
    bool enableMangoHud{};
    bool useDiscreteGpu{};
    bool useZink{};

    bool showGameTime{};
    bool showGlobalGameTime{};
    bool recordGameTime{};
    bool showGameTimeWithoutDays{};
    int64_t totalPlayTime{};
    bool totalPlayTimeMigrated{};

    bool modMetadataDisabled{};
    bool modDependenciesDisabled{};
    bool skipModpackUpdatePrompt{};
    bool showModIncompat{};
    bool downloadGameFilesDuringInstanceCreation{};

    QString lastOfflinePlayerName;

    QString wrapperCommand;
    QString preLaunchCommand;
    QString postExitCommand;

    bool enableCat{};
    bool theCat{};
    int catOpacity{};
    // TODO: switch with enum
    QString catFit;

    bool statusBarVisible{};
    bool toolbarsLocked{};

    QString instSortMode;
    QString instRenamingMode;
    bool editInstanceOnDoubleClick{};
    QString selectedInstance;

    PasteUpload::PasteType pastebinType{};
    QUrl pastebinCustomApiBase;

    QUrl metaUrlOverride;
    QUrl resourceUrlOverride;
    QUrl legacyFmlLibsUrlOverride;

    bool metaRefreshOnLaunch{};
    bool closeAfterLaunch{};
    bool quitAfterGameStop{};

    QVariantMap env;

    QString msaClientIdOverride;
    QString flameKeyOverride;
    bool fallbackModrinthBlockedMods{};
    QString modrinthToken;
    QString userAgentOverride;
    QString ftbAppInstancesPath;
    QString technicClientId;

    QString jsonEditorPath;
    QString mcEditPath;
    QString jProfilerPath;
    int jProfilerPort{};
    QString jVisualVmPath;

    QHash<QString, QByteArray> uiGeometry;
    QHash<QString, QByteArray> uiState;
    QHash<QString, QByteArray> uiWideBarState;
    QHash<QString, QHash<QString, bool>> uiColumnVisibility;

    bool load(const QString& path);

    bool save(const QString& path) const;

    bool operator==(const GlobalConfig&) const = default;
};
