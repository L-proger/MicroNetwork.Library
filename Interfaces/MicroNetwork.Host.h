#pragma once
//imported type: std::int32_t from module: cstdint
//imported type: std::uint32_t from module: cstdint
//imported type: LFramework::IUnknown from module: LFramework/COM/ComObject.h
//imported type: IDataReceiver from module: MicroNetwork.Common
//imported type: LFramework::Guid from module: LFramework/Guid.h
//imported type: std::uint16_t from module: cstdint
//imported type: bool from module: LanguagePrimitive
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
        static constexpr InterfaceID ID() { return { 0xce29c75f, 0x4632a57e, 0x6265888a, 0xa15544e0 }; }
        virtual Result LFRAMEWORK_COM_CALL startTask(MicroNetwork::Host::NodeHandle node, LFramework::Guid taskId, LFramework::InterfaceAbi<MicroNetwork::Common::IDataReceiver>* userDataReceiver, LFramework::ComPtr<MicroNetwork::Common::IDataReceiver>& result) = 0;
        virtual Result LFRAMEWORK_COM_CALL isTaskSupported(MicroNetwork::Host::NodeHandle node, LFramework::Guid taskId, std::int32_t& result) = 0;
        virtual Result LFRAMEWORK_COM_CALL getNodes(LFramework::ArrayOutMarshaler result) = 0;
        virtual Result LFRAMEWORK_COM_CALL getNodeState(MicroNetwork::Host::NodeHandle node, MicroNetwork::Host::NodeState& result) = 0;
        virtual Result LFRAMEWORK_COM_CALL getStateId(std::uint32_t& result) = 0;
    private:
        ~InterfaceAbi() = delete;
    }; //INetwork
    //Interface Remap
    template<class TImplementer>
    struct InterfaceRemap<MicroNetwork::Host::INetwork, TImplementer> : public InterfaceRemap<LFramework::IUnknown, TImplementer>{
        virtual Result LFRAMEWORK_COM_CALL startTask(MicroNetwork::Host::NodeHandle node, LFramework::Guid taskId, LFramework::InterfaceAbi<MicroNetwork::Common::IDataReceiver>* userDataReceiver, LFramework::ComPtr<MicroNetwork::Common::IDataReceiver>& result){
            LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> userDataReceiverMarshaler;
            userDataReceiverMarshaler.attach(userDataReceiver);
            try{
                result = this->implementer()->startTask(node, taskId, userDataReceiverMarshaler);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL isTaskSupported(MicroNetwork::Host::NodeHandle node, LFramework::Guid taskId, std::int32_t& result){
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
    class InterfaceWrapper<MicroNetwork::Host::INetwork> : public InterfaceWrapper<LFramework::IUnknown> {
    public:
        LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> startTask(MicroNetwork::Host::NodeHandle node, LFramework::Guid taskId, LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> userDataReceiver){
            LFramework::ComPtr<MicroNetwork::Common::IDataReceiver> result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::INetwork>*>(_abi)->startTask(node, taskId, userDataReceiver.detach(), result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        std::int32_t isTaskSupported(MicroNetwork::Host::NodeHandle node, LFramework::Guid taskId){
            std::int32_t result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::INetwork>*>(_abi)->isTaskSupported(node, taskId, result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        std::vector<MicroNetwork::Host::NodeHandle> getNodes(){
            std::vector<MicroNetwork::Host::NodeHandle> result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::INetwork>*>(_abi)->getNodes(result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        MicroNetwork::Host::NodeState getNodeState(MicroNetwork::Host::NodeHandle node){
            MicroNetwork::Host::NodeState result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::INetwork>*>(_abi)->getNodeState(node, result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
        std::uint32_t getStateId(){
            std::uint32_t result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::INetwork>*>(_abi)->getStateId(result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
    };
}
namespace MicroNetwork::Host{
    class ILibrary;
} //MicroNetwork::Host
namespace LFramework{
    //Interface ABI
    template<>
    struct InterfaceAbi<MicroNetwork::Host::ILibrary> : public InterfaceAbi<LFramework::IUnknown>{
        using Base = InterfaceAbi<LFramework::IUnknown>;
        //{193287cb-f6fd-4086-9de8-787f05b64f26}
        static constexpr InterfaceID ID() { return { 0x193287cb, 0x4086f6fd, 0x7f78e89d, 0x264fb605 }; }
        virtual Result LFRAMEWORK_COM_CALL createNetwork(std::uint16_t vid, std::uint16_t pid, LFramework::ComPtr<MicroNetwork::Host::INetwork>& result) = 0;
    private:
        ~InterfaceAbi() = delete;
    }; //ILibrary
    //Interface Remap
    template<class TImplementer>
    struct InterfaceRemap<MicroNetwork::Host::ILibrary, TImplementer> : public InterfaceRemap<LFramework::IUnknown, TImplementer>{
        virtual Result LFRAMEWORK_COM_CALL createNetwork(std::uint16_t vid, std::uint16_t pid, LFramework::ComPtr<MicroNetwork::Host::INetwork>& result){
            try{
                result = this->implementer()->createNetwork(vid, pid);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
    };
    //Interface Wrapper
    template<>
    class InterfaceWrapper<MicroNetwork::Host::ILibrary> : public InterfaceWrapper<LFramework::IUnknown> {
    public:
        LFramework::ComPtr<MicroNetwork::Host::INetwork> createNetwork(std::uint16_t vid, std::uint16_t pid){
            LFramework::ComPtr<MicroNetwork::Host::INetwork> result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::ILibrary>*>(_abi)->createNetwork(vid, pid, result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
    };
}
namespace MicroNetwork::Host{
    class ITask;
} //MicroNetwork::Host
namespace LFramework{
    //Interface ABI
    template<>
    struct InterfaceAbi<MicroNetwork::Host::ITask> : public InterfaceAbi<LFramework::IUnknown>{
        using Base = InterfaceAbi<LFramework::IUnknown>;
        //{3d620300-77e1-4711-87e5-351158c33326}
        static constexpr InterfaceID ID() { return { 0x3d620300, 0x471177e1, 0x1135e587, 0x2633c358 }; }
        virtual Result LFRAMEWORK_COM_CALL isConnected(bool& result) = 0;
    private:
        ~InterfaceAbi() = delete;
    }; //ITask
    //Interface Remap
    template<class TImplementer>
    struct InterfaceRemap<MicroNetwork::Host::ITask, TImplementer> : public InterfaceRemap<LFramework::IUnknown, TImplementer>{
        virtual Result LFRAMEWORK_COM_CALL isConnected(bool& result){
            try{
                result = this->implementer()->isConnected();
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
    };
    //Interface Wrapper
    template<>
    class InterfaceWrapper<MicroNetwork::Host::ITask> : public InterfaceWrapper<LFramework::IUnknown> {
    public:
        bool isConnected(){
            bool result;
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Host::ITask>*>(_abi)->isConnected(result);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
            return result;
        }
    };
}

