/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtTest module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtestfilelogger.h"
#include "qtestassert.h"
#include "QtTest/private/qtestlog_p.h"
#include "QtTest/private/qtestresult_p.h"

#include <stdlib.h>
#include <stdio.h>

QT_BEGIN_NAMESPACE

namespace QTest
{
    static FILE *stream = 0;
}

QTestFileLogger::QTestFileLogger()
{
}

QTestFileLogger::~QTestFileLogger()
{
    if(QTest::stream)
        fclose(QTest::stream);

    QTest::stream = 0;
}

void QTestFileLogger::init()
{
    char filename[100];
    QTest::qt_snprintf(filename, sizeof(filename), "%s.log",
                QTestResult::currentTestObjectName());

    #if defined(_MSC_VER) && _MSC_VER >= 1400 && !defined(Q_OS_WINCE)
    if (::fopen_s(&QTest::stream, filename, "wt")) {
        #else
        QTest::stream = ::fopen(filename, "wt");
        if (!QTest::stream) {
            #endif
            printf("Unable to open file for simple logging: %s", filename);
            ::exit(1);
        }
}

void QTestFileLogger::flush(const char *msg)
{
    QTEST_ASSERT(QTest::stream);

    ::fputs(msg, QTest::stream);
    ::fflush(QTest::stream);
}

QT_END_NAMESPACE
