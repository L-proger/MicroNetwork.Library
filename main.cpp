#include <LFramework/USB/Host/UsbService.h>
#include <LFramework/USB/Host/UsbHDevice.h>
#include <LFramework/Debug.h>
#include <iostream>
#include <iomanip>

#include <LFramework/Containers/ByteFifo.h>
#include <LFramework/Threading/Thread.h>
#include <LFramework/Threading/Semaphore.h>
#include <LFramework/Threading/CriticalSection.h>
#include <MicroNetwork/Host/UsbTransmitter.h>
#include <MicroNetwork/Host/Host.h>
#include "Library.h"
#include <thread>
#include <chrono>
#include <functional>
#include <atomic>
#include <unknwn.h>


using namespace MicroNetwork;

class TestTaskHost : public LFramework::ComImplement<TestTaskHost, LFramework::ComObject, Common::IDataReceiver>  {
public:

    ~TestTaskHost() {
        std::cout << "TestTaskHost::~dtor" << std::endl;
    }

    LFramework::Result setDataReceiver(LFramework::ComPtr<Common::IDataReceiver> receiver)  {
        _receiver = receiver;
        return LFramework::Result::Ok;
    }
    LFramework::Result packet(Common::PacketHeader header, const void* data) {
        lfDebug() << "TestTaskHost::handlePacket id=" << header.id << " size=" << header.size;
        ++_rxPacketId;
        if(_rxPacketId % 100 == 0){
            Common::MaxPacket packet;
            packet.header.id = 8;
            packet.header.size = 2;
            packet.payload[0] = 3;
            packet.payload[1] = 4;
            _receiver->packet(packet.header, packet.payload.data());
        }
        return LFramework::Result::Ok;
    }
private:
    int _rxPacketId = 0;
    LFramework::ComPtr<Common::IDataReceiver> _receiver;
};

int main() {
    auto network = MicroNetwork::Host::Library::createNetwork(0x0301, 0x1111);

    lfDebug() << "Waiting for node...";
    while(network->getStateId() == 0){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    lfDebug() << "Node found! Update ID = " << network->getStateId();

    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
