all:
	cargo build --release
	@echo "Moving binary to current directory"
	mv ./target/release/linux_standby ./standby
	@echo "Stripping binary"
	strip ./standby
	@echo "Setting ownership to root"
	sudo chown root:root ./standby
	@echo "Setting SETUID bit"
	sudo chmod u+s ./standby
	@echo "All done. Type \`./standby\` to send your computer to standby."
