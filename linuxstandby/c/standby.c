#include <stdio.h>
#include <string.h>
#include <errno.h>

#define POWER_STATE "/sys/power/state"
#define POWER_STATE_VALUE "mem\n"

#define COULD_NOT_WRITE_ERROR "Could not write to %s: %s\n"
#define COULD_NOT_COMPLETE_WRITE_ERROR "Could not complete the write to %s: %s"

int
main(int argc, const char *const *argv)
{
    FILE *power_state_file = fopen(POWER_STATE, "wb");
    size_t bytes_written = 0;


    if (power_state_file == NULL) {
        fprintf(stderr, COULD_NOT_WRITE_ERROR, POWER_STATE, strerror(errno));
        return 1;
    }
    bytes_written = fwrite(POWER_STATE_VALUE, sizeof(char), sizeof(POWER_STATE_VALUE), power_state_file);

    if (bytes_written != sizeof(POWER_STATE_VALUE)) {
       fprintf(stderr, "Could not complete the write to %s: %s", POWER_STATE_VALUE, strerror(errno));
       fclose(power_state_file);
       return 1;
    }
    return 0;
}

