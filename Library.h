#pragma once

#include <MicroNetwork.Host.h>
#include <LFramework/COM/ComLibrary.h>

namespace MicroNetwork::Host {

class Library : public LFramework::ComImplement<Library, LFramework::ComObject, MicroNetwork::Host::ILibrary>{
public:
    LFramework::ComPtr<MicroNetwork::Host::INetwork> createNetwork(std::uint16_t vid, std::uint16_t pid);
};

LFRAMEWORK_COM_LIBRARY_EXPORT_ENTRY_FUNCTION()

}
