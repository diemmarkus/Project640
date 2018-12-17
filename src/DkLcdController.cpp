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

#include "DkLcdController.h"

#include "DkUtils.h"
#include "DkSettings.h"

#include <iostream>
#include <iomanip>

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QSettings>
#include <QDebug>
#include <QWidget>
#pragma warning(pop)		// no warnings from includes - end

namespace p64 {

// DkLcdController --------------------------------------------------------------------
DkLcdController::DkLcdController(QWidget* widget) : QThread(widget) {
	readSettings();
}

DkLcdController::~DkLcdController() {
	writeSettings();
}

bool DkLcdController::init() {

	DCB dcb;

	FillMemory(&dcb, sizeof(dcb), 0);
	if (!GetCommState(hCOM, &dcb)) {     // get current DCB
		qDebug() << "error in get COM state...";
		return false;
	}

	//qDebug() << "dcb defaults ------------------------";
	//printComParams(dcb);

	// set params for serial port communication
	dcb.BaudRate = CBR_1200;
	dcb.ByteSize = 8;
	//dcb.fParity = 1;
	dcb.Parity = ODDPARITY;
	dcb.StopBits = ONESTOPBIT;
	//dcb.EofChar = 0xFE;		// þ - should be that... (small letter thorn)
	//dcb.ErrorChar = 0x01;		// SOH (start of heading)

	//dcb.fDtrControl = DTR_CONTROL_ENABLE;
	//dcb.fTXContinueOnXoff = 1;
	//dcb.XoffLim = 512;
	//dcb.XonLim = 2048;

	// Set new state.
	if (!SetCommState(hCOM, &dcb)) {
		qDebug() << "cannot set com state!";
		printf("SetCommState failed with error %d.\n", GetLastError());
		return false;
	}

	if (!GetCommState(hCOM, &dcb)) {     // get current DCB
		qDebug() << "error in get COM state...";
		return false;
	}

	qDebug() << "\nDCB params ------------------------";
	printComParams(dcb);
	qDebug() << "DCB params ------------------------\n";

	// clear all operations that were performed _before_ we started...
	PurgeComm(hCOM, PURGE_RXCLEAR);

	return true;
} 

void DkLcdController::readSettings() {
	
	QSettings& settings = Settings::instance().getSettings();
	settings.beginGroup("DkLcdController");
	
	comPort = settings.value("comPort", comPort).toString();
	
	settings.endGroup();
	
}

void DkLcdController::writeSettings() const {

	QSettings& settings = Settings::instance().getSettings();
	settings.beginGroup("DkLcdController");
	
	settings.setValue("comPort", comPort);

	settings.endGroup();

	qDebug() << "[DkLcdController] settings written...";
}

void DkLcdController::startDebug(HANDLE hCom) const {

	const char* seq = "+";
	DWORD w = 1;
	WriteFile(hCom, seq, sizeof(byte), &w, NULL);
}

void DkLcdController::sendMessage(const std::string& msg) const {

	WriteFile(hCOM, msg.c_str(), (DWORD)msg.size(), NULL, NULL);
}

void DkLcdController::createImageSequence() const {

	//char img[168] = "g01";
	//char* dp = &img[3];


	//byte data[80];

	//for (int idx = 0; idx < 80; idx++) {

	//	byte val = idx % 2 ? 15 : 0;
	//	data[idx] = val;
	//}

	//std::string dataS = dataToHex(&data);


	////ushort cs = 0;

	////for (int idx = 0; idx < 160; idx++) {
	////	
	////	char val = idx % 2 ? 'F' : '0';
	////	dp[idx] = val;
	////	
	////	if (val == 'F')
	////		cs += 15;
	////	//if (val == '1')
	////	//	cs += 1;
	////}

	//std::string css = checksumToHex(cs);	// css = checksum string : )
	//qDebug() << "checksum:" << QString::fromStdString(css);

	//const char* csh = css.c_str();

	////QString result = QString::number(cs, 16).toUpper();
	////const char* csh = result.toStdString().c_str();

	//char* dpc = &img[163];
	//dpc[0] = css.size() > 3 ? csh[css.size() - 4] : '0';
	//dpc[1] = css.size() > 2 ? csh[css.size() - 3] : '0';
	//dpc[2] = css.size() > 1 ? csh[css.size() - 2] : '0';
	//dpc[3] = css.size() > 0 ? csh[css.size() - 1] : '0';

	////img += cs;

	//qDebug().noquote() << "sending:" << QString::fromStdString(img);

	//sendMessage(img);
}

std::string DkLcdController::checksumToHex(ushort checksum) const {

	std::stringstream ss;
	ss	<< std::setfill('0') << std::setw(sizeof(ushort))
		<< std::hex << std::uppercase << checksum;

	return ss.str();
}

std::string DkLcdController::dataToHex(byte * data, size_t size) const {

	std::stringstream ss;

	for (size_t idx = 0; idx < size; idx++) {
		qDebug() << "data:" << data[idx];
		ss << std::uppercase << std::hex << data[idx];
	}
	
	std::cout << "hexStr " << ss.str() << std::endl;

	return ss.str();
}

QString DkLcdController::readMessage(HANDLE hCom) const {
	
	QString msg;

	// TODO: make a usefull stop criterion
	for (;;) {

		Sleep(5);

		//if (!GetCommModemStatus(hCOM, modemStat))
		//	printf("error in getCommModemStatus...\n");

		//qDebug() << "sending...";

		byte b;
		DWORD r;
		ReadFile(hCom, &b, sizeof(byte), &r, NULL);

		if (r == 1)
			msg += b;
		//else if (!r)
		//	break;

		if (b == '\n')
			break;
	}

	qDebug() << "[LCD] says:" << msg;
	return msg;
}

void DkLcdController::run() {

	qDebug() << "starting thread...";
	
	std::wstring comPortStd = DkUtils::qStringToStdWString(comPort);
	hCOM = CreateFileW((LPCWSTR)comPortStd.c_str(), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hCOM == INVALID_HANDLE_VALUE) {
		qDebug() << "hCOM " << comPort << " is NULL....";
		return;
	}

	if (init())
		qDebug() << comPort << "is up and running...\n";


	//startDebug(hCOM);
	//readMessage(hCOM);

	sendMessage("j");	// reset
	
	// start debugging...
	//sendMessage("+");
	//readMessage(hCOM);
	//createImageSequence();

	sendMessage("~FF");	// turn on the light

	for (;;) {

		//Sleep(5);
		if (stop)
			break;

		if (mLevel != mOldLevel) {

			std::string lvl = checksumToHex(mLevel);

			//qDebug() << "sending: " << QString::fromStdString(lvl);

			sendMessage("~" + lvl);	// turn on the light
		}
		 
		//Sleep(5);
		sendMessage("~00");

	}
	
	//readMessage(hCOM);

}

void DkLcdController::printComParams(const DCB& dcb) const {

	qDebug() << "BaudRate" << dcb.BaudRate;
	qDebug() << "ByteSize"<< dcb.ByteSize;
	//qDebug() << "DCBlength" << dcb.DCBlength;
	//qDebug() << "EofChar" << dcb.EofChar;
	//qDebug() << "ErrorChar" << dcb.ErrorChar;
	//qDebug() << "EvtChar" << dcb.EvtChar;
	//qDebug() << "fAbortOnError" << dcb.fAbortOnError;
	//qDebug() << "fBinary" << dcb.fBinary;
	//qDebug() << "fDsrSensitivity" << dcb.fDsrSensitivity;
	//qDebug() << "fDtrControl" << dcb.fDtrControl;
	//qDebug() << "fDummy2" << dcb.fDummy2;
	//qDebug() << "fErrorChar" << dcb.fErrorChar;
	//qDebug() << "fInX" << dcb.fInX;
	//qDebug() << "fNull" << dcb.fNull;
	//qDebug() << "fOutX" << dcb.fOutX;
	//qDebug() << "fOutxCtsFlow" << dcb.fOutxCtsFlow;
	//qDebug() << "fOutxDsrFlow" << dcb.fOutxDsrFlow;
	//qDebug() << "fParity" << dcb.fParity;
	//qDebug() << "fRtsControl" << dcb.fRtsControl;
	//qDebug() << "fTXContinueOnXoff" << dcb.fTXContinueOnXoff;
	qDebug() << "Parity" << dcb.Parity;
	qDebug() << "StopBits" << dcb.StopBits;
	//qDebug() << "wReserved" << dcb.wReserved;
	//qDebug() << "wReserved1" << dcb.wReserved1;
	//qDebug() << "XoffChar" << dcb.XoffChar;
	//qDebug() << "XoffLim" << dcb.XoffLim;
	//qDebug() << "XonChar" << dcb.XonChar;
	//qDebug() << "XonLim" << dcb.XonLim;
}

void DkLcdController::setLevel(double level) {

	mOldLevel = mLevel;
	mLevel = (byte)(qMax(qMin(level*255*2, 255.0), 0.0));
}

void DkLcdController::serialValue(unsigned short val) const {

	unsigned short value = (val & 0x03ff);
	unsigned short controller = ((val & 0xfc00) >> 10) ;

	emit controllerSignal((int)controller, (int)value);
}

}