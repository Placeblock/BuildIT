//
// Created by felix on 8/23/24.
//

#ifndef BUILDIT_NODEUPDATER_H
#define BUILDIT_NODEUPDATER_H


class NodeUpdater {
private:
    bool update = false;
public:
    void onUpdate() {
        update = false;
    }
    void reset() {update = true;}
    [[nodiscard]] bool shouldUpdate() const {return this->update;}
};


#endif //BUILDIT_NODEUPDATER_H
