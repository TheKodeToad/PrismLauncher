#pragma once

#include <QMetaObject>
#include <QObject>
#include <optional>

/// Represents a type which can be used in ConfigHolder since it supports saving, loading and comparison.
template <typename T>
concept ConfigObject = requires(T x) {
    { x.load(QString{}) } -> std::same_as<bool>;
    { x.save(QString{}) } -> std::same_as<bool>;
    { x == x } -> std::same_as<bool>;
};

/// Wrapper for ConfigObject which automatically handles saving and emits a signal on update.
/// Should only be called from one thread!
template <ConfigObject T>
class ConfigHolder {
    Q_OBJECT
   public:
    explicit ConfigHolder(QString path) : m_path(std::move(path))
    {
        m_saveTimer.setInterval(5000);
        m_saveTimer.setSingleShot(true);
        connect(m_saveTimer, &QTimer::timeout, this, [this] { m_config.save(); });
    }

    const T& get() const { return m_config; }

    T& update()
    {
        markDirty();
        return m_config;
    }

    bool reload() { return m_config.load(); }

   signals:
    void updated();

   private:
    void markDirty()
    {
        if (m_prevConfig.has_value()) {
            // already waiting for handleDirty to be called
            return;
        }

        m_prevConfig = m_config;
        QMetaObject::invokeMethod(this, &ConfigHolder::handleDirty, Qt::QueuedConnection);
    }

    void handleDirty()
    {
        const bool unchanged = *m_prevConfig == m_config;
        m_prevConfig = std::nullopt;
        if (unchanged) {
            return;
        }

        emit updated();

        if (m_saveTimer.isActive()) {
            qDebug() << "Delaying config save to happen in" << m_saveTimer.interval() << "ms";
        } else {
            qDebug() << "Scheduling config save to happen in" << m_saveTimer.interval() << "ms";
        }
        m_saveTimer.start();
    }

    QString m_path;
    T m_config;
    /// When the configuration is marked dirty, holds the old configuration until control returns to the event loop.
    std::optional<T> m_prevConfig;
    QTimer m_saveTimer;
};
