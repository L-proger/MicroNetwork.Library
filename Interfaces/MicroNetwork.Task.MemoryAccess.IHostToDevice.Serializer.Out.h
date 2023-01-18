#pragma once
#include <LFramework/COM/ComObject.h>
#include <MicroNetwork.Common.h>
#include <MicroNetwork.Task.MemoryAccess.h>
namespace MicroNetwork::Task::MemoryAccess{
    class IHostToDeviceSerializerOut : public LFramework::ComImplement<IHostToDeviceSerializerOut, LFramework::ComObject, MicroNetwork::Task::MemoryAccess::IHostToDevice>{
    public:
        IHostToDeviceSerializerOut(LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> next) : _next(next){
        }
        void read(MicroNetwork::Task::MemoryAccess::MemoryRegion request){
            //PacketID: 0
            MicroNetwork::Common::PacketHeader header;
            header.id = 0;
            header.size = sizeof(MicroNetwork::Task::MemoryAccess::MemoryRegion);
            _next->packet(header, &request);
        }
        void write(MicroNetwork::Task::MemoryAccess::MemoryBlob request){
            //PacketID: 1
            MicroNetwork::Common::PacketHeader header;
            header.id = 1;
            header.size = sizeof(MicroNetwork::Task::MemoryAccess::MemoryBlob);
            _next->packet(header, &request);
        }
    public:
        LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> _next;
    };
}

