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

#include "uieditor.h"
#include "ui_uieditor.h"
#include "misc/uitheme.h"

UiEditor::UiEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UiEditor) {
    ui->setupUi(this);
    toolbarButton = 0;
    firstLaunch = true;

    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.bottomRight().x() - rect().width(), 20);

    connect(ui->actionSave,        SIGNAL(triggered()), SLOT(save()));
    connect(ui->actionClose,       SIGNAL(triggered()), SLOT(close()));
    connect(ui->actionRefreshCode, SIGNAL(triggered()), SLOT(refresh()));

    ui->jsEditor->setTextWrapEnabled(false);
    ui->jsEditor->setLineNumbersVisible(true);
    ui->jsEditor->setCodeFoldingEnabled(true);
    ui->jsEditor->setBracketsMatchingEnabled(true);
    ui->jsEditor->setTabStopWidth(20);

    /*
    ui->jsEditor->setWindowTitle(QFileInfo(fileName).fileName());
    ui->jsEditor->setFrameShape(JSEdit::NoFrame);
    ui->jsEditor->setWordWrapMode(QTextOption::NoWrap);
    ui->jsEditor->setTabStopWidth(4);
    ui->jsEditor->resize(QApplication::desktop()->availableGeometry().size() / 2);
    */
    applyEditorColors();
    // Queued: UiTheme::apply() runs from another slot on this same signal and
    // may be invoked after us — defer so we read the settled theme state.
    connect(&Application::colorTheme, &UiBool::triggered, this, [this](bool) { applyEditorColors(); }, Qt::QueuedConnection);

    ui->splitter_2->setStretchFactor(0, 5);
    ui->splitter_2->setStretchFactor(1, 1);

    ui->statusBar->setVisible(false);
}

UiEditor::~UiEditor() {
    delete ui;
}

void UiEditor::applyEditorColors() {
    if(UiTheme::isLight()) {
        ui->jsEditor->setColor(JSEdit::Background,    QColor("#FDFCF8"));
        ui->jsEditor->setColor(JSEdit::Normal,        QColor("#1A1A1A"));
        ui->jsEditor->setColor(JSEdit::Comment,       QColor("#8A8A8A"));
        ui->jsEditor->setColor(JSEdit::Number,        QColor("#1A7A1A"));
        ui->jsEditor->setColor(JSEdit::String,        QColor("#9A2A1A"));
        ui->jsEditor->setColor(JSEdit::Operator,      QColor("#B25900"));
        ui->jsEditor->setColor(JSEdit::Identifier,    QColor("#1A1A1A"));
        ui->jsEditor->setColor(JSEdit::Keyword,       QColor("#0033AA"));
        ui->jsEditor->setColor(JSEdit::BuiltIn,       QColor("#007777"));
        ui->jsEditor->setColor(JSEdit::Sidebar,       QColor("#E8E6E0"));
        ui->jsEditor->setColor(JSEdit::LineNumber,    QColor("#909090"));
        ui->jsEditor->setColor(JSEdit::Cursor,        QColor("#F4EFD8"));
        ui->jsEditor->setColor(JSEdit::Marker,        QColor("#FFE94B"));
        ui->jsEditor->setColor(JSEdit::BracketMatch,  QColor("#B5E8E1"));
        ui->jsEditor->setColor(JSEdit::BracketError,  QColor("#FFB4B4"));
        ui->jsEditor->setColor(JSEdit::FoldIndicator, QColor("#AAAAAA"));
    } else {
        ui->jsEditor->setColor(JSEdit::Background,    QColor("#262626"));
        ui->jsEditor->setColor(JSEdit::Normal,        QColor("#DCDCDC"));
        ui->jsEditor->setColor(JSEdit::Comment,       QColor("#828282"));
        ui->jsEditor->setColor(JSEdit::Number,        QColor("#B5C78A"));
        ui->jsEditor->setColor(JSEdit::String,        QColor("#9DBF9E"));
        ui->jsEditor->setColor(JSEdit::Operator,      QColor("#C49A78"));
        ui->jsEditor->setColor(JSEdit::Identifier,    QColor("#DCDCDC"));
        ui->jsEditor->setColor(JSEdit::Keyword,       QColor("#D4C58A"));
        ui->jsEditor->setColor(JSEdit::BuiltIn,       QColor("#94ABBE"));
        ui->jsEditor->setColor(JSEdit::Sidebar,       QColor("#1F1F1F"));
        ui->jsEditor->setColor(JSEdit::LineNumber,    QColor("#7A7A7A"));
        ui->jsEditor->setColor(JSEdit::Cursor,        QColor("#363636"));
        ui->jsEditor->setColor(JSEdit::Marker,        QColor("#D4C58A"));
        ui->jsEditor->setColor(JSEdit::BracketMatch,  QColor("#3F5F5B"));
        ui->jsEditor->setColor(JSEdit::BracketError,  QColor("#6E3030"));
        ui->jsEditor->setColor(JSEdit::FoldIndicator, QColor("#555555"));
    }
    // Regenerate the commands/help pane HTML so its colors follow the theme.
    cursorChanged();
}

void UiEditor::setContent(const QString &content, bool raiseWindow) {
    //contents.replace("\t", "  ");
    quint16 cursorPos = ui->jsEditor->textCursor().position();
    quint16 scrollPos = ui->jsEditor->verticalScrollBar()->sliderPosition();
    ui->jsEditor->setPlainText(content);
    if((!isVisible()) && (raiseWindow)) {
        if(!firstLaunch) {
            show();
            Application::current->getMainWindow()->raise();
        }
        firstLaunch = false;
    }
    QTextCursor cursor = ui->jsEditor->textCursor();
    cursor.setPosition(cursorPos);
    ui->jsEditor->setTextCursor(cursor);
    ui->jsEditor->verticalScrollBar()->setSliderPosition(scrollPos);
}
const QString UiEditor::getContent() {
    return ui->jsEditor->toPlainText();
}

void UiEditor::cursorChanged() {
    ui->help->scriptHelp(ui->jsEditor, QStringList() << "commands" << "javascript" << "values");
}

void UiEditor::scriptError(const QStringList &errors, qint16 line) {
    if(line < 0)
        ui->statusBar->setVisible(false);
    else {
        QString errorsMessage = "";
        foreach(const QString & error, errors)
            errorsMessage += error + " - ";

        QTextCursor cursorLine = QTextCursor(ui->jsEditor->document()->findBlockByLineNumber(line-1));
        ui->jsEditor->setTextCursor(cursorLine);
        ui->statusBar->setVisible(true);
        ui->statusBar->showMessage(errorsMessage);
        raise();
    }
}


void UiEditor::changeEvent(QEvent *e) {
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::ActivationChange:
        refresh();
        break;
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void UiEditor::showEvent(QShowEvent *e) {
    if(toolbarButton)
        toolbarButton->setChecked(true);
    QMainWindow::showEvent(e);
    // The help pane skips rendering while hidden — refresh it now in case the
    // theme changed since the window was last shown.
    cursorChanged();
}
void UiEditor::closeEvent(QCloseEvent *e) {
    if(toolbarButton)
        toolbarButton->setChecked(false);
    QMainWindow::closeEvent(e);
}
