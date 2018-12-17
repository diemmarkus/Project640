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

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QThread>
#pragma warning(pop)		// no warnings from includes - end

#include <windows.h>	// needed to read from serial

#ifndef DllExport
#ifdef DK_DLL_EXPORT
#define DllExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllExport Q_DECL_IMPORT
#else
#define DllExport
#endif
#endif

namespace p64 {

class DllExport DkLcdController : public QThread {
	Q_OBJECT

public:
	DkLcdController(QWidget* widget = 0);
	~DkLcdController();

	void setComPort(const QString& cP) { comPort = cP; };

	void run();
	void quit() {
		stop = true;
	}
	
public slots:
	void setLevel(double level);

signals:
	void controllerSignal(int controller, int value) const;

protected:
	HANDLE hCOM;

	QString comPort = "COM5";
	byte mLevel = 0;
	byte mOldLevel = 0;
	bool stop = false;

	bool init();
	void serialValue(unsigned short val) const;
	void printComParams(const DCB& dcb) const;

	void readSettings();
	void writeSettings() const;

	void startDebug(HANDLE hCom) const;
	void sendMessage(const std::string& msg) const;
	void createImageSequence() const;

	std::string checksumToHex(ushort checksum) const;
	std::string dataToHex(byte* data, size_t size) const;

	QString readMessage(HANDLE hCom) const;

};

};