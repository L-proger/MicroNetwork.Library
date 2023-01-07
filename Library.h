#pragma once

#include <MicroNetwork.Host.h>

namespace MicroNetwork::Host {

class Library : public LFramework::ComImplement<Library, LFramework::ComObject, MicroNetwork::Host::ILibrary>{
public:
    LFramework::ComPtr<MicroNetwork::Host::INetwork> createNetwork(std::uint16_t vid, std::uint16_t pid);
};

}
