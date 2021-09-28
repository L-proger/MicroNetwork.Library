#pragma once
//imported type: std::int32_t from module: cstdint
//imported type: std::uint32_t from module: cstdint
//imported type: LFramework::IUnknown from module: LFramework/COM/ComObject.h
//imported type: IDataReceiver from module: MicroNetwork.Common
//imported type: LFramework::Guid from module: LFramework/Guid.h
#include <LFramework/COM/ComObject.h>
#include <cstdint>
#include <MicroNetwork.Common.h>
#include <LFramework/Guid.h>
namespace MicroNetwork::Host{
    enum NodeState : std::int32_t{
        NotReady = 0,
        Idle = 1,
        TaskLaunched = 2,
        InvalidNode = 3,
    }; //NodeState
}
namespace MicroNetwork::Host{
    struct NodeHandle{
        std::uint32_t value;
    }; //NodeHandle
}
namespace MicroNetwork::Host{
    class INetwork;
} //MicroNetwork::Host
namespace LFramework{
    //Interface ABI
    template<>
    struct InterfaceAbi<MicroNetwork::Host::INetwork> : public InterfaceAbi<LFramework::IUnknown>{
        using Base = InterfaceAbi<LFramework::IUnknown>;
        //{ce29c75f-a57e-4632-8a88-6562e04455a1}
        static constexpr InterfaceID ID() { return { 0xce29c75f, 0xa57e, 0x4632, { 0x8a, 0x88, 0x65, 0x62, 0xe0, 0x44, 0x55, 0xa1 } }; }
        virtual Result LFRAMEWORK_COM_CALL startTask(std::uint32_t node, LFramework::Guid taskId, MicroNetwork::Common::IDataReceiver userDataReceiver, MicroNetwork::Common::IDataReceiver& result) = 0;
        virtual Result LFRAMEWORK_COM_CALL isTaskSupported(std::uint32_t node, LFramework::Guid taskId, std::int32_t& result) = 0;
        virtual Result LFRAMEWORK_COM_CALL getNodes(LFramework::ArrayOutMarshaler result) = 0;
        virtual Result LFRAMEWORK_COM_CALL getNodeState(MicroNetwork::Host::NodeHandle node, MicroNetwork::Host::NodeState& result) = 0;
        virtual Result LFRAMEWORK_COM_CALL getStateId(std::uint32_t& result) = 0;
    private:
        ~InterfaceAbi() = delete;
    }; //INetwork
    //Interface Remap
    template<class TImplementer>
    struct InterfaceRemap<MicroNetwork::Host::INetwork, TImplementer> : public InterfaceRemap<LFramework::IUnknown, TImplementer>{
        virtual Result LFRAMEWORK_COM_CALL startTask(std::uint32_t node, LFramework::Guid taskId, MicroNetwork::Common::IDataReceiver userDataReceiver, MicroNetwork::Common::IDataReceiver& result){
            try{
                result = this->implementer()->startTask(node, taskId, userDataReceiver);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL isTaskSupported(std::uint32_t node, LFramework::Guid taskId, std::int32_t& result){
            try{
                result = this->implementer()->isTaskSupported(node, taskId);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL getNodes(LFramework::ArrayOutMarshaler result){
            try{
                result = this->implementer()->getNodes();
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL getNodeState(MicroNetwork::Host::NodeHandle node, MicroNetwork::Host::NodeState& result){
            try{
                result = this->implementer()->getNodeState(node);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL getStateId(std::uint32_t& result){
            try{
                result = this->implementer()->getStateId();
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
    };
    //Interface Wrapper
    template<>
    class LFramework::InterfaceWrapper<MicroNetwork::Host::INetwork>{
    public:
        MicroNetwork::Common::IDataReceiver& startTask(std::uint32_t node, LFramework::Guid taskId, MicroNetwork::Common::IDataReceiver userDataReceiver){
            MicroNetwork::Common::IDataReceiver result;
            auto comCallResult = _abi->startTask(node, taskId, userDataReceiver, result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        std::int32_t& isTaskSupported(std::uint32_t node, LFramework::Guid taskId){
            std::int32_t result;
            auto comCallResult = _abi->isTaskSupported(node, taskId, result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        std::vector<MicroNetwork::Host::NodeHandle> getNodes(){
            std::vector<MicroNetwork::Host::NodeHandle> result;
            LFramework::ArrayOutMarshaler resultMarshaler(result);
            auto comCallResult = _abi->getNodes(, resultMarshaler);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        MicroNetwork::Host::NodeState& getNodeState(MicroNetwork::Host::NodeHandle node){
            MicroNetwork::Host::NodeState result;
            auto comCallResult = _abi->getNodeState(node, result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        std::uint32_t& getStateId(){
            std::uint32_t result;
            auto comCallResult = _abi->getStateId(, result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
    private:
        InterfaceAbi<MicroNetwork::Host::INetwork>* _abi;
    };
}

