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

#ifndef UITHEME_H
#define UITHEME_H

#include <QString>

// Application-wide widget chrome theming (dark/light), applied through
// qApp->setStyleSheet(). The score render colors are themed separately
// through Render::colors ("darktheme_*" / "lighttheme_*" keys).
class UiTheme {
public:
    static void apply(bool light);
    static bool isLight() { return lightTheme; }

private:
    static bool lightTheme;
    static QString stylesheet(bool light);
};

#endif // UITHEME_H
