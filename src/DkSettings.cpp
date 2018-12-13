/*******************************************************************************************************
 Project640 is an equalizer app that removes the dust from our old-school LCD-640 Linienprojektor.

 Copyright (C) 2018 Markus Diem <diem@cvl.tuwien.ac.at>
 Copyright (C) 2018 Fabian Hollaus <holl@cvl.tuwien.ac.at>
 Copyright (C) 2018 Florian Kleber <kleber@cvl.tuwien.ac.at>

 This file is part of Project640.

 Pong is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Pong is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************************************/


#include "DkSettings.h"
#include "DkUtils.h"

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QImageReader>
#include <QDesktopServices>
#include <QTranslator>
#include <QFileInfo>
#include <QDebug>
#include <QTableView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QDir>
#include <QApplication>

#ifdef WIN32
#include "Shobjidl.h"
#endif
#pragma warning(pop)		// no warnings from includes - end

namespace p64 {

// settings
DkSettings::App DkSettings::app_p;
DkSettings::Display DkSettings::display_p;

// load settings
DkSettings::App DkSettings::app_d;
DkSettings::Display DkSettings::display_d;

DkSettings::App& DkSettings::app = DkSettings::getAppSettings();
DkSettings::Display& DkSettings::display = DkSettings::getDisplaySettings();

DkSettings::App& DkSettings::getAppSettings() {
	return app_p;
}

DkSettings::Display& DkSettings::getDisplaySettings() {
	return display_p;
}

void DkSettings::load() {

	setToDefaultSettings();

	QSettings& settings = Settings::instance().getSettings();
	qDebug() << "loading settings from: " << settings.fileName();

	settings.beginGroup("AppSettings");
	
	app_p.closeOnEsc = settings.value("closeOnEsc", app_p.closeOnEsc).toBool();
	
	settings.endGroup();

	// Display Settings --------------------------------------------------------------------
	settings.beginGroup("DisplaySettings");

	display_p.highlightColor = QColor::fromRgba(settings.value("highlightColorRGBA", display_p.highlightColor.rgba()).toInt());
	display_p.bgColorWidget = QColor::fromRgba(settings.value("bgColorWidgetRGBA", display_p.bgColorWidget.rgba()).toInt());
	display_p.fontColor = QColor::fromRgba(settings.value("fontColorRGBA", display_p.fontColor.rgba()).toInt());
	display_p.bgColor = QColor::fromRgba(settings.value("bgColorNoMacsRGBA", display_p.bgColor.rgba()).toInt());
	display_p.iconColor = QColor::fromRgba(settings.value("iconColorRGBA", display_p.iconColor.rgba()).toInt());

	settings.endGroup();

	// keep loaded settings in mind
	app_d = app_p;
	display_d = display_p;
}

void DkSettings::save(bool force) {
		
	QSettings& settings = Settings::instance().getSettings();
	
	settings.beginGroup("AppSettings");

	if (!force && app_p.closeOnEsc != app_d.closeOnEsc)
		settings.setValue("closeOnEsc", app_p.closeOnEsc);

	settings.endGroup();

	// Display Settings --------------------------------------------------------------------
	settings.beginGroup("DisplaySettings");

	if (!force && display_p.highlightColor != display_d.highlightColor)
		settings.setValue("highlightColorRGBA", display_p.highlightColor.rgba());
	if (!force && display_p.bgColorWidget != display_d.bgColorWidget)
		settings.setValue("bgColorWidgetRGBA", display_p.bgColorWidget.rgba());
	if (!force && display_p.fontColor != display_d.fontColor)
		settings.setValue("fontColorRGBA", display_p.fontColor.rgba());
	if (!force && display_p.bgColor != display_d.bgColor)
		settings.setValue("bgColorNoMacsRGBA", display_p.bgColor.rgba());
	if (!force && display_p.iconColor != display_d.iconColor)
		settings.setValue("iconColorRGBA", display_p.iconColor.rgba());

	settings.endGroup();

	// keep loaded settings in mind
	app_d = app_p;
	display_d = display_p;

	qDebug() << "settings saved";
}

void DkSettings::setToDefaultSettings() {

	app_p.closeOnEsc = false;

	display_p.highlightColor = QColor(0, 204, 255);
	display_p.bgColorWidget = QColor(0, 0, 0, 100);
	display_p.fontColor = QColor(255, 255, 255);
	display_p.bgColor = QColor(100, 100, 100, 255);
	display_p.iconColor = QColor(100,100,100,255);

	qDebug() << "ok... default settings are set";
}

bool DkSettings::isPortable() {

	QFileInfo settingsFile = getSettingsFile();
	return settingsFile.isFile() && settingsFile.exists();
}

QFileInfo DkSettings::getSettingsFile() {

	return QFileInfo(QCoreApplication::applicationDirPath(), "settings.nfo");
}

Settings::Settings() {
	m_settings = DkSettings::isPortable() ? QSharedPointer<QSettings>(new QSettings(DkSettings::getSettingsFile().absoluteFilePath(), QSettings::IniFormat)) : QSharedPointer<QSettings>(new QSettings());
	qDebug() << "portable nomacs: " << DkSettings::isPortable();
}

Settings& Settings::instance() { 
	static QSharedPointer<Settings> inst;
	if (!inst)
		inst = QSharedPointer<Settings>(new Settings());
	return *inst; 
}

QSettings& Settings::getSettings() {
	//QMutexLocker(&mutex);
	return *m_settings;
}

}
