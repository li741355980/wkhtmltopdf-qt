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

#include "qmatrix4x4.h"
#include <QtCore/qmath.h>
#include <QtGui/qmatrix.h>
#include <QtGui/qtransform.h>

QT_BEGIN_NAMESPACE

#ifndef QT_NO_MATRIX4X4

/*!
    \class QMatrix4x4
    \brief The QMatrix4x4 class represents a 4x4 transformation matrix in 3D space.
    \since 4.6

    \sa QVector3D, QGenericMatrix
*/

/*!
    \fn QMatrix4x4::QMatrix4x4()

    Constructs an identity matrix.
*/

/*!
    Constructs a matrix from the given 16 floating-point \a values.
    The contents of the array \a values is assumed to be in
    row-major order.

    If the matrix has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4 to optimize further
    calls to translate(), scale(), etc.

    \sa toValueArray(), inferSpecialType()
*/
QMatrix4x4::QMatrix4x4(const qreal *values)
{
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            m[col][row] = values[row * 4 + col];
    flagBits = General;
}

/*!
    \fn QMatrix4x4::QMatrix4x4(qreal m11, qreal m12, qreal m13, qreal m14, qreal m21, qreal m22, qreal m23, qreal m24, qreal m31, qreal m32, qreal m33, qreal m34, qreal m41, qreal m42, qreal m43, qreal m44)

    Constructs a matrix from the 16 elements \a m11, \a m12, \a m13, \a m14,
    \a m21, \a m22, \a m23, \a m24, \a m31, \a m32, \a m33, \a m34,
    \a m41, \a m42, \a m43, and \a m44.  The elements are specified in
    row-major order.

    If the matrix has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4 to optimize further
    calls to translate(), scale(), etc.

    \sa inferSpecialType()
*/

#if !defined(QT_NO_MEMBER_TEMPLATES) || defined(Q_QDOC)

/*!
    \fn QMatrix4x4::QMatrix4x4(const QGenericMatrix<N, M, qreal, float>& matrix)

    Constructs a 4x4 matrix from the left-most 4 columns and top-most
    4 rows of \a matrix.  If \a matrix has less than 4 columns or rows,
    the remaining elements are filled with elements from the identity
    matrix.

    \sa toGenericMatrix(), qGenericMatrixToMatrix4x4()
*/

/*!
    \fn QGenericMatrix<N, M, qreal, float> QMatrix4x4::toGenericMatrix() const

    Constructs a NxM generic matrix from the left-most N columns and
    top-most M rows of this 4x4 matrix.  If N or M is greater than 4,
    then the remaining elements are filled with elements from the
    identity matrix.

    \sa qGenericMatrixFromMatrix4x4()
*/

#endif

/*!
    \fn QMatrix4x4 qGenericMatrixToMatrix4x4(const QGenericMatrix<N, M, qreal, float>& matrix)
    \relates QMatrix4x4

    Returns a 4x4 matrix constructed from the left-most 4 columns and
    top-most 4 rows of \a matrix.  If \a matrix has less than 4 columns
    or rows, the remaining elements are filled with elements from the
    identity matrix.

    \sa qGenericMatrixFromMatrix4x4()
*/

/*!
    \fn QGenericMatrix<N, M, qreal, float> qGenericMatrixFromMatrix4x4(const QMatrix4x4& matrix)
    \relates QMatrix4x4

    Returns a NxM generic matrix constructed from the left-most N columns
    and top-most M rows of \a matrix.  If N or M is greater than 4,
    then the remaining elements are filled with elements from the
    identity matrix.

    \sa qGenericMatrixToMatrix4x4(), QMatrix4x4::toGenericMatrix()
*/

/*!
    \internal
*/
QMatrix4x4::QMatrix4x4(const float *values, int cols, int rows)
{
    for (int col = 0; col < 4; ++col) {
        for (int row = 0; row < 4; ++row) {
            if (col < cols && row < rows)
                m[col][row] = values[col * rows + row];
            else if (col == row)
                m[col][row] = 1.0f;
            else
                m[col][row] = 0.0f;
        }
    }
    flagBits = General;
}

/*!
    Constructs a 4x4 matrix from a conventional Qt 2D affine
    transformation \a matrix.

    If \a matrix has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4 to optimize further
    calls to translate(), scale(), etc.

    \sa toAffine(), inferSpecialType()
*/
QMatrix4x4::QMatrix4x4(const QMatrix& matrix)
{
    m[0][0] = matrix.m11();
    m[0][1] = matrix.m12();
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = matrix.m21();
    m[1][1] = matrix.m22();
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = matrix.dx();
    m[3][1] = matrix.dy();
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
    flagBits = General;
}

/*!
    Constructs a 4x4 matrix from the conventional Qt 2D
    transformation matrix \a transform.

    If \a transform has a special type (identity, translate, scale, etc),
    the programmer should follow this constructor with a call to
    inferSpecialType() if they wish QMatrix4x4 to optimize further
    calls to translate(), scale(), etc.

    \sa toTransform(), inferSpecialType()
*/
QMatrix4x4::QMatrix4x4(const QTransform& transform)
{
    m[0][0] = transform.m11();
    m[0][1] = transform.m12();
    m[0][2] = 0.0f;
    m[0][3] = transform.m13();
    m[1][0] = transform.m21();
    m[1][1] = transform.m22();
    m[1][2] = 0.0f;
    m[1][3] = transform.m23();
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = transform.dx();
    m[3][1] = transform.dy();
    m[3][2] = 0.0f;
    m[3][3] = transform.m33();
    flagBits = General;
}

/*!
    \fn qreal QMatrix4x4::operator()(int row, int column) const

    Returns the element at position (\a row, \a column) in this matrix.

    \sa column(), row()
*/

/*!
    \fn float& QMatrix4x4::operator()(int row, int column)

    Returns a reference to the element at position (\a row, \a column)
    in this matrix so that the element can be assigned to.

    \sa inferSpecialType(), setColumn(), setRow()
*/

/*!
    \fn QVector4D QMatrix4x4::column(int index) const

    Returns the elements of column \a index as a 4D vector.

    \sa setColumn(), row()
*/

/*!
    \fn void QMatrix4x4::setColumn(int index, const QVector4D& value)

    Sets the elements of column \a index to the components of \a value.

    \sa column(), setRow()
*/

/*!
    \fn QVector4D QMatrix4x4::row(int index) const

    Returns the elements of row \a index as a 4D vector.

    \sa setRow(), column()
*/

/*!
    \fn void QMatrix4x4::setRow(int index, const QVector4D& value)

    Sets the elements of row \a index to the components of \a value.

    \sa row(), setColumn()
*/

/*!
    \fn bool QMatrix4x4::isIdentity() const

    Returns true if this matrix is the identity; false otherwise.

    \sa setIdentity()
*/

/*!
    \fn void QMatrix4x4::setIdentity()

    Sets this matrix to the identity.

    \sa isIdentity()
*/

/*!
    \fn void QMatrix4x4::fill(qreal value)

    Fills all elements of this matrx with \a value.
*/

// The 4x4 matrix inverse algorithm is based on that described at:
// http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q24
// Some optimization has been done to avoid making copies of 3x3
// sub-matrices and to unroll the loops.

// Calculate the determinant of a 3x3 sub-matrix.
//     | A B C |
// M = | D E F |   det(M) = A * (EI - HF) - B * (DI - GF) + C * (DH - GE)
//     | G H I |
static inline float matrixDet3
    (const float m[4][4], int col0, int col1, int col2,
     int row0, int row1, int row2)
{
    return m[col0][row0] *
                (m[col1][row1] * m[col2][row2] -
                 m[col1][row2] * m[col2][row1]) -
           m[col1][row0] *
                (m[col0][row1] * m[col2][row2] -
                 m[col0][row2] * m[col2][row1]) +
           m[col2][row0] *
                (m[col0][row1] * m[col1][row2] -
                 m[col0][row2] * m[col1][row1]);
}

// Calculate the determinant of a 4x4 matrix.
static inline float matrixDet4(const float m[4][4])
{
    float det;
    det  = m[0][0] * matrixDet3(m, 1, 2, 3, 1, 2, 3);
    det -= m[1][0] * matrixDet3(m, 0, 2, 3, 1, 2, 3);
    det += m[2][0] * matrixDet3(m, 0, 1, 3, 1, 2, 3);
    det -= m[3][0] * matrixDet3(m, 0, 1, 2, 1, 2, 3);
    return det;
}

/*!
    Returns the determinant of this matrix.
*/
qreal QMatrix4x4::determinant() const
{
    return qreal(matrixDet4(m));
}

/*!
    Returns the inverse of this matrix.  Returns the identity if
    this matrix cannot be inverted; i.e. determinant() is zero.
    If \a invertible is not null, then true will be written to
    that location if the matrix can be inverted; false otherwise.

    If the matrix is recognized as the identity or an orthonormal
    matrix, then this function will quickly invert the matrix
    using optimized routines.

    \sa determinant(), normalMatrix()
*/
QMatrix4x4 QMatrix4x4::inverted(bool *invertible) const
{
    // Handle some of the easy cases first.
    if (flagBits == Identity) {
        if (invertible)
            *invertible = true;
        return QMatrix4x4();
    } else if (flagBits == Translation) {
        QMatrix4x4 inv;
        inv.m[3][0] = -m[3][0];
        inv.m[3][1] = -m[3][1];
        inv.m[3][2] = -m[3][2];
        inv.flagBits = Translation;
        if (invertible)
            *invertible = true;
        return inv;
    } else if (flagBits == Rotation || flagBits == (Rotation | Translation)) {
        if (invertible)
            *invertible = true;
        return orthonormalInverse();
    }

    QMatrix4x4 inv(1); // The "1" says to not load the identity.

    float det = matrixDet4(m);
    if (det == 0.0f) {
        if (invertible)
            *invertible = false;
        return QMatrix4x4();
    }
    det = 1.0f / det;

    inv.m[0][0] =  matrixDet3(m, 1, 2, 3, 1, 2, 3) * det;
    inv.m[0][1] = -matrixDet3(m, 0, 2, 3, 1, 2, 3) * det;
    inv.m[0][2] =  matrixDet3(m, 0, 1, 3, 1, 2, 3) * det;
    inv.m[0][3] = -matrixDet3(m, 0, 1, 2, 1, 2, 3) * det;
    inv.m[1][0] = -matrixDet3(m, 1, 2, 3, 0, 2, 3) * det;
    inv.m[1][1] =  matrixDet3(m, 0, 2, 3, 0, 2, 3) * det;
    inv.m[1][2] = -matrixDet3(m, 0, 1, 3, 0, 2, 3) * det;
    inv.m[1][3] =  matrixDet3(m, 0, 1, 2, 0, 2, 3) * det;
    inv.m[2][0] =  matrixDet3(m, 1, 2, 3, 0, 1, 3) * det;
    inv.m[2][1] = -matrixDet3(m, 0, 2, 3, 0, 1, 3) * det;
    inv.m[2][2] =  matrixDet3(m, 0, 1, 3, 0, 1, 3) * det;
    inv.m[2][3] = -matrixDet3(m, 0, 1, 2, 0, 1, 3) * det;
    inv.m[3][0] = -matrixDet3(m, 1, 2, 3, 0, 1, 2) * det;
    inv.m[3][1] =  matrixDet3(m, 0, 2, 3, 0, 1, 2) * det;
    inv.m[3][2] = -matrixDet3(m, 0, 1, 3, 0, 1, 2) * det;
    inv.m[3][3] =  matrixDet3(m, 0, 1, 2, 0, 1, 2) * det;

    if (invertible)
        *invertible = true;
    return inv;
}

/*!
    Returns the normal matrix corresponding to this 4x4 transformation.
    The normal matrix is the transpose of the inverse of the top-left
    3x3 part of this 4x4 matrix.  If the 3x3 sub-matrix is not invertible,
    this function returns the identity.

    \sa inverted()
*/
QMatrix3x3 QMatrix4x4::normalMatrix() const
{
    QMatrix3x3 inv;

    // Handle the simple cases first.
    if (flagBits == Identity || flagBits == Translation) {
        return inv;
    } else if (flagBits == Scale || flagBits == (Translation | Scale)) {
        if (m[0][0] == 0.0f || m[1][1] == 0.0f || m[2][2] == 0.0f)
            return inv;
        inv.data()[0] = 1.0f / m[0][0];
        inv.data()[4] = 1.0f / m[1][1];
        inv.data()[8] = 1.0f / m[2][2];
        return inv;
    }

    float det = matrixDet3(m, 0, 1, 2, 0, 1, 2);
    if (det == 0.0f)
        return inv;
    det = 1.0f / det;

    float *invm = inv.data();

    // Invert and transpose in a single step.
    invm[0 + 0 * 3] =  (m[1][1] * m[2][2] - m[2][1] * m[1][2]) * det;
    invm[1 + 0 * 3] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * det;
    invm[2 + 0 * 3] =  (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * det;
    invm[0 + 1 * 3] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * det;
    invm[1 + 1 * 3] =  (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * det;
    invm[2 + 1 * 3] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * det;
    invm[0 + 2 * 3] =  (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * det;
    invm[1 + 2 * 3] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * det;
    invm[2 + 2 * 3] =  (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * det;

    return inv;
}

/*!
    Returns this matrix, transposed about its diagonal.
*/
QMatrix4x4 QMatrix4x4::transposed() const
{
    QMatrix4x4 result(1); // The "1" says to not load the identity.
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            result.m[col][row] = m[row][col];
        }
    }
    return result;
}

/*!
    \fn QMatrix4x4& QMatrix4x4::operator+=(const QMatrix4x4& other)

    Adds the contents of \a other to this matrix.
*/

/*!
    \fn QMatrix4x4& QMatrix4x4::operator-=(const QMatrix4x4& other)

    Subtracts the contents of \a other from this matrix.
*/

/*!
    \fn QMatrix4x4& QMatrix4x4::operator*=(const QMatrix4x4& other)

    Multiplies the contents of \a other by this matrix.
*/

/*!
    \fn QMatrix4x4& QMatrix4x4::operator*=(qreal factor)
    \overload

    Multiplies all elements of this matrix by \a factor.
*/

/*!
    \overload

    Divides all elements of this matrix by \a divisor.
*/
QMatrix4x4& QMatrix4x4::operator/=(qreal divisor)
{
    m[0][0] /= divisor;
    m[0][1] /= divisor;
    m[0][2] /= divisor;
    m[0][3] /= divisor;
    m[1][0] /= divisor;
    m[1][1] /= divisor;
    m[1][2] /= divisor;
    m[1][3] /= divisor;
    m[2][0] /= divisor;
    m[2][1] /= divisor;
    m[2][2] /= divisor;
    m[2][3] /= divisor;
    m[3][0] /= divisor;
    m[3][1] /= divisor;
    m[3][2] /= divisor;
    m[3][3] /= divisor;
    flagBits = General;
    return *this;
}

/*!
    \fn bool QMatrix4x4::operator==(const QMatrix4x4& other) const

    Returns true if this matrix is identical to \a other; false otherwise.
    This operator uses an exact floating-point comparison.
*/

/*!
    \fn bool QMatrix4x4::operator!=(const QMatrix4x4& other) const

    Returns true if this matrix is not identical to \a other; false otherwise.
    This operator uses an exact floating-point comparison.
*/

/*!
    \fn QMatrix4x4 operator+(const QMatrix4x4& m1, const QMatrix4x4& m2)
    \relates QMatrix4x4

    Returns the sum of \a m1 and \a m2.
*/

/*!
    \fn QMatrix4x4 operator-(const QMatrix4x4& m1, const QMatrix4x4& m2)
    \relates QMatrix4x4

    Returns the difference of \a m1 and \a m2.
*/

/*!
    \fn QMatrix4x4 operator*(const QMatrix4x4& m1, const QMatrix4x4& m2)
    \relates QMatrix4x4

    Returns the product of \a m1 and \a m2.
*/

#ifndef QT_NO_VECTOR3D

/*!
    \fn QVector3D operator*(const QVector3D& vector, const QMatrix4x4& matrix)
    \relates QMatrix4x4

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied post-vector.
*/

/*!
    \fn QVector3D operator*(const QMatrix4x4& matrix, const QVector3D& vector)
    \relates QMatrix4x4

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied pre-vector.
*/

#endif

#ifndef QT_NO_VECTOR4D

/*!
    \fn QVector4D operator*(const QVector4D& vector, const QMatrix4x4& matrix)
    \relates QMatrix4x4

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied post-vector.
*/

/*!
    \fn QVector4D operator*(const QMatrix4x4& matrix, const QVector4D& vector)
    \relates QMatrix4x4

    Returns the result of transforming \a vector according to \a matrix,
    with the matrix applied pre-vector.
*/

#endif

/*!
    \fn QPoint operator*(const QPoint& point, const QMatrix4x4& matrix)
    \relates QMatrix4x4

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied post-point.
*/

/*!
    \fn QPointF operator*(const QPointF& point, const QMatrix4x4& matrix)
    \relates QMatrix4x4

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied post-point.
*/

/*!
    \fn QPoint operator*(const QMatrix4x4& matrix, const QPoint& point)
    \relates QMatrix4x4

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied pre-point.
*/

/*!
    \fn QPointF operator*(const QMatrix4x4& matrix, const QPointF& point)
    \relates QMatrix4x4

    Returns the result of transforming \a point according to \a matrix,
    with the matrix applied pre-point.
*/

/*!
    \fn QMatrix4x4 operator-(const QMatrix4x4& matrix)
    \overload
    \relates QMatrix4x4

    Returns the negation of \a matrix.
*/

/*!
    \fn QMatrix4x4 operator*(qreal factor, const QMatrix4x4& matrix)
    \relates QMatrix4x4

    Returns the result of multiplying all elements of \a matrix by \a factor.
*/

/*!
    \fn QMatrix4x4 operator*(const QMatrix4x4& matrix, qreal factor)
    \relates QMatrix4x4

    Returns the result of multiplying all elements of \a matrix by \a factor.
*/

/*!
    \relates QMatrix4x4

    Returns the result of dividing all elements of \a matrix by \a divisor.
*/
QMatrix4x4 operator/(const QMatrix4x4& matrix, qreal divisor)
{
    QMatrix4x4 m(1); // The "1" says to not load the identity.
    m.m[0][0] = matrix.m[0][0] / divisor;
    m.m[0][1] = matrix.m[0][1] / divisor;
    m.m[0][2] = matrix.m[0][2] / divisor;
    m.m[0][3] = matrix.m[0][3] / divisor;
    m.m[1][0] = matrix.m[1][0] / divisor;
    m.m[1][1] = matrix.m[1][1] / divisor;
    m.m[1][2] = matrix.m[1][2] / divisor;
    m.m[1][3] = matrix.m[1][3] / divisor;
    m.m[2][0] = matrix.m[2][0] / divisor;
    m.m[2][1] = matrix.m[2][1] / divisor;
    m.m[2][2] = matrix.m[2][2] / divisor;
    m.m[2][3] = matrix.m[2][3] / divisor;
    m.m[3][0] = matrix.m[3][0] / divisor;
    m.m[3][1] = matrix.m[3][1] / divisor;
    m.m[3][2] = matrix.m[3][2] / divisor;
    m.m[3][3] = matrix.m[3][3] / divisor;
    return m;
}

/*!
    \fn bool qFuzzyCompare(const QMatrix4x4& m1, const QMatrix4x4& m2)
    \relates QMatrix4x4

    Returns true if \a m1 and \a m2 are equal, allowing for a small
    fuzziness factor for floating-point comparisons; false otherwise.
*/

#ifndef QT_NO_VECTOR3D

/*!
    Multiplies this matrix by another that scales coordinates by
    the components of \a vector.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4& QMatrix4x4::scale(const QVector3D& vector)
{
    float vx = vector.xp;
    float vy = vector.yp;
    float vz = vector.zp;
    if (flagBits == Identity) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= vx;
        m[1][1] *= vy;
        m[2][2] *= vz;
    } else if (flagBits == Translation) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits |= Scale;
    } else {
        m[0][0] *= vx;
        m[0][1] *= vx;
        m[0][2] *= vx;
        m[0][3] *= vx;
        m[1][0] *= vy;
        m[1][1] *= vy;
        m[1][2] *= vy;
        m[1][3] *= vy;
        m[2][0] *= vz;
        m[2][1] *= vz;
        m[2][2] *= vz;
        m[2][3] *= vz;
        flagBits = General;
    }
    return *this;
}
#endif

/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    components \a x, and \a y.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4& QMatrix4x4::scale(qreal x, qreal y)
{
    float vx(x);
    float vy(y);
    if (flagBits == Identity) {
        m[0][0] = vx;
        m[1][1] = vy;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= vx;
        m[1][1] *= vy;
    } else if (flagBits == Translation) {
        m[0][0] = vx;
        m[1][1] = vy;
        flagBits |= Scale;
    } else {
        m[0][0] *= vx;
        m[0][1] *= vx;
        m[0][2] *= vx;
        m[0][3] *= vx;
        m[1][0] *= vy;
        m[1][1] *= vy;
        m[1][2] *= vy;
        m[1][3] *= vy;
        flagBits = General;
    }
    return *this;
}

/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    components \a x, \a y, and \a z.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4& QMatrix4x4::scale(qreal x, qreal y, qreal z)
{
    float vx(x);
    float vy(y);
    float vz(z);
    if (flagBits == Identity) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= vx;
        m[1][1] *= vy;
        m[2][2] *= vz;
    } else if (flagBits == Translation) {
        m[0][0] = vx;
        m[1][1] = vy;
        m[2][2] = vz;
        flagBits |= Scale;
    } else {
        m[0][0] *= vx;
        m[0][1] *= vx;
        m[0][2] *= vx;
        m[0][3] *= vx;
        m[1][0] *= vy;
        m[1][1] *= vy;
        m[1][2] *= vy;
        m[1][3] *= vy;
        m[2][0] *= vz;
        m[2][1] *= vz;
        m[2][2] *= vz;
        m[2][3] *= vz;
        flagBits = General;
    }
    return *this;
}

/*!
    \overload

    Multiplies this matrix by another that scales coordinates by the
    given \a factor.  Returns this matrix.

    \sa translate(), rotate()
*/
QMatrix4x4& QMatrix4x4::scale(qreal factor)
{
    if (flagBits == Identity) {
        m[0][0] = factor;
        m[1][1] = factor;
        m[2][2] = factor;
        flagBits = Scale;
    } else if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[0][0] *= factor;
        m[1][1] *= factor;
        m[2][2] *= factor;
    } else if (flagBits == Translation) {
        m[0][0] = factor;
        m[1][1] = factor;
        m[2][2] = factor;
        flagBits |= Scale;
    } else {
        m[0][0] *= factor;
        m[0][1] *= factor;
        m[0][2] *= factor;
        m[0][3] *= factor;
        m[1][0] *= factor;
        m[1][1] *= factor;
        m[1][2] *= factor;
        m[1][3] *= factor;
        m[2][0] *= factor;
        m[2][1] *= factor;
        m[2][2] *= factor;
        m[2][3] *= factor;
        flagBits = General;
    }
    return *this;
}

#ifndef QT_NO_VECTOR3D
/*!
    Multiplies this matrix by another that translates coordinates by
    the components of \a vector.  Returns this matrix.

    \sa scale(), rotate()
*/
QMatrix4x4& QMatrix4x4::translate(const QVector3D& vector)
{
    float vx = vector.xp;
    float vy = vector.yp;
    float vz = vector.zp;
    if (flagBits == Identity) {
        m[3][0] = vx;
        m[3][1] = vy;
        m[3][2] = vz;
        flagBits = Translation;
    } else if (flagBits == Translation) {
        m[3][0] += vx;
        m[3][1] += vy;
        m[3][2] += vz;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * vx;
        m[3][1] = m[1][1] * vy;
        m[3][2] = m[2][2] * vz;
        flagBits |= Translation;
    } else if (flagBits == (Scale | Translation)) {
        m[3][0] += m[0][0] * vx;
        m[3][1] += m[1][1] * vy;
        m[3][2] += m[2][2] * vz;
    } else {
        m[3][0] += m[0][0] * vx + m[1][0] * vy + m[2][0] * vz;
        m[3][1] += m[0][1] * vx + m[1][1] * vy + m[2][1] * vz;
        m[3][2] += m[0][2] * vx + m[1][2] * vy + m[2][2] * vz;
        m[3][3] += m[0][3] * vx + m[1][3] * vy + m[2][3] * vz;
        if (flagBits == Rotation)
            flagBits |= Translation;
        else if (flagBits != (Rotation | Translation))
            flagBits = General;
    }
    return *this;
}

#endif

/*!
    \overload

    Multiplies this matrix by another that translates coordinates
    by the components \a x, and \a y.  Returns this matrix.

    \sa scale(), rotate()
*/
QMatrix4x4& QMatrix4x4::translate(qreal x, qreal y)
{
    float vx(x);
    float vy(y);
    if (flagBits == Identity) {
        m[3][0] = vx;
        m[3][1] = vy;
        flagBits = Translation;
    } else if (flagBits == Translation) {
        m[3][0] += vx;
        m[3][1] += vy;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * vx;
        m[3][1] = m[1][1] * vy;
        m[3][2] = 0.;
        flagBits |= Translation;
    } else if (flagBits == (Scale | Translation)) {
        m[3][0] += m[0][0] * vx;
        m[3][1] += m[1][1] * vy;
    } else {
        m[3][0] += m[0][0] * vx + m[1][0] * vy;
        m[3][1] += m[0][1] * vx + m[1][1] * vy;
        m[3][2] += m[0][2] * vx + m[1][2] * vy;
        m[3][3] += m[0][3] * vx + m[1][3] * vy;
        if (flagBits == Rotation)
            flagBits |= Translation;
        else if (flagBits != (Rotation | Translation))
            flagBits = General;
    }
    return *this;
}

/*!
    \overload

    Multiplies this matrix by another that translates coordinates
    by the components \a x, \a y, and \a z.  Returns this matrix.

    \sa scale(), rotate()
*/
QMatrix4x4& QMatrix4x4::translate(qreal x, qreal y, qreal z)
{
    float vx(x);
    float vy(y);
    float vz(z);
    if (flagBits == Identity) {
        m[3][0] = vx;
        m[3][1] = vy;
        m[3][2] = vz;
        flagBits = Translation;
    } else if (flagBits == Translation) {
        m[3][0] += vx;
        m[3][1] += vy;
        m[3][2] += vz;
    } else if (flagBits == Scale) {
        m[3][0] = m[0][0] * vx;
        m[3][1] = m[1][1] * vy;
        m[3][2] = m[2][2] * vz;
        flagBits |= Translation;
    } else if (flagBits == (Scale | Translation)) {
        m[3][0] += m[0][0] * vx;
        m[3][1] += m[1][1] * vy;
        m[3][2] += m[2][2] * vz;
    } else {
        m[3][0] += m[0][0] * vx + m[1][0] * vy + m[2][0] * vz;
        m[3][1] += m[0][1] * vx + m[1][1] * vy + m[2][1] * vz;
        m[3][2] += m[0][2] * vx + m[1][2] * vy + m[2][2] * vz;
        m[3][3] += m[0][3] * vx + m[1][3] * vy + m[2][3] * vz;
        if (flagBits == Rotation)
            flagBits |= Translation;
        else if (flagBits != (Rotation | Translation))
            flagBits = General;
    }
    return *this;
}

#ifndef QT_NO_VECTOR3D

/*!
    Multiples this matrix by another that rotates coordinates through
    \a angle degrees about \a vector.  Returns this matrix.

    \sa scale(), translate()
*/
QMatrix4x4& QMatrix4x4::rotate(qreal angle, const QVector3D& vector)
{
    return rotate(angle, vector.x(), vector.y(), vector.z());
}

#endif

/*!
    \overload

    Multiplies this matrix by another that rotates coordinates through
    \a angle degrees about the vector (\a x, \a y, \a z).  Returns this matrix.

    \sa scale(), translate()
*/
QMatrix4x4& QMatrix4x4::rotate(qreal angle, qreal x, qreal y, qreal z)
{
    QMatrix4x4 m(1); // The "1" says to not load the identity.
    qreal a = angle * M_PI / 180.0f;
    qreal c = qCos(a);
    qreal s = qSin(a);
    qreal ic;
    bool quick = false;
    if (x == 0.0f) {
        if (y == 0.0f) {
            if (z != 0.0f) {
                // Rotate around the Z axis.
                m.setIdentity();
                m.m[0][0] = c;
                m.m[1][1] = c;
                if (z < 0.0f) {
                    m.m[1][0] = s;
                    m.m[0][1] = -s;
                } else {
                    m.m[1][0] = -s;
                    m.m[0][1] = s;
                }
                m.flagBits = General;
                quick = true;
            }
        } else if (z == 0.0f) {
            // Rotate around the Y axis.
            m.setIdentity();
            m.m[0][0] = c;
            m.m[2][2] = c;
            if (y < 0.0f) {
                m.m[2][0] = -s;
                m.m[0][2] = s;
            } else {
                m.m[2][0] = s;
                m.m[0][2] = -s;
            }
            m.flagBits = General;
            quick = true;
        }
    } else if (y == 0.0f && z == 0.0f) {
        // Rotate around the X axis.
        m.setIdentity();
        m.m[1][1] = c;
        m.m[2][2] = c;
        if (x < 0.0f) {
            m.m[2][1] = s;
            m.m[1][2] = -s;
        } else {
            m.m[2][1] = -s;
            m.m[1][2] = s;
        }
        m.flagBits = General;
        quick = true;
    }
    if (!quick) {
        qreal len = x * x + y * y + z * z;
        if (!qFuzzyIsNull(len - 1.0f) && !qFuzzyIsNull(len)) {
            len = qSqrt(len);
            x /= len;
            y /= len;
            z /= len;
        }
        ic = 1.0f - c;
        m.m[0][0] = x * x * ic + c;
        m.m[1][0] = x * y * ic - z * s;
        m.m[2][0] = x * z * ic + y * s;
        m.m[3][0] = 0.0f;
        m.m[0][1] = y * x * ic + z * s;
        m.m[1][1] = y * y * ic + c;
        m.m[2][1] = y * z * ic - x * s;
        m.m[3][1] = 0.0f;
        m.m[0][2] = x * z * ic - y * s;
        m.m[1][2] = y * z * ic + x * s;
        m.m[2][2] = z * z * ic + c;
        m.m[3][2] = 0.0f;
        m.m[0][3] = 0.0f;
        m.m[1][3] = 0.0f;
        m.m[2][3] = 0.0f;
        m.m[3][3] = 1.0f;
    }
    int flags = flagBits;
    *this *= m;
    if (flags != Identity)
        flagBits = flags | Rotation;
    else
        flagBits = Rotation;
    return *this;
}

#ifndef QT_NO_VECTOR4D

/*!
    Multiples this matrix by another that rotates coordinates according
    to a specified \a quaternion.  The \a quaternion is assumed to have
    been normalized.  Returns this matrix.

    \sa scale(), translate(), QQuaternion
*/
QMatrix4x4& QMatrix4x4::rotate(const QQuaternion& quaternion)
{
    // Algorithm from:
    // http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q54
    QMatrix4x4 m(1);
    float xx = quaternion.xp * quaternion.xp;
    float xy = quaternion.xp * quaternion.yp;
    float xz = quaternion.xp * quaternion.zp;
    float xw = quaternion.xp * quaternion.wp;
    float yy = quaternion.yp * quaternion.yp;
    float yz = quaternion.yp * quaternion.zp;
    float yw = quaternion.yp * quaternion.wp;
    float zz = quaternion.zp * quaternion.zp;
    float zw = quaternion.zp * quaternion.wp;
    m.m[0][0] = 1.0f - 2 * (yy + zz);
    m.m[1][0] =        2 * (xy - zw);
    m.m[2][0] =        2 * (xz + yw);
    m.m[3][0] = 0.0f;
    m.m[0][1] =        2 * (xy + zw);
    m.m[1][1] = 1.0f - 2 * (xx + zz);
    m.m[2][1] =        2 * (yz - xw);
    m.m[3][1] = 0.0f;
    m.m[0][2] =        2 * (xz - yw);
    m.m[1][2] =        2 * (yz + xw);
    m.m[2][2] = 1.0f - 2 * (xx + yy);
    m.m[3][2] = 0.0f;
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;
    int flags = flagBits;
    *this *= m;
    if (flags != Identity)
        flagBits = flags | Rotation;
    else
        flagBits = Rotation;
    return *this;
}

#endif

/*!
    \overload

    Multiplies this matrix by another that applies an orthographic
    projection for a window with boundaries specified by \a rect.
    The near and far clipping planes will be -1 and 1 respectively.
    Returns this matrix.

    \sa frustum(), perspective()
*/
QMatrix4x4& QMatrix4x4::ortho(const QRect& rect)
{
    // Note: rect.right() and rect.bottom() subtract 1 in QRect,
    // which gives the location of a pixel within the rectangle,
    // instead of the extent of the rectangle.  We want the extent.
    // QRectF expresses the extent properly.
    return ortho(rect.x(), rect.x() + rect.width(), rect.y() + rect.height(), rect.y(), -1.0f, 1.0f);
}

/*!
    \overload

    Multiplies this matrix by another that applies an orthographic
    projection for a window with boundaries specified by \a rect.
    The near and far clipping planes will be -1 and 1 respectively.
    Returns this matrix.

    \sa frustum(), perspective()
*/
QMatrix4x4& QMatrix4x4::ortho(const QRectF& rect)
{
    return ortho(rect.left(), rect.right(), rect.bottom(), rect.top(), -1.0f, 1.0f);
}

/*!
    Multiplies this matrix by another that applies an orthographic
    projection for a window with lower-left corner (\a left, \a bottom),
    upper-right corner (\a right, \a top), and the specified \a nearPlane
    and \a farPlane clipping planes.  Returns this matrix.

    \sa frustum(), perspective()
*/
QMatrix4x4& QMatrix4x4::ortho(qreal left, qreal right, qreal bottom, qreal top, qreal nearPlane, qreal farPlane)
{
    // Bail out if the projection volume is zero-sized.
    if (left == right || bottom == top || nearPlane == farPlane)
        return *this;

    // Construct the projection.
    qreal width = right - left;
    qreal invheight = top - bottom;
    qreal clip = farPlane - nearPlane;
#ifndef QT_NO_VECTOR3D
    if (clip == 2.0f && (nearPlane + farPlane) == 0.0f) {
        // We can express this projection as a translate and scale
        // which will be more efficient to modify with further
        // transformations than producing a "General" matrix.
        translate(QVector3D
            (-(left + right) / width,
             -(top + bottom) / invheight,
             0.0f, 1));
        scale(QVector3D
            (2.0f / width,
             2.0f / invheight,
             -1.0f, 1));
        return *this;
    }
#endif
    QMatrix4x4 m(1);
    m.m[0][0] = 2.0f / width;
    m.m[1][0] = 0.0f;
    m.m[2][0] = 0.0f;
    m.m[3][0] = -(left + right) / width;
    m.m[0][1] = 0.0f;
    m.m[1][1] = 2.0f / invheight;
    m.m[2][1] = 0.0f;
    m.m[3][1] = -(top + bottom) / invheight;
    m.m[0][2] = 0.0f;
    m.m[1][2] = 0.0f;
    m.m[2][2] = -2.0f / clip;
    m.m[3][2] = -(nearPlane + farPlane) / clip;
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;

    // Apply the projection.
    *this *= m;
    return *this;
}

/*!
    Multiplies this matrix by another that applies a perspective
    frustum projection for a window with lower-left corner (\a left, \a bottom),
    upper-right corner (\a right, \a top), and the specified \a nearPlane
    and \a farPlane clipping planes.  Returns this matrix.

    \sa ortho(), perspective()
*/
QMatrix4x4& QMatrix4x4::frustum(qreal left, qreal right, qreal bottom, qreal top, qreal nearPlane, qreal farPlane)
{
    // Bail out if the projection volume is zero-sized.
    if (left == right || bottom == top || nearPlane == farPlane)
        return *this;

    // Construct the projection.
    QMatrix4x4 m(1);
    qreal width = right - left;
    qreal invheight = top - bottom;
    qreal clip = farPlane - nearPlane;
    m.m[0][0] = 2.0f * nearPlane / width;
    m.m[1][0] = 0.0f;
    m.m[2][0] = (left + right) / width;
    m.m[3][0] = 0.0f;
    m.m[0][1] = 0.0f;
    m.m[1][1] = 2.0f * nearPlane / invheight;
    m.m[2][1] = (top + bottom) / invheight;
    m.m[3][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[1][2] = 0.0f;
    m.m[2][2] = -(nearPlane + farPlane) / clip;
    m.m[3][2] = -2.0f * nearPlane * farPlane / clip;
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = -1.0f;
    m.m[3][3] = 0.0f;

    // Apply the projection.
    *this *= m;
    return *this;
}

/*!
    Multiplies this matrix by another that applies a perspective
    projection.  The field of view will be \a angle degrees within
    a window with a given \a aspect ratio.  The projection will
    have the specified \a nearPlane and \a farPlane clipping planes.
    Returns this matrix.

    \sa ortho(), frustum()
*/
QMatrix4x4& QMatrix4x4::perspective(qreal angle, qreal aspect, qreal nearPlane, qreal farPlane)
{
    // Bail out if the projection volume is zero-sized.
    if (nearPlane == farPlane || aspect == 0.0f)
        return *this;

    // Construct the projection.
    QMatrix4x4 m(1);
    qreal radians = (angle / 2.0f) * M_PI / 180.0f;
    qreal sine = qSin(radians);
    if (sine == 0.0f)
        return *this;
    qreal cotan = qCos(radians) / sine;
    qreal clip = farPlane - nearPlane;
    m.m[0][0] = cotan / aspect;
    m.m[1][0] = 0.0f;
    m.m[2][0] = 0.0f;
    m.m[3][0] = 0.0f;
    m.m[0][1] = 0.0f;
    m.m[1][1] = cotan;
    m.m[2][1] = 0.0f;
    m.m[3][1] = 0.0f;
    m.m[0][2] = 0.0f;
    m.m[1][2] = 0.0f;
    m.m[2][2] = -(nearPlane + farPlane) / clip;
    m.m[3][2] = -(2.0f * nearPlane * farPlane) / clip;
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = -1.0f;
    m.m[3][3] = 0.0f;

    // Apply the projection.
    *this *= m;
    return *this;
}

#ifndef QT_NO_VECTOR3D

/*!
    Multiplies this matrix by another that applies an \a eye position
    transformation.  The \a center value indicates the center of the
    view that the \a eye is looking at.  The \a up value indicates
    which direction should be considered up with respect to the \a eye.
    Returns this matrix.
*/
QMatrix4x4& QMatrix4x4::lookAt(const QVector3D& eye, const QVector3D& center, const QVector3D& up)
{
    QVector3D forward = (center - eye).normalized();
    QVector3D side = QVector3D::crossProduct(forward, up).normalized();
    QVector3D upVector = QVector3D::crossProduct(side, forward);

    QMatrix4x4 m(1);

    m.m[0][0] = side.xp;
    m.m[1][0] = side.yp;
    m.m[2][0] = side.zp;
    m.m[3][0] = 0.0f;
    m.m[0][1] = upVector.xp;
    m.m[1][1] = upVector.yp;
    m.m[2][1] = upVector.zp;
    m.m[3][1] = 0.0f;
    m.m[0][2] = -forward.xp;
    m.m[1][2] = -forward.yp;
    m.m[2][2] = -forward.zp;
    m.m[3][2] = 0.0f;
    m.m[0][3] = 0.0f;
    m.m[1][3] = 0.0f;
    m.m[2][3] = 0.0f;
    m.m[3][3] = 1.0f;

    *this *= m;
    return translate(-eye);
}

#endif

/*!
    Flips between right-handed and left-handed coordinate systems
    by multiplying the y and z co-ordinates by -1.  This is normally
    used to create a left-handed orthographic view without scaling
    the viewport as ortho() does.  Returns this matrix.

    \sa ortho()
*/
QMatrix4x4& QMatrix4x4::flipCoordinates()
{
    if (flagBits == Scale || flagBits == (Scale | Translation)) {
        m[1][1] = -m[1][1];
        m[2][2] = -m[2][2];
    } else if (flagBits == Translation) {
        m[1][1] = -m[1][1];
        m[2][2] = -m[2][2];
        flagBits |= Scale;
    } else if (flagBits == Identity) {
        m[1][1] = -1.0f;
        m[2][2] = -1.0f;
        flagBits = Scale;
    } else {
        m[1][0] = -m[1][0];
        m[1][1] = -m[1][1];
        m[1][2] = -m[1][2];
        m[1][3] = -m[1][3];
        m[2][0] = -m[2][0];
        m[2][1] = -m[2][1];
        m[2][2] = -m[2][2];
        m[2][3] = -m[2][3];
        flagBits = General;
    }
    return *this;
}

/*!
    Retrieves the 16 items in this matrix and writes them to \a values
    in row-major order.
*/
void QMatrix4x4::toValueArray(qreal *values) const
{
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            values[row * 4 + col] = qreal(m[col][row]);
}

/*!
    Returns the conventional Qt 2D affine transformation matrix that
    corresponds to this matrix.  It is assumed that this matrix
    only contains 2D affine transformation elements.

    \sa toTransform()
*/
QMatrix QMatrix4x4::toAffine() const
{
    return QMatrix(qreal(m[0][0]), qreal(m[0][1]),
                   qreal(m[1][0]), qreal(m[1][1]),
                   qreal(m[3][0]), qreal(m[3][1]));
}

/*!
    Returns the conventional Qt 2D transformation matrix that
    corresponds to this matrix.  It is assumed that this matrix
    only contains 2D transformation elements.

    \sa toAffine()
*/
QTransform QMatrix4x4::toTransform() const
{
    return QTransform(qreal(m[0][0]), qreal(m[0][1]), qreal(m[0][3]),
                      qreal(m[1][0]), qreal(m[1][1]), qreal(m[1][3]),
                      qreal(m[3][0]), qreal(m[3][1]), qreal(m[3][3]));
}

/*!
    \fn QPoint QMatrix4x4::map(const QPoint& point) const

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

/*!
    \fn QPointF QMatrix4x4::map(const QPointF& point) const

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

#ifndef QT_NO_VECTOR3D

/*!
    \fn QVector3D QMatrix4x4::map(const QVector3D& point) const

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

#endif

#ifndef QT_NO_VECTOR4D

/*!
    \fn QVector4D QMatrix4x4::map(const QVector4D& point) const;

    Maps \a point by multiplying this matrix by \a point.

    \sa mapRect()
*/

#endif

/*!
    Maps \a rect by multiplying this matrix by the corners
    of \a rect and then forming a new rectangle from the results.
    The returned rectangle will be an ordinary 2D rectangle
    with sides parallel to the horizontal and vertical axes.

    \sa map()
*/
QRect QMatrix4x4::mapRect(const QRect& rect) const
{
    if (flagBits == (Translation | Scale) || flagBits == Scale) {
        qreal x = rect.x() * m[0][0] + m[3][0];
        qreal y = rect.y() * m[1][1] + m[3][1];
        qreal w = rect.width() * m[0][0];
        qreal h = rect.height() * m[1][1];
        if (w < 0) {
            w = -w;
            x -= w;
        }
        if (h < 0) {
            h = -h;
            y -= h;
        }
        return QRect(qRound(x), qRound(y), qRound(w), qRound(h));
    } else if (flagBits == Translation) {
        return QRect(qRound(rect.x() + m[3][0]),
                     qRound(rect.y() + m[3][1]),
                     rect.width(), rect.height());
    }

    QPoint tl = map(rect.topLeft());
    QPoint tr = map(QPoint(rect.x() + rect.width(), rect.y()));
    QPoint bl = map(QPoint(rect.x(), rect.y() + rect.height()));
    QPoint br = map(QPoint(rect.x() + rect.width(),
                           rect.y() + rect.height()));

    int xmin = qMin(qMin(tl.x(), tr.x()), qMin(bl.x(), br.x()));
    int xmax = qMax(qMax(tl.x(), tr.x()), qMax(bl.x(), br.x()));
    int ymin = qMin(qMin(tl.y(), tr.y()), qMin(bl.y(), br.y()));
    int ymax = qMax(qMax(tl.y(), tr.y()), qMax(bl.y(), br.y()));

    return QRect(xmin, ymin, xmax - xmin, ymax - ymin);
}

/*!
    Maps \a rect by multiplying this matrix by the corners
    of \a rect and then forming a new rectangle from the results.
    The returned rectangle will be an ordinary 2D rectangle
    with sides parallel to the horizontal and vertical axes.

    \sa map()
*/
QRectF QMatrix4x4::mapRect(const QRectF& rect) const
{
    if (flagBits == (Translation | Scale) || flagBits == Scale) {
        qreal x = rect.x() * m[0][0] + m[3][0];
        qreal y = rect.y() * m[1][1] + m[3][1];
        qreal w = rect.width() * m[0][0];
        qreal h = rect.height() * m[1][1];
        if (w < 0) {
            w = -w;
            x -= w;
        }
        if (h < 0) {
            h = -h;
            y -= h;
        }
        return QRectF(x, y, w, h);
    } else if (flagBits == Translation) {
        return rect.translated(m[3][0], m[3][1]);
    }

    QPointF tl = map(rect.topLeft()); QPointF tr = map(rect.topRight());
    QPointF bl = map(rect.bottomLeft()); QPointF br = map(rect.bottomRight());

    qreal xmin = qMin(qMin(tl.x(), tr.x()), qMin(bl.x(), br.x()));
    qreal xmax = qMax(qMax(tl.x(), tr.x()), qMax(bl.x(), br.x()));
    qreal ymin = qMin(qMin(tl.y(), tr.y()), qMin(bl.y(), br.y()));
    qreal ymax = qMax(qMax(tl.y(), tr.y()), qMax(bl.y(), br.y()));

    return QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));
}

/*!
    \fn float *QMatrix4x4::data()

    Returns a pointer to the raw data of this matrix.  This is intended
    for use with raw GL functions.

    \sa constData(), inferSpecialType()
*/

/*!
    \fn const float *QMatrix4x4::data() const

    Returns a constant pointer to the raw data of this matrix.
    This is intended for use with raw GL functions.

    \sa constData()
*/

/*!
    \fn const float *QMatrix4x4::constData() const

    Returns a constant pointer to the raw data of this matrix.
    This is intended for use with raw GL functions.

    \sa data()
*/

// Helper routine for inverting orthonormal matrices that consist
// of just rotations and translations.
QMatrix4x4 QMatrix4x4::orthonormalInverse() const
{
    QMatrix4x4 result(1);  // The '1' says not to load identity

    result.m[0][0] = m[0][0];
    result.m[1][0] = m[0][1];
    result.m[2][0] = m[0][2];

    result.m[0][1] = m[1][0];
    result.m[1][1] = m[1][1];
    result.m[2][1] = m[1][2];

    result.m[0][2] = m[2][0];
    result.m[1][2] = m[2][1];
    result.m[2][2] = m[2][2];

    result.m[0][3] = 0.0f;
    result.m[1][3] = 0.0f;
    result.m[2][3] = 0.0f;

    result.m[3][0] = -(result.m[0][0] * m[3][0] + result.m[1][0] * m[3][1] + result.m[2][0] * m[3][2]);
    result.m[3][1] = -(result.m[0][1] * m[3][0] + result.m[1][1] * m[3][1] + result.m[2][1] * m[3][2]);
    result.m[3][2] = -(result.m[0][2] * m[3][0] + result.m[1][2] * m[3][1] + result.m[2][2] * m[3][2]);
    result.m[3][3] = 1.0f;

    return result;
}

#ifndef QT_NO_VECTOR3D
/*!
    Decomposes the current rotation matrix into an \a axis of rotation plus
    an \a angle.  The result can be used to construct an equivalent rotation
    matrix using glRotate().  It is assumed that the homogenous coordinate
    is 1.0.  The returned vector is guaranteed to be normalized.

    \code
        qreal angle;
        QVector3D axis;

        matrix.extractAxisAngle(angle, axis);
        glRotate(angle, axis[0], axis[1], axis[2]);
    \endcode

    \sa rotate()
*/
void QMatrix4x4::extractAxisRotation(qreal &angle, QVector3D &axis) const
{
    // Orientation is dependent on the upper 3x3 matrix; subtract the
    // homogeneous scaling element from the trace of the 4x4 matrix
    float tr = m[0][0] + m[1][1] + m[2][2];
    qreal cosa = qreal(0.5f * (tr - 1.0f));
    angle = acos(cosa) * 180.0f / M_PI;

    // Any axis will work if r is zero (means no rotation)
    if (qFuzzyIsNull(angle)) {
        axis.setX(1.0f);
        axis.setY(0.0f);
        axis.setZ(0.0f);
        return;
    }

    if (angle < 180.0f) {
        axis.xp = m[1][2] - m[2][1];
        axis.yp = m[2][0] - m[0][2];
        axis.zp = m[0][1] - m[1][0];
        axis.normalize();
        return;
    }

    // rads == PI
    float tmp;

    // r00 is maximum
    if ((m[0][0] >= m[2][2]) && (m[0][0] >= m[1][1])) {
        axis.xp = 0.5f * qSqrt(m[0][0] - m[1][1] - m[2][2] + 1.0f);
        tmp = 0.5f / axis.x();
        axis.yp = m[1][0] * tmp;
        axis.zp = m[2][0] * tmp;
    }

    // r11 is maximum
    if ((m[1][1] >= m[2][2]) && (m[1][1] >= m[0][0])) {
        axis.yp = 0.5f * qSqrt(m[1][1] - m[0][0] - m[2][2] + 1.0f);
        tmp = 0.5f / axis.y();
        axis.xp = tmp * m[1][0];
        axis.zp = tmp * m[2][1];
    }

    // r22 is maximum
    if ((m[2][2] >= m[1][1]) && (m[2][2] >= m[0][0])) {
        axis.zp = 0.5f * qSqrt(m[2][2] - m[0][0] - m[1][1] + 1.0f);
        tmp = 0.5f / axis.z();
        axis.xp = m[2][0]*tmp;
        axis.yp = m[2][1]*tmp;
    }
}

/*!
    If this is an orthonormal transformation matrix (e.g. only rotations and
    translations have been applied to the matrix, no scaling, or shearing)
    then the world translational component can be obtained by calling this function.

    This is most useful for camera matrices, where the negation of this vector
    is effectively the camera world coordinates.
*/
QVector3D QMatrix4x4::extractTranslation() const
{
    return QVector3D
        (m[0][0] * m[3][0] + m[0][1] * m[3][1] + m[0][2] * m[3][2],
         m[1][0] * m[3][0] + m[1][1] * m[3][1] + m[1][2] * m[3][2],
         m[2][0] * m[3][0] + m[2][1] * m[3][1] + m[2][2] * m[3][2], 1);
}
#endif

/*!
    Infers the special type of this matrix from its current elements.

    Some operations such as translate(), scale(), and rotate() can be
    performed more efficiently if the matrix being modified is already
    known to be the identity, a previous translate(), a previous
    scale(), etc.

    Normally the QMatrix4x4 class keeps track of this special type internally
    as operations are performed.  However, if the matrix is modified
    directly with operator()() or data(), then QMatrix4x4 will lose track of
    the special type and will revert to the safest but least efficient
    operations thereafter.

    By calling inferSpecialType() after directly modifying the matrix,
    the programmer can force QMatrix4x4 to recover the special type if
    the elements appear to conform to one of the known optimized types.

    \sa operator()(), data(), translate()
*/
void QMatrix4x4::inferSpecialType()
{
    // If the last element is not 1, then it can never be special.
    if (m[3][3] != 1.0f) {
        flagBits = General;
        return;
    }

    // If the upper three elements m12, m13, and m21 are not all zero,
    // or the lower elements below the diagonal are not all zero, then
    // the matrix can never be special.
    if (m[1][0] != 0.0f || m[2][0] != 0.0f || m[2][1] != 0.0f) {
        flagBits = General;
        return;
    }
    if (m[0][1] != 0.0f || m[0][2] != 0.0f || m[0][3] != 0.0f ||
        m[1][2] != 0.0f || m[1][3] != 0.0f || m[2][3] != 0.0f) {
        flagBits = General;
        return;
    }

    // Determine what we have in the remaining regions of the matrix.
    bool identityAlongDiagonal
        = (m[0][0] == 1.0f && m[1][1] == 1.0f && m[2][2] == 1.0f);
    bool translationPresent
        = (m[3][0] != 0.0f || m[3][1] != 0.0f || m[3][2] != 0.0f);

    // Now determine the special matrix type.
    if (translationPresent && identityAlongDiagonal)
        flagBits = Translation;
    else if (translationPresent)
        flagBits = (Translation | Scale);
    else if (identityAlongDiagonal)
        flagBits = Identity;
    else
        flagBits = Scale;
}

#ifndef QT_NO_DEBUG_STREAM

QDebug operator<<(QDebug dbg, const QMatrix4x4 &m)
{
    // Create a string that represents the matrix type.
    QByteArray bits;
    if ((m.flagBits & QMatrix4x4::Identity) != 0)
        bits += "Identity,";
    if ((m.flagBits & QMatrix4x4::General) != 0)
        bits += "General,";
    if ((m.flagBits & QMatrix4x4::Translation) != 0)
        bits += "Translation,";
    if ((m.flagBits & QMatrix4x4::Scale) != 0)
        bits += "Scale,";
    if ((m.flagBits & QMatrix4x4::Rotation) != 0)
        bits += "Rotation,";
    if (bits.size() > 0)
        bits = bits.left(bits.size() - 1);

    // Output in row-major order because it is more human-readable.
    dbg.nospace() << "QMatrix4x4(type:" << bits.constData() << endl
        << qSetFieldWidth(10)
        << m(0, 0) << m(0, 1) << m(0, 2) << m(0, 3) << endl
        << m(1, 0) << m(1, 1) << m(1, 2) << m(1, 3) << endl
        << m(2, 0) << m(2, 1) << m(2, 2) << m(2, 3) << endl
        << m(3, 0) << m(3, 1) << m(3, 2) << m(3, 3) << endl
        << qSetFieldWidth(0) << ')';
    return dbg.space();
}

#endif

#endif

QT_END_NAMESPACE