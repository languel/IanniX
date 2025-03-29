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

#include "options.h"

QList<UiOption*> UiOptions::options;

UiOption::UiOption()
    : QObject() {
    syncItem = 0;
}

UiBool::UiBool(bool _value) :
    UiOption() {
    value          = _value;
    toolbarButton = 0;
    checkBox      = 0;
    button        = 0;
}
UiBool::UiBool(const UiBool& _value) :
    UiOption() {
    value = _value.value;
    toolbarButton = 0;
    checkBox      = 0;
    button        = 0;
}
UiBool& UiBool::operator= (const UiBool &_value) {
    value = _value.value;
    applyToGui();
    emit(triggered(value));
    return *this;
}
UiBool& UiBool::operator= (bool _value) {
    value = _value;
    applyToGui();
    emit(triggered(value));
    return *this;
}
void UiBool::applyToGui() {
    if((toolbarButton) && (((value) && (!toolbarButton->isChecked())) || ((!value) && (toolbarButton->isChecked()))))
        toolbarButton->setChecked(value);
    if((checkBox) && (((value) && (!checkBox->isChecked())) || ((!value) && (checkBox->isChecked()))))
        checkBox->setChecked(value);
    if((button) && (((value) && (!button->isChecked())) || ((!value) && (button->isChecked()))))
        button->setChecked(value);
    if(syncItem)
        syncItem->dataChanged();
}

UiBool::operator bool() const {     return value;    }
bool UiBool::val()      const {     return value;    }
void UiBool::setAction(QAction *_toolbarButton, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(toolbarButton)
        toolbarButton->disconnect(this, SLOT(guiTrigged(bool)));
    toolbarButton = _toolbarButton;
    if(toolbarButton) {
        if(changeUi)
            applyToGui();
        toolbarButton->connect(toolbarButton, SIGNAL(triggered(bool)), this, SLOT(guiTrigged(bool)));
        if(trigEvent)
            guiTrigged(toolbarButton->isChecked());
    }
}
void UiBool::setAction(QCheckBox *_checkBox, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(checkBox)
        checkBox->disconnect(this, SLOT(guiTrigged(bool)));
    checkBox = _checkBox;
    if(checkBox) {
        if(changeUi)
            applyToGui();
        checkBox->connect(checkBox, SIGNAL(toggled(bool)), this, SLOT(guiTrigged(bool)));
        if(trigEvent)
            guiTrigged(checkBox->isChecked());
    }
}
void UiBool::setAction(QPushButton *_button, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(button)
        button->disconnect(this, SLOT(guiTrigged(bool)));
    button = _button;
    if(button) {
        if(changeUi)
            applyToGui();
        button->connect(button, SIGNAL(toggled(bool)), this, SLOT(guiTrigged(bool)));
        if(trigEvent)
            guiTrigged(button->isChecked());
    }
}
void UiBool::guiTrigged(bool _value) {
    value = _value;
    if(syncItem)
        syncItem->dataChanged();
    emit(triggered(value));
}








UiReal::UiReal(qreal _value) :
    UiOption() {
    value         = _value;
    spinBox       = 0;
    doubleSpinBox = 0;
    comboBox      = 0;
    slider        = 0;
    radios.clear();
}
UiReal::UiReal(const UiReal& _value) :
    UiOption() {
    value = _value.value;
    spinBox       = 0;
    doubleSpinBox = 0;
    comboBox      = 0;
    slider        = 0;
    radios.clear();
}
UiReal& UiReal::operator= (const UiReal &_value) {
    value = _value.value;
    applyToGui();
    emit(triggered(value));
    return *this;
}
UiReal& UiReal::operator= (qreal _value) {
    value = _value;
    applyToGui();
    emit(triggered(value));
    return *this;
}
void UiReal::applyToGui() {
    if((spinBox) && (spinBox->value() != value))
        spinBox->setValue(value);
    if((doubleSpinBox) && (doubleSpinBox->value() != value))
        doubleSpinBox->setValue(value);
    if((comboBox) && (comboBox->currentIndex() != value))
        comboBox->setCurrentIndex(value);
    if((slider) && (slider->value() != value))
        slider->setValue(value);
    if((radios.count()) && (value < radios.count()))
        radios.at(value)->setChecked(true);
    if(syncItem)
        syncItem->dataChanged();
}
UiReal::operator qreal() const {     return value;    }
qreal UiReal::val()      const {     return value;    }
void UiReal::setAction(QSpinBox *_spinBox, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(spinBox)
        spinBox->disconnect(this, SLOT(guiTrigged(int)));
    spinBox = _spinBox;
    if(spinBox) {
        if(changeUi)
            applyToGui();
        spinBox->connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(guiTrigged(int)));
        if(trigEvent)
            guiTrigged(spinBox->value());
    }
}
void UiReal::setAction(QDoubleSpinBox *_doubleSpinBox, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(doubleSpinBox)
        doubleSpinBox->disconnect(this, SLOT(guiTrigged(qreal)));
    doubleSpinBox = _doubleSpinBox;
    if(doubleSpinBox) {
        if(changeUi)
            applyToGui();
        doubleSpinBox->connect(doubleSpinBox, SIGNAL(valueChanged(double)), this, SLOT(guiTrigged(double)));
        if(trigEvent)
            guiTrigged(doubleSpinBox->value());
    }
}
void UiReal::setAction(QSlider *_slider, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(slider)
        slider->disconnect(this, SLOT(guiTrigged(qreal)));
    slider = _slider;
    if(slider) {
        if(changeUi)
            applyToGui();
        slider->connect(slider, SIGNAL(valueChanged(int)), this, SLOT(guiTrigged(int)));
        if(trigEvent)
            guiTrigged(slider->value());
    }
}
void UiReal::setAction(QComboBox *_comboBox, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(comboBox)
        comboBox->disconnect(this, SLOT(guiTrigged(qreal)));
    comboBox = _comboBox;
    if(comboBox) {
        if(changeUi)
            applyToGui();
        comboBox->connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(guiTrigged(int)));
        if(trigEvent)
            guiTrigged(comboBox->currentIndex());
    }
}
void UiReal::setAction(const QList<QRadioButton*> &_radios, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    foreach(QRadioButton *radio, radios)
        radio->disconnect(this, SLOT(guiTrigged(qreal)));
    radios = _radios;
    if(radios.count()) {
        if(changeUi)
            applyToGui();
        foreach(QRadioButton *radio, radios)
            radio->connect(radio, SIGNAL(toggled(bool)), this, SLOT(guiTrigged(bool)));

        if(trigEvent) {
            qreal tmpVal = 0;
            for(quint16 i = 0 ; i < radios.count() ; i++)
                if(radios.at(i)->isChecked()) {
                    tmpVal = i;
                    break;
                }
            guiTrigged(tmpVal);
        }
    }
}

void UiReal::guiTrigged(double _value) {
    value = _value;
    if(syncItem)
        syncItem->dataChanged();
    emit(triggered(value));
}
void UiReal::guiTrigged(int _value) {
    value = _value;
    if(syncItem)
        syncItem->dataChanged();
    emit(triggered(value));
}
void UiReal::guiTrigged(bool) {
    value = 0;
    for(quint16 i = 0 ; i < radios.count() ; i++)
        if(radios.at(i)->isChecked()) {
            value = i;
            break;
        }
    if(syncItem)
        syncItem->dataChanged();
    emit(triggered(value));
}




UiString::UiString(const QString &_value) :
    UiOption() {
    value         = _value;
    edit          = 0;
    spin          = 0;
    combo         = 0;
    plainTextEdit = 0;
}
UiString::UiString(const UiString &_value) :
    UiOption() {
    value = _value.value;
    edit          = 0;
    spin          = 0;
    combo         = 0;
    plainTextEdit = 0;
}
UiString& UiString::operator= (const UiString &_value) {
    value = _value.value;
    applyToGui();
    emit(triggered(value));
    return *this;
}
UiString& UiString::operator= (const QString &_value) {
    value = _value;
    applyToGui();
    emit(triggered(value));
    return *this;
}
void UiString::applyToGui() {
    if((edit) && (edit->text() != value))
        edit->setText(value);
    if((spin) && (QString::number(spin->value()) != value))
        spin->setValue(value.toInt());
    if((plainTextEdit) && (plainTextEdit->toPlainText() != value))
        plainTextEdit->setPlainText(value);
    if(combo)
        combo->setCurrentIndex(combo->findText(value, Qt::MatchFixedString));
    if(syncItem)
        syncItem->dataChanged();
}

UiString::operator QString() const {     return value;    }
QString UiString::val()      const {     return value;    }
void UiString::setAction(QSpinBox *_spin, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(spin)
        spin->disconnect(this, SLOT(guiTrigged(QString)));
    spin = _spin;
    if(spin) {
        if(changeUi)
            applyToGui();
        spin->connect(spin, SIGNAL(valueChanged(QString)), this, SLOT(guiTrigged(QString)));
        if(trigEvent)
            guiTrigged();
    }
}
void UiString::setAction(QLineEdit *_edit, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(edit)
        edit->disconnect(this, SLOT(guiTrigged(QString)));
    edit = _edit;
    if(edit) {
        if(changeUi)
            applyToGui();
        edit->connect(edit, SIGNAL(textChanged(QString)), this, SLOT(guiTrigged(QString)));
        if(trigEvent)
            guiTrigged();
    }
}
void UiString::setAction(QPlainTextEdit *_plainTextEdit, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(plainTextEdit)
        plainTextEdit->disconnect(this, SLOT(guiTrigged()));
    plainTextEdit = _plainTextEdit;
    if(plainTextEdit) {
        if(changeUi)
            applyToGui();
        plainTextEdit->connect(plainTextEdit, SIGNAL(textChanged()), this, SLOT(guiTrigged()));
        if(trigEvent)
            guiTrigged();
    }
}
void UiString::setAction(QComboBox *_combo, const QString &_settingName, bool trigEvent, bool changeUi) {
    UiOptions::add(this, _settingName);
    if(combo)
        combo->disconnect(this, SLOT(guiTrigged(QString)));
    combo = _combo;
    if(combo) {
        if(changeUi)
            applyToGui();
        combo->connect(combo, SIGNAL(currentIndexChanged(QString)), this, SLOT(guiTrigged(QString)));
        combo->connect(combo, SIGNAL(editTextChanged(QString)),     this, SLOT(guiTrigged(QString)));
        if(trigEvent)
            guiTrigged();
    }
}
void UiString::guiTrigged() {
    if(edit)
        value = edit->text();
    if(spin)
        value = QString::number(spin->value());
    if(plainTextEdit)
        value = plainTextEdit->toPlainText();
    if(combo)
        value = combo->currentText();
    if(syncItem)
        syncItem->dataChanged();
    emit(triggered(value));
}
void UiString::guiTrigged(QString _value) {
    value = _value;
    if(syncItem)
        syncItem->dataChanged();
    emit(triggered(value));
}



UiFile::UiFile(const QString &_value) :
    UiString(_value) {
}
UiFile::UiFile(const UiFile &_value) :
    UiString(_value.value) {
}
UiFile::UiFile(const QFileInfo &_value) :
    UiString() {
    file = _value;
    value = file.completeBaseName();
}
UiFile& UiFile::operator= (const UiFile &_value) {
    UiString::operator =(_value.value);
    return *this;
}
UiFile& UiFile::operator= (const QString &_value) {
    UiString::operator =(_value);
    return *this;
}
UiFile& UiFile::operator= (const QFileInfo &_value) {
    file = _value;
    UiString::operator =(file.completeBaseName());
    return *this;
}
UiFile::operator QString() const {     return value;    }

