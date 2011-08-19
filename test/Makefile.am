check_PROGRAMS = 
dist_check_SCRIPTS = 
TESTS = 

if WANT_PARSER
check_PROGRAMS += Parser
dist_check_SCRIPTS += Parser-RelayStatus.sh
TESTS += Parser-RelayStatus.sh

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
