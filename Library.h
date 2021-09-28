#pragma once

#include <MicroNetwork.Host.h>

namespace MicroNetwork::Host {

class Library {
public:
    static LFramework::ComPtr<MicroNetwork::Host::INetwork> createNetwork(std::uint16_t vid, std::uint16_t pid);
};

}
