//
// Created by felix on 08.05.25.
//

#include "event/listener/create_chip_event.h"
#include <entt/entt.hpp>

using namespace buildit::event;

class test_sim_node final : public sim::node_t {
    sim::pin_t out;
    sim::pin_sink_t in;

public:
    void update(const std::function<void(const sim::pin_t *pin)> &on_updated) override {

    }

    [[nodiscard]] std::vector<sim::pin_t> get_pins() const override;

    [[nodiscard]] std::vector<sim::pin_sink_t> get_pin_sinks() const override;

};

class test_chip_type : public buildit::ecs::chip_type_t<>

int main() {
    buildit::ecs::chip_type_registry_t chip_type_registry;
    entt::registry registry;
    entt::dispatcher dispatcher;

    create_chip_event_handler_t handler{chip_type_registry, registry};

    dispatcher.sink<create_chip_event_t>().connect<&create_chip_event_handler_t::on>(handler);
}
