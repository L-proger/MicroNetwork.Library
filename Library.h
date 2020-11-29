#pragma once

#include <MicroNetwork/Host/UsbTransmitter.h>
#include <MicroNetwork/Host/Host.h>
#include <MicroNetwork/Host/Network.h>
#include <memory>

namespace MicroNetwork::Host {

class Library {
public:
    static std::shared_ptr<Network> createNetwork(std::uint16_t vid, std::uint16_t pid);
};

}
