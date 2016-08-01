/*
* C++ Assembler Interpreter
* Copyright (C) 2016 Chair of Computer Architecture
* at Technical University of Munich
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

// This is a dummy class, which manages the syntax highlighter objects for
// different editor instances.
// ONLY FOR TESTING!

#ifndef INCLUDE_UI_EDITOR_QPROJECT_H
#define INCLUDE_UI_EDITOR_QPROJECT_H

#include <QObject>
#include <QQuickTextDocument>
#include <QRegExp>
#include <QTextCharFormat>
#include <QTextDocument>
#include <memory>
#include "ui/syntaxhighlighter.hpp"

class QProject : public QObject {
	Q_OBJECT
 public:
	explicit QProject(QObject *parent = 0);
	Q_INVOKABLE void createHighlighter(QQuickTextDocument *qDocument);

 private:
    std::unique_ptr<SyntaxHighlighter> _highlighter;
	std::vector<KeywordRule> _keywords;
 signals:

 public slots:
};

#endif// INCLUDE_UI_EDITOR_QPROJECT_H