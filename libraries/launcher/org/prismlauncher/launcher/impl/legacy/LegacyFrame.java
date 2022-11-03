// SPDX-License-Identifier: GPL-3.0-only
/*
 *  Prism Launcher
 *
 *  Copyright (C) 2022 icelimetea <fr3shtea@outlook.com>
 *  Copyright (C) 2022 Sefa Eyeoglu <contact@scrumplex.net>
 *  Copyright (C) 2022 flow <flowlnlnln@gmail.com>
 *  Copyright (C) 2022 Samisafool <thenerdiestguy@gmail.com>
 *  Copyright (C) 2022 TheKodeToad <TheKodeToad@proton.me>
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
 *  Linking this library statically or dynamically with other modules is
 *  making a combined work based on this library. Thus, the terms and
 *  conditions of the GNU General Public License cover the whole
 *  combination.
 *
 *  As a special exception, the copyright holders of this library give
 *  you permission to link this library with independent modules to
 *  produce an executable, regardless of the license terms of these
 *  independent modules, and to copy and distribute the resulting
 *  executable under terms of your choice, provided that you also meet,
 *  for each linked independent module, the terms and conditions of the
 *  license of that module. An independent module is a module which is
 *  not derived from or based on this library. If you modify this
 *  library, you may extend this exception to your version of the
 *  library, but you are not obliged to do so. If you do not wish to do
 *  so, delete this exception statement from your version.
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

package org.prismlauncher.launcher.impl.legacy;

import net.minecraft.Launcher;

import javax.imageio.ImageIO;
import java.applet.Applet;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

@SuppressWarnings({ "serial", "removal" })
public final class LegacyFrame extends Frame /* TODO consider JFrame */ {

    private static final Logger LOGGER = Logger.getLogger("LegacyFrame");

    private final Launcher launcher;

    public LegacyFrame(String title, Applet applet) {
        super(title);

        launcher = new Launcher(applet);

        applet.setStub(launcher);

        try {
            setIconImage(ImageIO.read(new File("icon.png")));
        } catch (IOException e) {
            LOGGER.log(Level.WARNING, "Unable to read Minecraft icon!", e);
        }

        addWindowListener(new ForceExitHandler());
    }

    public void start (
        String user,
        String session,
        int width,
        int height,
        boolean maximize,
        String serverAddress,
        String serverPort,
        boolean isDemo
    ) {
        // Implements support for launching in to multiplayer on classic servers using a mpticket
        // file generated by an external program and stored in the instance's root folder.

        Path mpticketFile =
                Paths.get(System.getProperty("user.dir"), "..", "mpticket");

        Path mpticketFileCorrupt =
                Paths.get(System.getProperty("user.dir"), "..", "mpticket.corrupt");

        if (Files.exists(mpticketFile)) {
            try {
                List<String> lines = Files.readAllLines(mpticketFile, StandardCharsets.UTF_8);

                if (lines.size() < 3) {
                    Files.move(
                            mpticketFile,
                            mpticketFileCorrupt,
                            StandardCopyOption.REPLACE_EXISTING
                    );

                    LOGGER.warning("Mpticket file is corrupted!");
                } else {
                    // Assumes parameters are valid and in the correct order
                    launcher.setParameter("server", lines.get(0));
                    launcher.setParameter("port", lines.get(1));
                    launcher.setParameter("mppass", lines.get(2));
                }
            } catch (IOException e) {
                LOGGER.log(Level.WARNING, "Unable to read mpticket file!", e);
            }
        }

        if (serverAddress != null) {
            launcher.setParameter("server", serverAddress);
            launcher.setParameter("port", serverPort);
        }

        launcher.setParameter("username", user);
        launcher.setParameter("sessionid", session);
        launcher.setParameter("stand-alone", "true"); // Show the quit button. TODO: why won't this work?
        launcher.setParameter("haspaid", "true"); // Some old versions need this for world saves to work.
        launcher.setParameter("demo", isDemo ? "true" : "false");
        launcher.setParameter("fullscreen", "false");

        add(launcher);

        launcher.setPreferredSize(new Dimension(width, height));

        pack();

        setLocationRelativeTo(null);
        setResizable(true);

        if (maximize)
            this.setExtendedState(MAXIMIZED_BOTH);

        validate();

        launcher.init();
        launcher.start();

        setVisible(true);
    }

    private final class ForceExitHandler extends WindowAdapter {

        @Override
        public void windowClosing(WindowEvent event) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Thread.sleep(30000L);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }

                    LOGGER.info("Forcing exit!");

                    System.exit(0);
                }
            }).start();

            if (launcher != null) {
                launcher.stop();
                launcher.destroy();
            }

            // old minecraft versions can hang without this >_<
            System.exit(0);
        }

    }

}
