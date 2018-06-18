#ifndef CAMDWIDGET_H
#define CAMDWIDGET_H

#include <QWidget>
#include <QVector>
#include <QMouseEvent>
#include <QPainter>
#include "unit.h"
#include "record.h"
typedef struct point{
    QPoint p;
    int num;
}*ptr_P, P ;

namespace Ui {
class camdWidget;
}

class camdWidget : public QWidget
{
    Q_OBJECT

public:
    explicit camdWidget(QWidget *parent = 0);
    ~camdWidget();
    int get_role();
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void addUnit(QString type);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * e);
    void paintEvent(QPaintEvent *);
private slots:
    void slot_return_record(bool);
    void clear_unit();

signals:
    void send_record(Record* );
private:
    Ui::camdWidget *ui;
    QVector<unit *> ui_unit;
    Record *r;
    QVector<ptr_P> lastPoints;
    QVector<ptr_P> endPoints;
    QPoint beginPos;
    int role;

    void append_lastPoints(ptr_P cur);
};

#endif // CAMDWIDGET_H
