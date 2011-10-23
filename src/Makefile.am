bin_PROGRAMS = velbusd

velbusd_SOURCES = velbusd.cpp \
                  VelbusMessage/VelbusMessage.hpp VelbusMessage/VelbusMessage.cpp \
                  VelbusMessage/Registrar.hpp VelbusMessage/Registrar.cpp \
                  VelbusMessage/Unknown.cpp VelbusMessage/Unknown.hpp \
                  VelbusMessage/ModuleTypeRequest.cpp VelbusMessage/ModuleTypeRequest.hpp \
                  VelbusMessage/BusOff.cpp VelbusMessage/BusOff.hpp \
                  VelbusMessage/BusActive.cpp VelbusMessage/BusActive.hpp \
                  VelbusMessage/RxBuffFull.cpp VelbusMessage/RxBuffFull.hpp \
                  VelbusMessage/RxReady.cpp VelbusMessage/RxReady.hpp \
                  VelbusMessage/IntStatusRequest.cpp VelbusMessage/IntStatusRequest.hpp \
                  utils/output.hpp utils/output.cpp \
                  utils/TimestampLog.hpp utils/TimestampLog.cpp \
                  utils/Registrar.t \
                  SockAddr.hpp SockAddr.cpp \
                  Socket.hpp Socket.cpp

if WANT_PARSER
velbusd_SOURCES += VelbusMessage/PushButtonStatus.cpp VelbusMessage/PushButtonStatus.hpp \
                   VelbusMessage/SetLeds.cpp VelbusMessage/SetLeds.hpp \
                   VelbusMessage/ClearLeds.cpp VelbusMessage/ClearLeds.hpp \
                   VelbusMessage/SlowBlinkLeds.cpp VelbusMessage/SlowBlinkLeds.hpp \
                   VelbusMessage/FastBlinkLeds.cpp VelbusMessage/FastBlinkLeds.hpp \
                   VelbusMessage/VFastBlinkLeds.cpp VelbusMessage/VFastBlinkLeds.hpp \
                   VelbusMessage/RelayStatus.cpp VelbusMessage/RelayStatus.hpp \
                   VelbusMessage/UpdateLeds.cpp VelbusMessage/UpdateLeds.hpp \
                   VelbusMessage/MemoryDataRequest.cpp VelbusMessage/MemoryDataRequest.hpp \
                   VelbusMessage/MemoryData.cpp VelbusMessage/MemoryData.hpp \
                   VelbusMessage/MemoryDataWrite.cpp VelbusMessage/MemoryDataWrite.hpp \
                   VelbusMessage/MemoryBlockRequest.cpp VelbusMessage/MemoryBlockRequest.hpp \
                   VelbusMessage/MemoryBlock.cpp VelbusMessage/MemoryBlock.hpp \
                   VelbusMessage/MemoryBlockWrite.cpp VelbusMessage/MemoryBlockWrite.hpp \
                   VelbusMessage/MemoryDumpRequest.cpp VelbusMessage/MemoryDumpRequest.hpp \
                   VelbusMessage/BusErrorCounterRequest.cpp VelbusMessage/BusErrorCounterRequest.hpp \
                   VelbusMessage/BusErrorCounter.cpp VelbusMessage/BusErrorCounter.hpp \
                   VelbusMessage/BlindStatus.cpp VelbusMessage/BlindStatus.hpp \
                   VelbusMessage/ModuleType.cpp VelbusMessage/ModuleType.hpp \
                   VelbusMessage/NameRequest.cpp VelbusMessage/NameRequest.hpp \
                   VelbusMessage/Name.cpp VelbusMessage/Name.hpp \
                   VelbusMessage/SwitchBlindOff.cpp VelbusMessage/SwitchBlindOff.hpp \
                   VelbusMessage/SwitchBlindUpDown.cpp VelbusMessage/SwitchBlindUpDown.hpp \
                   VelbusMessage/BlindRelayStatusRequest.cpp VelbusMessage/BlindRelayStatusRequest.hpp \
                   VelbusMessage/SwitchRelayOnOff.cpp VelbusMessage/SwitchRelayOnOff.hpp \
                   VelbusMessage/StartRelayTimer.cpp VelbusMessage/StartRelayTimer.hpp \
                   VelbusMessage/WriteAddr.cpp VelbusMessage/WriteAddr.hpp \
                   VelbusMessage/SensorTempRequest.cpp VelbusMessage/SensorTempRequest.hpp \
                   VelbusMessage/SensorTemp.cpp VelbusMessage/SensorTemp.hpp
endif
