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

#ifndef UIFILEITEM_H
#define UIFILEITEM_H

#include <QProcess>
#include <QDir>
#include <QDateTime>
#include <QMessageBox>
#include <QFileSystemWatcher>
#include <QTreeWidgetItem>
#include "uitreeview.h"
#include "misc/options.h"
#include "gui/uimessagebox.h"

class UiFileItem : public QObject, public UiSyncItem {
    Q_OBJECT

private:
    UiBool       openInFinder;
    qint16       currentDepth;
public:
    UiFile filename;
    bool isFile;
    bool isOpened;
    static QIcon iconFile, iconFileOpened, iconFolder;
    static bool showDateTime;

public:
    explicit UiFileItem(const QFileInfo &file, UiFileItem *_parent, QFileSystemWatcher *_watcher);
protected:
    QVariant data(int column, int role) const;
    void setData(int column, int role, const QVariant &value);
public:
    virtual void fileOpen()   { emit(askFileOpen());   }
    virtual void fileSave()   { emit(askFileSave());   }
    virtual void fileReload() { emit(askFileReload()); }
    virtual void fileClose()  { emit(askFileClose());  }
signals:
    void askFileOpen();
    void askFileSave();
    void askFileReload();
    void askFileClose();
public:
    static const QString dateToString(const QDateTime &date);

public:
    static void syncWith(const QFileInfoList &files, QTreeWidget *treeWidget);
    static void configure(UiTreeView *, bool _showDateTime = true);
    UiFileItem* find(const QFileInfo &search);
    static UiFileItem* find(const QFileInfo &search, QTreeWidget *tree);
protected:
    void syncWith(qint16 depth);
    QFileSystemWatcher *watcher;
protected:
    void populate(const QFileInfo &file);
    QStringList syncWith(const QDir &dir, qint16 depth = -1);
    bool highlight(UiSyncItem *item = 0);
    static const QFileInfo fileGetName(const QFileInfo &base);
    static bool fileCopy  (const QFileInfo &source, const QFileInfo &dest);
    static bool fileRename(const QFileInfo &source, const QString &newNameWithoutSuffix, UiFileItem *item = 0);
    static bool fileRename(const QFileInfo &source, const QFileInfo &destination, UiFileItem *item = 0);
public slots:
    void fileShowInFinder();
    void fileShowInOS();
    bool askForOpen(UiSyncItem*);
    bool askForClose(UiSyncItem*);
    bool askForSave(UiSyncItem*, bool as = false);
    bool askForImport(UiSyncItem *i, const QFileInfoList &list);
protected slots:
    void dataChanged() { emitDataChanged(); }
    void fileRename();
    void fileWatcherDirChanged(QString);
    void fileWatcherFileChanged(QString);
    bool askForDeletion(UiSyncItem*, bool dialog = false);
    UiSyncItem *askForNewChild(UiSyncItem*, bool isRoot);
    bool askForMove(UiSyncItem*, UiSyncItem *destination);
    bool askForPaste(UiSyncItem*, const QList<UiSyncItem*> &);
    bool canOpen(UiSyncItem*) const { return isFile; }
    bool canSave(UiSyncItem*) const { return filename.file.exists() && (isOpened); }

public:
    static bool conformFile(const QFileInfo &file);
    static QStringList forbiddenDirs, allowedExtensions;
};

#endif // UIFILEITEM_H
