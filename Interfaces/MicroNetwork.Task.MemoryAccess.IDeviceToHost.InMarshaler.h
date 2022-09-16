#pragma once
//imported type: std::uint32_t from module: cstdint
//imported type: LFramework::IUnknown from module: LFramework/COM/ComObject.h
//imported type: void from module: LanguagePrimitive
//imported type: bool from module: LanguagePrimitive
//imported type: IDeviceToHost from module: MicroNetwork.Task.MemoryAccess
//imported type: IDataReceiver from module: MicroNetwork.Common
#include <LFramework/COM/ComObject.h>
#include <cstdint>
#include <MicroNetwork.Task.MemoryAccess.h>
#include <MicroNetwork.Common.h>
namespace MicroNetwork::Task::MemoryAccess{
    class IDeviceToHostInMarshaler : public LFramework::ComImplement<IDeviceToHostInMarshaler, LFramework::ComObject, MicroNetwork::Common::IDataReceiver>{
    public:
        IDeviceToHostInMarshaler(LFramework::ComPtr<MicroNetwork::Task::MemoryAccess::IDeviceToHost> next) : _next(next){
        } //ctor
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
    protected:
        LFramework::ComPtr<MicroNetwork::Task::MemoryAccess::IDeviceToHost> _next; 
    };
}

