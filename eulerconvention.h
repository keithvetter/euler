#ifndef _EulerConvention_H
#define _EulerConvention_H

#include <QString>
#include <QVector3D>

class EulerConvention
{

public:
    enum RotationName { Roll=0, Pitch=1, Yaw=2 } ;

public:

    EulerConvention(const QVector3D& roll_axis  =QVector3D(1,0,0),
                    const QVector3D& pitch_axis =QVector3D(0,1,0),
                    const QVector3D& yaw_axis   =QVector3D(0,0,1));


    ~EulerConvention(){};

    QVector3D roll_axis() const;
    QVector3D pitch_axis() const;
    QVector3D yaw_axis() const;

    void set_axes(const QVector3D& roll_axis,
                  const QVector3D& pitch_axis,
                  const QVector3D& yaw_axis);

    QVector3D to_axis(const QString& str) const;

    static QString to_string(RotationName name);


private:

    QVector3D _roll_axis;
    QVector3D _pitch_axis;
    QVector3D _yaw_axis;

    bool _is_convention_proper(const QVector3D& roll_axis,
                               const QVector3D& pitch_axis,
                               const QVector3D& yaw_axis);
};
#endif // _EulerConvention_H
