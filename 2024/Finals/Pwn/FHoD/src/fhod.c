#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

// Constraints and constants
#define MAX_WRITES_ALLOWED 0x5
#define MAX_READS_ALLOWED 0x5
#define MAX_MODIFIES_ALLOWED 0x2

#define MAX_CONTENT_SIZE 0x100
#define MAX_IO_FILE_SIZE 0x1E0

#define PROMPT "(FHoD) $ "

__attribute__((constructor))
void __constructor__(){
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

typedef enum _IO_ERROR_ {
    NULL_PTR          = 0xFFFF,
    IS_ALREADY_OPEN   = 0xFE,
    NOT_OPEN          = 0xDE,
    INVAL_FILE_NAME   = 0xAD,
    ERR_INVAL         = 0x151,
    READ_FAILED       = 0x0A,
    WRITE_FAILED      = 0x0B,
    MODIFY_FAILED     = 0x0C,
    INVAL_MODE        = 0x0D,
    DO_NOT_EXIT       = 0x0E,
    CONSTRAINTS_ERROR = 0x0F,
    SUCCESS           = 0xDEADBEEF
} _IO_ERROR_;

typedef enum _IO_MODE_ {
    _IO_OPEN_         = 0x0, // Opening file using fopen
    _IO_READ_         = 0x1, // Reading from file using fread
    _IO_WRITE_        = 0x2, // Writing to file using fwrite
    _IO_CLOSE_        = 0x3, // Closing file using fclose
    _IO_MODIFY_       = 0x4  // Modify the contents of the file.
} _IO_MODE_;

typedef struct _CUSTOM_IO_FILE_ {
    FILE *file;
    char content[MAX_CONTENT_SIZE];
    bool is_open;
    _IO_MODE_ mode;
} _CUSTOM_IO_FILE_;

typedef struct _IO_COUNTER_HANDLER_ {
    __uint16_t read;
    __uint16_t write;
    __uint16_t modify;
}_IO_COUNTER_HANDLER_;

_IO_COUNTER_HANDLER_ global_counter_handler = {
    .read   = 0,
    .write  = 0,
    .modify = 0
};

void __err(const char* msg, int exit_code) {
    printf("%s\n", msg);
    if(exit_code != DO_NOT_EXIT)
        exit(exit_code);
}

void get_input(const char* msg, int *in) {
    // Secure integer input function.
    // https://stackoverflow.com/questions/41145908/how-to-stop-user-entering-char-as-int-input-in-c
    printf("%s", msg);
    char next;
    if (scanf("%d", in) < 0 || *in < 0 || ((next = getchar()) != EOF && next != '\n')) {
         clearerr(stdin);
         do next = getchar(); while (next != EOF && next != '\n');
         clearerr(stdin);
    }
}

_CUSTOM_IO_FILE_*
_init_IO() {
    _CUSTOM_IO_FILE_* io = (_CUSTOM_IO_FILE_*)malloc(sizeof(_CUSTOM_IO_FILE_));
    io->file = NULL;
    io->is_open = false;
    return io;
}

_IO_ERROR_
_IO_precheck_(const _CUSTOM_IO_FILE_ *io, _IO_MODE_ MODE) {

    if(!io) {
        __err(
            "_IO_precheck_(): Uninitialized _CUSTOM_IO_FILE*", NULL_PTR);
    } else if(MODE == _IO_READ_) { // _IO_read_
        if(!io->file || !io->is_open) {
            __err(
                "_IO_read_(): File not opened", DO_NOT_EXIT);
            return NULL_PTR;
        }

        if(io->mode != _IO_READ_) {
            __err(
                "_IO_read_(): File was not opened in READ mode", DO_NOT_EXIT);
            return INVAL_MODE;
        }

        if(global_counter_handler.read >= MAX_READS_ALLOWED) {
            __err(
                "_IO_read_(): Constraint reached. Cannot perform reading.\n", DO_NOT_EXIT);
            return CONSTRAINTS_ERROR;
        }

    } else if(MODE == _IO_WRITE_) { // _IO_write_
        if(!io->file || !io->is_open) {
            __err(
                "_IO_write_(): File not opened", DO_NOT_EXIT);
            return NULL_PTR;
        }

        if(io->mode != _IO_WRITE_) {
            __err(
                "_IO_write_(): File was not opened in WRITE mode", DO_NOT_EXIT);
            return INVAL_MODE;
        }

        if(global_counter_handler.write >= MAX_WRITES_ALLOWED) {
            __err(
                "_IO_write_(): Constraint reached. Cannot perform writing.\n", DO_NOT_EXIT);
            return CONSTRAINTS_ERROR;
        }

    } else if(MODE == _IO_CLOSE_) { // _IO_close_
        if(!io->file || !io->is_open) {
            __err(
                "_IO_close_(): File not opened", DO_NOT_EXIT);
            return NULL_PTR;
        }
    } else if(MODE == _IO_MODIFY_) {
        if(!io->file || !io->is_open) {
            __err(
                "_IO_modify_(): File not opened", DO_NOT_EXIT);
            return NULL_PTR;
        }

        if(global_counter_handler.modify >= MAX_MODIFIES_ALLOWED) {
            __err(
                "_IO_modify_(): Constraint reached. Cannot modify the contents of the file.\n", DO_NOT_EXIT);
            return CONSTRAINTS_ERROR;
        }
    }
    return SUCCESS;
}

_IO_ERROR_
_IO_open_(_CUSTOM_IO_FILE_* io, const char* file, _IO_MODE_ mode) {

    _IO_ERROR_ err;
    if((err = _IO_precheck_(io, _IO_OPEN_)) != SUCCESS) return err; 
    if(err != SUCCESS) return err;

    if(!(io->file = fopen(file, ((mode == _IO_READ_) ? "r" : "w")))) {
        __err(
            "_IO_open_(): invalid file.", DO_NOT_EXIT);
            return ERR_INVAL;
    }

    io->is_open = true;
    io->mode = mode;
    return SUCCESS;
}

_IO_ERROR_
_IO_read_(_CUSTOM_IO_FILE_* io) {
    int nbytes;
    _IO_ERROR_ err;
    if((err = _IO_precheck_(io, _IO_READ_)) != SUCCESS) return err; 

    memset(io->content, '\0', MAX_CONTENT_SIZE);
    if((nbytes = fread(io->content, sizeof(char), MAX_CONTENT_SIZE, io->file)) <= 0) {
        __err(
            "_IO_read(): fread failed", DO_NOT_EXIT);
        return READ_FAILED;
    }
    global_counter_handler.read += 1;
    return SUCCESS;
}

_IO_ERROR_
_IO_write_(_CUSTOM_IO_FILE_* io, const char* buffer) {
    _IO_ERROR_ err;
    if((err = _IO_precheck_(io, _IO_WRITE_)) != SUCCESS) return err;
    memset(io->content, '\0', MAX_CONTENT_SIZE);
    strncpy(io->content, buffer, MAX_CONTENT_SIZE);

    if(fwrite(io->content, sizeof(char), strlen(io->content), io->file) <= 0) {
        __err(
            "_IO_write(): fwrite failed", DO_NOT_EXIT);
        return WRITE_FAILED;
    }
    global_counter_handler.write += 1;
    return SUCCESS;
}

_IO_ERROR_
_IO_close_(_CUSTOM_IO_FILE_* io) {
    _IO_ERROR_ err;
    if((err = _IO_precheck_(io, _IO_CLOSE_)) != SUCCESS) return err; 
    fclose(io->file);
    io->is_open = false;
    return SUCCESS;
}

_IO_ERROR_
_IO_modify_(_CUSTOM_IO_FILE_* io) {
    _IO_ERROR_ err;
    if((err = _IO_precheck_(io, _IO_MODIFY_)) != SUCCESS) return err;

    printf("Enter the content you want to write: ");
    if(read(0, io->file, MAX_IO_FILE_SIZE) < 0) {
        return MODIFY_FAILED;
    }
    global_counter_handler.modify += 1;
    return SUCCESS;
}

void menu() {
    puts(" === File Handling on Demand ===\n");
    puts("1. Open File");
    puts("2. Read File");
    puts("3. Write to a File");
    puts("4. Close File");
    puts("5. Modify File");
    puts("0. Exit");
}

int main(int argc, char* argv[], char* envp[]) {

    _CUSTOM_IO_FILE_ *io = _init_IO();
    _IO_ERROR_ err;
    int choice;
    int mode;
    char buffer[MAX_CONTENT_SIZE];

    while(true) {
        printf("\n\n");
        menu();
        get_input(PROMPT, &choice);
        switch(choice) {
            case 0:
                goto _end;
            case 1:
                if(_IO_precheck_(io, _IO_OPEN_) != SUCCESS) break;
                printf("Enter the name of the file you want to open: ");
                fgets(buffer, MAX_CONTENT_SIZE, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';
                if(strstr(buffer, "flag")) {
                    printf("*** _IO_open_(): failed. flag is an invalid keyword ***\n");
                    break;
                }

                puts("Enter the mode you want to open the file in: ");
                puts("1. READ");
                puts("2. WRITE");
                _input:
                get_input(PROMPT, &mode);
                if(mode != 1 && mode != 2) {
                    printf("Invalid choice. Try again: ");
                    goto _input;
                }

                if((err = _IO_open_(io, buffer, (_IO_MODE_)mode)) != SUCCESS)
                    printf("*** _IO_open_(): failed. Error: 0x%x ***\n", err);
                else
                    printf("*** _IO_open_(): File opened. Descriptor: 0x%x\n", io->file);
                break;

            case 2:
                ((err = _IO_read_(io)) != SUCCESS) ?
                    printf("*** _IO_read_(): failed. Error: 0x%x ***\n", err)
                : printf("*** _IO_read_(): Read %ld bytes. ***\n", strlen(io->content));
                break;

            case 3:
                if(_IO_precheck_(io, _IO_WRITE_) != SUCCESS) break;
                printf("Enter the content you want to write to file: ");
                fgets(buffer, MAX_CONTENT_SIZE, stdin);
                buffer[strcspn(buffer, "\n")] = '\0';

                ((err = _IO_write_(io, buffer)) != SUCCESS) ?
                    printf("*** _IO_write_(): failed. Error: 0x%x ***\n", err)
                    : printf("*** _IO_write_(): Wrote %ld bytes to file. ***\n", strlen(io->content));
                memset(io->content, '\0', MAX_CONTENT_SIZE); // Clear content POST-Write
                break;

            case 4:
                ((err = _IO_close_(io)) != SUCCESS) ?
                    printf("*** _IO_close_(): failed. Error: 0x%x ***\n", err) :
                    printf("*** _IO_close_(): closed file successfully.\n");
                break;

            case 5:
                ((err = _IO_modify_(io)) != SUCCESS) ?
                    printf("*** _IO_modify_(): failed. Error: 0x%x ***\n", err) :
                    puts("*** _IO_modify_(): Updated the contents of the file. ***");
                break;

            default:
                printf("Invalid choice. Try again...");
                break;
        }
    }
    _end:
    if(io && io->file && io->is_open)
        fclose(io->file);
    
    return 0;
}
