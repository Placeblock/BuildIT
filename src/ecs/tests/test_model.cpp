//
// Created by felix on 20.04.25.
//

#include "model/snapshot.hpp"
#include <cstdio>

#include <bitsery/traits/string.h>
#include <fstream>
#include <iostream>
#include <model/components.hpp>

using namespace buildit::ecs;

struct Test {
    static inline const std::string Key = "de.codelix:Test";
    uint8_t test;

    template<typename S>
    void serialize(S &s) {
        s.value1b(test);
    }
};

int main() {
    const std::string fileName = "test_file.buildit";
    global_entity global_entt = 128;

    registry reg;

    const entity &entt = reg.create(global_entt);
    reg.emplace<Test>(entt, 255);

    std::ofstream s{fileName, std::ofstream::binary | std::ofstream::trunc};

    if (!s.is_open()) {
        throw std::runtime_error{"Could not open file"};
    }

    serializer ser{s};

    const registry_serializer reg_ser{&reg};
    reg_ser.serialize<Test>(ser);

    ser.adapter().flush();
    s.close();

    std::ifstream ls{fileName, std::ifstream::binary};

    if (!ls.is_open()) {
        std::cerr << "Error: " << strerror(errno);
        throw std::runtime_error{"Could not open file"};
    }

    deserializer deser{ls};

    loader_registry loader_reg;
    loader_reg.register_loader<Test>();
    registry load_reg;
    registry_deserializer reg_deser{&load_reg, &loader_reg};
    reg_deser.deserialize(deser);

    Test test = load_reg.get<Test>(global_entt);

    return 1;
}
