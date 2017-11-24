/*  By downloading or using this file, the user agrees to be bound by the terms of the license 
 *  agreement located at http://ocaalliance.com/EULA as an original contracting party.
 */

/*
 *  Description         : Stubs for libc
 *
 */

// ---- Include system wide include files ----

// ---- Include local include files ----
#include <errno.h>
#include <sys/stat.h>
// #include <sys/times.h> // Not in MinGW!
#ifndef WIN32
#include <sys/unistd.h>
#endif

//----- Definitions ----

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

//----- Type declaration ----

#ifndef _CLOCK_T_
#define	_CLOCK_T_	unsigned long         /* See: arm-none-eabi\include\machine\types.h */
#endif

#ifndef __clock_t_defined
typedef _CLOCK_T_ clock_t;	              /* See: arm-none-eabi\include\sys\times.h */
#define __clock_t_defined
#endif

struct tms {	                          /* See: arm-none-eabi\include\sys\times.h */
    clock_t	tms_utime;		/* user time */
    clock_t	tms_stime;		/* system time */
    clock_t	tms_cutime;		/* user time, children */
    clock_t	tms_cstime;		/* system time, children */
};

// ---- Helper functions ----

// ---- Local data ----

/*
 environ
 A pointer to a list of environment variables and their values. 
 For a minimal environment, this empty list is adequate:
 */
char *__env[1] = { 0 };
char **environ = __env;

//----- Local Function declarations ----

int _write(int file, char *ptr, int len);

//----- Externals ----

extern void Rs232DebugPutChar(char character);

#undef errno
extern int errno;


//****************************************************************************

int _close(int file) {
    return -1;
}

/*
 fstat
 Status of an open file. For consistency with other minimal implementations in these examples,
 all files are regarded as character special devices.
 The `sys/stat.h' header file required is distributed in the `include' subdirectory for this C library.
 */
int _fstat(int file, struct stat *st) {
    st->st_mode = S_IFCHR;
    return 0;
}

/*
 isatty
 Query whether output stream is a terminal. For consistency with the other minimal implementations,
 */
int _isatty(int file) {
    switch (file){
    case STDOUT_FILENO:
    case STDERR_FILENO:
    case STDIN_FILENO:
        return 1;
    default:
        errno = EBADF;
        return 0;
    }
}

/*
 kill
 Send a signal. Minimal implementation:
 */
int _kill(int pid, int sig) {
    errno = EINVAL;
    return (-1);
}

/*
 lseek
 Set position in a file. Minimal implementation:
 */
int _lseek(int file, int ptr, int dir) {
    return 0;
}

/*
 read
 Read a character to a file. `libc' subroutines will use this system routine for input from all files, including stdin
 Returns -1 on error or blocks until the number of characters have been read.
 */
int _read(int file, char *ptr, int len) {
    return -1;
}

extern int __putchar(int ch);
/*
 write
 Write a character to a file. `libc' subroutines will use this system routine for output to all files, including stdout
 Returns -1 on error or number of bytes sent
 */
int _write(int file, char *ptr, int len) {
    int n;

    switch (file) {
    case STDOUT_FILENO: /*stdout*/
        for (n = 0; n < len; n++) {
        	__putchar(*ptr++);
        }
        break;
    case STDERR_FILENO: /* stderr */
        for (n = 0; n < len; n++) {
        	__putchar(*ptr++);
        }
        break;
    default:
        errno = EBADF;
        return -1;
    }
    return len;
}
