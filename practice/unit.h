#ifndef UNIT_H
#define UNIT_H

#include <QWidget>
#include <QPair>
namespace Ui {
class unit;
}

class unit : public QWidget
{
    Q_OBJECT

public:
    explicit unit(QWidget *parent = 0);
    ~unit();
    void set_label_text(QString text);
    void set_label_objname(QString name);
    QPair<QString,int> get_info();
private:
    Ui::unit *ui;
    QPair<QString,int> info;
};

#endif // UNIT_H
