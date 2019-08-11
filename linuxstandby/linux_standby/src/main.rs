use std::fs::OpenOptions;
use std::io::Write;

const POWER_STATE: &str = "/sys/power/state";
const POWER_STATE_VALUE: &[u8] = b"mem\n";

fn c_main() -> i32 {
    let mut filestream = match OpenOptions::new().write(true).read(false).open(POWER_STATE) {
        Ok(file) => file,
        Err(e) => {
            eprintln!("ERROR: {}", e);
            return 1
        }
    };

    match filestream.write_all(POWER_STATE_VALUE) {
        Ok(_) => {}
        Err(e) => {
            eprintln!("ERROR: {}", e);
            return 1
        }
    }
    0

}


fn main() {
    std::process::exit(c_main())
}
