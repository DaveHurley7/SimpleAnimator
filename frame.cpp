#include "frame.h"

Frame::Frame(QList<CompInfo*> *c, QList<Bone*> *b){
    components = c;
    bones = b;
}

Frame Frame::copy(){
    QList<CompInfo*> *fComps = new QList<CompInfo*>;
    for(CompInfo *c : *components){
        fComps->append(new CompInfo(c->cIndex,c->coord,c->angle,c->leq));
    }
    QList<Bone*> *fBones = new QList<Bone*>;
    for(Bone *b : *bones){
        fBones->append(new Bone(b->coords,b->from,b->to));
    }
    return Frame(fComps,fBones);
}
