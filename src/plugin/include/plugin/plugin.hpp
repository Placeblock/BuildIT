//
// Created by felix on 25.05.25.
//

#ifndef PLUGIN_HPP
#define PLUGIN_HPP
#include <string>

// Register Circuitboard components
// Register Serialization handlers
// Register Copy handlers

extern "C" {
namespace buildit::plugin {

class plugin {
protected:
    const std::string name;

public:
    explicit plugin(std::string name);

    virtual ~plugin() = default;

    virtual void on_init() = 0;

    virtual void on_simulation_layer() = 0;

    virtual void on_server_layer() = 0;

    virtual void on_client_layer() = 0;

    virtual void on_serialization_layer() = 0;

    virtual void on_graphics_layer() = 0;
};

} // namespace buildit::plugin
}

#endif //PLUGIN_HPP
