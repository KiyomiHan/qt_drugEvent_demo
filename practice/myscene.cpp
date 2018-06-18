#include "myscene.h"
#include <QMessageBox>
myScene::myScene(QObject *parent) :
    QGraphicsScene(parent)
{
    wel_item = new QGraphicsPixmapItem;
    wel_item->setPixmap(QPixmap(":/bj/bj0.jpg"));
    wel_item->setPos(QPoint(0,0));
    this->addItem(wel_item);

    cur_index = 1;
    count_apple = 0;

    role = nullptr;
    proxy = nullptr;
    apple_items = nullptr;
    princess_item = nullptr;
    get_apple_label = nullptr;
    get_apple_proxy = nullptr;

    timer = new QTimer();
    get_apple_timer = new QTimer();

    haschoosen = 0;
    connect(timer, SIGNAL(timeout()), this, SLOT(update_pos()));
    connect(get_apple_timer, SIGNAL(timeout()), this, SLOT(hide_get_apple()));
}

void myScene::receive_type(int mod)
{
    this->mod = mod;
    if(apple_items){
        for(int i = 0; i<apple_nums; i++){
            qDebug()<<"[remove]"<<"apple:"<<i;
            removeItem(&apple_items[i]);
        }
        delete []apple_items;
        apple_items = nullptr;
    }
    if(princess_item){
        qDebug()<<"[remove]"<<"princess";
        removeItem(princess_item);
        delete princess_item;
        princess_item = nullptr;
    }

    if(get_apple_proxy){
        qDebug()<<"[remove]"<<"get_proxy";
        removeItem(get_apple_proxy);
        delete get_apple_proxy;
        get_apple_proxy = nullptr;
    }
//    if(get_apple_label){
//        qDebug()<<"[remove]"<<"get_label";
//        delete get_apple_label;
//        get_apple_label = nullptr;
//    }
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    if(mod == 0){
        count_apple = 0;
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem;
        item->setPixmap(QPixmap(":/bj/bj1.jpg"));
        item->setPos(QPoint(0,0));
        this->addItem(item);
        // set apples
        apple_items = new QGraphicsPixmapItem[10];
        for(int i = 0 ; i<apple_nums; i++) {
            apple_items[i].setPixmap(QPixmap(":/ui/apple.png"));
            apple_items[i].setPos(QPoint(135,501-i*50));
            this->addItem(&apple_items[i]);
        }
        // set get apples
        get_apple_label = new QLabel();
        get_apple_label->setObjectName(QStringLiteral("get_apple_label"));
        QMovie *get_apples = new QMovie(":/ui/get_apple.gif");
        get_apple_label->setMovie(get_apples);
        get_apple_label->setScaledContents(true);
        get_apples->start();
        get_apple_proxy = this->addWidget(get_apple_label);
        get_apple_proxy ->setGeometry(QRect(0, 0, 30, 30));
         get_apple_proxy->setPalette(pal);
//        get_apple_proxy ->setPalette(pal);
        get_apple_proxy->hide();


    }
    else if(mod == 1){
        haschoosen = 0;
        QGraphicsPixmapItem *item = new QGraphicsPixmapItem;
        item->setPixmap(QPixmap(":/bj/bj2.jpg"));
        item->setPos(QPoint(0,0));
        this->addItem(item);
        // set princess
        princess_item = new QGraphicsPixmapItem;
        princess_item->setPixmap(QPixmap(":/ui/princess.png"));
        princess_item->setPos(QPoint(124,227));
        this->addItem(princess_item);
    }
    else if(mod == -1){
        QList<QGraphicsItem *> collideItems = collidingItems(wel_item);
        for (int i = collideItems.size()-1; i >= 0; --i)
                collideItems.at(i)->stackBefore(wel_item);
    }
    update();
    emit(send_update());
}

void myScene::receive_result(QVector<QPoint> pos, int cur_role)
{
    qDebug()<<"receive_result";
    curPos = pos;
//    for(int i =0; i<curPos.length();i++){
//        qDebug()<<curPos[i];
//    }
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    if(cur_role == 0){
        role = new QLabel();
        role->setObjectName(QStringLiteral("role_label"));
        QMovie *Gif = new QMovie(":/role/01.gif");
        role->setMovie(Gif);
        role->setScaledContents(true);
        Gif->start();
        proxy = this->addWidget(role);
        proxy->setGeometry(QRect(curPos[0].x(), curPos[0].y(), 50, 50));
        proxy->setPalette(pal);
//        update();
//        emit(send_update());
    }
    else{
        role = new QLabel();
        role->setObjectName(QStringLiteral("role_label"));
        QMovie *Gif = new QMovie(":/role/02.gif");
        role->setMovie(Gif);
        role->setScaledContents(true);
        Gif->start();
        proxy = this->addWidget(role);
        proxy->setGeometry(QRect(curPos[0].x(), curPos[0].y(), 50, 50));
        proxy->setPalette(pal);
//        update();
//        emit(send_update());
    }
    timer->start(400);
}

void myScene::update_pos()
{
    if(cur_index< curPos.length()){
        proxy->setGeometry(QRect(curPos[cur_index].x(), curPos[cur_index].y(), 50, 50));
        if(this->mod == 0){
            int cur_y_index = (501-curPos[cur_index].y())/50;
            if(curPos[cur_index].x()>=105 && curPos[cur_index].x()<=135 && (501-curPos[cur_index].y())%50==0){
                get_apple_proxy->setGeometry(QRect(curPos[cur_index].x()+70, curPos[cur_index].y()-20, 30, 30));
                get_apple_proxy->show();
                if(apple_items && (&apple_items[cur_y_index])){
                    if(apple_items[cur_y_index].isVisible())count_apple += 1;
                    qDebug()<<"cur count" <<count_apple;
                    apple_items[cur_y_index].hide();

                }
                get_apple_timer->start(2100);
            }
            if(count_apple == apple_nums){
                QMessageBox message(QMessageBox::NoIcon, "成功", "任务完成", QMessageBox::Ok);
                message.setWindowFlags(Qt::FramelessWindowHint);
                message.setIconPixmap(QPixmap(":/ui/finish.png"));
                message.setStyleSheet("QWidget{font: 16pt \"Wawati SC\"; background-color:rgb(250, 215, 215); color:rgb(136, 95, 98);border-radius:8px} "
                                      "QPushButton{border:0px solid white;border-radius:5px;background-color:rgb(218, 211, 235); min-width:28px;}"
                                      "QPushButton:hover{border:0px solid white; border-radius:5px; background-color:rgb(228, 221, 245);min-width:28px;}");
                message.exec();
                this->timer->stop();
            }
        }
        else if(this->mod == 1 && !haschoosen){
            if(curPos[cur_index].x()>=80 && curPos[cur_index].x()<=200 && curPos[cur_index].y()>=227 && curPos[cur_index].y()<=288){
                qDebug()<<"arrived princess";
                QMessageBox message(QMessageBox::NoIcon, "提示", "小公主迷路了，你要送她回城吗", QMessageBox::Yes|QMessageBox::No);
                message.setWindowFlags(Qt::FramelessWindowHint);
                message.setIconPixmap(QPixmap(":/ui/choosen_princess.gif"));
                message.setStyleSheet("QWidget{font: 16pt \"Wawati SC\"; background-color:rgb(250, 205, 205); color:rgb(136, 95, 98);}"
                                      "QPushButton{border:0px solid white;border-radius:5px;background-color:rgb(218, 211, 235);min-width:20px;}"
                                      "QPushButton:hover{border:0px solid white; border-radius:5px; background-color:rgb(228, 221, 245); min-width:20px;}");

                if(message.exec() == QMessageBox::Yes){
                    haschoosen = 1;
                    int last_length = curPos.length()-cur_index;
                    for(int i = 0; i<last_length; i++){
                        curPos.pop_back();
                    }
                    curPos.append(curPos.back()+QPoint(10,0));
                    for(int i = 0; i<25; i++){
                        curPos.append(curPos.back()+QPoint(0,-10));
                    }
                }
                else{
                    haschoosen = 2;

                }
            }
        }
        else if(this->mod == 1 && haschoosen==1){
            if(princess_item) princess_item->setPos(curPos[cur_index]+QPoint(30,20));
        }
        cur_index += 1;
        update();
        emit(send_update());
    }
    else{
        timer->stop();
        if(haschoosen==1){
            QMessageBox message(QMessageBox::NoIcon, "成功", "任务完成", QMessageBox::Ok);
            message.setWindowFlags(Qt::FramelessWindowHint);
            message.setIconPixmap(QPixmap(":/ui/finish.png"));
            message.setStyleSheet("QWidget{font: 16pt \"Wawati SC\"; background-color:rgb(250, 215, 215); color:rgb(136, 95, 98);border-radius:8px} "
                                  "QPushButton{border:0px solid white;border-radius:5px;background-color:rgb(218, 211, 235); min-width:28px}"
                                  "QPushButton:hover{border:0px solid white; border-radius:5px; background-color:rgb(228, 221, 245);min-width:28px;}");
            message.exec();
        }
    }
}

void myScene::clear_role()
{
    cur_index = 0;
    haschoosen = 0;
    count_apple = 0;
    if(proxy){
        qDebug()<<"[remove]"<<"proxy";
        removeItem(proxy);
        delete proxy;
        proxy = nullptr;
    }
//    if(role){
//        delete role;
//        role = nullptr;
//    }
    if(apple_items){
        for(int i = 0; i<apple_nums; i++){
            apple_items[i].show();
        }
    }
    qDebug()<<"myScene[101] OK clear role";
    update();
    emit(send_update());
}

void myScene::hide_get_apple()
{
    if(get_apple_proxy){
        get_apple_proxy->hide();
    }
    get_apple_timer->stop();
}
