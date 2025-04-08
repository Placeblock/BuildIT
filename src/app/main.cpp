#include <iostream>
#include <memory>
#include <thread>
#include <entt/entt.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/xml.hpp>

#include "components.hpp"
#include "graph.hpp"
#include "simulation.hpp"
#include "storage/serialization.hpp"

int updates = 0;

/*[[noreturn]] void measure() {
    while (true) {
        std::cout << updates << "\n";
        updates = 0;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


int main(int argc, char *argv[]) {
    std::thread measureThread(measure);
    measureThread.detach();
    auto graph = std::make_unique<Sim::Graph>();
    const auto notNode = std::make_unique<Sim::NotNode>();
    Sim::Graph::connect(notNode->outputPins[0].get(), notNode.get(), 0);
    Sim::simulation simulation(graph);
    simulation.update(notNode.get());
    while (!simulation.isEmpty()) {
        simulation.pollAndUpdate();
        updates++;
    }
}*/

#include <fstream>

int main(int argc, char *argv[]) {
    entt::registry reg;
    auto entity = reg.create();
    reg.emplace<Models::Position>(entity, 1, 3);
    reg.emplace<Models::Rotation>(entity, 2);
    auto entity2 = reg.create();
    reg.emplace<Models::Rotation>(entity2, 4);

    {
        std::ofstream outFile("cerealTest.xml");
        cereal::XMLOutputArchive output{outFile};
        Models::Serialization archive{output};
        entt::snapshot{reg}
            .get<entt::entity>(archive)
            .get<Models::Position>(archive)
            .get<Models::Rotation>(archive);
    }

    entt::registry dest;
    {
        std::ifstream inFile("cerealTest.xml");
        cereal::XMLInputArchive input{inFile};
        entt::snapshot_loader{dest}
            .get<entt::entity>(input)
            .get<Models::Position>(input)
            .get<Models::Rotation>(input);
    }
    for (auto loaded : dest.view<Models::Rotation>()) {
        printf("%d\n", loaded);
    }
}
