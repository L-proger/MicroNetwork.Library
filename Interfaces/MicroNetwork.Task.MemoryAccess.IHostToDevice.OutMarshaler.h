#pragma once
//imported type: std::uint32_t from module: cstdint
//imported type: LFramework::IUnknown from module: LFramework/COM/ComObject.h
//imported type: void from module: LanguagePrimitive
//imported type: bool from module: LanguagePrimitive
//imported type: IHostToDevice from module: MicroNetwork.Task.MemoryAccess
//imported type: IDataReceiver from module: MicroNetwork.Common
#include <LFramework/COM/ComObject.h>
#include <cstdint>
#include <MicroNetwork.Task.MemoryAccess.h>
#include <MicroNetwork.Common.h>
namespace MicroNetwork::Task::MemoryAccess{
    class IHostToDeviceOutMarshaler : public LFramework::ComImplement<IHostToDeviceOutMarshaler, LFramework::ComObject, MicroNetwork::Task::MemoryAccess::IHostToDevice>{
    public:
        IHostToDeviceOutMarshaler(LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> next) : _next(next){
        } //ctor
        void read(MicroNetwork::Task::MemoryAccess::MemoryRegion request){
            //PacketID: 0
            MicroNetwork::Common::PacketHeader header;
            header.id = 0;
            header.size = sizeof(MicroNetwork::Task::MemoryAccess::MemoryRegion);
            _next->packet(header, &request);
        } //read
        void write(MicroNetwork::Task::MemoryAccess::MemoryBlob request){
            //PacketID: 1
            MicroNetwork::Common::PacketHeader header;
            header.id = 1;
            header.size = sizeof(MicroNetwork::Task::MemoryAccess::MemoryBlob);
            _next->packet(header, &request);
        } //write
    protected:
        LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> _next;
    };
}

