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

#include <thread>
#include <chrono>
#include <functional>
#include <atomic>

#include <unknwn.h>

std::string findDevice(std::shared_ptr<UsbService> service){
    auto devices = service->enumerateDevices();

    for(auto& device : devices){
        if(device.vid == 0x0301 && device.pid == 0x1111){
            return device.path;
        }
    }
    return {};
}

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

void onUsbDeviceChange1() {
    std::cout << "USB DEVICE CHANGE 1" << std::endl;
}

void onUsbDeviceChange2() {
    std::cout << "USB DEVICE CHANGE 2" << std::endl;
}

int main() {

    auto taskTest = LFramework::ComPtr<LFramework::IUnknown>::create<TestTaskHost>();
    auto taskTest2 = taskTest.queryInterface<Common::IDataReceiver>();

    taskTest = nullptr;
    taskTest2 = nullptr;

    auto usbService1 = std::make_shared<UsbService>();

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    lfDebug() << "Test";
    auto devicePath = findDevice(usbService1);

    if(!devicePath.empty()){
        std::cout << "Device found!" << std::endl;

        auto device = std::make_shared<LFramework::USB::UsbHDevice>(devicePath);
        std::cout << "Device open!" << std::endl;

        Host::UsbTransmitter* transmitter = new Host::UsbTransmitter(device);

        Host::Host* host = new Host::Host();
        host->bind(transmitter);
        transmitter->bind(host);

        transmitter->start();

        while(host->getState() == 0){

        }

        lfDebug() << "New network state: " << host->getState();

        auto nodes = host->getNodes();

        TestTaskHost* testTask = new TestTaskHost();
        LFramework::ComPtr<Common::IDataReceiver> testTaskPtr(testTask->queryInterface<Common::IDataReceiver>());


        auto networkDataReceiver = host->startTask(0, testTaskPtr);
        testTask->setDataReceiver(networkDataReceiver);

        while(true){
            LFramework::Threading::ThisThread::sleepForMs(10);
        }

    }

    return 0;
}
