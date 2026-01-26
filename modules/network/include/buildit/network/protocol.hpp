//
// Created by felix on 1/18/26.
//

#ifndef BUILDIT_PROTOCOL_HPP
#define BUILDIT_PROTOCOL_HPP
#include <zmq_addon.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

constexpr uint32_t PROTOCOL_VERSION = 0;

template<typename T>
concept MessageType = requires(T t)
{
    { T::id } -> std::convertible_to<entt::id_type>;
    { T::version } -> std::convertible_to<uint32_t>;
};

namespace message_types {
struct request_registry {
    constexpr static entt::id_type id = entt::hashed_string("request_registry");
    constexpr static uint32_t version = 0;
};

struct registry {
    constexpr static entt::id_type id = entt::hashed_string("registry");
    constexpr static uint32_t version = 0;
};

struct commit {
    constexpr static entt::id_type id = entt::hashed_string("commit");
    constexpr static uint32_t version = 0;
};
}

class zmq_membuf final : public std::streambuf {
public:
    zmq_membuf(const void *data, const std::size_t size) {
        const auto p = static_cast<char *>(const_cast<void *>(data));
        setg(p, p, p + size);
    }
};

template<MessageType T>
void archive_header(cereal::PortableBinaryOutputArchive &archive) {
    archive(PROTOCOL_VERSION);
    uint32_t id = T::id;
    archive(id);
    spdlog::info("sending packet of type {}", T::id);
    archive(T::version);
}

template<MessageType T>
void deserialize_header(cereal::PortableBinaryInputArchive &archive) {
    uint32_t protocol_version;
    archive(protocol_version);
    if (protocol_version != PROTOCOL_VERSION) {
        throw std::runtime_error("received packet with invalid protocol version");
    }
    uint32_t id;
    archive(id);
    if (id != T::id) {
        spdlog::warn("received packet with invalid id {} but {} is required", id, T::id);
        throw std::runtime_error("received packet with invalid type");
    }
    uint32_t version;
    archive(version);
    if (version != T::version) {
        throw std::runtime_error("received packet with invalid version");
    }
}

inline std::vector<zmq::message_t> recv_multipart(zmq::socket_t &socket) {
    std::vector<zmq::message_t> recv_msgs;
    if (const auto ret = zmq::recv_multipart(socket, std::back_inserter(recv_msgs));
        !ret || recv_msgs.size() != 2) {
        throw std::runtime_error("receiving commit failed");
    }
    return recv_msgs;
}

struct message_input_t {
    zmq_membuf buffer;
    std::istream istream;
    cereal::PortableBinaryInputArchive archive;

    explicit message_input_t(zmq::message_t &msg) : buffer(msg.data(), msg.size()),
                                                    istream(&buffer),
                                                    archive(istream) {

    }
};

inline void get_archive(zmq::message_t &msg,
                        const std::function<void(cereal::PortableBinaryInputArchive &)> &
                        callback) {
    zmq_membuf buffer{msg.data(), msg.size()};
    std::istream istream{&buffer};
    cereal::PortableBinaryInputArchive archive{istream};
    callback(archive);
}

struct string_buffer {
    std::string commit_string;
    zmq::const_buffer buffer;
};

inline string_buffer get_buffer(
    const std::function<void(cereal::PortableBinaryOutputArchive &)> &callback) {
    std::stringstream oss{};
    cereal::PortableBinaryOutputArchive out_archive(oss);
    callback(out_archive);
    string_buffer result;
    result.commit_string = oss.str();
    result.buffer = zmq::const_buffer{result.commit_string.data(), result.commit_string.size()};
    return result;
}

#endif //BUILDIT_PROTOCOL_HPP