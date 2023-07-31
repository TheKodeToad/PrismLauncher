// SPDX-License-Identifier: GPL-3.0-only
/*
 *  PolyMC - Minecraft Launcher
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
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
 *
 * This file incorporates work covered by the following copyright and
 * permission notice:
 *
 *      Copyright 2013-2021 MultiMC Contributors
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#include "MSALoginDialog.h"
#include "ui_MSALoginDialog.h"

#include "Application.h"
#include "BuildConfig.h"
#include "CustomMessageBox.h"
#include "DesktopServices.h"
#include "minecraft/auth/AccountTask.h"
#include "minecraft/auth/flows/AuthFlow.h"

#include <qfontdatabase.h>
#include <QApplication>
#include <QClipboard>
#include <QUrl>
#include <QtWidgets/QPushButton>

MSALoginDialog::MSALoginDialog(QWidget* parent) : QDialog(parent), ui(new Ui::MSALoginDialog)
{
    ui->setupUi(this);

    // make font monospace
    QFont font;
    font.setPixelSize(ui->code->fontInfo().pixelSize());
    font.setFamily(APPLICATION->settings()->get("ConsoleFont").toString());
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    ui->code->setFont(font);

    ui->buttonBox->button(QDialogButtonBox::Help)->setDefault(false);

    connect(ui->copyCode, &QPushButton::clicked, this, [this] { QApplication::clipboard()->setText(ui->code->text()); });
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, m_loginTask.get(), &AccountTask::abort);
}

int MSALoginDialog::exec()
{
    // Setup the login task and start it
    m_account = MinecraftAccount::createBlankMSA();
    m_loginTask = m_account->loginMSA();
    connect(m_loginTask.get(), &Task::failed, this, &MSALoginDialog::reject);
    connect(m_loginTask.get(), &Task::succeeded, this, &MSALoginDialog::onTaskSucceeded);
    connect(m_loginTask.get(), &Task::aborted, this, &MSALoginDialog::reject);
    connect(m_loginTask.get(), &Task::status, this, &MSALoginDialog::onTaskStatus);
    connect(m_loginTask.get(), &AccountTask::showVerificationUriAndCode, this, &MSALoginDialog::showVerificationUriAndCode);
    connect(m_loginTask.get(), &AccountTask::hideVerificationUriAndCode, this, &MSALoginDialog::hideVerificationUriAndCode);
    m_loginTask->start();

    return QDialog::exec();
}

MSALoginDialog::~MSALoginDialog()
{
    delete ui;
}

void MSALoginDialog::showVerificationUriAndCode(const QUrl& uri, const QString& code, [[maybe_unused]] int expiresIn)
{
    ui->stackedWidget->setCurrentIndex(1);

    const QString urlString = uri.toString();
    const QString linkString = QString("<a href=\"%1\">%2</a>").arg(urlString, urlString);
    ui->code->setText(code);
    ui->codeInfo->setText(tr("<p>Enter this code into %1 and choose your account.</p>").arg(linkString));
}

void MSALoginDialog::hideVerificationUriAndCode()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MSALoginDialog::onTaskSucceeded()
{
    QDialog::accept();
}

void MSALoginDialog::onTaskStatus(const QString& status)
{
    ui->status->setText(status);
}

// Public interface
MinecraftAccountPtr MSALoginDialog::newAccount(QWidget* parent)
{
    MSALoginDialog dlg(parent);
    if (dlg.exec() == QDialog::Accepted) {
        return dlg.m_account;
    }
    if (!dlg.m_loginTask->failReason().isNull()) {
        CustomMessageBox::selectable(parent, tr("Failed"), dlg.m_loginTask->getStatus(), QMessageBox::Critical)->show();
    }
    return nullptr;
}
