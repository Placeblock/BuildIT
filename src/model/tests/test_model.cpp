//
// Created by felix on 20.04.25.
//

#include "model/snapshot.hpp"
#include <cstdio>

#include <bitsery/traits/string.h>
#include <fstream>
#include <model/components.hpp>

using namespace buildit::ecs;

struct Test {
    inline static const std::string Key = "de.codelix:Test";
    uint8_t test;

    template<typename S>
    void serialize(S &s) {
        s.value1b(test);
    }
};

int main() {
    registry reg;

    const entity &entt = reg.create(128);
    reg.emplace<Test>(entt, 255);

    const std::string fileName = "test_file.buildit";
    std::fstream s{fileName, s.binary | s.trunc | s.out};

    if (!s.is_open()) {
        throw std::runtime_error{"Could not open file"};
    }

    serialization::serializer ser{s};

    serialization::registry_serializer reg_ser{&reg};
    reg_ser.serialize<Test>(ser);

    ser.adapter().flush();
    s.close();
}
