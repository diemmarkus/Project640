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


#include "DkAudioProcessor.h"

//#include "DkUtils.h"
//#include "DkSettings.h"

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QDebug>
#include <QAudioRecorder>
#include <QUrl>
#pragma warning(pop)		// no warnings from includes - end

namespace p64 {

	DkAudioProcessor::DkAudioProcessor() {

		mRecorder = new QAudioRecorder;

		QAudioEncoderSettings audioSettings;
		audioSettings.setCodec("audio/amr");
		audioSettings.setQuality(QMultimedia::HighQuality);

		mRecorder->setEncodingSettings(audioSettings);

		// select recording device
		QString dname = findDevice();

		if (!dname.isEmpty())
			mRecorder->setAudioInput(findDevice());

		qDebug() << "Recording from:" << mRecorder->audioInput();

		mRecorder->setOutputLocation(QUrl::fromLocalFile("C:/temp/test.amr"));
		mRecorder->record();
	}

	DkAudioProcessor::~DkAudioProcessor() {
		mRecorder->stop();
	}

	QString DkAudioProcessor::findDevice(const QString& name) const {

		auto devices = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);

		QString vCable = "";
		QStringList dnames;

		for (auto cd : devices) {
			if (cd.deviceName().contains(name))
				vCable = cd.deviceName();

			dnames << cd.deviceName();
		}

		// by these means we can record sound using vCABLE: https://www.vb-audio.com/Cable/index.htm
		if (vCable.isEmpty()) {
			qDebug() << "could not find vCABLE - is it installed?";
			qDebug() << "available sound inputs are:" << dnames;
		}

		return vCable;
	}
}