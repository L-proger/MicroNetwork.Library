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

#include <MicroNetwork.Common.h>

#include <MicroNetwork/User/TaskContext.h>
#include <MicroNetwork/User/TaskContextConstructor.h>




using namespace MicroNetwork;

class ITaskContext;
namespace LFramework {
    template<>
    struct InterfaceAbi<ITaskContext> : public InterfaceAbi<IUnknown> {
        using Base = InterfaceAbi<IUnknown>;
        static constexpr InterfaceID ID() { return { 0x52c189a3, 0xaad9, 0x464f, { 0x9c, 0x85, 0x16, 0x3e, 0x4, 0x49, 0xc, 0x26 } }; }
        virtual LFramework::Result isConnected(bool& result) = 0;
    };

    template<class TImplementer>
    struct InterfaceRemap<ITaskContext, TImplementer> : public InterfaceRemap<IUnknown, TImplementer> {
    public:
        virtual LFramework::Result isConnected(bool& result) { return this->implementer()->isConnected(result); }
    };
}

class ITestTaskContext;
namespace LFramework {
    template<>
    struct InterfaceAbi<ITestTaskContext> : public InterfaceAbi<ITaskContext> {
        using Base = InterfaceAbi<ITaskContext>;
        static constexpr InterfaceID ID() { return { 0x4f1e8f7c, 0x4629, 0x4cce, { 0xad, 0x12, 0xb2, 0x2e, 0x7b, 0x4c, 0x86, 0x74 } }; }
        virtual LFramework::Result getPackets(std::vector<std::vector<std::uint8_t>>& result) = 0;
        virtual LFramework::Result sendPacket(std::vector<std::uint8_t>& data) = 0;
    };

    template<class TImplementer>
    struct InterfaceRemap<ITestTaskContext, TImplementer> : public InterfaceRemap<ITaskContext, TImplementer> {
    public:
        virtual LFramework::Result getPackets(std::vector<std::vector<std::uint8_t>>& result) { return this->implementer()->getPackets(result); }
        virtual LFramework::Result sendPacket(std::vector<std::uint8_t>& data) { return this->implementer()->sendPacket(data); }
    };
}

class TestTaskContext : public User::TaskContext {
public:
    static constexpr LFramework::Guid ID = { 0x292464d1, 0xaf7a, 0x4e01, { 0x98, 0xc6, 0x27, 0x30, 0x48, 0x3d, 0x36, 0x6f } };
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
    LFramework::Result getPackets(std::vector<std::vector<std::uint8_t>>& result) { 
        std::lock_guard<std::mutex> lock(_packetsMutex);
        result = _packets;
        _packets.clear();
        return LFramework::Result::Ok;
    }
    LFramework::Result sendPacket(std::vector<std::uint8_t>& data) { 
        return LFramework::Result::Ok;
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


class TestClass : public LFramework::ComImplement<TestClass, LFramework::ComObject, MicroNetwork::Common::IDataReceiver> {
public:
    LFramework::Result LFRAMEWORK_COM_CALL packet(MicroNetwork::Common::PacketHeader header, const void* data) {
        return LFramework::Result::ErrorPointer;
    }
};

int main() {
    auto network = MicroNetwork::Host::Library::createNetwork(0x0301, 0x1111);

    while(true){
        lfDebug() << "Waiting for node...";

        while(true){
             auto nodes = network->getNodes();
             if((nodes.size() != 0) && (network->getNodeState(nodes[0]) == MicroNetwork::Host::NodeState::Idle)){
                 break;
             }
        }

        auto nodes = network->getNodes();
        auto node = nodes[0];

        lfDebug() << "Starting task on node: " <<  node;

        auto taskContext = User::TaskContextConstructor<ITestTaskContext, TestTaskContext>::construct(network, node);

        lfDebug() << "Task started";
        std::vector<std::vector<std::uint8_t>> packets;
        bool isConnected = true;

        int pc = 0;
        while(pc < 100){
            taskContext->isConnected(isConnected);
            if (isConnected) {

                taskContext->getPackets(packets);
                if (packets.size() != 0) {
                    pc += packets.size();
                    lfDebug() << "Received " << packets.size() << "packets";
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
