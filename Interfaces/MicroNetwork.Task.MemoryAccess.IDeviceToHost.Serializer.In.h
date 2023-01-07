#pragma once
#include <MicroNetwork.Task.MemoryAccess.h>
namespace MicroNetwork::Task::MemoryAccess{
    class IDeviceToHostSerializerIn : public LFramework::ComImplement<IDeviceToHostSerializerIn, LFramework::ComObject, MicroNetwork::Common::IDataReceiver>{
    public:
        IDeviceToHostSerializerIn(LFramework::ComPtr<MicroNetwork::Task::MemoryAccess::IDeviceToHost> next) : _next(next){
        }
        void packet(MicroNetwork::Common::PacketHeader header, const void* data){
            switch (header.id){
            case 0:
                _next->readResponse(*(reinterpret_cast<const MicroNetwork::Task::MemoryAccess::MemoryBlob*>(data)));
                return;
            case 1:
                _next->writeResponse(*(reinterpret_cast<const bool*>(data)));
                return;
            default:
                throw LFramework::ComException(LFramework::Result::NotImplemented);
            }
        }
    public:
        LFramework::ComPtr<MicroNetwork::Task::MemoryAccess::IDeviceToHost> _next;
    };
}

