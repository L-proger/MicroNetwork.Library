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

#include <MicroNetwork/User/TaskContext.h>
#include <MicroNetwork/User/TaskContextConstructor.h>

#include <MicroNetwork.Task.MemoryAccess.IHostToDevice.Serializer.Out.h>
#include <MicroNetwork.Task.MemoryAccess.IDeviceToHost.Serializer.In.h>
#include <MicroNetwork.Host.MemoryAccess.h>
#include <iomanip>

using namespace MicroNetwork;

class TestTaskContext : public User::TaskContext {
public:
    static constexpr LFramework::Guid ID = { 0x292464d1, 0x4e01af7a, 0x3027c698, 0x6f363d48 };
    ~TestTaskContext() {
        std::cout << "TestTaskHost::~dtor" << std::endl;
    }
    void setDataReceiver(LFramework::ComPtr<Common::IDataReceiver> receiver) override {
        _receiver = receiver;
    }
    LFramework::Result packet(Common::PacketHeader header, const void* data) {
        std::lock_guard<std::mutex> lock(_packetsMutex);
        _packets.push_back(std::vector<std::uint8_t>((const std::uint8_t*)data, (const std::uint8_t*)data + header.size));
        return LFramework::Result::Ok;
    }
    std::vector<std::uint8_t> getPacket() { 
        std::lock_guard<std::mutex> lock(_packetsMutex);
        if(_packets.empty()){
            return {}; 
        }else{
            auto result = _packets[0];
            _packets.erase(_packets.begin());
            return result;
        }
    }
    void sendPacket(const std::vector<std::uint8_t>& data) { 
        
    }
    void onUserRelease() override {
         _receiver = nullptr;
        lfDebug() << "User release";
    }
    void onNetworkRelease() override {
        TaskContext::onNetworkRelease();
        lfDebug() << "Network release";
    }
private:
    int _rxPacketId = 0;
    std::vector<std::vector<std::uint8_t>> _packets;
    LFramework::ComPtr<Common::IDataReceiver> _receiver;
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

        auto taskContext = User::TaskContextConstructor<MicroNetwork::Host::MemoryAccess::ITask, TestTaskContext>::construct(network, node);

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
