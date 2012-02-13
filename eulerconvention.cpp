#include "eulerconvention.h"
#include <QDebug>


EulerConvention::EulerConvention(const QVector3D& roll_axis,
                                 const QVector3D& pitch_axis,
                                 const QVector3D& yaw_axis):
    _roll_axis(roll_axis),
    _pitch_axis(pitch_axis),
    _yaw_axis(yaw_axis)
{
    bool is_convention_proper = _is_convention_proper(roll_axis,
                                                      pitch_axis,
                                                      yaw_axis);
    // TODO: throw exception
    Q_ASSERT(is_convention_proper);
}

QVector3D EulerConvention::roll_axis() const
{
    return _roll_axis;
}

QVector3D EulerConvention::pitch_axis() const
{
    return _pitch_axis;
}

QVector3D EulerConvention::yaw_axis() const
{
    return _yaw_axis;
}


void EulerConvention::set_axes(const QVector3D& roll_axis,
                               const QVector3D& pitch_axis,
                               const QVector3D& yaw_axis)
{
    bool is_convention_proper = _is_convention_proper(roll_axis,
                                                      pitch_axis,
                                                      yaw_axis);
    Q_ASSERT(is_convention_proper);
    // TODO: throw exception
    _roll_axis  = roll_axis;
    _pitch_axis = pitch_axis;
    _yaw_axis   = yaw_axis;
}

QString EulerConvention::to_string(RotationName name)
{
    QString str;

    switch (name) {
    case Roll:
        str = QString("roll");
        break;
    case Pitch:
        str = QString("pitch");
        break;
    case Yaw:
        str = QString("yaw");
        break;
    }

    return str;
}

QVector3D EulerConvention::to_axis(const QString& str) const
{
    QVector3D axis;

    QString strl = str.toLower();

    if ( strl == QString("roll") ) {
        axis = _roll_axis;
    } else if ( strl == QString("pitch") ) {
        axis = _pitch_axis;
    } else if ( strl == QString("yaw") ) {
        axis = _yaw_axis;
    } else {
        Q_ASSERT(0);
        // TODO: throw exception on bad direction string
    }

    return axis;
}

bool EulerConvention::_is_convention_proper(const QVector3D& roll_axis,
                                            const QVector3D& pitch_axis,
                                            const QVector3D& yaw_axis)
{
    bool is_proper = false;

    if ( roll_axis.length() > 0 &&
         pitch_axis.length() > 0 &&
         yaw_axis.length() > 0 ) {

        QVector3D cross = QVector3D::crossProduct(roll_axis,pitch_axis);
        if ( cross == yaw_axis || cross == -yaw_axis ) {
            is_proper = true;
        }
    }

    return is_proper;

}
