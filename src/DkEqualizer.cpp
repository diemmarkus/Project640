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

#include "DkEqualizer.h"

//#include "DkUtils.h"
#include "DkAudioProcessor.h"
#include "DkLcdController.h"

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QPainter>
#pragma warning(pop)		// no warnings from includes - end

namespace p64 {

	DkMainWindow::DkMainWindow(QWidget* parent) : QMainWindow(parent) {

		// init
		mAudioProcessor = new DkAudioProcessor(this);
		mProjector = new DkLcdController(this);

		// show 'em what we got
		createLayout();

		connect(mAudioProcessor, SIGNAL(newLevel(double)), mProjector, SLOT(setLevel(double)));
	}

	void DkMainWindow::setComPort(const QString & cp) {
		mProjector->setComPort(cp);
	}

	void DkMainWindow::start() {
		mAudioProcessor->record();
		mProjector->start();
	}

	void DkMainWindow::createLayout() {

		DkEqualizer* eq = new DkEqualizer(this);

		setCentralWidget(eq);
		connect(mAudioProcessor, SIGNAL(newLevel(double)), eq, SLOT(setLevel(double)));
	}

	DkEqualizer::DkEqualizer(QWidget* parent) : QWidget(parent) {

	}

	void DkEqualizer::setLevel(double level) {
		mLevel = level;
		update();
	}

	void DkEqualizer::paintEvent(QPaintEvent * ev) {

		QRectF r = geometry();
		r.setWidth(r.width()*mLevel);

		QPainter p(this);
		p.setPen(Qt::NoPen);
		p.setBackground(QColor(0, 0, 0));
		p.setBrush(QColor(0,255,0));
		p.drawRect(r);

		QWidget::paintEvent(ev);
	}

}