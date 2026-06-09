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

#include "uitheme.h"
#include <QApplication>
#include <QMap>
#include <QTimer>
#include <QStyleFactory>
#include <QStyle>
#include <QWidget>

bool UiTheme::lightTheme = false;

void UiTheme::apply(bool light) {
    // The native macOS style repaints toolbar/tab-bar/header backgrounds from
    // stale caches when the application stylesheet changes at runtime; the
    // software-drawn Fusion style restyles reliably.
    static bool styleInitialized = false;
    if(!styleInitialized) {
        styleInitialized = true;
        qApp->setStyle(QStyleFactory::create("Fusion"));
    }

    lightTheme = light;
    // Clear first: replacing one application stylesheet with another leaves
    // stale cached backgrounds on toolbars, tab bars and item view headers.
    // Dropping to an empty stylesheet forces a full teardown in between.
    qApp->setStyleSheet(QString());
    qApp->setStyleSheet(stylesheet(light));

    // Widgets carrying their own (structural) stylesheet do not re-resolve
    // the new application stylesheet for their subtree — re-set them.
    foreach(QWidget *widget, qApp->allWidgets())
        if(!widget->styleSheet().isEmpty())
            widget->setStyleSheet(widget->styleSheet());
}

QString UiTheme::stylesheet(bool light) {
    QMap<QString, QString> c;
    if(light) {
        c["TEXT_STRONG"]         = "rgb(25, 25, 25)";
        c["TEXT"]                = "rgb(50, 50, 50)";
        c["TEXT_HEADER"]         = "rgb(70, 70, 70)";
        c["TEXT_DIM"]            = "rgb(120, 120, 120)";
        c["WINDOW_BG"]           = "rgb(225, 224, 220)";
        c["SURFACE"]             = "rgb(242, 241, 237)";
        c["SURFACE_ALT"]         = "rgb(235, 234, 229)";
        c["CONTROL_BG"]          = "rgb(255, 255, 255)";
        c["CONTROL_ALT"]         = "rgb(228, 226, 220)";
        c["CONTROL_BORDER"]      = "rgb(190, 188, 180)";
        c["BORDER"]              = "rgb(206, 204, 197)";
        c["TAB_BG"]              = "rgb(209, 207, 200)";
        c["TAB_TEXT"]            = "rgb(70, 70, 70)";
        c["TAB_SEL_BG"]          = "rgb(252, 251, 247)";
        c["TAB_SEL_TEXT"]        = "black";
        c["TAB_DISABLED"]        = "rgb(155, 153, 147)";
        c["HEADER_BG"]           = "rgb(202, 200, 192)";
        c["TREE_BG"]             = "rgb(248, 247, 243)";
        c["TREE_ALT"]            = "rgb(240, 239, 234)";
        c["GRIDLINE"]            = "rgb(218, 216, 209)";
        c["SCROLL_TRACK"]        = "rgba(0, 0, 0, 28)";
        c["SCROLL_HANDLE"]       = "rgb(150, 148, 142)";
        c["SPLITTER"]            = "rgba(0, 0, 0, 30)";
        c["DOCK_TITLE"]          = "rgb(216, 214, 207)";
        c["TOOLBAR_BG"]          = "rgb(233, 232, 228)";
        c["TOOLBAR_BORDER"]      = "rgba(202, 202, 202, 255)";
        c["TOOLBTN_TEXT"]        = "black";
        c["TOOLBTN_PRESS"]       = "rgba(0, 0, 0, 20)";
        c["STATUS_BG"]           = "rgb(232, 231, 226)";
        c["ACCENT_HOVER_BORDER"] = "rgb(0, 150, 210)";
        c["ACCENT_HOVER_BG"]     = "rgb(173, 221, 242)";
        c["LOG_BG"]              = "rgb(242, 241, 237)";
        c["LOG_TEXT"]            = "black";
        c["TREE_BRANCH_CLOSED"]  = "url(:/items/res_tree_close_dark.png)";
        c["TREE_BRANCH_OPEN"]    = "url(:/items/res_tree_open_dark.png)";
        c["LOGO"]                = "url(:/general/res_logo_dark.png)";
        c["EDITOR_BG"]           = "#FDFCF8";
        c["EDITOR_TEXT"]         = "#1A1A1A";
        c["EDITOR_SCROLL"]       = "#C9C6BD";
    } else {
        c["TEXT_STRONG"]         = "white";
        c["TEXT"]                = "rgb(210, 210, 210)";
        c["TEXT_HEADER"]         = "rgb(200, 200, 200)";
        c["TEXT_DIM"]            = "rgb(140, 140, 140)";
        c["WINDOW_BG"]           = "black";
        c["SURFACE"]             = "rgb(40, 40, 40)";
        c["SURFACE_ALT"]         = "rgb(50, 50, 50)";
        c["CONTROL_BG"]          = "rgb(70, 70, 70)";
        c["CONTROL_ALT"]         = "rgb(60, 60, 60)";
        c["CONTROL_BORDER"]      = "rgb(55, 55, 55)";
        c["BORDER"]              = "rgb(50, 50, 50)";
        c["TAB_BG"]              = "rgb(110, 110, 110)";
        c["TAB_TEXT"]            = "rgb(220, 220, 220)";
        c["TAB_SEL_BG"]          = "rgb(125, 125, 125)";
        c["TAB_SEL_TEXT"]        = "white";
        c["TAB_DISABLED"]        = "gray";
        c["HEADER_BG"]           = "rgb(90, 90, 90)";
        c["TREE_BG"]             = "rgb(50, 50, 50)";
        c["TREE_ALT"]            = "rgb(55, 55, 55)";
        c["GRIDLINE"]            = "rgb(60, 60, 60)";
        c["SCROLL_TRACK"]        = "rgba(255, 255, 255, 32)";
        c["SCROLL_HANDLE"]       = "rgb(161, 161, 161)";
        c["SPLITTER"]            = "rgba(255, 255, 255, 30)";
        c["DOCK_TITLE"]          = "rgb(70, 70, 70)";
        c["TOOLBAR_BG"]          = "rgb(58, 58, 58)";
        c["TOOLBAR_BORDER"]      = "rgba(30, 30, 30, 255)";
        c["TOOLBTN_TEXT"]        = "white";
        c["TOOLBTN_PRESS"]       = "rgba(255, 255, 255, 45)";
        c["STATUS_BG"]           = "rgb(30, 30, 30)";
        c["ACCENT_HOVER_BORDER"] = "rgb(28, 124, 195)";
        c["ACCENT_HOVER_BG"]     = "rgb(21, 91, 143)";
        c["LOG_BG"]              = "rgb(35, 35, 35)";
        c["LOG_TEXT"]            = "rgb(220, 220, 220)";
        c["TREE_BRANCH_CLOSED"]  = "url(:/items/res_tree_close.png)";
        c["TREE_BRANCH_OPEN"]    = "url(:/items/res_tree_open.png)";
        c["LOGO"]                = "url(:/general/res_logo.png)";
        c["EDITOR_BG"]           = "#0C152B";
        c["EDITOR_TEXT"]         = "#FFFFFF";
        c["EDITOR_SCROLL"]       = "#2E3A5C";
    }
    c["ACCENT"] = "rgb(0, 187, 255)";

    QString qss = QString::fromUtf8(R"QSS(
QTabWidget, QLabel, QCheckBox, QLineEdit, QPlainTextEdit, QPushButton, QSpinBox, QDoubleSpinBox, QTreeView, QHeaderView, QTabBar, QComboBox, QFrame#globalFrame, QTabBar::tab, QDockWidget, QStatusBar, QRadioButton, QToolButton {
	font:        10px "Museo Sans", "Museo Sans 500", "Arial";
	padding:     0px;
	margin:      0px;
	min-height:  20px;
	color:       %TEXT_STRONG%;
	border:      0px solid black;
}
QPushButton, QSpinBox, QDoubleSpinBox, QLineEdit, QPlainTextEdit, QCheckBox::indicator, QRadioButton::indicator, QTreeView::indicator, QComboBox, QTreeView, QHeaderView {
	border:           1px solid %CONTROL_BORDER%;
	border-radius:    2px;
	background-color: %CONTROL_BG%;
}
QSpinBox, QDoubleSpinBox, QLineEdit, QPlainTextEdit, QCheckBox {
	margin-top:    4px;
	margin-bottom: 4px;
}

QFrame#globalFrame, QFrame#transportFrame, QFrame#timeFrame, QFrame#optionFrame, QFrame#speedFrame, QFrame#logoFrame, QFrame#perfFrame, QDialog {
	background-color: %SURFACE%;
}
QLabel, QCheckBox, QRadioButton {
	color: %TEXT%;
}

QTabWidget::pane {
	border: 1px solid %BORDER%;
}
QTabWidget::tab-bar {
	left: 0px;
}
QTabBar::tab {
	min-height:              25px;
	background-color:        %TAB_BG%;
	border-top-left-radius:  4px;
	border-top-right-radius: 4px;
	margin-right:            2px;
	padding-left:            5px;
	padding-right:           5px;
	color:                   %TAB_TEXT%;
}
QTabBar::tab:selected {
	background-color: %TAB_SEL_BG%;
	color:            %TAB_SEL_TEXT%;
}
QTabBar::tab:hover {
	color: %TAB_SEL_TEXT%;
}
QTabBar::tab:!selected {
	margin-top: 2px;
}
QTabBar::tab:disabled {
	color: %TAB_DISABLED%;
}

QPushButton {
	margin-left:   6px;
	padding-left:  3px;
	padding-right: 3px;
}
QPushButton:hover {
	border:           1px solid %ACCENT_HOVER_BORDER%;
	background-color: %ACCENT_HOVER_BG%;
}
QPushButton::checked, QPushButton:pressed, QCheckBox::indicator:checked, QTreeView::indicator:checked, QRadioButton::indicator:checked {
	background-color: %ACCENT%;
}

QCheckBox, QRadioButton {
	spacing: 5px;
}
QCheckBox::indicator, QTreeView::indicator, QRadioButton::indicator  {
	width:  10px;
	height: 10px;
}

QSpinBox, QDoubleSpinBox, QLineEdit {
	padding-left: 5px;
}
QSpinBox::up-button, QDoubleSpinBox::up-button, QSpinBox::down-button, QDoubleSpinBox::down-button {
	width:            5px;
	padding:          2px;
	padding-left:     3px;
	background-color: %CONTROL_ALT%;
	border:           0px solid black;
	border-left:      1px solid %CONTROL_BORDER%;
}
QSpinBox::up-button, QDoubleSpinBox::up-button {
	image:         url(:/icons/res_icon_plus.png);
	border-bottom: 1px solid %CONTROL_BORDER%;
}
QSpinBox::down-button, QDoubleSpinBox::down-button {
	image: url(:/icons/res_icon_minus.png);
}

QComboBox {
	background:      %CONTROL_BG%;
	color:           %TEXT_STRONG%;
	margin-left:     3px;
	border-radius:   3px;
	selection-color: black;
	padding-left:    5px;
}
QComboBox::drop-down {
	width:            10px;
	padding:          2px;
	border:           0px solid black;
	background-color: %CONTROL_ALT%;
	image:            url(:/icons/res_icon_down.png);
}

QTreeView {
	background:                  %TREE_BG%;
	alternate-background-color:  %TREE_ALT%;
	selection-background-color:  %ACCENT%;
	gridline-color:              %GRIDLINE%;
}
QHeaderView {
	background: transparent;
	border:     0px solid black;
	color:      %TEXT_HEADER%;
	min-height: 14px;
}
QHeaderView::section {
	background:              %HEADER_BG%;
	padding-left:            5px;
	border-top-left-radius:  3px;
	border-top-right-radius: 3px;
	margin-right:            1px;
	font:                    9px "Arial";
}
QTreeView::item {
	min-height: 20px;
}
QTreeView::branch:has-children:!has-siblings:closed, QTreeView::branch:closed:has-children:has-siblings {
	border-image: none;
	image:        %TREE_BRANCH_CLOSED%;
}
QTreeView::branch:open:has-children:!has-siblings, QTreeView::branch:open:has-children:has-siblings  {
	border-image: none;
	image:        %TREE_BRANCH_OPEN%;
}

QScrollBar:vertical, QScrollBar:horizontal {
	border:     0px solid black;
	background: %SCROLL_TRACK%;
}
QScrollBar::handle:vertical, QScrollBar::handle:horizontal {
	border:        0px solid black;
	background:    %SCROLL_HANDLE%;
	border-radius: 3px;
}
QScrollBar:vertical                { width:      8px; }
QScrollBar:horizontal              { height:     8px; }
QScrollBar::handle:vertical        { min-height: 8px; }
QScrollBar::handle:horizontal      { min-width:  8px; }
QScrollBar::add-line:vertical, QScrollBar::add-line:horizontal, QScrollBar::sub-line:vertical, QScrollBar::sub-line:horizontal, QScrollBar::up-arrow:vertical, QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:vertical, QScrollBar::down-arrow:horizontal {
	border:     0px solid black;
	background: transparent;
	width:      1px;
	height:     1px;
}
QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical, QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
	background: none;
}

QSlider:horizontal {
	min-width: 100px;
}
QSlider::groove:horizontal {
	border:           0px solid transparent;
	background-color: %CONTROL_BG%;
	height:           3px;
}
QSlider::handle:horizontal {
	width:            11px;
	height:           10px;
	border-radius:    5px;
	margin-top:       -4px;
	margin-bottom:    -4px;
	border:           0px solid black;
	background-color: %ACCENT%;
}

QSplitter::handle {
	background: %SPLITTER%;
}
QSplitter::handle:pressed {
	background: %ACCENT%;
}
QSplitter::handle:horizontal { width:  4px; }
QSplitter::handle:vertical   { height: 4px; }

QDockWidget::title {
	text-align:       center;
	background-color: %DOCK_TITLE%;
}

QToolBar {
	background: %TOOLBAR_BG%;
	border:     1px solid %TOOLBAR_BORDER%;
	spacing:    0px;
}
QToolButton {
	border:        0px solid black;
	color:         %TOOLBTN_TEXT%;
	border-radius: 3px;
	margin:        3px;
}
UiView QToolBar QToolButton {
	width:  22px;
	height: 22px;
}
QMainWindow::separator {
	background: %SURFACE%;
	width:      3px;
	height:     3px;
}
UiInspector QTabWidget, UiInspector QTabBar, UiInspector QTreeView, UiInspector QToolButton {
	icon-size: 14px;
}
QToolButton:pressed, QToolButton:checked {
	background-color: %TOOLBTN_PRESS%;
}
QToolButton#actionPlaySelected:pressed, QToolButton#actionPlaySelected:checked {
	background-color: red;
}
QToolButton#actionLockPos:pressed, QToolButton#actionLockPos:checked {
	background-color: red;
}

QStatusBar {
	color:            %TEXT%;
	border-top:       1px solid %BORDER%;
	background-color: %STATUS_BG%;
}

/* TRANSPORT */
QFrame#transportFrame, QFrame#timeFrame, QFrame#optionFrame, QFrame#speedFrame, QFrame#logoFrame, QFrame#perfFrame {
	border:      0px solid black;
	border-left: 1px solid %BORDER%;
}
QWidget#speedSlider {
	margin-top: 7px;
}
QWidget#timeEdit, QWidget#perfCpuEdit, QWidget#perfOpenGLEdit, QWidget#perfSchedulerEdit {
	background-color: transparent;
	border:           0px solid black;
	font:             9px;
	color:            %TEXT_DIM%;
}
QLineEdit#timeEdit {
	font:  40px "Lucida Sans", "Lucida Grande", "Lucida Sans Unicode", "Lucida";
	color: %ACCENT%;
}
QPushButton#logoButton {
	border:           0px solid black;
	background-color: transparent;
	margin:           -2px;
	qproperty-icon:   %LOGO%;
}
QPushButton:hover#logoButton {
	border:           1px solid %ACCENT_HOVER_BORDER%;
	background-color: %ACCENT_HOVER_BG%;
}
QPushButton#ffButton, QPushButton#playButton {
	min-width:        40px;
	min-height:       40px;
	border-radius:    20px;
	padding:          0px;
	border:           1px solid %CONTROL_BORDER%;
	background-color: %CONTROL_BG%;
}

/* INSPECTOR */
QWidget#ssTabInfoGeneral, QWidget#ssTabInfoSpace, QWidget#ssTabInfoTime, QWidget#ssTabInfoMessage, QWidget#ssTabConfigLog, QWidget#ssTabConfigNetwork, QWidget#ssTabConfigMIDI, QWidget#ssTabConfigArduino, QWidget#ssTabConfigSyphon, QWidget#ssTabConfigWeb, QWidget#ssTabRessource {
	background-color: %SURFACE_ALT%;
}
QWidget#tabProject, QWidget#tabView, QWidget#tabInfo, QWidget#tabControlCenter, QWidget#tabConfig {
	background-color: transparent;
}
QWidget#equationEdit {
	font: 10px "Lucida Console", "Monaco", "Monospace";
}

/* MAIN */
QWidget#centralwidget {
	background-color: %WINDOW_BG%;
}

/* DIALOG */
QDialogButtonBox QPushButton {
	padding-left:  10px;
	padding-right: 10px;
	min-height:    25px;
	margin:        5px;
}

/* SCRIPT EDITOR — colors are driven by JSEdit's palette and highlighter
   (UiEditor::applyEditorColors); keep the generic QPlainTextEdit rule from
   overriding them. */
JSEdit {
	background-color: %EDITOR_BG%;
	color:            %EDITOR_TEXT%;
	border:           0px solid black;
}
JSEdit QScrollBar:vertical, JSEdit QScrollBar:horizontal {
	background: %EDITOR_BG%;
}
JSEdit QScrollBar::handle:vertical, JSEdit QScrollBar::handle:horizontal {
	background: %EDITOR_SCROLL%;
}

/* MESSAGE LOG */
QWidget#logSend, QWidget#logReceive {
	font:             10px "Lucida Console", "Monaco", "Monospace";
	background-color: %LOG_BG%;
	color:            %LOG_TEXT%;
}

/* OSC PATTERN EDITOR */
ExtOscPatternEditor QLabel {
	font:  9px "Museo Sans", "Museo Sans 500", "Arial";
	color: %TEXT%;
}
ExtOscPatternEditor QFrame#global {
	background-color: %SURFACE_ALT%;
}
ExtOscPatternEditor QPlainTextEdit, ExtOscPatternEditor QComboBox {
	min-height:       20px;
	color:            %TEXT_STRONG%;
	border:           0px solid black;
	border-radius:    2px;
	background-color: %CONTROL_ALT%;
}
ExtOscPatternEditor QComboBox::drop-down {
	width:      10px;
	padding:    2px;
	border:     0px solid black;
	background: %ACCENT%;
	image:      url(:/icons/res_icon_down.png);
}
ExtOscPatternEditor QTextEdit#help {
	color:            %TEXT%;
	background-color: transparent;
}
)QSS");

    QMapIterator<QString, QString> colorIterator(c);
    while(colorIterator.hasNext()) {
        colorIterator.next();
        qss.replace("%" + colorIterator.key() + "%", colorIterator.value());
    }
    return qss;
}
