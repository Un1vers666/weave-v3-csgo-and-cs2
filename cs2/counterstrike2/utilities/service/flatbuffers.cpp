#include "flatbuffers.hpp"
#include <lz4/lz4.h>
#include <time.h>
#include "../UnixTime.h"
#include "../tls/tls.h"

void c_flatbuffers::create_stream(bool has_compressed)
{
    auto span = m_builder.GetBufferSpan();

    if (has_compressed) {

        auto size_penis = std::to_string(span.size());

        g_tls_client->send(size_penis.data(), size_penis.size());

        int maxCompressedSize = LZ4_compressBound(span.size());

        m_buffer.resize(maxCompressedSize);

        int compressedSize = LZ4_compress((char*)span.data(), (char*)m_buffer.data(), span.size());
        m_buffer.erase(m_buffer.begin() + compressedSize, m_buffer.end());
    }
    else {
        m_buffer = std::vector<int8_t>(span.begin(), span.end());
    }
}

void c_flatbuffers::create_request(uint64_t type, int game, std::vector<int8_t>  memory)
{
    m_builder = flatbuffers::FlatBufferBuilder(0);
    auto offset = CreateRequestDirect(m_builder, type, game, &memory);

    m_builder.Finish(offset);
    m_builder.Finished();
}

void c_flatbuffers::create_authentication(std::string token, std::string build)
{
    m_builder = flatbuffers::FlatBufferBuilder(0);
    auto offset = CreateAuthenticationDirect(m_builder, token.c_str(), build.c_str());

    m_builder.Finish(offset);
    m_builder.Finished();
}

void c_flatbuffers::create_crash(std::string message, int stage, int hook)
{
    m_builder = flatbuffers::FlatBufferBuilder(0);
    auto offset = CreateCrashDirect(m_builder, message.c_str(), stage, hook);

    m_builder.Finish(offset);
    m_builder.Finished();
}

void c_flatbuffers::create_config(std::string token, std::string share, std::string value, std::string name)
{
    m_builder = flatbuffers::FlatBufferBuilder(0);
    auto offset = Createconfig_requestDirect(m_builder, token.c_str(), share.c_str(), value.c_str(), name.c_str());

    m_builder.Finish(offset);
    m_builder.Finished();
}

#include <chrono>

void c_flatbuffers::create_message(std::string username, std::string message) {
    m_builder = flatbuffers::FlatBufferBuilder(0);

    auto offset = CreateMessageDirect(m_builder, username.c_str(), message.c_str(), false, std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    m_builder.Finish(offset);
    m_builder.Finished();
}

void c_flatbuffers::create_request_sticky_message(int index, bool sticky) {
    m_builder = flatbuffers::FlatBufferBuilder(0);
    auto offset = CreateRequestStickyMessage(m_builder, index, sticky);

    m_builder.Finish(offset);
    m_builder.Finished();
}

void c_flatbuffers::create_loader_register(std::string username, std::string build) {
    m_builder = flatbuffers::FlatBufferBuilder(0);
    auto offset = CreateLoaderRegisterDirect(m_builder, username.c_str(), build.c_str());

    m_builder.Finish(offset);
    m_builder.Finished();
}

void c_flatbuffers::create_loader_module(std::vector<std::uint64_t> modules) {
    m_builder = flatbuffers::FlatBufferBuilder(0);

    auto offset = CreateLoaderModuleDirect(m_builder, &modules);

    m_builder.Finish(offset);
    m_builder.Finished();
}

void c_flatbuffers::create_loader_interface(std::vector<std::uint64_t> interfaces) {
    m_builder = flatbuffers::FlatBufferBuilder(0);

    auto offset = CreateLoaderModuleDirect(m_builder, &interfaces);

    m_builder.Finish(offset);
    m_builder.Finished();
}

std::string c_flatbuffers::get_response(char* data, int size, int original_size) {

    int size_ = original_size;

    if (original_size < size)
        size_ = size;

    char* decompressed = new char[size_];

    int out_size = LZ4_decompress_safe(data, decompressed, size, size_);

    if (out_size < 0) {
        out_size = size;
    }

    std::string message(decompressed, decompressed + out_size);

    delete[] decompressed;

    return message;
}

void c_flatbuffers::release() {
    m_buffer.clear();
    m_builder.Release();
}