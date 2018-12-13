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

#pragma once

#pragma warning(push, 0)	// no warnings from includes - begin
#include <QFileInfo>
#include <QVector>
#pragma warning(pop)		// no warnings from includes - end

#ifdef QT_NO_DEBUG_OUTPUT
#pragma warning(disable: 4127)		// no 'conditional expression is constant' if qDebug() messages are removed
#endif

#ifndef Q_OS_WIN
	#include <time.h>
#endif

//#define int64 long long;
#include <sstream>
#define DK_PI 3.141592653589793238462643383279

#ifndef DllExport
#ifdef DK_DLL_EXPORT
#define DllExport Q_DECL_EXPORT
#elif DK_DLL_IMPORT
#define DllExport Q_DECL_IMPORT
#else
#define DllExport
#endif
#endif

#ifdef WIN32
// fixes Qt's damn no latin1 on tr() policy
#define dk_degree_str QString::fromLatin1("°")
#else
#define dk_degree_str "\u00B0"
#endif

// Qt defines
class QColor;
class QUrl;

namespace p64 {

// nomacs defines
class TreeItem;

/**
 * This class contains general functions which are useful.
 **/
class DllExport DkUtils {

public:

	static bool compDateCreated(const QFileInfo& lhf, const QFileInfo& rhf);

	static bool compDateCreatedInv(const QFileInfo& lhf, const QFileInfo& rhf);

	static bool compDateModified(const QFileInfo& lhf, const QFileInfo& rhf);

	static bool compDateModifiedInv(const QFileInfo& lhf, const QFileInfo& rhf);

	static bool compRandom(const QFileInfo& lhf, const QFileInfo& rhf);

	static bool checkFile(const QFileInfo& file);
	static QFileInfo urlToLocalFile(const QUrl& url);
	static QString colorToString(const QColor& col);
	static QString readableByte(float bytes);
	static QStringList filterStringList(const QString& query, const QStringList& list);

	/**
	 * Appends an attribute name to the filename given.
	 * generates: image0001.tif -> img0001_mask.tif
	 * @param fName the filename with extension.
	 * @param ext the new file extension if it is "" the old extension is used.
	 * @param attribute the attribute which extends the filename.
	 * @return the generated filename.
	 **/
	static std::string createFileName(std::string fName, std::string attribute, std::string ext = "") {

		if (ext == "") ext = fName.substr(fName.length()-4, fName.length()); // use the old extension

		// generate: img0001.tif -> img0001_mask.tif
		return fName.substr(0, fName.length()-4) + attribute + ext;
	}

	static std::string removeExtension(std::string fName) {

		return fName.substr(0, fName.find_last_of("."));
	}

	static std::string getFileNameFromPath(std::string fName) {
		return fName.substr(fName.find_last_of("/")+1); //TODO: Schiach!!
	}

	/**
	 * Converts a number to a string.
	 * @throws an exception if number is not a number.
	 * @param number any number.
	 * @return a string representing the number.
	 **/
	template <typename numFmt>
	static std::string stringify(numFmt number) {

		std::stringstream stream;
		if (! (stream << number)) {
			std::string msg = "Sorry, I could not cast it to a string";
			//throw DkCastException(msg, __LINE__, __FILE__);
			printf("%s", msg.c_str());	// TODO: we need a solution for DkSnippet here...
		}

		return stream.str();
	}

	/**
	 * Converts a number to a string.
	 * @throws an exception if number is not a number.
	 * @param number any number.
	 * @param n the number of decimal places.
	 * @return a string representing the number.
	 **/
	template <typename numFmt>
	static std::string stringify(numFmt number, double n) {

		int rounded = qRound(number * std::pow(10,n));

		return stringify(rounded/std::pow(10,n));
	};

	static QDateTime getConvertableDate(const QString& date);
	static QDateTime convertDate(const QString& date, const QFileInfo& file = QFileInfo());
	static QString convertDateString(const QString& date, const QFileInfo& file = QFileInfo());
	static QString cleanFraction(const QString& frac);
	static QString resolveFraction(const QString& frac);
	static std::wstring qStringToStdWString(const QString &str);
	static QString stdWStringToQString(const std::wstring &str);

	static std::string stringTrim(const std::string str) {


		std::string strT = str;

		if (strT.length() <= 1) return strT;	// .empty() may result in errors

		// remove whitespace
		size_t b = strT.find_first_not_of(" ");
		size_t e = strT.find_last_not_of(" ");
		strT = strT.substr(b, e+1);

		if (strT.length() <= 1) return strT;	// nothing to trim left

		// remove tabs
		b = strT.find_first_not_of("\t");
		e = strT.find_last_not_of("\t");
		strT = strT.substr(b, e+1);

		return strT;
	};

	static std::string stringRemove(const std::string str, const std::string repStr) {

		std::string strR = str;

		if (strR.length() <= 1) return strR;

		size_t pos = 0;

		while ((pos = strR.find_first_of(repStr)) < strR.npos) {
			
			strR.erase(pos, repStr.length());
		}

		return strR;
	};

};

};
