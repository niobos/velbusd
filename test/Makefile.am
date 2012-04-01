check_PROGRAMS = NBcat Parser VMB1RS-sim

NBcat_SOURCES = NBcat.cpp

Parser_SOURCES = Parser.cpp \
                 ../src/utils/output.cpp ../src/utils/output.hpp \
                 ../src/VelbusMessage/VelbusMessage.cpp ../src/VelbusMessage/VelbusMessage.hpp \
                 ../src/VelbusMessage/Registrar.cpp ../src/VelbusMessage/Registrar.hpp \
                 ../src/VelbusMessage/Unknown.cpp ../src/VelbusMessage/Unknown.hpp \
                 ../src/VelbusMessage/ModuleTypeRequest.cpp ../src/VelbusMessage/ModuleTypeRequest.hpp \
                 ../src/VelbusMessage/BusOff.cpp ../src/VelbusMessage/BusOff.hpp \
                 ../src/VelbusMessage/BusActive.cpp ../src/VelbusMessage/BusActive.hpp \
                 ../src/VelbusMessage/RxBuffFull.cpp ../src/VelbusMessage/RxBuffFull.hpp \
                 ../src/VelbusMessage/RxReady.cpp ../src/VelbusMessage/RxReady.hpp \
                 ../src/VelbusMessage/IntStatusRequest.cpp ../src/VelbusMessage/IntStatusRequest.hpp

VMB1RS_sim_SOURCES = VMB1RS-sim.cpp \
                     ../src/utils/output.cpp ../src/utils/output.hpp
VMB1RS_sim_LDADD = -lutil

testscripts = simply-run.sh fork.sh \
              echo.sh echo-with-garbage.sh \
              RxBuffFull.sh \
              logging.sh log-rotating.sh \
              slow-client.sh \
              Parser-garbage.sh \
              Parser-BusOff.sh Parser-BusActive.sh \
              Parser-RxBuffFull.sh Parser-RxReady.sh \
              Parser-IntStatusRequest.sh

dist_check_SCRIPTS = $(testscripts)
TESTS = $(testscripts)

if WANT_PARSER
testscripts += Parser-PushButtonStatus.sh \
               Parser-SetLeds.sh Parser-ClearLeds.sh \
               Parser-SlowBlinkLeds.sh Parser-FastBlinkLeds.sh Parser-VFastBlinkLeds.sh \
               Parser-UpdateLeds.sh \
               Parser-RelayStatus.sh \
               Parser-MemoryDataRequest.sh Parser-MemoryData.sh Parser-MemoryDataWrite.sh \
               Parser-MemoryBlockRequest.sh Parser-MemoryBlock.sh Parser-MemoryBlockWrite.sh \
               Parser-MemoryDumpRequest.sh \
               Parser-BusErrorCounterRequest.sh Parser-BusErrorCounter.sh \
               Parser-BlindStatus.sh \
               Parser-ModuleType.sh \
               Parser-Name.sh \
               Parser-SwitchBlindOff.sh \
               Parser-SwitchBlindUpDown.sh \
               Parser-ModuleStatusRequest.sh \
               Parser-SwitchRelayOnOff.sh \
               Parser-StartRelayTimer.sh \
               Parser-WriteAddr.sh \
               Parser-SensorTempRequest.sh \
               Parser-SensorTemp.sh \
               Parser-TempSensorStatus.sh \
               Parser-SwitchToMode.sh

Parser_SOURCES += ../src/VelbusMessage/PushButtonStatus.cpp ../src/VelbusMessage/PushButtonStatus.hpp \
                  ../src/VelbusMessage/SetLeds.cpp ../src/VelbusMessage/SetLeds.hpp \
                  ../src/VelbusMessage/ClearLeds.cpp ../src/VelbusMessage/ClearLeds.hpp \
                  ../src/VelbusMessage/SlowBlinkLeds.cpp ../src/VelbusMessage/SlowBlinkLeds.hpp \
                  ../src/VelbusMessage/FastBlinkLeds.cpp ../src/VelbusMessage/FastBlinkLeds.hpp \
                  ../src/VelbusMessage/VFastBlinkLeds.cpp ../src/VelbusMessage/VFastBlinkLeds.hpp \
                  ../src/VelbusMessage/RelayStatus.cpp ../src/VelbusMessage/RelayStatus.hpp \
                  ../src/VelbusMessage/UpdateLeds.cpp ../src/VelbusMessage/UpdateLeds.hpp \
                  ../src/VelbusMessage/MemoryDataRequest.cpp ../src/VelbusMessage/MemoryDataRequest.hpp \
                  ../src/VelbusMessage/MemoryData.cpp ../src/VelbusMessage/MemoryData.hpp \
                  ../src/VelbusMessage/MemoryDataWrite.cpp ../src/VelbusMessage/MemoryDataWrite.hpp \
                  ../src/VelbusMessage/MemoryBlockRequest.cpp ../src/VelbusMessage/MemoryBlockRequest.hpp \
                  ../src/VelbusMessage/MemoryBlock.cpp ../src/VelbusMessage/MemoryBlock.hpp \
                  ../src/VelbusMessage/MemoryBlockWrite.cpp ../src/VelbusMessage/MemoryBlockWrite.hpp \
                  ../src/VelbusMessage/MemoryDumpRequest.cpp ../src/VelbusMessage/MemoryDumpRequest.hpp \
                  ../src/VelbusMessage/BusErrorCounterRequest.cpp ../src/VelbusMessage/BusErrorCounterRequest.hpp \
                  ../src/VelbusMessage/BusErrorCounter.cpp ../src/VelbusMessage/BusErrorCounter.hpp \
                  ../src/VelbusMessage/BlindStatus.cpp ../src/VelbusMessage/BlindStatus.hpp \
                  ../src/VelbusMessage/ModuleType.cpp ../src/VelbusMessage/ModuleType.hpp \
                  ../src/VelbusMessage/NameRequest.cpp ../src/VelbusMessage/NameRequest.hpp \
                  ../src/VelbusMessage/Name.cpp ../src/VelbusMessage/Name.hpp \
                  ../src/VelbusMessage/SwitchBlindOff.cpp ../src/VelbusMessage/SwitchBlindOff.hpp \
                  ../src/VelbusMessage/SwitchBlindUpDown.cpp ../src/VelbusMessage/SwitchBlindUpDown.hpp \
                  ../src/VelbusMessage/ModuleStatusRequest.cpp ../src/VelbusMessage/ModuleStatusRequest.hpp \
                  ../src/VelbusMessage/SwitchRelayOnOff.cpp ../src/VelbusMessage/SwitchRelayOnOff.hpp \
                  ../src/VelbusMessage/StartRelayTimer.cpp ../src/VelbusMessage/StartRelayTimer.hpp \
                  ../src/VelbusMessage/WriteAddr.cpp ../src/VelbusMessage/WriteAddr.hpp \
                  ../src/VelbusMessage/SensorTempRequest.cpp ../src/VelbusMessage/SensorTempRequest.hpp \
                  ../src/VelbusMessage/SensorTemp.cpp ../src/VelbusMessage/SensorTemp.hpp \
                  ../src/VelbusMessage/RTCstatus.cpp ../src/VelbusMessage/RTCstatus.hpp \
                  ../src/VelbusMessage/TempSensorStatus.cpp ../src/VelbusMessage/TempSensorStatus.hpp \
                  ../src/VelbusMessage/SwitchToMode.cpp ../src/VelbusMessage/SwitchToMode.hpp
endif
