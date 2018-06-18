#ifndef GRAPH_H
#define GRAPH_H
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QList>
#include <QTimer>
#include <QLabel>
#include <QMovie>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QPoint>
#include <QVector>
class myScene : public QGraphicsScene
{
    Q_OBJECT
public:
    myScene(QObject *parent = 0);
private slots:
    void receive_type(int mod);
    void receive_result(QVector<QPoint> pos, int role);
    void update_pos();
    void clear_role();
    void hide_get_apple();
signals:
    void send_update();
private:
    QGraphicsPixmapItem *wel_item;

    QTimer *timer;
    QLabel *role;
    QGraphicsProxyWidget *proxy;
    QVector<QPoint> curPos;

    QGraphicsPixmapItem *apple_items;
    const int apple_nums = 7;

    QTimer *get_apple_timer;
    QLabel *get_apple_label;
    QGraphicsProxyWidget *get_apple_proxy;

    QGraphicsPixmapItem *princess_item;

    int cur_index;
    int mod;
    short haschoosen; // 0 not choosen 1 yes 2 no
    int count_apple;
};


#endif // GRAPH_H
