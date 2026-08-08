#include "GlobalConfig.h"
#include "SysInfo.h"
#include "settings/INIFile.h"

#include <QSettings>
#include <QVariant>
#include <utility>

using namespace Qt::Literals;

bool GlobalConfig::load(const QString& path)
{
    qDebug() << u"Loading global config from" << path;

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
            qWarning() << u"Global config value under" << key << u"was not of the correct type - expected" << expected.name() << u"but got"
                       << got.name();
            return def;
        }

        return val.value<T>();
    };

    iconTheme = value("IconTheme", QString());
    applicationTheme = value("ApplicationTheme", QString());
    backgroundCat = value("BackgroundCat", QString("Kitteh"));
    lastUsedGroupForNewInstance = value("LastUsedGroupForNewInstance", QString());
    menuBarInsteadOfToolBar = value("MenuBarInsteadOfToolBar", false);

    numberOfConcurrentTasks = value("NumberOfConcurrentTasks", 10);
    numberOfConcurrentDownloads = value("NumberOfConcurrentDownloads", 6);
    numberOfManualRetries = value("NumberOfManualRetries", 1);
    requestTimeout = value("RequestTimeout", 60);

    consoleFont = value("ConsoleFont", QString("Courier New"));  // FIXME: don't hardcode this!
    consoleFontSize = value("ConsoleFontSize", 11);              // FIXME: no hardcode!
    consoleMaxLines = value("ConsoleMaxLines", 100'000);
    consoleOverflowStop = value("ConsoleOverflowStop", true);

    instanceDir = value("InstanceDir", QString("instances"));
    additionalInstanceDirs = value("AdditionalInstanceDirs", QStringList());
    lastUsedInstDirForNewInstance = value("LastUsedInstDirForNewInstance", QString());
    centralModsDir = value("CentralModsDir", QString("mods"));
    iconsDir = value("IconsDir", QString("icons"));
    downloadsDir = value("DownloadsDir", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
    downloadsDirWatchRecursive = value("DownloadsDirWatchRecursive", false);
    moveModsFromDownloadsDir = value("MoveModsFromDownloadsDir", false);
    skinsDir = value("SkinsDir", QString("skins"));
    javaDir = value("JavaDir", QString("java"));

    language = value("Language", QString());
    useSystemLocale = value("UseSystemLocale", false);

    showConsole = value("ShowConsole", false);
    autoCloseConsole = value("AutoCloseConsole", false);
    showConsoleOnError = value("ShowConsoleOnError", true);
    logPrePostOutput = value("LogPrePostOutput", true);

    launchMaximized = value("LaunchMaximized", false);
    minecraftWinWidth = value("MinecraftWinWidth", 854);
    minecraftWinHeight = value("MinecraftWinHeight", 480);

    proxyType = value("ProxyType", QString());
    proxyAddr = value("ProxyAddr", QString("127.0.0.1"));
    proxyPort = value("ProxyPort", 8080);
    proxyUser = value("ProxyUser", QString());
    proxyPass = value("ProxyPass", QString());

    minMemAlloc = value("MinMemAlloc", 512);
    // FIXME: probably should be cached per instance, but technically doesn't matter (I think)
    static const int s_defaultMaxJvmMem = SysInfo::defaultMaxJvmMem();
    maxMemAlloc = value("MaxMemAlloc", s_defaultMaxJvmMem);
    permGen = value("PermGen", 128);
    lowMemWarning = value("LowMemWarning", true);

    javaPath = value("JavaPath", QString());
    javaSignature = value("JavaSignature", QString());
    javaArchitecture = value("JavaArchitecture", QString());
    javaRealArchitecture = value("JavaRealArchitecture", QString());
    javaVersion = value("JavaVersion", QString());
    javaVendor = value("JavaVendor", QString());
    lastHostname = value("LastHostname", QString());
    jvmArgs = value("JvmArgs", QString());
    ignoreJavaCompatibility = value("IgnoreJavaCompatibility", false);
    ignoreJavaWizard = value("IgnoreJavaWizard", false);
    automaticJavaSwitch = value("AutomaticJavaSwitch", javaPath.isEmpty());
    automaticJavaDownload = value("AutomaticJavaDownload", javaPath.isEmpty());
    userAskedAboutAutomaticJavaDownload = value("UserAskedAboutAutomaticJavaDownload", false);

    onlineFixes = value("OnlineFixes", false);

    useNativeOpenAL = value("UseNativeOpenAL", false);
    customOpenALPath = value("CustomOpenALPath", QString());
    useNativeGLFW = value("UseNativeGLFW", false);
    customGLFWPath = value("CustomGLFWPath", QString());
    useNativeSDL = value("UseNativeSDL", false);
    customSDLPath = value("CustomSDLPath", QString());

    enableFeralGamemode = value("EnableFeralGamemode", false);
    enableMangoHud = value("EnableMangoHud", false);
    useDiscreteGpu = value("UseDiscreteGpu", false);
    useZink = value("UseZink", false);

    showGameTime = value("ShowGameTime", true);
    showGlobalGameTime = value("ShowGlobalGameTime", true);
    recordGameTime = value("RecordGameTime", true);
    showGameTimeWithoutDays = value("ShowGameTimeWithoutDays", true);
    totalPlayTime = static_cast<int64_t>(value("TotalPlayTime", qlonglong{ 0 }));
    totalPlayTimeMigrated = value("TotalPlayTimeMigrated", false);

    modMetadataDisabled = value("ModMetadataDisabled", false);
    modDependenciesDisabled = value("ModDependenciesDisabled", false);
    skipModpackUpdatePrompt = value("SkipModpackUpdatePrompt", false);
    showModIncompat = value("ShowModIncompat", false);
    downloadGameFilesDuringInstanceCreation = value("DownloadGameFilesDurationInstanceCreation", true);

    lastOfflinePlayerName = value("LastOfflinePlayerName", QString());

    wrapperCommand = value("WrapperCommand", QString());
    preLaunchCommand = value("PreLaunchCommand", QString());
    postExitCommand = value("PostExitCommand", QString());

    enableCat = value("EnableCat", true);
    theCat = value("TheCat", false);
    catOpacity = value("CatOpacity", 100);
    catFit = value("CatFit", QString("Fit"));

    statusBarVisible = value("StatusBarVisible", true);
    toolbarsLocked = value("ToolbarsLocked", false);

    instSortMode = value("InstSortMode", QString("Name"));
    instRenamingMode = value("InstRenamingMode", QString("AskEverytime"));
    editInstanceOnDoubleClick = value("EditInstanceOnDoubleClick", false);
    selectedInstance = value("SelectedInstance", QString());

    pastebinType = value("PastebinType", PasteUpload::PasteType::Mclogs);
    if (pastebinType < PasteUpload::PasteType::First || pastebinType > PasteUpload::PasteType::Last) {
        pastebinType = PasteUpload::PasteType::Mclogs;
    }

    const QString pastebinURL = value("PastebinURL", QString());
    if (!pastebinURL.isEmpty() && pastebinURL != "https://0x0.st") {
        pastebinType = PasteUpload::PasteType::NullPointer;
        pastebinCustomApiBase = pastebinURL;
    } else {
        pastebinCustomApiBase = value("PastebinCustomAPIBase", QString());
    }

    // FIXME: no longer resets URL
    metaUrlOverride = value("MetaURLOverride", QString());
    resourceUrlOverride = value("ResourceURLOverride", value("ResourceURL", QString()));
    legacyFmlLibsUrlOverride = value("LegacyFMLLibsURLOverride", QString());

    metaRefreshOnLaunch = value("MetaRefreshOnLaunch", true);
    closeAfterLaunch = value("CloseAfterLaunch", false);
    quitAfterGameStop = value("QuitAfterGameStop", false);

    env = Json::toMap(value("Env", QString("{}")));

    msaClientIdOverride = value("MSAClientIDOverride", QString());
    flameKeyOverride = value("FlameKeyOverride", value("CFKeyOverride", QString()));
    fallbackModrinthBlockedMods = value("FallbackMRBlockedMods", true);
    modrinthToken = value("ModrinthToken", QString());
    userAgentOverride = value("UserAgentOverride", QString());
    ftbAppInstancesPath = value("FTBAppInstancesPath", QString());
    technicClientId = value("TechnicClientID", QString());

    jsonEditorPath = value("JsonEditor", QString());
    mcEditPath = value("MCEditPath", QString());
    jProfilerPath = value("JProfilerPath", QString());
    jProfilerPort = value("JProfilerPort", 42042);
    jVisualVmPath = value("JVisualVMPath", QString());

    uiGeometry.clear();
    uiState.clear();
    uiWideBarState.clear();
    uiColumnVisibility.clear();

    for (auto iter = file.begin(); iter != file.end(); ++iter) {
        QString key = iter.key();
        auto removePrefix = [](QString& key, QStringView prefix) {
            if (!key.startsWith(prefix)) {
                return false;
            }

            key = key.mid(prefix.length());
            return true;
        };

        if (removePrefix(key, u"UIGeometry/"_s)) {
            const auto decoded = QByteArray::fromBase64(iter.value().toByteArray());
            uiGeometry[key] = decoded;
        } else if (removePrefix(key, u"UIState/"_s)) {
            const auto decoded = QByteArray::fromBase64(iter.value().toByteArray());
            uiState[key] = decoded;
        } else if (removePrefix(key, u"UIWideBarState/"_s)) {
            const auto decoded = QByteArray::fromBase64(iter.value().toByteArray());
            uiWideBarState[key] = decoded;
        } else if (removePrefix(key, u"UIColumnVisibility/"_s)) {
            const auto doc = QJsonDocument::fromJson(iter.value().toByteArray());
            if (!doc.isObject()) {
                qWarning() << u"Expected JSON object under global config key" << iter.key();
                continue;
            }

            const auto obj = doc.object();
            QHash<QString, bool> map;

            for (auto iter = obj.begin(); iter != obj.end(); ++iter) {
                map[iter.key()] = iter.value().toBool();
            }

            uiColumnVisibility[key] = std::move(map);
        }
    }

    return true;
}

bool GlobalConfig::save(const QString& path) const
{
    qDebug() << u"Saving global config to" << path;

    INIFile file;

    file["IconTheme"] = iconTheme;
    file["ApplicationTheme"] = applicationTheme;
    file["BackgroundCat"] = backgroundCat;
    file["LastUsedGroupForNewInstance"] = lastUsedGroupForNewInstance;
    file["MenuBarInsteadOfToolBar"] = menuBarInsteadOfToolBar;

    file["NumberOfConcurrentTasks"] = numberOfConcurrentTasks;
    file["NumberOfConcurrentDownloads"] = numberOfConcurrentDownloads;
    file["NumberOfManualRetries"] = numberOfManualRetries;
    file["RequestTimeout"] = requestTimeout;

    file["ConsoleFont"] = consoleFont;
    file["ConsoleFontSize"] = consoleFontSize;
    file["ConsoleMaxLines"] = consoleMaxLines;
    file["ConsoleOverflowStop"] = consoleOverflowStop;

    file["InstanceDir"] = instanceDir;
    file["AdditionalInstanceDirs"] = additionalInstanceDirs;
    file["LastUsedInstDirForNewInstance"] = lastUsedInstDirForNewInstance;
    file["CentralModsDir"] = centralModsDir;
    file["IconsDir"] = iconsDir;
    file["DownloadsDir"] = downloadsDir;
    file["DownloadsDirWatchRecursive"] = downloadsDirWatchRecursive;
    file["MoveModsFromDownloadsDir"] = moveModsFromDownloadsDir;
    file["SkinsDir"] = skinsDir;
    file["JavaDir"] = javaDir;

    file["Language"] = language;
    file["UseSystemLocale"] = useSystemLocale;

    file["ShowConsole"] = showConsole;
    file["AutoCloseConsole"] = autoCloseConsole;
    file["ShowConsoleOnError"] = showConsoleOnError;
    file["LogPrePostOutput"] = logPrePostOutput;

    file["LaunchMaximized"] = launchMaximized;
    file["MinecraftWinWidth"] = minecraftWinWidth;
    file["MinecraftWinHeight"] = minecraftWinHeight;

    file["ProxyType"] = proxyType;
    file["ProxyAddr"] = proxyAddr;
    file["ProxyPort"] = proxyPort;
    file["ProxyUser"] = proxyUser;
    file["ProxyPass"] = proxyPass;

    file["MinMemAlloc"] = minMemAlloc;
    file["MaxMemAlloc"] = maxMemAlloc;
    file["PermGen"] = permGen;
    file["LowMemWarning"] = lowMemWarning;

    file["JavaPath"] = javaPath;
    file["JavaSignature"] = javaSignature;
    file["JavaArchitecture"] = javaArchitecture;
    file["JavaRealArchitecture"] = javaRealArchitecture;
    file["JavaVersion"] = javaVersion;
    file["JavaVendor"] = javaVendor;
    file["LastHostname"] = lastHostname;
    file["JvmArgs"] = jvmArgs;
    file["IgnoreJavaCompatibility"] = ignoreJavaCompatibility;
    file["IgnoreJavaWizard"] = ignoreJavaWizard;
    file["AutomaticJavaSwitch"] = automaticJavaSwitch;
    file["AutomaticJavaDownload"] = automaticJavaDownload;
    file["UserAskedAboutAutomaticJavaDownload"] = userAskedAboutAutomaticJavaDownload;

    file["OnlineFixes"] = onlineFixes;

    file["UseNativeOpenAL"] = useNativeOpenAL;
    file["CustomOpenALPath"] = customOpenALPath;
    file["UseNativeGLFW"] = useNativeGLFW;
    file["CustomGLFWPath"] = customGLFWPath;
    file["UseNativeSDL"] = useNativeSDL;
    file["CustomSDLPath"] = customSDLPath;

    file["EnableFeralGamemode"] = enableFeralGamemode;
    file["EnableMangoHud"] = enableMangoHud;
    file["UseDiscreteGpu"] = useDiscreteGpu;
    file["UseZink"] = useZink;

    file["ShowGameTime"] = showGameTime;
    file["ShowGlobalGameTime"] = showGlobalGameTime;
    file["RecordGameTime"] = recordGameTime;
    file["ShowGameTimeWithoutDays"] = showGameTimeWithoutDays;
    file["TotalPlayTime"] = static_cast<qlonglong>(totalPlayTime);
    file["TotalPlayTimeMigrated"] = totalPlayTimeMigrated;

    file["ModMetadataDisabled"] = modMetadataDisabled;
    file["ModDependenciesDisabled"] = modDependenciesDisabled;
    file["SkipModpackUpdatePrompt"] = skipModpackUpdatePrompt;
    file["ShowModIncompat"] = showModIncompat;
    file["DownloadGameFilesDuringInstanceCreation"] = downloadGameFilesDuringInstanceCreation;

    file["LastOfflinePlayerName"] = lastOfflinePlayerName;

    file["WrapperCommand"] = wrapperCommand;
    file["PreLaunchCommand"] = preLaunchCommand;
    file["PostExitCommand"] = postExitCommand;

    file["EnableCat"] = enableCat;
    file["TheCat"] = theCat;
    file["CatOpacity"] = catOpacity;
    file["CatFit"] = catFit;

    file["StatusBarVisible"] = statusBarVisible;
    file["ToolbarsLocked"] = toolbarsLocked;

    file["InstSortMode"] = instSortMode;
    file["InstRenamingMode"] = instRenamingMode;
    file["EditInstanceOnDoubleClick"] = editInstanceOnDoubleClick;
    file["SelectedInstance"] = selectedInstance;

    file["PastebinType"] = pastebinType;
    file["PastebinCustomAPIBase"] = pastebinCustomApiBase.toString();

    file["MetaURLOverride"] = metaUrlOverride.toString();
    file["ResourceURLOverride"] = resourceUrlOverride.toString();
    file["LegacyFMLLibsURLOverride"] = legacyFmlLibsUrlOverride.toString();

    file["MetaRefreshOnLaunch"] = metaRefreshOnLaunch;
    file["CloseAfterLaunch"] = closeAfterLaunch;
    file["QuitAfterGameStop"] = quitAfterGameStop;

    file["Env"] = Json::fromMap(env);

    file["MSAClientIDOverride"] = msaClientIdOverride;
    file["FlameKeyOverride"] = flameKeyOverride;
    file["FallbackMRBlockedMods"] = fallbackModrinthBlockedMods;
    file["ModrinthToken"] = modrinthToken;
    file["UserAgentOverride"] = userAgentOverride;
    file["FTBAppInstancesPath"] = ftbAppInstancesPath;
    file["TechnicClientID"] = technicClientId;

    file["JsonEditor"] = jsonEditorPath;
    file["MCEditPath"] = mcEditPath;
    file["JProfilerPath"] = jProfilerPath;
    file["JProfilerPort"] = jProfilerPort;
    file["JVisualVMPath"] = jVisualVmPath;

    for (auto iter = uiGeometry.begin(); iter != uiGeometry.end(); ++iter) {
        file["UIGeometry/" + iter.key()] = QString::fromLatin1(iter.value().toBase64());
    }

    for (auto iter = uiState.begin(); iter != uiState.end(); ++iter) {
        file["UIState/" + iter.key()] = QString::fromLatin1(iter.value().toBase64());
    }

    for (auto iter = uiWideBarState.begin(); iter != uiWideBarState.end(); ++iter) {
        file["UIWideBarState/" + iter.key()] = QString::fromLatin1(iter.value().toBase64());
    }

    for (auto iter = uiColumnVisibility.begin(); iter != uiColumnVisibility.end(); ++iter) {
        QJsonObject obj;
        for (auto mapIter = iter.value().begin(); mapIter != iter.value().end(); ++mapIter) {
            obj[mapIter.key()] = mapIter.value();
        }

        const QJsonDocument doc{ obj };
        file["UIColumnVisibility/" + iter.key()] = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
    }

    return file.saveFile(path);
}
