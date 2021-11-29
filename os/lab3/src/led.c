#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_LEDS 10
#define FILE_EXPORT     "/sys/class/gpio/export"

void    help        ();                                     // print help to terminal
int     write_pin   (char *pin, char value);                // write value to pin

int main(int argc, char *argv[]) {
    
    int quiet = 0;
    
    // 0. help if need:
    if (argc <= 1) {
        help ();
        return -1;
    } else {
        if ((strcmp(argv[1], "-h") == 0)) {
            help ();
            return 0;
        } else {
            if ((strcmp(argv[1], "-q") == 0)) {
                quiet = 1;
            }
        }
    }
    
    if (!quiet) printf("\nThe LED application was started\n\n");
    
    
    // 1. parse arguments:
    char *mode = argv[1 + quiet];
    
	// 1.1. put data to stdout:
	if (strcmp(mode, "-s") == 0) {
		char data[32];
		while (!feof(stdin)) {
			int n = read(STDIN_FILENO, data, sizeof(data));
			data[n] = '\0';
			if (n > 0) {
				printf 	("%s", data);
				fflush 	(stdout);
			}
		}
		return 0;
	}

	// 1.2. send data to file:
	if (strcmp(mode, "-f") == 0) {
		char *file = argv[2 + quiet];

		FILE *f = fopen(file, "w");

		if (f == 0) {
			fprintf (stderr, "ERROR: can't open %s file\n", file);
			return -1;
		}

		char data[32];
		int ret;
		int a = 0;
		int n;
		while (!feof(stdin))  {
			
			n = read(STDIN_FILENO, data, sizeof(data));
			fflush 	(stdin);
			if (n > 0) {			
				data[n] = '\0';
				fflush 	(stdin);

				ret = fwrite(data, strlen(data), 1, f);
				//ret = fwrite("\n", 1, 1, f);
				fflush(f);

				//fprintf(stderr, "a = %d, datalen = %d\n", a++, strlen(data));
			}

		}

		fclose(f);
		return 0;

	}

   // 1.3. put data to GPIO driver:
    int pinstartpos = 1;
    int pinnums;
    char *strtolend;
    
    if (quiet) pinstartpos++;
    pinnums = argc - pinstartpos;
    
    if (pinnums <= 0) {
        fprintf (stderr, "ERROR: no input pins\n");
        return -1;
    }
    
    int pins[MAX_LEDS];
    
    for (int i = pinstartpos; i < (pinstartpos + pinnums); i++) {
        pins[i - pinstartpos] = strtol(argv[i], &strtolend, 10);
        if (strcmp(strtolend, "\0") != 0) {
            fprintf (stderr, "ERROR: invalid pin '%s' \n", argv[i]);
            return -1;
        }
    }
    
    
    
    // 2. pins init:
    int fd;
  
    for (int i = pinstartpos; i < (pinstartpos + pinnums); i++) {
        
        fd = open(FILE_EXPORT, O_WRONLY);
        if (fd == -1) {
            fprintf (stderr, "ERROR: can't open %s file\n", FILE_EXPORT);
            return -1;
        }
        write (fd, argv[i], strlen(argv[i]));
        write (fd, "\n", 1);
        close (fd);
        
        
        char filename[32] = {0};
        strcat(filename, "/sys/class/gpio/gpio");
        strcat(filename, argv[i]);
        strcat(filename, "/direction");
    
        fd = open(filename, O_WRONLY);
        if (fd == -1) {
            fprintf (stderr, "ERROR: can't open %s file\n", filename);
            return -1;
        }
        write (fd, "out\n", 4);
        close (fd);

    }

    
    // 3. main loop:
    int     i;
    char    values[MAX_LEDS];
    int     skip = 0;
    while (1) {
        
        // 3.1. get and check values from stdin:
        scanf("%s", values);
        fflush 	(stdin);
        
        for (i = 0; i < pinnums; i++) {
            if ((values[i] != '0') && (values[i] != '1')) {
                fprintf (stderr, "ERROR: invalid value %c for LED %s\n", values[i], argv[pinstartpos+i]);
                skip = 1;
                break;
            }
        }
        
        if (skip) {
            skip = 0;
            continue;
        }
        
        // 3.2. put values to pins:
        for (i = 0; i < pinnums; i++) {
            if (write_pin (argv[pinstartpos+i], values[i]) == -1) {
                return -1;
            }
        }
        
    }

    return 0;
    
}


int write_pin (char *pin, char value) {
    
    char filename[32] = {0};
    strcat(filename, "/sys/class/gpio/gpio");
    strcat(filename, pin);
    strcat(filename, "/value");
    
    int fd = open(filename, O_WRONLY);
    if (fd == -1) {
        fprintf (stderr, "ERROR: can't open %s file\n", filename);
        return -1;
    }
    
    char v[2] = {0, 0};
    v[0] = value;
    write (fd, v, 1);
    write (fd, "\n", 1);
    close (fd);
    
    return 0;
    
}

void help () {

	printf("    use this application for LEDs state control\n");
	printf("    execute format: ./leds [-h] [-q] [-s] [-f filename] [PINs]\n");
	printf("    -h - help\n");
	printf("    -q - quiet\n");
	printf("    -s - send data to stdout\n");
	printf("    -f - send data to filename\n");
	printf("    -PINs - outputs of the Raspberry PI for led\n");
	printf("    input format (from stdout):\n");
	printf("        XXXX\n");
	printf("        X - state of each led relative to the sequence of input arguments\n");
	printf("    \n");
	printf("    Examples:\n\n");
	printf("    ./led -q 16 18 20\n");
	printf("    010 	// from stdin: led 16 off, led 18 n, led 20 off\n");
	printf("    ./led -s\n");
	printf("    010 	// from stdin\n");
	printf("    010 	// to stdout\n");
	printf("    ./led -f file.txt\n");
	printf("    010 	// from stdin and write to file.txt\n");
    
}
