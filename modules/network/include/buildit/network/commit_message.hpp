//
// Created by felix on 11.01.26.
//

#ifndef BUILDIT_COMMIT_MESSAGE_HPP
#define BUILDIT_COMMIT_MESSAGE_HPP
#include "ecs_history/commit.hpp"
#include "ecs_history/serialization/serialization.hpp"
#include <zmq.hpp>
#include <cereal/archives/portable_binary.hpp>

class zmq_membuf final : public std::streambuf {
public:
    zmq_membuf(const void *data, const std::size_t size) {
        const auto p = static_cast<char *>(const_cast<void *>(data));
        setg(p, p, p + size);
    }
};

struct commit_message_t {
    ecs_history::commit_id base_id{0, 0};
    ecs_history::commit_id id{0, 0};
    std::unique_ptr<ecs_history::commit_t> commit;
    const zmq::const_buffer header;
    const zmq::const_buffer commit_data;

    commit_message_t(const zmq::const_buffer &header, const zmq::const_buffer &commit_data)
        : header(header), commit_data(commit_data) {
        zmq_membuf header_membuf(header.data(), header.size());
        std::istream header_is(&header_membuf);
        cereal::PortableBinaryInputArchive header_archive(header_is);
        header_archive(this->base_id);
        header_archive(this->id);

        zmq_membuf commit_membuf(commit_data.data(), commit_data.size());
        std::istream commit_is(&commit_membuf);
        cereal::PortableBinaryInputArchive commit_archive(commit_is);
        this->commit = ecs_history::serialization::deserialize_commit(commit_archive);
    }

    commit_message_t(const ecs_history::commit_id &base_id,
                     const ecs_history::commit_id &id,
                     const zmq::const_buffer &header,
                     const zmq::const_buffer &commit_data)
        : base_id(base_id), id(id), header(header), commit_data(commit_data) {
    }
};

#endif //BUILDIT_COMMIT_MESSAGE_HPP