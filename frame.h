#ifndef FRAME_H
#define FRAME_H

#include <QList>
#include "component.h"
#include "bone.h"

class Frame{

public:
    Frame(QList<CompInfo*> *c, QList<Bone*> *b);
    QList<CompInfo*> *components;
    QList<Bone*> *bones;
    Frame copy();
};

#endif // FRAME_H
