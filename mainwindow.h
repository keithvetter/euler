#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTableView>
#include <QStandardItemModel>
#include <QMatrix4x4>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QColor>
#include <QModelIndex>
#include "eulerconvention.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QGraphicsView* _gview;
    QGraphicsScene* _scene;
    QTableView* _sequenceview;
    QTableView* _matrixview;

    QStandardItemModel* _model;
    QMatrix4x4 _mat;
    QModelIndex _idx_seq1 ;
    QModelIndex _idx_seq2 ;
    QModelIndex _idx_seq3 ;
    QModelIndex _idx_ang1 ;
    QModelIndex _idx_ang2 ;
    QModelIndex _idx_ang3 ;

    QGraphicsLineItem* _xinertial;
    QGraphicsLineItem* _yinertial;
    QGraphicsLineItem* _zinertial;
    int _inertial_len;

    QGraphicsLineItem* _xitem;
    QGraphicsLineItem* _yitem;
    QGraphicsLineItem* _zitem;
    int _axis_len;
    QMatrix4x4 _proj_mat;


    EulerConvention _euler_convention;

    void _update_scene_matrix(const QMatrix4x4& mat);

private slots:
    void _slot_model_data_changed(const QModelIndex& idx1,
                                  const QModelIndex& idx2);

};

#endif // MAINWINDOW_H
