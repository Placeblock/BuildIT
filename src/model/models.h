//
// Created by felix on 25.03.25.
//

#ifndef MODELS_H
#define MODELS_H
#include <memory>

struct Position {
    int x, y;
};

class Node {
    Position position;

public:
    virtual std::unique_ptr<char> serialize();

    virtual void deserialize(std::unique_ptr<char> data);

    virtual ~Node() = default;
};

class Text : public Node {
    std::string text;
};

class AndGate : public Node {
}


#endif //MODELS_H
