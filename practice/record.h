#ifndef RECORD_H
#define RECORD_H
#include <QPair>
#include <QPoint>
#include <QVector>
enum TYPE { ROLE, RECYCLE, ACT};
typedef struct recy{
    int times;
    QPair<int, int> filed;
} rec;
typedef struct item
{
    TYPE t;
    rec *r;
    QPoint pos;
} Item;

class Record
{
public:
    Record();
    ~ Record();
    int get_lenth();
    void append_item(Item* i);
    Item* get_item(int index);
    void delete_all();
    bool is_item_recy(int index);
    void set_re(int index, int first);

private:
    QVector<Item*> records;
};

#endif // RECORD_H
