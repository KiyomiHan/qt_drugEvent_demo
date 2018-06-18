#include "unit.h"
#include "ui_unit.h"
#include <QStringList>
#include <QDebug>
#include <QPoint>
unit::unit(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::unit)
{
    ui->setupUi(this);
}

unit::~unit()
{
    delete ui;
}

void unit::set_label_text(QString text)
{
    QStringList curInfo = text.split(',');
    try{
        if(curInfo[0] == "act_gbox"){
            info = QPair<QString, int>(curInfo[0], curInfo[1].toInt());
            ui->pic_label->setPixmap(QPixmap(":/ui/act.png"));
            if(curInfo[1] == "0"){
                ui->text_label->setText("向左一步");
            }
            else if(curInfo[1] == "1"){
                ui->text_label->setText("向右一步");
            }
            else if(curInfo[1] == "2"){
                ui->text_label->setText("向前一步");
            }
            else if(curInfo[1] == "3"){
                ui->text_label->setText("向后一步");
            }
        }
        else if(curInfo[0] == "re_gbox"){
            ui->pic_label->setPixmap(QPixmap(":/ui/re.png"));
            ui->text_label->setText("次数:"+curInfo[1]);
            info = QPair<QString, int>(curInfo[0],curInfo[1].toInt());
        }
        else if(curInfo[0] == "role_gbox"){
            ui->pic_label->setPixmap(QPixmap(":/ui/role.png"));
            info = QPair<QString, int>(curInfo[0],curInfo[1].toInt());
            if(curInfo[1] == "0"){
                ui->text_label->setText("皮宝宝");
            }
            else if(curInfo[1] == "1"){
                ui->text_label->setText("龙猫");
            }
        }
    }catch(std::out_of_range& e){
        qDebug()<<e.what();
    }
}

void unit::set_label_objname(QString index)
{
    ui->widget->setObjectName(ui->widget->objectName()+"_"+index);
    ui->text_label->setObjectName(ui->text_label->objectName()+"_"+index);
    ui->pic_label->setObjectName(ui->pic_label->objectName()+"_"+index);
}

QPair<QString, int> unit::get_info()
{
    return info;
}
