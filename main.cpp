#include <LFramework/USB/Host/UsbService.h>
#include <LFramework/USB/Host/UsbHDevice.h>
#include <LFramework/Debug.h>
#include <iostream>
#include <iomanip>
#include <LFramework/Debug.h>
#include <LFramework/DeviceNetwork/Node.h>
#include <LFramework/DeviceNetwork/Router.h>
#include <LFramework/DeviceNetwork/Packet.h>
#include <LFramework/Containers/ByteFifo.h>
#include <LFramework/DeviceNetwork/NetworkInterface.h>
#include <LFramework/Threading/Semaphore.h>
#include <LFramework/Threading/CriticalSection.h>
#include <LFramework/DeviceNetwork/Host/UsbTransmitter.h>
#include <LFramework/DeviceNetwork/Host/Host.h>

#include <LFramework/DeviceNetwork/Packet.h>

#include <thread>
#include <chrono>
#include <functional>
#include <atomic>

std::string findDevice(std::shared_ptr<UsbService> service){
    auto devices = service->enumerateDevices();

    for(auto& device : devices){
        if(device.vid == 0x0301 && device.pid == 0x1111){
            return device.path;
        }
    }
    return {};
}

using namespace LFramework::DeviceNetwork;


class TestTaskHost : public Host::IDataReceiver {
public:
    void setDataReceiver(Host::IDataReceiver* receiver)  {
        _receiver = receiver;
    }
    bool handlePacket(PacketHeader header, const void* data) override {
        lfDebug() << "TestTaskHost::handlePacket id=" << header.id << " size=" << header.size;
        ++_rxPacketId;
        if(_rxPacketId % 100 == 0){
            MaxPacket packet;
            packet.header.id = 8;
            packet.header.size = 2;
            packet.payload[0] = 3;
            packet.payload[1] = 4;
            _receiver->handlePacket(packet.header, packet.payload.data());
        }
        return true;
    }
    void release() override {
        lfDebug() << "TestTaskHost::Release";
    }
private:
    int _rxPacketId = 0;
    Host::IDataReceiver* _receiver;
};


void onUsbDeviceChange1() {
    std::cout << "USB DEVICE CHANGE 1" << std::endl;
}

void onUsbDeviceChange2() {
    std::cout << "USB DEVICE CHANGE 2" << std::endl;
}

int main() {

    auto usbService1 = std::make_shared<UsbService>();
    usbService1->startEventsListening(onUsbDeviceChange1);

    auto usbService2 = std::make_shared<UsbService>();
    usbService2->startEventsListening(onUsbDeviceChange2);

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    usbService1->stopEventsListening();

    while(true){
        LFramework::Threading::ThisThread::sleepForMs(10);
    }

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

        TestTaskHost testTask;

        auto networkDataReceiver = host->startTask(0, &testTask);
        testTask.setDataReceiver(networkDataReceiver);

        while(true){
            LFramework::Threading::ThisThread::sleepForMs(10);
        }

    }

    return 0;
}
