#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QtGui>
#include <QTabWidget>
#include <QSplitter>
#include <QFrame>
#include <QIcon>
#include <QHeaderView>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _axis_len(97),_inertial_len(150)
{
    ui->setupUi(this);

    // Model
    _model = new QStandardItemModel(5,3,parent);

    _idx_seq1 = _model->index(0,0);
    _idx_seq2 = _model->index(0,1);
    _idx_seq3 = _model->index(0,2);
    _idx_ang1 = _model->index(1,0);
    _idx_ang2 = _model->index(1,1);
    _idx_ang3 = _model->index(1,2);

    _model->setData(_idx_seq1,QString("Roll"));
    _model->setData(_idx_seq2,QString("Pitch"));
    _model->setData(_idx_seq3,QString("Yaw"));
    _model->setHeaderData(0,
                          Qt::Horizontal,
                          _model->data(_idx_seq1).toString());
    _model->setHeaderData(1,
                          Qt::Horizontal,
                          _model->data(_idx_seq2).toString());
    _model->setHeaderData(2,
                          Qt::Horizontal,
                          _model->data(_idx_seq3).toString());

    _model->setData(_idx_ang1,0.0);
    _model->setData(_idx_ang2,0.0);
    _model->setData(_idx_ang3,0.0);


    for ( int ii = 0 ; ii < 3 ; ii++ ) {
        for ( int jj = 0 ; jj < 3 ; jj++ ) {
            QModelIndex idx = _model->index(ii+2,jj);
            qreal val = _mat(ii,jj);
            _model->setData(idx,val);
        }
    }

    // Minimum size policy ( i don't care about growing)
    QSizePolicy size_policy_min(QSizePolicy::Minimum, QSizePolicy::Minimum);

    QSizePolicy size_policy_fixed(QSizePolicy::Fixed,
                                  QSizePolicy::Fixed);

    // Sequence view
    _sequenceview = new QTableView(parent);
    _sequenceview->setModel(_model);
    _sequenceview->verticalHeader()->setVisible(false);
    _sequenceview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _sequenceview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _sequenceview->horizontalHeader()->setMovable(true);
    for ( int ii = 0 ; ii < 6; ii++ ) {
        _sequenceview->setRowHidden(ii,true);
    }
    _sequenceview->setRowHidden(1,false);

    // Matrix view
    _matrixview = new QTableView(parent);
    _matrixview->setModel(_model);
    _matrixview->horizontalHeader()->setVisible(false);
    _matrixview->verticalHeader()->setVisible(false);
    _matrixview->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _matrixview->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    _matrixview->setRowHidden(0,true);
    _matrixview->setRowHidden(1,true);

    // Scene view
    _scene = new QGraphicsScene(parent);
    _scene->setBackgroundBrush(QBrush(Qt::white));
    _gview = new QGraphicsView(_scene);
    _gview->setRenderHint(QPainter::Antialiasing);
    _gview->horizontalScrollBar()->setVisible(false);
    _gview->verticalScrollBar()->setVisible(false);
    _gview->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    _xinertial = new QGraphicsLineItem(QLine(QPoint(0,0),
                                       QPoint(_inertial_len,0)));
    _scene->addItem(_xinertial);
    QGraphicsSimpleTextItem* xlabel = new
            QGraphicsSimpleTextItem(QString("x"));
    _scene->addItem(xlabel);

    QRectF bb = xlabel->boundingRect();
    xlabel->setPos(_inertial_len+10,-bb.height()/2.0);


    _yinertial = new QGraphicsLineItem(QLine(QPoint(0,0),
                                       QPoint(0,-_inertial_len)));
    _scene->addItem(_yinertial);
    QGraphicsSimpleTextItem* ylabel = new QGraphicsSimpleTextItem(QString("y"));
    _scene->addItem(ylabel);
    bb = ylabel->boundingRect();
    ylabel->setPos(-bb.width()/2.0,-_inertial_len-10-bb.height()/2.0 );

    _xitem = new QGraphicsLineItem(QLine(QPoint(0,0),QPoint(_axis_len,0)));
    QPen xpen(Qt::red);
    xpen.setWidth(6);

    _xitem->setPen(xpen);
    _yitem = new QGraphicsLineItem(QLine(QPoint(0,0),QPoint(0,-_axis_len)));
    QPen ypen(Qt::green);
    ypen.setWidth(6);
    _yitem->setPen(ypen);
    _zitem = new QGraphicsLineItem(QLine(QPoint(0,0),QPoint(0,0)));
    QPen zpen(Qt::blue);
    zpen.setWidth(6);
    _zitem->setPen(zpen);

    QGraphicsEllipseItem* ell = new QGraphicsEllipseItem(
                                     -100,-100,200,200);
    _scene->addItem(_xitem);
    _scene->addItem(_yitem);
    _scene->addItem(_zitem);
    _scene->addItem(ell);
    QPointF center = _scene->itemsBoundingRect().center();
    _gview->centerOn(center);


    // Connect
    connect(_model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),
            this,SLOT(_slot_model_data_changed(QModelIndex,QModelIndex)));

    //
    // Widget Layout
    //
    QWidget *mainwidget = new QWidget;
    setCentralWidget(mainwidget);
    setWindowTitle(tr("Euler"));
    setUnifiedTitleAndToolBarOnMac(true);

    QHBoxLayout *mainlayout = new QHBoxLayout;
    QVBoxLayout *leftbox = new QVBoxLayout;

    leftbox->addWidget(_sequenceview);
    leftbox->addWidget(_matrixview);

    mainlayout->addLayout(leftbox);
    mainlayout->addWidget(_gview);

    mainwidget->setLayout(mainlayout);
}

void MainWindow::_slot_model_data_changed(const QModelIndex& idx1,
                                          const QModelIndex& idx2)
{
    if ( idx1.row() > 1 ) {
        return ;
    }


    QString seq1 = _model->data(_idx_seq1).toString();
    QString seq2 = _model->data(_idx_seq2).toString();
    QString seq3 = _model->data(_idx_seq3).toString();
    QVector3D axis1 = _euler_convention.to_axis(seq1);
    QVector3D axis2 = _euler_convention.to_axis(seq2);
    QVector3D axis3 = _euler_convention.to_axis(seq3);

    qreal ang1 = _model->data(_idx_ang1).toDouble();
    qreal ang2 = _model->data(_idx_ang2).toDouble();
    qreal ang3 = _model->data(_idx_ang3).toDouble();

    _mat.setToIdentity();
    _mat.rotate(ang1,axis1);
    _mat.rotate(ang2,axis2);
    _mat.rotate(ang3,axis3);

    for ( int ii = 0 ; ii < 3 ; ii++ ) {
        for ( int jj = 0 ; jj < 3 ; jj++ ) {
            QModelIndex idx = _model->index(ii+2,jj);
            qreal val = _mat(ii,jj);
            _model->setData(idx,val);
        }
    }

    _update_scene_matrix(_mat);

}

void MainWindow::_update_scene_matrix(const QMatrix4x4& mat)
{
    QMatrix4x4 moo;

    QVector3D eye(sqrt(2.0)*_axis_len/2.0,
                  0,
                  sqrt(2.0)*_axis_len/2.0);
    QVector3D center(0,0,0);
    QVector3D up(0,1,0);

    //moo.lookAt(eye,center,up);

    QVector4D xvec(_axis_len,0,0,1);
    QVector4D yvec(0,_axis_len,0,1);
    QVector4D zvec(0,0,_axis_len,1);

    xvec = moo*_mat*xvec;
    yvec = moo*_mat*yvec;
    zvec = moo*_mat*zvec;

    _xitem->setLine(0,0,xvec.x(),-xvec.y());
    _yitem->setLine(0,0,yvec.x(),-yvec.y());
    _zitem->setLine(0,0,zvec.x(),-zvec.y());
}

MainWindow::~MainWindow()
{
    delete ui;
}
