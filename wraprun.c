#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

#define PROC_PATH "/proc/systemtap/attach_gdb/wrapper_path"

void proc_write (const char *const data)
{
    int fd = open (PROC_PATH, O_WRONLY);
    if (fd == -1) {
        perror ("Open proc interface");
        exit (EXIT_FAILURE);
    }

    if (write (fd, data, strlen(data)) == -1) {
        perror ("Write to proc interface");
        close (fd);
        exit (EXIT_FAILURE);
    }
    close (fd);
}

#define proc_read(x) proc_read_impl ((x), sizeof ((x) - 1))
void proc_read_impl (char *buf, size_t buf_size)
{
    int fd = open (PROC_PATH, O_RDONLY);
    if (fd == -1) {
        perror ("Open proc interface");
        exit (EXIT_FAILURE);
    }
    
    if (read (fd, buf, buf_size) == -1) {
        perror ("Read from proc interface");
        close (fd);
        exit (EXIT_FAILURE);
    }
    close (fd);
}


/* Because pause() */
void unfreeze (int unused)
{
    return;
}

int main (int argc, char **argv)
{
    char **new_argv = NULL;
    struct sigaction sig;
    memset (&sig, 0, sizeof(sig));

    char buf[1024] = { 0 };
    proc_read (buf);
    if (strncmp (buf, argv[0], strlen (argv[0]) != 0)) {
        proc_write (argv[0]);
        execve (argv[0], argv, NULL);
        return EXIT_FAILURE;
    }

    sig.sa_handler = &unfreeze;
    sigaction (SIGUSR1, &sig, NULL);

    pause ();
    
    new_argv = argv;
    ++new_argv;
    execvp (argv[1], new_argv);
    
    return EXIT_FAILURE;
}
