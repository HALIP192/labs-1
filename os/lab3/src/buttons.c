#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_BUTTONS 10
#define FILE_EXPORT     "/sys/class/gpio/export"

void    help        	();                                     // print help to terminal
char    read_pin    	(char *pin);                            // read pin value
int 	read_pins_file 	(char *file);

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
    
    if (!quiet) printf("\nThe Buttons application was started\n\n");
    
    
    
    // 1. parse arguments:
    char *mode = argv[1 + quiet];

    // 1.1. Get data from stdin:
    if (strcmp(mode, "-s") == 0) {
    	char data[32];
    	while (1) {
    		scanf 	("%s", data);
    		fflush 	(stdin);
    		printf 	("%s\n", data);
    		fflush 	(stdout);
    	}
    }

    // 1.2. Get data from file:
    if (strcmp(mode, "-f") == 0) {
    	char *file = argv[2 + quiet];
    	if (read_pins_file(file) < 0) 	return -1;
    	else 				return  0;
    }

    // 1.3. get data from GPIO driver:
    int pinstartpos = 1;
    int pinnums;
    char *strtolend;
    
    if (quiet) pinstartpos++;
    pinnums = argc - pinstartpos;
    
    if (pinnums <= 0) {
        fprintf (stderr, "ERROR: no input pins\n");
        return -1;
    }
    
    int pins[MAX_BUTTONS];
    
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
        write (fd, "in\n", 3);
        close (fd);

    }

    
    // 3. main loop:
    int update = 1;
    char pinv_prev[MAX_BUTTONS];
    char pinv[MAX_BUTTONS];
    
    for (int i = 0; i < MAX_BUTTONS; i++) {
        pinv_prev[i]    = 0;
        pinv[i]         = 0;
    }
    
    while (1) {
        
        // 3.1. read pins value:
        for (int i = pinstartpos; i < (pinstartpos + pinnums); i++) {
            pinv[i - pinstartpos] = read_pin(argv[i]);
                if (pinv[i - pinstartpos] == -1) {
                    return -1;
                }
        }
        
        // 3.2. check if was update:
        if (strcmp(pinv_prev, pinv) != 0) {
            strcpy(pinv_prev, pinv);
            printf("%s\n", pinv);
            fflush(stdout);
        }
        
        // 3.3. pause:
        usleep (0.2 * 1000000);
        
    }
    

    return 0;
    
}


int read_pins_file (char *file) {

	FILE *f = fopen(file,"r");
	if (f == 0) {
		fprintf (stderr, "ERROR: can't open %s file\n", file);
		return -1;
	}

	char str[32];
	while(!feof(f)) {
		if(fscanf(f,"%s\n",str)) printf("%s\n",str);
		fflush(stdout);
		sleep(1);
	}
	fclose(f);

	return 0;

}

char read_pin (char *pin) {
    
    char filename[32] = {0};
    strcat(filename, "/sys/class/gpio/gpio");
    strcat(filename, pin);
    strcat(filename, "/value");
    
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        fprintf (stderr, "ERROR: can't open %s file\n", filename);
        return -1;
    }
    
    char value[3];
    read (fd, value, 3);
    close (fd);
    
    return value[0];

}

void help () {
    
	printf("    use this application for buttons state capture\n");
	printf("    execute format: ./buttons [-h] [-q] [-s] [-f filename] [PINs]\n");
	printf("    -h - help\n");
	printf("    -q - quiet\n");
	printf("    -s - capture data from stdin\n");
	printf("    -f - capture data from filename\n");
	printf("    -PINs - number of inputs the Raspberry PI for buttons\n");
	printf("    \n");
	printf("    output format (to stdout):\n");
    printf("        XXXX\n");
    printf("        X - state of each pin relative to the sequence of input arguments\n");
    printf("    \n");
	printf("    Examples:\n\n");
	printf("    ./buttons -q 16 18 20\n");
	printf("    010\n");
	printf("    000\n");
	printf("    ./buttons -q -s\n");
	printf("    010 	// entered in stdin\n");
	printf("    010 	// sended to stdout\n");
	printf("    ./buttons -q -f file.txt\n");
	printf("    010 	// first line from file\n");
	printf("    000 	// second line from file\n");
	printf("    100 	// e.t.c. \n");
	printf("    \n");
    
}
