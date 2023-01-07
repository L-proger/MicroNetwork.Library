#pragma once
//imported type: ITask from module: MicroNetwork.Host
//imported type: std::uint8_t from module: cstdint
//imported type: void from module: LanguagePrimitive
#include <cstdint>
#include <MicroNetwork.Host.h>
namespace MicroNetwork::Host::MemoryAccess{
    class ITask;
} //MicroNetwork::Host::MemoryAccess
namespace LFramework{
    //Interface ABI
    template<>
    struct InterfaceAbi<MicroNetwork::Host::MemoryAccess::ITask> : public InterfaceAbi<MicroNetwork::Host::ITask>{
        using Base = InterfaceAbi<MicroNetwork::Host::ITask>;
        //{0008fd4f-2b3c-4410-b1e7-0563d041aa84}
        static constexpr InterfaceID ID() { return { 0x0008fd4f, 0x44102b3c, 0x6305e7b1, 0x84aa41d0 }; }
        virtual Result LFRAMEWORK_COM_CALL getPacket(LFramework::ArrayOutMarshaler result) = 0;
        virtual Result LFRAMEWORK_COM_CALL sendPacket(LFramework::ArrayInMarshaler data) = 0;
    private:
        ~InterfaceAbi() = delete;
    }; //ITask
    //Interface Remap
    template<class TImplementer>
    struct InterfaceRemap<MicroNetwork::Host::MemoryAccess::ITask, TImplementer> : public InterfaceRemap<MicroNetwork::Host::ITask, TImplementer>{
        virtual Result LFRAMEWORK_COM_CALL getPacket(LFramework::ArrayOutMarshaler result){
            try{
                result = this->implementer()->getPacket();
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL sendPacket(LFramework::ArrayInMarshaler data){
            try{
                this->implementer()->sendPacket(data);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
    };
    //Interface Wrapper
    template<>
    class InterfaceWrapper<MicroNetwork::Host::MemoryAccess::ITask> : public InterfaceWrapper<MicroNetwork::Host::ITask> {
    public:
        std::vector<std::uint8_t> getPacket(){
            std::vector<std::uint8_t> result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::MemoryAccess::ITask>*>(_abi)->getPacket(result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        void sendPacket(const std::vector<std::uint8_t>& data){
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::MemoryAccess::ITask>*>(_abi)->sendPacket(data);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
        }
    };
}

