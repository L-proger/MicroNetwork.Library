#include <LFramework/USB/Host/IUsbService.h>
#include <LFramework/USB/Host/IUsbDevice.h>
#include <LFramework/USB/Host/Linux/UsbService.h>

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
//#include <unknwn.h>

#include <MicroNetwork.Common.h>
#include <MicroNetwork.Task.MemoryAccess.h>

#include <MicroNetwork/User/Link.h>
#include <MicroNetwork/User/LinkConstructor.h>

#include <MicroNetwork.Task.MemoryAccess.IHostToDevice.Serializer.Out.h>
#include <MicroNetwork.Task.MemoryAccess.IDeviceToHost.Serializer.In.h>
#include <MicroNetwork.Task.MemoryAccess.IStream.IoStreamDescriptor.h>
#include <MicroNetwork.Host.MemoryAccess.h>
#include <iomanip>

using namespace MicroNetwork;

class TestTaskContext : public LFramework::ComImplement<TestTaskContext, User::Link<MicroNetwork::Task::MemoryAccess::IStreamIoStreamDescriptor>, MicroNetwork::Task::MemoryAccess::IDeviceToHost> {
public: 
    ~TestTaskContext() {
        std::cout << "TestTaskHost::~dtor" << std::endl;
    }
    void setOutputStream(LFramework::ComPtr<MicroNetwork::Task::MemoryAccess::IHostToDevice> outStream) override {
        _outStream = outStream;
    }

    //Device to host
    void readResponse(MicroNetwork::Task::MemoryAccess::MemoryBlob response) {
        lfDebug() << "readResponse: address = " << response.header.address << " size = " << response.header.size;
        const std::uint8_t* data = reinterpret_cast<const std::uint8_t*>(&response.data);
        lfDebug() << "data = " << data[0] << ", " << data[1] << " ...";

    }
    void writeResponse(bool success) {
        lfDebug() << "writeResponse";
    }


    //User interface
    void sendPacket(const std::vector<std::uint8_t>& data) { 
        
    }
    std::vector<std::uint8_t> getPacket() {
        return {};
    }


    void onUserRelease() override {
        _outStream = nullptr;
        lfDebug() << "User release";
    }
    void onNetworkRelease() override {
        Link::onNetworkRelease();
        lfDebug() << "Network release";
    }
protected:
    
private:
    int _rxPacketId = 0;
    LFramework::ComPtr<MicroNetwork::Task::MemoryAccess::IHostToDevice> _outStream;
    std::mutex _packetsMutex;
};


int main() {

    auto library = LFramework::ComPtr<MicroNetwork::Host::ILibrary>::create<MicroNetwork::Host::Library>();

    auto network = library->createNetwork(0x0301, 0x1111);

    while(true){
        lfDebug() << "Waiting for node...";

        while(true){
            auto nodes = network->getNodes();
            auto nodesSize = nodes.size();

            if(nodesSize != 0){
                auto node = nodes[0];

                auto nodeState = network->getNodeState(node);
                if(nodeState == MicroNetwork::Host::NodeState::Idle){
                    break;
                }
            }
        }

        auto nodes = network->getNodes();
        auto node = nodes[0];

        lfDebug() << "Starting task on node: " <<  node.value;

        auto taskContext = User::LinkConstructor<MicroNetwork::Host::MemoryAccess::ITask, TestTaskContext>::constructLink(network, node);

        lfDebug() << "Task started";
        std::vector<std::uint8_t> packet;
        bool isConnected = true;

        int pc = 0;
        while(pc < 100){
            isConnected = taskContext->isConnected();
            if (isConnected) {

                packet = taskContext->getPacket();
                if (packet.size() != 0) {
                    //pc++;
                    lfDebug() << "Received packet with " << packet.size() << " bytes";
                    std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }
            }
        }

        taskContext = nullptr;
        lfDebug() << "Task stopped";
    }


    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}
