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

#include "uimessagebox.h"
#include "ui_uimessagebox.h"
#include "misc/application.h"

UiMessageBox::UiMessageBox(QWidget *parent) :
    QDialog(parent, Qt::Tool),
    ui(new Ui::UiMessageBox) {
    ui->setupUi(this);
    setStyleSheet(Application::current->getMainWindow()->styleSheet());
}

UiMessageBox::~UiMessageBox() {
    delete ui;
}

QPair<bool, bool> UiMessageBox::getCheckboxes() {
    return qMakePair(ui->checkSmooth->isChecked(), ui->checkTrigger->isChecked());
}


qreal UiMessageBox::getDouble(const QString &title, const QString &description, qreal value, qreal min, qreal max, qreal step, quint16 decimals, const QString &suffix, bool *ok) {
    setUpdatesEnabled(false);
    ui->choices->setCurrentIndex(0);
    ui->spinLabel->setText(description);
    setWindowTitle(title);
    ui->spinSpin->setMinimum(min);
    ui->spinSpin->setMaximum(max);
    ui->spinSpin->setDecimals(decimals);
    ui->spinSpin->setSingleStep(step);
    ui->spinSpin->setValue(value);
    ui->spinSpin->setSuffix(" " + suffix);
    ui->spinSpin->setFocus();
    setUpdatesEnabled(true);

    show();
    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - QPoint(200, 100));

    exec();
    if(ok) {
        if(result())    *ok = true;
        else            *ok = false;
    }
    qreal ret = ui->spinSpin->value();
    delete this;
    return ret;
}
qreal UiMessageBox::getDouble(const QString &title, const QString &description, const QPixmap &pixmap, qreal value, qreal min, qreal max, qreal step, quint16 decimals, const QString &suffix, bool resample, bool *ok) {
    setUpdatesEnabled(false);
    ui->choices->setCurrentIndex(1);
    ui->checkSmooth->setVisible(resample);
    ui->checkTrigger->setVisible(resample);
    ui->spinpLabel->setText(description);
    setWindowTitle(title);
    ui->spinpSpin->setMinimum(min);
    ui->spinpSpin->setMaximum(max);
    ui->spinpSpin->setDecimals(decimals);
    ui->spinpSpin->setSingleStep(step);
    ui->spinpSpin->setValue(value);
    ui->spinpPicture->resize(pixmap.size()/2);
    ui->spinpPicture->setMinimumSize(pixmap.size()/2);
    ui->spinpPicture->setPixmap(pixmap);
    ui->spinpPicture->setScaledContents(true);
    ui->spinpSpin->setSuffix(" " + suffix);
    ui->spinpSpin->setFocus();
    setUpdatesEnabled(true);

    updateGeometry();
    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - QPoint(200, 100));

    exec();
    if(ok) {
        if(result())    *ok = true;
        else            *ok = false;
    }
    qreal ret = ui->spinpSpin->value();
    if(!resample)
        delete this;
    return ret;
}
int UiMessageBox::display(const QString &title, const QString &description, QDialogButtonBox::StandardButtons buttons, bool *ok) {
    setUpdatesEnabled(false);
    ui->choices->setCurrentIndex(2);
    ui->messageLabel->setText(description);
    ui->messageButtons->setStandardButtons(buttons);
    setWindowTitle(title);
    setUpdatesEnabled(true);

    updateGeometry();
    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - QPoint(200, 100));

    exec();
    if(ok) {
        if(result())    *ok = true;
        else            *ok = false;
    }
    int ret = result();
    delete this;
    return ret;
}
void UiMessageBox::display(const QString &title, const QString &description) {
    setUpdatesEnabled(false);
    ui->choices->setCurrentIndex(2);
    ui->messageLabel->setText(description);
    ui->messageButtons->setStandardButtons(QDialogButtonBox::Ok);
    setWindowTitle(title);
    setUpdatesEnabled(true);

    updateGeometry();
    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - QPoint(200, 100));
    show();
    raise();
}
QString UiMessageBox::getText(const QString &title, const QString &description, const QString &value, bool *ok) {
    setUpdatesEnabled(false);
    ui->choices->setCurrentIndex(3);
    ui->textLabel->setText(description);
    setWindowTitle(title);
    ui->textEdit->setText(value);
    setUpdatesEnabled(true);

    updateGeometry();
    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - QPoint(200, 100));

    exec();
    if(ok) {
        if(result())    *ok = true;
        else            *ok = false;
    }
    QString ret = ui->textEdit->text();
    delete this;
    return ret;
}

int UiMessageBox::display(const QString &title, const QString &description1, const QString &description2, const QString &description3, const QPixmap &pixmap, QDialogButtonBox::StandardButtons buttons, bool *ok) {
    setUpdatesEnabled(false);
    ui->choices->setCurrentIndex(4);
    ui->updateLabel->setText(description1);
    ui->updatePlain->setMinimumSize(0, 200);
    ui->updatePlain->setPlainText(description2);
    ui->updateLabel2->setText(description3);
    ui->updatePicture->setMinimumSize(pixmap.size()/2);
    ui->updatePicture->resize(pixmap.size()/2);
    ui->updatePicture->setPixmap(pixmap);
    ui->updatePicture->setScaledContents(true);
    ui->messageButtons->setStandardButtons(buttons);
    setWindowTitle(title);
    setUpdatesEnabled(true);

    updateGeometry();
    QRect screen = QApplication::desktop()->screenGeometry();
    move(screen.center() - QPoint(200, 300));

    exec();
    if(ok) {
        if(result())    *ok = true;
        else            *ok = false;
    }
    int ret = result();
    delete this;
    return ret;
}
