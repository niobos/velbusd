bin_PROGRAMS = velbusd

velbusd_SOURCES = velbusd.cpp \
                  VelbusMessage/VelbusMessage.hpp VelbusMessage/VelbusMessage.cpp \
                  VelbusMessage/Registrar.hpp VelbusMessage/Registrar.cpp \
                  utils/output.hpp utils/output.cpp \
                  utils/TimestampLog.hpp utils/TimestampLog.cpp \
                  utils/Registrar.t \
                  SockAddr.hpp SockAddr.cpp \
                  Socket.hpp Socket.cpp
velbusd_LDFLAGS = -lev

if WANT_PARSER
velbusd_SOURCES += VelbusMessage/PushButtonStatus.cpp VelbusMessage/PushButtonStatus.hpp \
                   VelbusMessage/SetLeds.cpp VelbusMessage/SetLeds.hpp \
                   VelbusMessage/ClearLeds.cpp VelbusMessage/ClearLeds.hpp \
                   VelbusMessage/SlowBlinkLeds.cpp VelbusMessage/SlowBlinkLeds.hpp \
                   VelbusMessage/FastBlinkLeds.cpp VelbusMessage/FastBlinkLeds.hpp \
                   VelbusMessage/VFastBlinkLeds.cpp VelbusMessage/VFastBlinkLeds.hpp \
                   VelbusMessage/RelayStatus.cpp VelbusMessage/RelayStatus.hpp \
                   VelbusMessage/UpdateLeds.cpp VelbusMessage/UpdateLeds.hpp \
                   VelbusMessage/MemoryData.cpp VelbusMessage/MemoryData.hpp
endif

