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

#include "DkUtils.h"
#include "DkMath.h"
#include "DkSettings.h"

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QString>
#include <QFileInfo>
#include <QDate>
#include <QRegExp>
#include <QStringList>
#include <QColor>
#include <QPixmap>
#include <QPainter>
#include <QFuture>
#include <QtConcurrentRun>
#include <QDir>
#include <QCoreApplication>
#include <QUrl>
#include <QStandardPaths>
#include <QApplication>
#pragma warning(pop)		// no warnings from includes - end


namespace p64 {

// DkUtils --------------------------------------------------------------------
bool DkUtils::compDateCreated(const QFileInfo& lhf, const QFileInfo& rhf) {

	return lhf.created() < rhf.created();
}

bool DkUtils::compDateCreatedInv(const QFileInfo& lhf, const QFileInfo& rhf) {

	return !compDateCreated(lhf, rhf);
}

bool DkUtils::compDateModified(const QFileInfo& lhf, const QFileInfo& rhf) {

	return lhf.lastModified() < rhf.lastModified();
}

bool DkUtils::compDateModifiedInv(const QFileInfo& lhf, const QFileInfo& rhf) {

	return !compDateModified(lhf, rhf);
}

bool DkUtils::compRandom(const QFileInfo&, const QFileInfo&) {

	return qrand() % 2 != 0;
}

bool DkUtils::checkFile(const QFileInfo& file) {

	return file.exists();
}

QFileInfo DkUtils::urlToLocalFile(const QUrl& url) {

	QUrl lurl = QUrl::fromUserInput(url.toString());

	// try manual conversion first, this fixes the DSC#josef.jpg problems (url fragments)
	QString fString = lurl.toString();
	fString = fString.replace("file:///", "");

	QFileInfo file = QFileInfo(fString);
	if (!file.exists())	// try an alternative conversion
		file = QFileInfo(lurl.toLocalFile());

	return file;
}

QDateTime DkUtils::getConvertableDate(const QString& date) {

	QDateTime dateCreated;
	QStringList dateSplit = date.split(QRegExp("[/: \t]"));

	if (date.count(":") != 4 /*|| date.count(QRegExp("\t")) != 1*/)
		return dateCreated;

	if (dateSplit.size() >= 3) {

		int y = dateSplit[0].toInt();
		int m = dateSplit[1].toInt();
		int d = dateSplit[2].toInt();

		if (y == 0 || m == 0 || d == 0)
			return dateCreated;

		QDate dateV = QDate(y, m, d);
		QTime time;

		if (dateSplit.size() >= 6)
			time = QTime(dateSplit[3].toInt(), dateSplit[4].toInt(), dateSplit[5].toInt());

		dateCreated = QDateTime(dateV, time);
	}

	return dateCreated;
}

QDateTime DkUtils::convertDate(const QString& date, const QFileInfo& file) {

	// convert date
	QDateTime dateCreated;
	QStringList dateSplit = date.split(QRegExp("[/: \t]"));

	if (dateSplit.size() >= 3) {

		QDate dateV = QDate(dateSplit[0].toInt(), dateSplit[1].toInt(), dateSplit[2].toInt());
		QTime time;

		if (dateSplit.size() >= 6)
			time = QTime(dateSplit[3].toInt(), dateSplit[4].toInt(), dateSplit[5].toInt());

		dateCreated = QDateTime(dateV, time);
	}
	else if (file.exists())
		dateCreated = file.created();

	return dateCreated;
};

QString DkUtils::convertDateString(const QString& date, const QFileInfo& file) {

	// convert date
	QString dateConverted;
	QStringList dateSplit = date.split(QRegExp("[/: \t]"));

	if (dateSplit.size() >= 3) {

		QDate dateV = QDate(dateSplit[0].toInt(), dateSplit[1].toInt(), dateSplit[2].toInt());
		dateConverted = dateV.toString(Qt::SystemLocaleShortDate);

		if (dateSplit.size() >= 6) {
			QTime time = QTime(dateSplit[3].toInt(), dateSplit[4].toInt(), dateSplit[5].toInt());
			dateConverted += " " + time.toString(Qt::SystemLocaleShortDate);
		}
	}
	else if (file.exists()) {
		QDateTime dateCreated = file.created();
		dateConverted += dateCreated.toString(Qt::SystemLocaleShortDate);
	}
	else
		dateConverted = "unknown date";

	return dateConverted;
}

QString DkUtils::colorToString(const QColor& col) {

	return "rgba(" + QString::number(col.red()) + "," + QString::number(col.green()) + "," + QString::number(col.blue()) + "," + QString::number((float)col.alpha()/255.0f*100.0f) + "%)";
}

QStringList DkUtils::filterStringList(const QString& query, const QStringList& list) {

	// white space is the magic thingy
	QStringList queries = query.split(" ");
	QStringList resultList = list;

	for (int idx = 0; idx < queries.size(); idx++) {
		resultList = resultList.filter(queries[idx], Qt::CaseInsensitive);
		qDebug() << "query: " << queries[idx];
	}

	// if string match returns nothing -> try a regexp
	if (resultList.empty()) {
		QRegExp regExp(query);
		resultList = list.filter(regExp);

		if (resultList.empty()) {
			regExp.setPatternSyntax(QRegExp::Wildcard);
			resultList = list.filter(regExp);
		}
	}

	return resultList;
}

QString DkUtils::readableByte(float bytes) {

	if (bytes >= 1024*1024*1024) {
		return QString::number(bytes/(1024.0f*1024.0f*1024.0f), 'f', 2) + " GB";
	}
	else if (bytes >= 1024*1024) {
		return QString::number(bytes/(1024.0f*1024.0f), 'f', 2) + " MB";
	}
	else if (bytes >= 1024) {
		return QString::number(bytes/1024.0f, 'f', 2) + " KB";
	}
	else {
		return QString::number(bytes, 'f', 2) + " B";
	}

}

QString DkUtils::cleanFraction(const QString& frac) {

	QStringList sList = frac.split('/');
	QString cleanFrac = frac;

	if (sList.size() == 2) {
		int nom = sList[0].toInt();		// nominator
		int denom = sList[1].toInt();	// denominator

		// if exposure time is less than a second -> compute the gcd for nice values (1/500 instead of 2/1000)
		if (nom != 0 && denom != 0) {
			int gcd = DkMath::gcd(denom, nom);
			cleanFrac = QString::number(nom/gcd);

			// do not show fractions like 9/1 -> it is more natural to write 9 in these cases
			if (denom/gcd != 1)
				 cleanFrac += QString("/") + QString::number(denom/gcd);

			qDebug() << frac << " >> " << cleanFrac;
		}
	}
	
	return cleanFrac;
}

QString DkUtils::resolveFraction(const QString& frac) {

	QString result = frac;
	QStringList sList = frac.split('/');

	if (sList.size() == 2) {
	
		bool nok = false;
		bool dok = false;
		int nom = sList[0].toInt(&nok);
		int denom = sList[1].toInt(&dok);

		if (nok && dok && denom)
			result = QString::number((double)nom/denom);
	}

	return result;
}

// code from: http://stackoverflow.com/questions/5625884/conversion-of-stdwstring-to-qstring-throws-linker-error
std::wstring DkUtils::qStringToStdWString(const QString &str) {
#ifdef _MSC_VER
	return std::wstring((const wchar_t *)str.utf16());
#else
	return str.toStdWString();
#endif
}

// code from: http://stackoverflow.com/questions/5625884/conversion-of-stdwstring-to-qstring-throws-linker-error
QString DkUtils::stdWStringToQString(const std::wstring &str) {
#ifdef _MSC_VER
	return QString::fromUtf16((const ushort *)str.c_str());
#else
	return QString::fromStdWString(str);
#endif
}

}
