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

#include <QObject>
#include <QTabletEvent>

class ExtWacomManager {
public:
    explicit ExtWacomManager();
    ~ExtWacomManager();

public:
    qreal absoluteX, absoluteY, absoluteZ, pressure, tangentialPressure, tiltX, tiltY, rotation;
    quint16 deviceID;

protected:
    quint32 mLastUsedTablet, mContextID, mTabletOfContext;

public:
    void setMouseControl(bool val);
    void setValuesFromEvent(void*);
};
