//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H


#include <set>
#include <memory>
#include <unordered_set>
#include <list>

class Selectable;

class SelectionAccessor {
public:
    virtual std::list<Selectable*>* getSelected() = 0;
    virtual void clearSelection() = 0;
};

class Selection : public SelectionAccessor {
private:
    std::list<Selectable*> selected;
public:
    void clearSelection() override;
    void select(Selectable *component);
    void deselect(Selectable *component);
    std::list<Selectable*>* getSelected() override;
};


#endif //BUILDIT_SELECTION_H
