#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "record.h"
#include <QLabel>
#include <QMovie>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    mod_radioptn = new QButtonGroup();
    mod_radioptn->addButton(ui->radioButton, 0);
    mod_radioptn->addButton(ui->radioButton_2, 1);
    mod_radioptn->setExclusive(true);

    comd = new camdWidget(ui->comd_widget);
    connect(ui->run_pushButton,SIGNAL(clicked(bool)),comd,SLOT(slot_return_record(bool)));
    connect(comd, SIGNAL(send_record(Record*)), this, SLOT(get_Records(Record*)));
    connect(this, SIGNAL(send_clear_unit()), comd, SLOT(clear_unit()));
    toolboxWidget = new toolbox(ui->tool_widget);

    sence = new myScene;
    connect(this, SIGNAL(send_mod_map(int)), sence, SLOT(receive_type(int)));
    connect(sence, SIGNAL(send_update()), this, SLOT(rece_update()));
    connect(this, SIGNAL(send_info(QVector<QPoint>,int)), sence, SLOT(receive_result(QVector<QPoint>,int)));
    connect(this, SIGNAL(send_clean_role()), sence, SLOT(clear_role()));
    ui->graphicsView->setScene(sence);

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_run_pushButton_clicked()
{
    /* run button clicked */
    emit(send_clean_role());
    // initial
    // compile()
//    QLabel *role = new QLabel(ui->result_widget);
//    role->setObjectName(QStringLiteral("role_label"));
//    role->setGeometry(QRect(10, 30, 30, 30));
//    role->show();
//    QMovie *Gif = new QMovie(":/role/01");
//    role->setMovie(Gif);
//    role->setScaledContents(true);
//    Gif->start();
//    role->setGeometry(QRect(100, 200, 30, 30));
}

void MainWindow::get_Records(Record * cur)
{
    r = cur;
    qDebug()<<"received record"<<r->get_lenth();
    if(compile()){
        int get_role = comd->get_role();
        qDebug()<<"send role"<<get_role;
        emit(send_info(curpos, get_role));
    }
}

bool MainWindow::compile()
{
    curpos.clear();
    if(!r){
        qDebug()<<"records is NULL";
    }
    if(r->get_lenth()==0){return 0;}
    QPoint cur;
    for(int i = 0; i < r->get_lenth(); i++){
        Item * tmp = r->get_item(i);
       //check role
        if(i == 0){
            if(!(tmp->t == ROLE)){
                QMessageBox::warning(this,"编译错误", "请先选择角色",QMessageBox::Ok);
                return 0;
            }
            curpos.append(QPoint(120,541));
            continue;
        }
        if(tmp->t == ACT){
            curpos.append(curpos.back()+tmp->pos);
        }
        if(tmp->t == RECYCLE){
            int another = tmp->r->filed.first;
            int times = tmp->r->times;
            qDebug()<<"times:"<<times << tmp->r->filed;
            if(i > another){
                for(int k = 0; k<times; k++){
                    for(int j= another; j<i; j++){
//                        qDebug()<<"[91] mainwindow.type"<<r->get_item(j)->t;
                        if(r->get_item(j)->t == ACT){
                            curpos.append(curpos.back()+r->get_item(j)->pos);
                        }
                        else{
                            QMessageBox::warning(this, "编译错误","不能嵌套循环",QMessageBox::Ok);
                            return 0;
                        }
                        cur = curpos.back();
                        if(cur.y()<0 || cur.x()<0 ||cur.y()>590 ||cur.x()>300){
                             QMessageBox::warning(this, "编译错误","超出画布范围",QMessageBox::Ok);
                             return 0;
                        }
                    }
                }
            }
            else{
                for(int k = 0; k<times; k++){
                    for(int j= i+1; j<=another;j++){
//                        qDebug()<<"[106] mainwindow.type"<<r->get_item(j)->t;
                        if(r->get_item(j)->t == ACT){
                            curpos.append(curpos.back()+r->get_item(j)->pos);
                        }
                        else{
                            QMessageBox::warning(this, "编译错误","不能嵌套循环",QMessageBox::Ok);
                            return 0;
                        }
                        cur = curpos.back();
                        if(cur.y()<0 || cur.x()<0 ||cur.y()>590 ||cur.x()>300){
                             QMessageBox::warning(this, "编译错误","超出画布范围",QMessageBox::Ok);
                             return 0;
                        }
                    }
                }
            }
        }
        cur = curpos.back();
        if(cur.y()<0 || cur.x()<0 ||cur.y()>590 ||cur.x()>300){
             QMessageBox::warning(this, "编译错误","超出画布范围",QMessageBox::Ok);
             return 0;
        }
    }

    return 1;

}
//void deal_recycle(Item* tmp){

//}


void MainWindow::on_mod_ptn_clicked()
{
    QString content;
    QPixmap pic;
    int choosen = mod_radioptn->checkedId();
    if(  choosen == 0){
        /* 循环 */
        content = "循环练习！我们一起捡起所有的小苹果吧！";
        pic = QPixmap(":/ui/message_apple.png");

    }
    else if( choosen == 1){
        /* 条件 */
        content = "条件选择练习！邂逅小公主，看看有什么惊喜！";
        pic = QPixmap(":/ui/message_princess.png");
    }
    else{
        return;
    }
    QMessageBox message(QMessageBox::NoIcon, "提示", content, QMessageBox::Ok);
    message.setWindowFlags(Qt::FramelessWindowHint);
    message.setIconPixmap(pic);
    message.setStyleSheet("QWidget{font: 16pt \"Wawati SC\"; background-color:rgb(250, 205, 205); color:rgb(136, 95, 98);} QPushButton{border:0px solid white;border-radius:5px; background-color:rgb(218, 211, 235); min-width:28px;} QPushButton:hover{border:0px solid white;border-radius:5px;background-color:rgb(228, 221, 245);min-width:28px;}");
    message.button(QMessageBox::Ok)->setStyleSheet("min-width:20px");

    if(message.exec() == QMessageBox::Ok){
        emit(send_mod_map(choosen));
    }
    ui->widget->hide();
}

void MainWindow::rece_update()
{
    ui->graphicsView->update();
}

void MainWindow::on_restart_ptn_clicked()
{
    emit(send_mod_map(-1));
    emit(send_clear_unit());
    emit(send_clean_role());
    ui->widget->show();
}

void MainWindow::on_clear_pushButton_clicked()
{
    emit(send_clear_unit());
    emit(send_clean_role());
    curpos.clear();
}

void MainWindow::on_close_pushButton_clicked()
{
    this->close();
}
