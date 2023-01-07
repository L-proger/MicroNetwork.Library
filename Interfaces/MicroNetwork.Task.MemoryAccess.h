#pragma once
//imported type: std::uint32_t from module: cstdint
//imported type: LFramework::IUnknown from module: LFramework/COM/ComObject.h
//imported type: void from module: LanguagePrimitive
//imported type: bool from module: LanguagePrimitive
#include <LFramework/COM/ComObject.h>
#include <cstdint>
namespace MicroNetwork::Task::MemoryAccess{
    struct Memory4{
        std::uint32_t value;
    }; //Memory4
}
namespace MicroNetwork::Task::MemoryAccess{
    struct Memory8{
        MicroNetwork::Task::MemoryAccess::Memory4 value1;
        MicroNetwork::Task::MemoryAccess::Memory4 value2;
    }; //Memory8
}
namespace MicroNetwork::Task::MemoryAccess{
    struct Memory16{
        MicroNetwork::Task::MemoryAccess::Memory8 value1;
        MicroNetwork::Task::MemoryAccess::Memory8 value2;
    }; //Memory16
}
namespace MicroNetwork::Task::MemoryAccess{
    struct Memory32{
        MicroNetwork::Task::MemoryAccess::Memory16 value1;
        MicroNetwork::Task::MemoryAccess::Memory16 value2;
    }; //Memory32
}
namespace MicroNetwork::Task::MemoryAccess{
    struct Memory64{
        MicroNetwork::Task::MemoryAccess::Memory32 value1;
        MicroNetwork::Task::MemoryAccess::Memory32 value2;
    }; //Memory64
}
namespace MicroNetwork::Task::MemoryAccess{
    struct MemoryRegion{
        std::uint32_t address;
        std::uint32_t size;
    }; //MemoryRegion
}
namespace MicroNetwork::Task::MemoryAccess{
    struct MemoryBlob{
        MicroNetwork::Task::MemoryAccess::MemoryRegion header;
        MicroNetwork::Task::MemoryAccess::Memory64 data;
    }; //MemoryBlob
}
namespace MicroNetwork::Task::MemoryAccess{
    class IHostToDevice;
} //MicroNetwork::Task::MemoryAccess
namespace LFramework{
    //Interface ABI
    template<>
    struct InterfaceAbi<MicroNetwork::Task::MemoryAccess::IHostToDevice> : public InterfaceAbi<LFramework::IUnknown>{
        using Base = InterfaceAbi<LFramework::IUnknown>;
        //{cc3bcc31-5bba-4be5-b3bd-c7fd854a9c2d}
        static constexpr InterfaceID ID() { return { 0xcc3bcc31, 0x4be55bba, 0xfdc7bdb3, 0x2d9c4a85 }; }
        virtual Result LFRAMEWORK_COM_CALL read(MicroNetwork::Task::MemoryAccess::MemoryRegion request) = 0;
        virtual Result LFRAMEWORK_COM_CALL write(MicroNetwork::Task::MemoryAccess::MemoryBlob request) = 0;
    private:
        ~InterfaceAbi() = delete;
    }; //IHostToDevice
    //Interface Remap
    template<class TImplementer>
    struct InterfaceRemap<MicroNetwork::Task::MemoryAccess::IHostToDevice, TImplementer> : public InterfaceRemap<LFramework::IUnknown, TImplementer>{
        virtual Result LFRAMEWORK_COM_CALL read(MicroNetwork::Task::MemoryAccess::MemoryRegion request){
            try{
                this->implementer()->read(request);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL write(MicroNetwork::Task::MemoryAccess::MemoryBlob request){
            try{
                this->implementer()->write(request);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
    };
    //Interface Wrapper
    template<>
    class InterfaceWrapper<MicroNetwork::Task::MemoryAccess::IHostToDevice> : public InterfaceWrapper<LFramework::IUnknown> {
    public:
        void read(MicroNetwork::Task::MemoryAccess::MemoryRegion request){
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Task::MemoryAccess::IHostToDevice>*>(_abi)->read(request);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
        }
        void write(MicroNetwork::Task::MemoryAccess::MemoryBlob request){
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Task::MemoryAccess::IHostToDevice>*>(_abi)->write(request);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
        }
    };
}
namespace MicroNetwork::Task::MemoryAccess{
    class IDeviceToHost;
} //MicroNetwork::Task::MemoryAccess
namespace LFramework{
    //Interface ABI
    template<>
    struct InterfaceAbi<MicroNetwork::Task::MemoryAccess::IDeviceToHost> : public InterfaceAbi<LFramework::IUnknown>{
        using Base = InterfaceAbi<LFramework::IUnknown>;
        //{da97a152-9e0c-43c1-bd60-571c8b4582df}
        static constexpr InterfaceID ID() { return { 0xda97a152, 0x43c19e0c, 0x1c5760bd, 0xdf82458b }; }
        virtual Result LFRAMEWORK_COM_CALL readResponse(MicroNetwork::Task::MemoryAccess::MemoryBlob response) = 0;
        virtual Result LFRAMEWORK_COM_CALL writeResponse(bool success) = 0;
    private:
        ~InterfaceAbi() = delete;
    }; //IDeviceToHost
    //Interface Remap
    template<class TImplementer>
    struct InterfaceRemap<MicroNetwork::Task::MemoryAccess::IDeviceToHost, TImplementer> : public InterfaceRemap<LFramework::IUnknown, TImplementer>{
        virtual Result LFRAMEWORK_COM_CALL readResponse(MicroNetwork::Task::MemoryAccess::MemoryBlob response){
            try{
                this->implementer()->readResponse(response);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
        virtual Result LFRAMEWORK_COM_CALL writeResponse(bool success){
            try{
                this->implementer()->writeResponse(success);
            }
            catch(...){
                return LFramework::Result::UnknownFailure;
            }
            return LFramework::Result::Ok;
        }
    };
    //Interface Wrapper
    template<>
    class InterfaceWrapper<MicroNetwork::Task::MemoryAccess::IDeviceToHost> : public InterfaceWrapper<LFramework::IUnknown> {
    public:
        void readResponse(MicroNetwork::Task::MemoryAccess::MemoryBlob response){
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Task::MemoryAccess::IDeviceToHost>*>(_abi)->readResponse(response);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
        }
        void writeResponse(bool success){
            auto comCallResult = reinterpret_cast<InterfaceAbi<MicroNetwork::Task::MemoryAccess::IDeviceToHost>*>(_abi)->writeResponse(success);
            if(comCallResult != Result::Ok){
                throw ComException(comCallResult);
            }
        }
    };
}

