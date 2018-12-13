/*******************************************************************************************************
 
 main.cpp
 Created on:	19.12.2015
 
 Pong is a homage to the famous arcade game Pong with the capability of old-school controllers using an Arduino Uno board. 

 Copyright (C) 2015-2016 Markus Diem <markus@nomacs.org>

 This file is part of Pong.

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


#pragma warning(push, 0)	// no warnings from includes
#include <QApplication>
#include <QCommandLineParser>
#pragma warning(pop)

#include "DkSettings.h"
#include "DkEqualizer.h"

int main(int argc, char** argv) {
	
	QCoreApplication::setOrganizationName("TU Wien");
	QCoreApplication::setOrganizationDomain("https://www.nomacs.org");
	QCoreApplication::setApplicationName("Project640");

	QApplication app(argc, argv);

	// CMD parser --------------------------------------------------------------------
	QCommandLineParser parser;

	//parser.setApplicationDescription("Test helper");
	parser.addHelpOption();
	parser.addVersionOption();
	//parser.addPositionalArgument("image", QObject::tr("An input image."));

	//// fullscreen (-f)
	//QCommandLineOption fullScreenOpt(QStringList() << "f" << "fullscreen", QObject::tr("Start in fullscreen."));
	//parser.addOption(fullScreenOpt);

	// set com port
	QCommandLineOption comOpt(QStringList() << "c" << "comport",
		QObject::tr("Open  com<com> port."),
		QObject::tr("com"));
	parser.addOption(comOpt);

	parser.process(app);
	// CMD parser --------------------------------------------------------------------


	//p64::DkEqualizer* eq = new p64::DkEqualizer();

	//// set COM port
	//if (!parser.value(comOpt).isEmpty()) {
	//	eq->setComPort(parser.value(comOpt));
	//}

	//eq->start();

	//// run pong
	//int rVal = app.exec();

	//delete eq;

	//return rVal;

	return 0;

}