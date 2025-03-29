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

#ifndef UIEDITOR_H
#define UIEDITOR_H

#include <QMainWindow>
#include <QFileInfo>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QFile>
#include <QTextCursor>
#include "misc/help.h"
#include "misc/application.h"


namespace Ui {
    class UiEditor;
}

class UiEditor : public QMainWindow {
    Q_OBJECT

public:
    explicit UiEditor(QWidget *parent = 0);
    ~UiEditor();

public:
    void setContent(const QString &content, bool raiseWindow);
    const QString getContent();

signals:
    void askSave();
    void askRefresh();

public slots:
    void save()     { emit(askSave()); }
    void refresh()  { emit(askRefresh()); }
    void cursorChanged();
    void scriptError(const QStringList &errors, qint16 line);

public:
    QAction *toolbarButton;
    bool firstLaunch;
protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
private:
    Ui::UiEditor *ui;
};

#endif // UIEDITOR_H
