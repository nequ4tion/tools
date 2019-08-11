CC=cc
# being cautious since this is a SETUID binary
CFLAGS=-fstack-protector-all -fPIE

all:
	@echo "Compiling standby.c"
	${CC} ${CFLAGS} standby.c -o standby
	@echo "Stripping binary"
	strip ./standby
	@echo "Changing the binary's ownership to root"
	sudo chown root:root ./standby
	@echo "Setting the SETUID bit"
	sudo chmod u+s ./standby
	@echo "All done. Type \`./standby\` to send your computer to standby."

