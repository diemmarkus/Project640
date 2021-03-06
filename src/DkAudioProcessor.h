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
#include <QAudioBuffer>
#include <QVector>
#pragma warning(pop)		// no warnings from includes - end

#pragma warning(disable: 4251)

#ifndef DllExport
#ifdef DK_DLL_EXPORT
#define DllExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllExport Q_DECL_IMPORT
#else
#define DllExport
#endif
#endif

class QAudioRecorder;
class QAudioBuffer;

namespace p64 {

class DllExport DkAudioProcessor : public QObject {
	Q_OBJECT

public:
	DkAudioProcessor(QObject* parent = 0);
	~DkAudioProcessor();

	void record(bool start = true);

signals:
	void newLevel(double level) const;

protected slots:
	void processBuffer(QAudioBuffer buffer);

private:
	QString findDevice(const QString& name = "CABLE Output") const;
	double getLevel(const QAudioBuffer& buffer, int channelIdx) const;

	template <class T>
	double getMaxLevel(const T *buffer, int numFrames, int channelIdx) const {
		
		double ml = 0;

		for (int idx = 0; idx < numFrames; idx++) {
			double val = qAbs((double)buffer[idx * channelIdx]);
			if (val > ml)
				ml = val;
		}

		return ml;
	};

	QAudioRecorder* mRecorder = 0;
	QVector<double> mLevelSeries;
};

};