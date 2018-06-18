#include "record.h"

Record::Record()
{

}

Record::~Record()
{
    delete this;
}

int Record::get_lenth()
{
    return records.length();
}

void Record::append_item(Item *i)
{
    records.push_back(i);
}

Item* Record::get_item(int index)
{
    if(index < records.length()){
        return records[index];
    }
}
void Record::delete_all()
{
    for(int i = 0; i<records.length(); i++){
        if(records[i]->r){
            delete records[i]->r;
        }
        delete records[i];
    }
    records.clear();
}

bool Record::is_item_recy(int index)
{
    if(index < records.length()){
        return records[index]->t == RECYCLE;
    }
    return false;
}

void Record::set_re(int index,int first)
{
    if(index<records.length()){
        records[index]->r->filed.first = first;
        records[index]->r->filed.second = index;
    }
}
