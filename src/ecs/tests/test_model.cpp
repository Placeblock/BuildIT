//
// Created by felix on 20.04.25.
//

#include "entt/entt.hpp"
#include "ecs/registry_node.hpp"
#include "ecs_history/gather_strategy/change_mixin.hpp"

struct rectangle_t {
    uint32_t x, y;
};


template<>
struct entt::storage_type<rectangle_t> {
    /*! @brief Type-to-storage conversion result. */
    using type = change_storage_t<rectangle_t>;
};

template<typename T>
void emplace(entt::registry &registry, const entt::entity entt, const T &value) {
    registry.emplace_or_replace<T>(entt, value);
}

int main() {
    ecs_net::serialization::initialize_component_meta_types();
    entt::meta_factory<rectangle_t>{}
        .func<ecs_net::serialization::deserialize_change_set<
            cereal::PortableBinaryInputArchive, rectangle_t> >("deserialize_change_set"_hs)
        .func<emplace<rectangle_t> >("emplace"_hs)
        .data<&rectangle_t::x, entt::as_ref_t>("x"_hs)
        .data<&rectangle_t::y, entt::as_ref_t>("y"_hs);

    entt::registry reg;
    ecs_net::registry_t net_reg{reg};
    net_reg.record_changes<entt::entity>();
    net_reg.record_changes<rectangle_t>();

    history_t history{net_reg};

    const auto entity = net_reg.create();
    reg.emplace<rectangle_t>(entity, static_cast<uint32_t>(1), static_cast<uint32_t>(2));

    ecs_net::commit_id_generator_t id_generator{};
    auto base_commit = net_reg.commit_changes();
    const auto base_commit_id = id_generator.next();
    history.add_commit(base_commit_id, base_commit);
    spdlog::info("created base commit");
    spdlog::info("entity version is now {}", net_reg.version_handler.get_version(0));

    reg.replace<rectangle_t>(entity, static_cast<uint32_t>(2), static_cast<uint32_t>(4));
    auto commit_1 = net_reg.commit_changes();
    const auto commit_1_id = id_generator.next();
    history.add_commit(commit_1_id, commit_1);
    spdlog::info("created commit 1");
    spdlog::info("entity version is now {}", net_reg.version_handler.get_version(0));

    auto [x, y] = reg.get<rectangle_t>(entity);
    spdlog::info("value of rectangle is now {}, {}", x, y);

    net_reg.apply_commit(history.commits.back().commit->invert());
    spdlog::info("rolled back commit 1");
    auto [x2, y2] = reg.get<rectangle_t>(entity);
    spdlog::info("value of rectangle is now {}, {}", x2, y2);
    spdlog::info("entity version is now {}", net_reg.version_handler.get_version(0));
    reg.replace<rectangle_t>(entity, static_cast<uint32_t>(4), static_cast<uint32_t>(8));
    auto commit_2 = net_reg.commit_changes();
    const auto commit_2_id = id_generator.next();
    const auto commit_2_base_id = base_commit_id;
    spdlog::info("created commit 2");
    net_reg.apply_commit(commit_2->invert());
    spdlog::info("rolled back commit 2");

    auto [x3, y3] = reg.get<rectangle_t>(entity);
    spdlog::info("value of rectangle is now {}, {}", x3, y3);
    spdlog::info("entity version is now {}", net_reg.version_handler.get_version(0));

    net_reg.apply_commit(*history.commits.back().commit);
    spdlog::info("reapplied commit 1");
    auto [x4, y4] = reg.get<rectangle_t>(entity);
    spdlog::info("value of rectangle is now {}, {}", x4, y4);
    spdlog::info("entity version is now {}", net_reg.version_handler.get_version(0));

    history.apply_commit(commit_2_base_id, commit_2_id, commit_2);

    return 1;
}
