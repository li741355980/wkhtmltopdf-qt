/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
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

#ifndef QBASICMOUSEEVENTTRANSITION_P_H
#define QBASICMOUSEEVENTTRANSITION_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/qabstracttransition.h>

#ifndef QT_NO_STATEMACHINE

#include <QtGui/qevent.h>

QT_BEGIN_NAMESPACE

class QPainterPath;

class QBasicMouseEventTransitionPrivate;
class Q_AUTOTEST_EXPORT QBasicMouseEventTransition : public QAbstractTransition
{
    Q_OBJECT
public:
    QBasicMouseEventTransition(QState *sourceState = 0);
    QBasicMouseEventTransition(QEvent::Type type, Qt::MouseButton button,
                               QState *sourceState = 0);
    ~QBasicMouseEventTransition();

    QEvent::Type eventType() const;
    void setEventType(QEvent::Type type);

    Qt::MouseButton button() const;
    void setButton(Qt::MouseButton button);

    Qt::KeyboardModifiers modifiersMask() const;
    void setModifiersMask(Qt::KeyboardModifiers modifiers);

    QPainterPath path() const;
    void setPath(const QPainterPath &path);

protected:
    bool eventTest(QEvent *event);
    void onTransition(QEvent *);

private:
    Q_DISABLE_COPY(QBasicMouseEventTransition)
    Q_DECLARE_PRIVATE(QBasicMouseEventTransition)
};

QT_END_NAMESPACE

#endif //QT_NO_STATEMACHINE

#endif