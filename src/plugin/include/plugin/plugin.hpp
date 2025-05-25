//
// Created by felix on 25.05.25.
//

#ifndef PLUGIN_HPP
#define PLUGIN_HPP
#include <string>

// Register Circuitboard components
// Register Serialization handlers
// Register Copy handlers

class plugin {
protected:
    const std::string name;

public:
    explicit plugin(std::string name);
    virtual ~plugin() = default;

    virtual void on_enable() = 0;
    virtual void on_disable() = 0;
};

#endif //PLUGIN_HPP
