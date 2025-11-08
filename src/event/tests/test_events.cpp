//
// Created by felix on 08.05.25.
//

#include "event/listener/create_chip_event.h"
#include <entt/entt.hpp>

using namespace buildit::event;

struct test_sim_node final : sim::node_t {
    sim::pin_t<bool> out{0, false};
    sim::pin_sink_t<bool> in{0};

    void update(const std::function<void(const sim::base_pin_t *pin)> &on_updated) override {
        if (!*this->in.pin_value != this->out.value) {
            this->out.value = !*this->in.pin_value;
            on_updated(&this->out);
        }
    }

    [[nodiscard]] std::vector<sim::base_pin_t *> get_pins() override {
        return {&out};
    }

    [[nodiscard]] std::vector<sim::base_pin_sink_t *> get_pin_sinks() override {
        return {&in};
    }
};

struct test_graphics_component_t {
    bool out;
};

class test_chip_type final : public buildit::ecs::chip_type_t<
        test_sim_node, test_graphics_component_t> {
public:
    test_chip_type()
        : chip_type_t("de.codelix:test", 3, 3) {
    }

    [[nodiscard]] simulation_type create_chip() const override {
        return test_sim_node{};
    }

    [[nodiscard]] graphics_type get_graphics(simulation_type *sim) const override {
        return {sim->out.value};
    }

};

int main() {
    buildit::ecs::chip_type_registry_t chip_type_registry;
    entt::registry registry;
    entt::dispatcher dispatcher;

    create_chip_event_handler_t handler{chip_type_registry, registry};

    dispatcher.sink<create_chip_event_t>().connect<&create_chip_event_handler_t::on>(handler);

    std::unique_ptr<buildit::ecs::base_chip_type_t> chip_type = std::make_unique<test_chip_type>();
    chip_type_registry.register_chip_type("de.codelix:test", chip_type);

    dispatcher.enqueue<create_chip_event_t>(nullptr, "de.codelix:test", glm::vec2{0, 0});
    dispatcher.update();
}
