SUBDIRS = VelbusMessage

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
velbusd_LDADD = VelbusMessage/libvelbusmessage.a
endif
