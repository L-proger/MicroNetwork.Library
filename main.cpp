#include <LFramework/USB/Host/IUsbService.h>
#include <LFramework/USB/Host/IUsbDevice.h>
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
#include <MicroNetwork.Task.MemoryAccess.IHostToDevice.OutMarshaler.h>
#include <MicroNetwork.Task.MemoryAccess.IDeviceToHost.InMarshaler.h>


using namespace MicroNetwork;

class ITaskContext;
namespace LFramework {
    template<>
    struct InterfaceAbi<ITaskContext> : public InterfaceAbi<IUnknown> {
        using Base = InterfaceAbi<IUnknown>;
        static constexpr InterfaceID ID() { return { 0x52c189a3, 0x464faad9, 0x3e16859c, 0x260c4904  }; }
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
        static constexpr InterfaceID ID() { return { 0x4f1e8f7c, 0x4cce4629, 0x2eb212ad, 0x74864c7b }; }
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

class TestDataReceiver : public LFramework::ComImplement<TestDataReceiver, LFramework::ComObject, MicroNetwork::Common::IDataReceiver> {
public:
    void packet(MicroNetwork::Common::PacketHeader header, const void* data) {
        std::cout << "Packet id: " << (int)header.id << " size: " << (int)header.size << std::endl;
    }
};

#include <LFramework/USB/Host/Linux/NetlinkClient.h>
#include <LFramework/USB/Host/Linux/NetlinkReader.h>

void deviceCallback(const LFramework::USB::NetlinkReader::DeviceEvent& event) {
    std::cout << "#Event: " << (event.removed ? "removed" : "added") << std::endl;
    std::cout << "\tBusNumber: " << (int)event.busNumber << std::endl;
    std::cout << "\tDeviceNumber: " << (int)event.deviceNumber << std::endl;
    std::cout << "\tFileName: " << event.sysfsFileName << std::endl;
}

int main() {



    auto netlinkClient = std::make_shared<LFramework::USB::NetlinkClient>();
    auto netlinkReader = std::make_shared<LFramework::USB::NetlinkReader>(netlinkClient, deviceCallback);


    while(true){
        std::cout << "LOL" << std::endl;
        sleep(1);
    }

    auto rx = LFramework::ComPtr<MicroNetwork::Common::IDataReceiver>::create<TestDataReceiver>();
    auto outPtr = LFramework::ComPtr<MicroNetwork::Task::MemoryAccess::IHostToDevice>::create<MicroNetwork::Task::MemoryAccess::IHostToDeviceOutMarshaler>(rx);
    outPtr->read({ 1, 2 });
    outPtr->write({ {1, 2}, {6, 7, 8} });

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

        lfDebug() << "Starting task on node: " <<  node.value;

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
