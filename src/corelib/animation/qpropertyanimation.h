/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#ifndef QPROPERTYANIMATION_H
#define QPROPERTYANIMATION_H

#include <QtCore/qvariantanimation.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Core)

#ifndef QT_NO_ANIMATION

class QPropertyAnimationPrivate;
class Q_CORE_EXPORT QPropertyAnimation : public QVariantAnimation
{
    Q_OBJECT
    Q_PROPERTY(QByteArray propertyName READ propertyName WRITE setPropertyName)
    Q_PROPERTY(QObject* targetObject READ targetObject WRITE setTargetObject)

public:
    QPropertyAnimation(QObject *parent = 0);
    QPropertyAnimation(QObject *target, const QByteArray &propertyName, QObject *parent = 0);
    ~QPropertyAnimation();

    QObject *targetObject() const;
    void setTargetObject(QObject *target);

    QByteArray propertyName() const;
    void setPropertyName(const QByteArray &propertyName);

protected:
    bool event(QEvent *event);
    void updateCurrentValue(const QVariant &value);
    void updateState(QAbstractAnimation::State oldState, QAbstractAnimation::State newState);

private:
    Q_PRIVATE_SLOT(d_func(), void _q_targetDestroyed())
    Q_DISABLE_COPY(QPropertyAnimation)
    Q_DECLARE_PRIVATE(QPropertyAnimation)
};

#endif //QT_NO_ANIMATION

QT_END_NAMESPACE

QT_END_HEADER

#endif // QPROPERTYANIMATION_H