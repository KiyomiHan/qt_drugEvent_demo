#include "toolbox.h"
#include "ui_toolbox.h"
#include <QMessageBox>
toolbox::toolbox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::toolbox)
{
    ui->setupUi(this);

    role_bp = new QButtonGroup;
    role_bp->addButton(ui->role1_radioButton, 0);
    role_bp->addButton(ui->role2_radioButton, 1);
    role_bp->setExclusive(true);

    act_bp = new QButtonGroup;
    act_bp->addButton(ui->left_ptn,0);
    act_bp->addButton(ui->right_ptn,1);
    act_bp->addButton(ui->up_ptn,2);
    act_bp->addButton(ui->down_ptn,3);
}

toolbox::~toolbox()
{
    delete ui;
}

void toolbox::mousePressEvent(QMouseEvent *event){

    QPushButton *ptn_child = static_cast<QPushButton*>(childAt(event->pos()));
    if (ptn_child){
        qDebug()<<"ptn";
        beginPos = event->pos();
        return;
    }
    QRadioButton *role_child = static_cast<QRadioButton*>(childAt(event->pos()));
    if (role_child){
        qDebug()<<"role";
        beginPos = event->pos();
        return;
    }
    QLineEdit *line_child = static_cast<QLineEdit*>(childAt(event->pos()));
    if (line_child){
        qDebug()<<"line";
        beginPos = event->pos();
        return;
    }
    QGroupBox *grpBox_child = static_cast<QGroupBox*>(childAt(event->pos()));
    if (grpBox_child){
        qDebug()<<"grp";
        beginPos = event->pos();
        return;
    }
//    QPushButton *ptn_child = static_cast<QPushButton*>(childAt(event->pos()));
//    if (ptn_child){
//        beginPos = event->pos();
//        return;
//    }
//    QRadioButton *role_child = static_cast<QRadioButton*>(childAt(event->pos()));
//    if (role_child){
//        beginPos = event->pos();
//        return;
//    }
//    QLineEdit *line_child = static_cast<QLineEdit*>(childAt(event->pos()));
//    if (line_child){
//        beginPos = event->pos();
//        return;
//    }
}

void toolbox::mouseMoveEvent(QMouseEvent *e)
{
    QGroupBox *child = static_cast<QGroupBox*>(childAt(beginPos));
    QString childName = child->objectName();
//    qDebug()<<childName;
    if(e->buttons() & Qt::LeftButton)
    {
        if((e->pos() - beginPos).manhattanLength() >= QApplication::startDragDistance()){
            if(childName == "role_gbox" || childName =="role1_radioButton" || childName=="role2_radioButton" ){
                //drag->setPixmap();
                int add = role_bp->checkedId();
                if(add == -1){
                    QMessageBox::information(this,"警告","选择角色",QMessageBox::Ok);
                    return;
                }
                QMimeData *mimeData = new QMimeData;
                QString text = "role_gbox," + QString::number(add);
                mimeData->setText(text);
                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->exec(Qt::MoveAction);
            }
            else if(childName == "re_gbox" || childName == "recount_lineEdit"){
                QString add = ui->recount_lineEdit->text();
                if(add==""){
                    QMessageBox::information(this,"警告","请输入循环次数",QMessageBox::Ok);
                    return;
                }
                QMimeData *mimeData = new QMimeData;
                QString text = childName + "," + add;
                mimeData->setText(text);
                drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->exec(Qt::MoveAction);
            }
            else if(childName == "act_gbox"||childName =="right_ptn"||childName =="left_ptn"||childName =="down_ptn"||childName=="up_ptn"){
                int add = act_bp->checkedId();
                if(add == -1){
                    QMessageBox::information(this,"警告","选择方向",QMessageBox::Ok);
                    return;
                }
                QMimeData *mimeData = new QMimeData;
                QString text = "act_gbox," + QString::number(add);
                mimeData->setText(text);
                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->exec(Qt::MoveAction);
                delete drag;
                drag = nullptr;
            }
            else{
                return;
            }
        }
    }//if
}
