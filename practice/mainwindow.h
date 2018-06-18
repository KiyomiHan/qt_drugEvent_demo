#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QButtonGroup>
#include <QPoint>
#include "myscene.h"
#include "unit.h"
#include "comdwidget.h"
#include "toolbox.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_run_pushButton_clicked();
    void get_Records(Record*);
    void on_mod_ptn_clicked();
    void rece_update();
    void on_restart_ptn_clicked();

    void on_clear_pushButton_clicked();

    void on_close_pushButton_clicked();

signals:
    void send_mod_map(int mod);
    void send_clear_unit();
    void send_info(QVector<QPoint>, int role);
    void send_clean_role();
private:
    Ui::MainWindow *ui;
    camdWidget *comd;
    toolbox *toolboxWidget;
    Record *r;
    myScene *sence;
    QButtonGroup *mod_radioptn;
    bool compile();
    QVector<QPoint> curpos;
};

#endif // MAINWINDOW_H
