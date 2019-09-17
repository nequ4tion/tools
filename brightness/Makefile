CC=cc
# being cautious since this is a SETUID binary
CFLAGS=-fstack-protector-all -fPIE

all:
	@echo "Compiling brightness.c"
	${CC} ${CFLAGS} brightness.c -o brightness
	@echo "Stripping binary"
	strip ./brightness
	@echo "Changing the binary's ownership to root"
	sudo chown root:root ./brightness
	@echo "Setting the SETUID bit"
	sudo chmod u+s ./brightness
	@echo "All done."

