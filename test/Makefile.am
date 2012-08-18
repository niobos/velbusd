check_PROGRAMS = NBcat Parser VMB1RS-sim

NBcat_SOURCES = NBcat.cpp

Parser_SOURCES = Parser.cpp \
                 ../src/utils/output.cpp ../src/utils/output.hpp \
                 ../src/VelbusMessage/VelbusMessage.cpp ../src/VelbusMessage/VelbusMessage.hpp \
                 ../src/VelbusMessage/Registrar.cpp ../src/VelbusMessage/Registrar.hpp \
                 ../src/VelbusMessage/Unknown.cpp ../src/VelbusMessage/Unknown.hpp \
                 ../src/VelbusMessage/BusOff.cpp ../src/VelbusMessage/BusOff.hpp \
                 ../src/VelbusMessage/BusActive.cpp ../src/VelbusMessage/BusActive.hpp \
                 ../src/VelbusMessage/RxBuffFull.cpp ../src/VelbusMessage/RxBuffFull.hpp \
                 ../src/VelbusMessage/RxReady.cpp ../src/VelbusMessage/RxReady.hpp

VMB1RS_sim_SOURCES = VMB1RS-sim.cpp \
                     ../src/utils/output.cpp ../src/utils/output.hpp
VMB1RS_sim_LDADD = -lutil

testscripts = Parser-BusOff.sh Parser-BusActive.sh \
              Parser-RxBuffFull.sh Parser-RxReady.sh \
              Parser-PushButtonStatus.sh \
              Parser-SetLeds.sh Parser-ClearLeds.sh \
              Parser-SlowBlinkLeds.sh Parser-FastBlinkLeds.sh Parser-VFastBlinkLeds.sh \
              Parser-UpdateLeds.sh \
              Parser-RelayStatus.sh \
              Parser-MemoryDataRequest.sh Parser-MemoryData.sh Parser-MemoryDataWrite.sh \
              Parser-MemoryBlockRequest.sh Parser-MemoryBlock.sh Parser-MemoryBlockWrite.sh \
              Parser-MemoryDumpRequest.sh \
              Parser-BusErrorCounterRequest.sh Parser-BusErrorCounter.sh \
              Parser-BlindStatus.sh \
              Parser-ModuleTypeRequest.sh Parser-ModuleType.sh \
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
              Parser-SwitchToMode.sh \
              Parser-ModuleStatus.sh \
              Parser-garbage.sh \
              simply-run.sh fork.sh \
              echo.sh echo-with-garbage.sh \
              RxBuffFull.sh \
              logging.sh log-rotating.sh \
              slow-client.sh

dist_check_SCRIPTS = $(testscripts)
TESTS = $(testscripts)

