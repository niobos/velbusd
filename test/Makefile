test: VelbusMessage-1
	./VelbusMessage-1

clean:
	rm -f *.o
	rm -f VelbusMessage-1

VelbusMessage-1: VelbusMessage-1.o ../VelbusMessage.o
	$(CXX) $(CXXFLAGS) -o $@ $+
