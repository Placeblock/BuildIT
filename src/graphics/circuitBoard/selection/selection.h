//
// Created by felix on 8/9/24.
//

#ifndef BUILDIT_SELECTION_H
#define BUILDIT_SELECTION_H

#include <list>
#include <functional>

class Selectable;

class SelectionAccessor {
public:
    virtual std::list<Selectable*>* getSelected() = 0;
    virtual void clearSelection() = 0;
    virtual bool isSelected(Selectable* selectable) const = 0;

    virtual ~SelectionAccessor() = default;
};

class Selection final : public SelectionAccessor {
    std::list<Selectable*> selected;
public:
    void clearSelection() override;
    void select(Selectable *component);
    void deselect(Selectable *component);
    void deselectIf(std::function<bool(Selectable*)> predicate);
    std::list<Selectable*>* getSelected() override;
    bool isSelected(Selectable* selectable) const override;
};


#endif //BUILDIT_SELECTION_H
