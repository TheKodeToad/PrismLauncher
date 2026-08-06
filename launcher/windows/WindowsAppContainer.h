// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher - Minecraft Launcher
 *  Copyright (C) 2026 Octol1ttle <l1ttleofficial@outlook.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, version 3.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#ifdef Q_OS_WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#endif

#include <expected>

class WindowsAppContainer final {
   public:
    enum class AccessMode {
        Traverse,
        Read,
        ReadWrite
    };

    static std::expected<std::unique_ptr<WindowsAppContainer>, HRESULT> create();
    static std::expected<void, HRESULT> deleteProfile();

    ~WindowsAppContainer() = default;

    void attach(QProcess* process);
    std::expected<void, std::error_code> grantFileSystemAccess(const QString& path, AccessMode mode);
    void addCapability(const QString& capability);

   private:
    static std::wstring appContainerName();
    static std::expected<PSID, std::error_code> getCapabilitySid(const QString& name);
    static std::expected<bool, std::error_code> aclsEqual(PACL acl1, PACL acl2);

    explicit WindowsAppContainer(PSID appContainerSid);

    std::expected<STARTUPINFOEXW*, std::error_code> createStartupInfo(const Q_STARTUPINFO& original) const;
    std::expected<void, std::error_code> buildCapabilities(PSID_AND_ATTRIBUTES* capabilities, DWORD* capabilityCount) const;
    std::expected<void, std::error_code> addGrantToFileAcl(const QString& path, DWORD permissions, DWORD inheritance) const;
    std::expected<void, std::error_code> elevateToRetryGrants();
    std::expected<void, std::error_code> grantWindowStationWritePermissions() const;

   private:
    std::unique_ptr<std::remove_pointer_t<PSID>, decltype(&FreeSid)> m_appContainerSid;
    QStringList m_allowedTraversePaths;
    QStringList m_deniedTraversePaths;
    QStringList m_requestedCapabilities;
};