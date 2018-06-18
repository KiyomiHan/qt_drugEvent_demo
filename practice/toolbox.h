#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QWidget>
#include <QDrag>
#include <QMouseEvent>
#include <QMimeData>
#include <QDebug>
#include <QByteArray>
#include <QPoint>
#include <QDataStream>
#include <QButtonGroup>
namespace Ui {
class toolbox;
}

class toolbox : public QWidget
{
    Q_OBJECT

public:
    explicit toolbox(QWidget *parent = 0);
    ~toolbox();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent * e);

private:
    Ui::toolbox *ui;
    QPoint beginPos;
    QButtonGroup *role_bp;
    QButtonGroup *act_bp;
    QDrag *drag;
};

#endif // TOOLBOX_H
