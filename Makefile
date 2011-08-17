all: open_serial_with_power

clean:
	rm -f *.o
	rm open_serial_with_power

open_serial_with_power: open_serial_with_power.o
