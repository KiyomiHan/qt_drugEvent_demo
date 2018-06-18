#include "comdwidget.h"
#include "ui_camdwidget.h"
#include <QGroupBox>
#include <QDebug>
#include <QDrag>
#include <QDropEvent>
#include <QMimeData>
#include <QRect>
#include <QPen>
#include <QtWidgets/QLabel>
#include <QPair>
#include "toolbox.h"
camdWidget::camdWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::camdWidget)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    r = new Record();
    role = 0;
}

camdWidget::~camdWidget()
{
    delete ui;
}

int camdWidget::get_role()
{
    return role;
}

void camdWidget::dragEnterEvent(QDragEnterEvent *event)
{
    toolbox* source = qobject_cast<toolbox*>(
                event->source());
    if(source != NULL)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    camdWidget* source1 = qobject_cast<camdWidget*>(
                event->source());
    if(source1 != NULL)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }

}

void camdWidget::dragMoveEvent(QDragMoveEvent *event)
{
    toolbox* source = qobject_cast<toolbox*>(
                event->source());
    if(source != NULL)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
    camdWidget* source1 = qobject_cast<camdWidget*>(
                event->source());
    if(source1 != NULL)
    {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    }
}

void camdWidget::dropEvent(QDropEvent *event)
{
    toolbox* source = qobject_cast<toolbox*>(
                event->source());
    if(source != NULL)
    {
        const QMimeData* data = event->mimeData();
        QString text = data->text();
        addUnit(text);
        event->setDropAction(Qt::MoveAction);
        event->accept();
        return;
    }

    camdWidget* source1 = qobject_cast<camdWidget*>(
                event->source());
    if(source1 != NULL)
    {
        const QMimeData* data = event->mimeData();
        QString index = data->text();

        if(!r->is_item_recy(index.toInt())) return;
        event->setDropAction(Qt::MoveAction);
        event->accept();
        if(lastPoints.length()<1) return;
        ptr_P end = new P();
        end->p = QPoint(event->pos());
        end->num = (end->p.y()-10)/40;
        endPoints.append(end);
        qDebug()<<"[99]last length"<<lastPoints.length()<<"end length"<<endPoints.length();
//        qDebug()<<"**********************************";
//        for(int i = 0; i<lastPoints.length();i++){
//            qDebug()<<"lastpoint["<<i<<"]"<< lastPoints[i]->num<< lastPoints[i]->p;
//        }
//        for(int i = 0; i<endPoints.length();i++){
//            qDebug()<<"endPoints["<<i<<"]"<< endPoints[i]->num<< endPoints[i]->p;
//        }
//        qDebug()<<"**********************************";
        ptr_P tmp = lastPoints.back();
        qDebug()<<"连接"<<tmp->num<<end->num;
        r->set_re(tmp->num, end->num);
        update();
    }
}

void camdWidget::addUnit(QString type)
{
    int curIndex = ui_unit.length();
    QString s_curIndex = QString::number(curIndex);
    unit * curUnit = new unit(this);
    curUnit->set_label_objname("unit_"+s_curIndex);
    curUnit->setGeometry(QRect(90, curIndex*40+10, 101, 40));
    curUnit->set_label_text(type);
    curUnit->show();
    QPair<QString,int> info = curUnit->get_info();
    Item *cur_item = new Item();
    if(info.first=="re_gbox"){
        cur_item->t = RECYCLE;
        rec *r = new rec;
        r->times = info.second;
        cur_item->r = r;
        cur_item->pos = QPoint(0,0);
    }
    else if(info.first == "role_gbox"){
        cur_item->t = ROLE;
        cur_item->r = nullptr;
        cur_item->pos = QPoint(0,0);
        role = info.second;
    }
    else if(info.first == "act_gbox"){
        cur_item->t = ACT;
        cur_item->r = nullptr;
        if(info.second == 0){
            cur_item->pos = QPoint(-10,0);
        }
        else if(info.second == 1){
            cur_item->pos = QPoint(10, 0);
        }
        else if(info.second == 2){
            cur_item->pos = QPoint(0, -10);
        }
        else if(info.second == 3){
            cur_item->pos = QPoint(0, 10);
        }
    }
    r->append_item(cur_item);
    ui_unit.push_back(curUnit);
}
void camdWidget::mousePressEvent(QMouseEvent *event){
    unit *child = static_cast<unit*>(childAt(event->pos()));
//    qDebug()<< child->objectName();
    if (!child) return;
    QStringList text_index = child->objectName().split('_');
//    qDebug()<<"press:"<<r->is_item_recy(text_index.back().toInt());
    if(r->is_item_recy(text_index.back().toInt())){
        beginPos = event->pos();
        ptr_P last = new P();
        last->p = QPoint(event->pos());
        last->num = (last->p.y()-10)/40;
        append_lastPoints(last);
        qDebug()<<"[169]last length"<<lastPoints.length()<<"end length"<<endPoints.length();
        // test
//        qDebug()<<"**********************************";
//        for(int i = 0; i<lastPoints.length();i++){
//            qDebug()<<"lastpoint["<<i<<"]"<< lastPoints[i]->num<< lastPoints[i]->p;
//        }
//        for(int i = 0; i<endPoints.length();i++){
//            qDebug()<<"endPoints["<<i<<"]"<< endPoints[i]->num<< endPoints[i]->p;
//        }
//        qDebug()<<"**********************************";
    }
}

void camdWidget::mouseMoveEvent(QMouseEvent *e)
{
    unit *child = static_cast<unit*>(childAt(beginPos));
    if(!child) return;
    QString childName = child->objectName();

    if(e->buttons() & Qt::LeftButton)
    {
        QStringList text_index = childName.split('_');
        QString index = text_index.back();
//        qDebug()<< child->objectName()<<r->is_item_recy(index.toInt());
        if(r->is_item_recy(index.toInt())){
            if((e->pos() - beginPos).manhattanLength() >= QApplication::startDragDistance()){
                QMimeData *mimeData = new QMimeData;
                mimeData->setText(index);
                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->exec(Qt::MoveAction);
            }//if
        }
    }//if
}

void camdWidget::paintEvent(QPaintEvent *)
{
    QPainter pp(this);
    QPen pen;                                 //创建一个画笔
    pen.setColor(Qt::yellow);
    pen.setWidth(2);
    pp.setPen(pen);
//    qDebug()<<"**********************************";
//    for(int i = 0; i<lastPoints.length();i++){
//        qDebug()<<"lastpoint["<<i<<"]"<< lastPoints[i]->num<< lastPoints[i]->p;
//    }
//    for(int i = 0; i<endPoints.length();i++){
//        qDebug()<<"endPoints["<<i<<"]"<< endPoints[i]->num<< endPoints[i]->p;
//    }
//    qDebug()<<"**********************************";
//    qDebug()<<"[220]last length"<<lastPoints.length()<<"end length"<<endPoints.length();
    for(int i = 0; i<lastPoints.length()&& lastPoints.length() == endPoints.length(); i++){
        QPoint extension = lastPoints[i]->p+QPoint(70,0);
        QPoint y_extension = QPoint(extension.x(),endPoints[i]->p.y());
        pp.drawLine(lastPoints[i]->p, extension);
        pp.drawLine(extension, y_extension);
        pp.drawLine(endPoints[i]->p, y_extension);
    }
}

void camdWidget::slot_return_record(bool)
{
    qDebug()<<"send_record"<<r->get_lenth();
    emit(send_record(r));
}

void camdWidget::clear_unit()
{
    for(int i = 0; i < ui_unit.length(); i++){
        ui_unit[i] -> hide();
        delete ui_unit[i];
    }
    ui_unit.clear();
    for(int i= 0; i<lastPoints.length() && i<endPoints.length(); i++){
        delete lastPoints[i];
        delete endPoints[i];
    }
    r->delete_all();
    lastPoints.clear();
    endPoints.clear();
    update();
}

void camdWidget::append_lastPoints(ptr_P cur)
{
    for(int i = 0; i<lastPoints.length(); i++){
        if(lastPoints[i]->num == cur->num){
            lastPoints.remove(i);
            endPoints.remove(i);
            break;
        }
    }
    lastPoints.append(cur);
}

