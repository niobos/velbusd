check_PROGRAMS = NBcat
dist_check_SCRIPTS = slow-client.sh
TESTS = slow-client.sh

NBcat_SOURCES = NBcat.cpp

if WANT_PARSER
check_PROGRAMS += Parser
Parser_TESTS = Parser-PushButtonStatus.sh \
               Parser-SetLeds.sh Parser-ClearLeds.sh \
               Parser-SlowBlinkLeds.sh Parser-FastBlinkLeds.sh Parser-VFastBlinkLeds.sh \
               Parser-RelayStatus.sh
dist_check_SCRIPTS += $(Parser_TESTS)
TESTS += $(Parser_TESTS)

Parser_SOURCES = Parser.cpp \
                 ../src/VelbusMessage/VelbusMessage.cpp ../src/VelbusMessage/VelbusMessage.hpp \
                 ../src/VelbusMessage/Registrar.cpp ../src/VelbusMessage/Registrar.hpp \
                 ../src/utils/output.cpp ../src/utils/output.hpp \
                 ../src/VelbusMessage/PushButtonStatus.cpp ../src/VelbusMessage/PushButtonStatus.hpp \
                 ../src/VelbusMessage/SetLeds.cpp ../src/VelbusMessage/SetLeds.hpp \
                 ../src/VelbusMessage/ClearLeds.cpp ../src/VelbusMessage/ClearLeds.hpp \
                 ../src/VelbusMessage/SlowBlinkLeds.cpp ../src/VelbusMessage/SlowBlinkLeds.hpp \
                 ../src/VelbusMessage/FastBlinkLeds.cpp ../src/VelbusMessage/FastBlinkLeds.hpp \
                 ../src/VelbusMessage/VFastBlinkLeds.cpp ../src/VelbusMessage/VFastBlinkLeds.hpp \
                 ../src/VelbusMessage/RelayStatus.cpp ../src/VelbusMessage/RelayStatus.hpp
endif
