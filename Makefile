BINARIES=`pwd`/linuxstandby/c/standby `pwd`/linuxstandby/linux_standby/standby `pwd`/linuxstandby/linux_standby/target `pwd`/brightness/brightness

all: linuxstandby brightness

brightness:
	@make -C brightness
	@echo "Binary ready at `pwd`/brightness/brightness"

linuxstandby: linuxstandby-c linuxstandby-rust

linuxstandby-c:
	@make -C linuxstandby/c
	@echo "Binary ready at `pwd`/linuxstandby/c/standby"
linuxstandby-rust:
	@make -C linuxstandby/linux_standby
	@echo "Binary ready at `pwd`/linuxstandby/linux_standby/standby`

clean:
	# using sudo since we have SETUID binaries
	sudo rm -rf ${BINARIES}
