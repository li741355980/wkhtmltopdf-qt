/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtScript module of the Qt Toolkit.
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
** contact the sales department at http://qt.nokia.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#include "config.h"
#include "qscriptvariant_p.h"

#include "../api/qscriptengine.h"
#include "../api/qscriptengine_p.h"

#include "Error.h"
#include "PrototypeFunction.h"
#include "JSString.h"

namespace JSC
{
QT_USE_NAMESPACE
ASSERT_CLASS_FITS_IN_CELL(QScript::QVariantPrototype);
}

QT_BEGIN_NAMESPACE

namespace QScript
{

QVariantDelegate::QVariantDelegate(const QVariant &value)
    : m_value(value)
{
}

QVariantDelegate::~QVariantDelegate()
{
}

QVariant &QVariantDelegate::value()
{
    return m_value;
}

void QVariantDelegate::setValue(const QVariant &value)
{
    m_value = value;
}

QScriptObjectDelegate::Type QVariantDelegate::type() const
{
    return Variant;
}

static JSC::JSValue JSC_HOST_CALL variantProtoFuncValueOf(JSC::ExecState *exec, JSC::JSObject*,
                                                          JSC::JSValue thisValue, const JSC::ArgList&)
{
    QScriptEnginePrivate *engine = scriptEngineFromExec(exec);
    thisValue = engine->toUsableValue(thisValue);
    if (!thisValue.isObject(&QScriptObject::info))
        return throwError(exec, JSC::TypeError);
    QScriptObjectDelegate *delegate = static_cast<QScriptObject*>(JSC::asObject(thisValue))->delegate();
    if (!delegate || (delegate->type() != QScriptObjectDelegate::Variant))
        return throwError(exec, JSC::TypeError);
    const QVariant &v = static_cast<QVariantDelegate*>(delegate)->value();
    switch (v.type()) {
    case QVariant::Invalid:
        return JSC::jsUndefined();
    case QVariant::String:
        return JSC::jsString(exec, v.toString());

    case QVariant::Int:
        return JSC::jsNumber(exec, v.toInt());

    case QVariant::Bool:
        return JSC::jsBoolean(v.toBool());

    case QVariant::Double:
        return JSC::jsNumber(exec, v.toDouble());

//    case QVariant::Char:
//        return JSC::jsNumber(exec, v.toChar().unicode());

    case QVariant::UInt:
        return JSC::jsNumber(exec, v.toUInt());

    default:
        ;
    }
    return thisValue;
}

static JSC::JSValue JSC_HOST_CALL variantProtoFuncToString(JSC::ExecState *exec, JSC::JSObject *callee,
                                                           JSC::JSValue thisValue, const JSC::ArgList &args)
{
    QScriptEnginePrivate *engine = scriptEngineFromExec(exec);
    thisValue = engine->toUsableValue(thisValue);
    if (!thisValue.isObject(&QScriptObject::info))
        return throwError(exec, JSC::TypeError, "This object is not a QVariant");
    QScriptObjectDelegate *delegate = static_cast<QScriptObject*>(JSC::asObject(thisValue))->delegate();
    if (!delegate || (delegate->type() != QScriptObjectDelegate::Variant))
        return throwError(exec, JSC::TypeError, "This object is not a QVariant");
    const QVariant &v = static_cast<QVariantDelegate*>(delegate)->value();
    JSC::UString result;
    JSC::JSValue value = variantProtoFuncValueOf(exec, callee, thisValue, args);
    if (value.isObject()) {
        result = v.toString();
        if (result.isEmpty() && !v.canConvert(QVariant::String)) {
            result = "QVariant(";
            result += v.typeName();
            result += ")";
        }
    } else {
        result = value.toString(exec);
    }
    return JSC::jsString(exec, result);
}

QVariantPrototype::QVariantPrototype(JSC::ExecState* exec, WTF::PassRefPtr<JSC::Structure> structure,
                                     JSC::Structure* prototypeFunctionStructure)
    : QScriptObject(structure)
{
    setDelegate(new QVariantDelegate(QVariant()));

    putDirectFunction(exec, new (exec) JSC::PrototypeFunction(exec, prototypeFunctionStructure, 0, exec->propertyNames().toString, variantProtoFuncToString), JSC::DontEnum);
    putDirectFunction(exec, new (exec) JSC::PrototypeFunction(exec, prototypeFunctionStructure, 0, exec->propertyNames().valueOf, variantProtoFuncValueOf), JSC::DontEnum);
}

} // namespace QScript

QT_END_NAMESPACE