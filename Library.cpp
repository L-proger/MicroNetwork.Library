#include "Library.h"
#include <MicroNetwork/Host/Network.h>

namespace MicroNetwork::Host {

LFramework::ComPtr<MicroNetwork::Host::INetwork> Library::createNetwork(std::uint16_t vid, std::uint16_t pid){
    return LFramework::ComPtr<MicroNetwork::Host::INetwork>::create<Network>(vid, pid);
}

}
