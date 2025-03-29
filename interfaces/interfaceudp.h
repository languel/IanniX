/*
    This file is part of IanniX, a graphical real-time open-source sequencer for digital art
    Copyright (C) 2010-2015 — IanniX Association

    Project Manager: Thierry Coduys (http://www.le-hub.org)
    Development:     Guillaume Jacquemin (https://www.buzzinglight.com)

    This file was written by Guillaume Jacquemin.

    IanniX is a free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INTERFACEUDP_H
#define INTERFACEUDP_H

#include <QWidget>
#include "misc/options.h"
#include "messages/messagemanager.h"

namespace Ui {
class InterfaceUdp;
}

class InterfaceUdp : public NetworkInterface {
    Q_OBJECT
    
public:
    explicit InterfaceUdp(QWidget *parent = 0);
    ~InterfaceUdp();

private:
    UiReal port;
    UiBool enable;
private slots:
    void portChanged();
    void openExamples() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(QFileInfo(Application::pathPatches.absoluteFilePath() + "/PureData/").absoluteFilePath()));
    }

private:
    QUdpSocket *socket;
    char bufferI[4096];
    quint16 bufferISize;
private slots:
    void parseOSC();

public:
    bool send(const Message &message, QStringList *messageSent = 0);

private:
    Ui::InterfaceUdp *ui;
};

#endif // INTERFACEUDP_H
