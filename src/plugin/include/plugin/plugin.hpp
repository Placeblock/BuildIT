//
// Created by felix on 25.05.25.
//

#ifndef PLUGIN_HPP
#define PLUGIN_HPP
#include "ecs/circuitboard.h"
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

    virtual void on_init(buildit::ecs::circuitboard_registry& reg) = 0;

    virtual void on_simulation_layer() = 0;

    virtual void on_server_layer() = 0;

    virtual void on_client_layer() = 0;

    virtual void on_serialization_layer() = 0;

    virtual void on_graphics_layer() = 0;
};

#endif //PLUGIN_HPP
