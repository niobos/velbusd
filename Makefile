all: velbusd

clean:
	rm -f velbusd
	rm -f *.o *.d
	rm -f config.log config.status
	rm -rf autom4te.cache
	$(MAKE) -C test clean

mrproper: clean
	rm -f configure config.h

.PHONY: test
test:
	$(MAKE) -C test

%.d: %.cpp
	set -e; rm -f "$@"; \
	$(CXX) -M -MG -MM -MF "$@.$$$$" $(CPPFLAGS) "$<"; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < "$@.$$$$" > "$@"; \
	rm -f "$@.$$$$"
DEPS := $(shell find . -name '*.o' )
include $(DEPS:.o=.d)

velbusd: velbusd.o \
		VelbusMessage/VelbusMessage.o VelbusMessage/Registrar.o \
		utils/output.o \
		SockAddr.o Socket.o TimestampLog.o
	$(CXX) $(CXXFLAGS) -o $@ -lev $+
