//
// Created by felix on 9/1/24.
//

#ifndef BUILDIT_COMPONENTFEATURE_H
#define BUILDIT_COMPONENTFEATURE_H

template<typename S>
class ComponentFeature {
protected:
    virtual S *getSubject() = 0;
};

#endif //BUILDIT_COMPONENTFEATURE_H
