#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

#define BACKLIGHT_DIR "/sys/class/backlight/"
#define BRIGHTNESS_FILE "brightness"

#define MAX_BRIGHTNESS 999

void print_help(char * const *argv);
char *locate_brightness_file();
int check_if_brightness_file_in_dir(char *dirname);

int
main(int argc, char *const *argv) {
    FILE *brightness_file = fopen(BRIGHTNESS_FILE, "rb+");
    if (brightness_file == NULL) {
        char *brightness_file_location = locate_brightness_file();
        if (brightness_file_location == NULL) {
            fprintf(stderr, "Could not open the brightness file: %s\n", strerror(errno));
            return 1;
        } else {
            brightness_file = fopen(brightness_file_location, "rb+");
            free(brightness_file_location);

            if (brightness_file == NULL) {
                fprintf(stderr, "Could not open the brightness file: %s\n", strerror(errno));
                return 1;
            }
        }
    }

    char current_brightness_str[4] = { 0 };
    fread(current_brightness_str, 1, 3, brightness_file);
    int current_brightness = atoi(current_brightness_str);

    int opt, increase, decrease, value;
    opt = increase = decrease = value = 0;

    while((opt = getopt(argc, argv, "hi:d:")) != -1) {
        switch(opt) {
            case 'h': {
                print_help(argv);
                return 0;
                break;
            }
            case 'i': {
                increase = 1;
                value = atoi(optarg);
                break;
            }
            case 'd': {
                decrease = 1;
                value = atoi(optarg);
                break;
            }
            default: {
                print_help(argv);
                return 1;
                break;
            }
        }
    }

    if (increase != 0 && decrease != 0) {
        fprintf(stderr, "You can only either increase or decrease the brightness.");
        print_help(argv);
        return 1;
    }

    if (decrease == 1 && value > current_brightness) {
        fwrite("000", 1, sizeof("000"), brightness_file);
        return 0;
    }
    if (increase == 1 && current_brightness + value >= MAX_BRIGHTNESS) {
        fwrite("999", 1, sizeof("999"), brightness_file);
        return 0;
    }

    if (increase == 1) {
        int new_brightness = value + current_brightness;
        fprintf(brightness_file, "%d", new_brightness);
    } else if (decrease == 1) {
        int new_brightness = current_brightness - value;
        fprintf(brightness_file, "%d", new_brightness);
    } else {
        print_help(argv);
        return 1;
    }

}

void print_help(char * const *argv) {
    fprintf(stderr, "Usage: %s -[hid]<VALUE>\n"
               "-h          print this help message\n"
               "-i <VALUE>  increase the brightness by the specified value\n"
               "-d <VALUE>  decrease the brightness by the specified value\n", 
               argv[0]);
}

char *locate_brightness_file() {
    DIR *backlight_dir = opendir(BACKLIGHT_DIR);
    struct dirent *dir = NULL;
    char *full_path_to_backlight = NULL;

    if (!backlight_dir)
        return NULL;

    while ((dir = readdir(backlight_dir)) != NULL) {
        if (strncmp(dir->d_name, ".", 1) != 0 && strncmp(dir->d_name, "..", 2) != 0) {
            size_t d_name_len = strlen(dir->d_name);
            full_path_to_backlight = calloc(1, sizeof (BACKLIGHT_DIR) + d_name_len + 1);
            if (full_path_to_backlight == NULL) {
                closedir(backlight_dir);
                return NULL;
            }
            strncpy(full_path_to_backlight, BACKLIGHT_DIR, sizeof(BACKLIGHT_DIR));
            strncat(full_path_to_backlight, dir->d_name, d_name_len);
            if (check_if_brightness_file_in_dir(full_path_to_backlight) == 1) {
                char *tmp = full_path_to_backlight;
                full_path_to_backlight = realloc(full_path_to_backlight, sizeof(BACKLIGHT_DIR) + d_name_len + sizeof(BRIGHTNESS_FILE) + 2);

                if (full_path_to_backlight == NULL) {
                    free(tmp);
                    return NULL;
                }
                strncat(full_path_to_backlight, "/", sizeof("/"));
                strncat(full_path_to_backlight, BRIGHTNESS_FILE, sizeof(BRIGHTNESS_FILE));
            } else {
                free(full_path_to_backlight);
                full_path_to_backlight = NULL;
            }
        }
        if (full_path_to_backlight != NULL)
            return full_path_to_backlight;
    }
    closedir(backlight_dir);
    return NULL;
}

/* returns 0 if not in directory and 1 if in directory */
int check_if_brightness_file_in_dir(char *dirname) {
    DIR *dir_to_check = opendir(dirname);
    struct dirent *entry = NULL;

    if (!dir_to_check)
        return 0;

    while ((entry = readdir(dir_to_check)) != NULL) {
        if (strncmp(entry->d_name, BRIGHTNESS_FILE, sizeof(BRIGHTNESS_FILE)) == 0) {
            closedir(dir_to_check);
            return 1;
        }
    }
    return 0;
}