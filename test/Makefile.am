check_PROGRAMS = NBcat Parser
dist_check_SCRIPTS = simply-run.sh fork.sh \
                     echo.sh echo-with-garbage.sh read-only-socket.sh \
                     logging.sh \
                     slow-client.sh \
                     Parser-garbage.sh \
                     Parser-BusOff.sh Parser-BusActive.sh \
                     Parser-RxBuffFull.sh Parser-RxReady.sh \
                     Parser-IntStatusRequest.sh
TESTS = $(dist_check_SCRIPTS)

NBcat_SOURCES = NBcat.cpp

Parser_SOURCES = Parser.cpp \
                 ../src/VelbusMessage/VelbusMessage.cpp ../src/VelbusMessage/VelbusMessage.hpp \
                 ../src/VelbusMessage/Registrar.cpp ../src/VelbusMessage/Registrar.hpp \
                 ../src/utils/output.cpp ../src/utils/output.hpp \
                 ../src/VelbusMessage/BusOff.cpp ../src/VelbusMessage/BusOff.hpp \
                 ../src/VelbusMessage/BusActive.cpp ../src/VelbusMessage/BusActive.hpp \
                 ../src/VelbusMessage/RxBuffFull.cpp ../src/VelbusMessage/RxBuffFull.hpp \
                 ../src/VelbusMessage/RxReady.cpp ../src/VelbusMessage/RxReady.hpp \
                 ../src/VelbusMessage/IntStatusRequest.cpp ../src/VelbusMessage/IntStatusRequest.hpp

if WANT_PARSER
Parser_TESTS = Parser-PushButtonStatus.sh \
               Parser-SetLeds.sh Parser-ClearLeds.sh \
               Parser-SlowBlinkLeds.sh Parser-FastBlinkLeds.sh Parser-VFastBlinkLeds.sh \
               Parser-UpdateLeds.sh \
               Parser-RelayStatus.sh \
               Parser-MemoryData.sh Parser-MemoryBlock.sh
dist_check_SCRIPTS += $(Parser_TESTS)

Parser_SOURCES += ../src/VelbusMessage/PushButtonStatus.cpp ../src/VelbusMessage/PushButtonStatus.hpp \
                  ../src/VelbusMessage/SetLeds.cpp ../src/VelbusMessage/SetLeds.hpp \
                  ../src/VelbusMessage/ClearLeds.cpp ../src/VelbusMessage/ClearLeds.hpp \
                  ../src/VelbusMessage/SlowBlinkLeds.cpp ../src/VelbusMessage/SlowBlinkLeds.hpp \
                  ../src/VelbusMessage/FastBlinkLeds.cpp ../src/VelbusMessage/FastBlinkLeds.hpp \
                  ../src/VelbusMessage/VFastBlinkLeds.cpp ../src/VelbusMessage/VFastBlinkLeds.hpp \
                  ../src/VelbusMessage/RelayStatus.cpp ../src/VelbusMessage/RelayStatus.hpp \
                  ../src/VelbusMessage/UpdateLeds.cpp ../src/VelbusMessage/UpdateLeds.hpp \
                  ../src/VelbusMessage/MemoryData.cpp ../src/VelbusMessage/MemoryData.hpp \
                  ../src/VelbusMessage/MemoryBlock.cpp ../src/VelbusMessage/MemoryBlock.hpp
endif
