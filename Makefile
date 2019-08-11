BINARIES=`pwd`/linuxstandby/c/standby `pwd`/linuxstandby/linux_standby/standby `pwd`/linuxstandby/linux_standby/target

all: linuxstandby

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
