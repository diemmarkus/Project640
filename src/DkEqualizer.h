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
#include <QObject>
#include <QMainWindow>
#pragma warning(pop)		// no warnings from includes - end


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

	class DkEqualizer;
	class DkAudioProcessor;
	class DkLcdController;

	class DllExport DkMainWindow : public QMainWindow {
			Q_OBJECT

	public:
		DkMainWindow(QWidget* parent = 0);

		void setComPort(const QString& cp);
		void start();

	private:
		void createLayout();

		DkAudioProcessor* mAudioProcessor = 0;
		DkLcdController* mProjector = 0;
	};

	class DkEqualizer : public QWidget {
		Q_OBJECT

	public:
		DkEqualizer(QWidget* parent = 0);

	public slots:
		void setLevel(double level);

	protected:
		void paintEvent(QPaintEvent* ev) override;

	private:
		double mLevel = 0;

	};

};