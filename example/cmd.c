/* Capture output, error and status of shell command */
#include <stz/arena.h>
#include <stz/buf.h>
#include <stz/str.h>

#include <stdio.h>    // perror
#include <stdlib.h>   // EXIT_FAILURE
#include <sys/wait.h> // pid
#include <unistd.h>   // linux related

typedef enum {
    CMD_SUCCESS = 0,
    CMD_FAIL_OOM,
    CMD_FAIL_PIPE,
    CMD_FAIL_FORK,
    CMD_FAIL_UNKNOWN,
} CmdError;

typedef struct {
    Str      out;
    Str      err;
    CmdError status;
} CmdResult;

CmdResult command_execute(Str command, isize outlen, isize errlen, Arena* perm);

CmdResult command_execute(Str command, isize outlen, isize errlen, Arena* perm)
{
    // Get null terminated string of command
    const char* command_cstr = str_c(command, perm);

    // Default result
    CmdResult result = {{}, {}, CMD_FAIL_UNKNOWN};

    int   stdout_pipe[2], stderr_pipe[2];
    pid_t pid;

    // Create pipes for stdout and stderr
    if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1)
    {
        perror("pipe");
        return (CmdResult){.status = CMD_FAIL_PIPE};
    }

    // Fork the process
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (CmdResult){.status = CMD_FAIL_FORK};
    }

    if (pid == 0)
    { // Child process
        // Redirect stdout to the write end of the stdout pipe
        dup2(stdout_pipe[1], STDOUT_FILENO);
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);

        // Redirect stderr to the write end of the stderr pipe
        dup2(stderr_pipe[1], STDERR_FILENO);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);

        // Execute the command
        int err = execl("/bin/sh", "sh", "-c", command_cstr, (char*)NULL);
        if (err)
        {
            perror("execl"); // If execl fails
            exit(EXIT_FAILURE);
        }
    }
    else
    { // Parent process
        // Close the write ends of the pipes
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Keep track of capacity for out of memory error
        // Fortunately, capacity is signed (ptrdiff_t)
        isize cap, num_bytes;

        // Read stdout from the pipe NOTE: Pointer arithmatic
        Buf out   = buf_new(outlen, perm);
        cap       = out.cap;
        num_bytes = 0;
        do {
            if (cap <= 0) return (CmdResult){.status = CMD_FAIL_OOM};
            num_bytes  = read(stdout_pipe[0], out.buf + out.len, cap);
            out.len   += num_bytes;
            cap       -= num_bytes;
        } while (num_bytes != 0);
        result.out = buf_final(&out, perm);
        close(stdout_pipe[0]);

        // Read stderr from the pipe NOTE: Pointer arithmatic
        Buf err   = buf_new(errlen, perm);
        num_bytes = 0;
        cap       = err.cap;
        do {
            if (cap <= 0) return (CmdResult){.status = CMD_FAIL_OOM};
            num_bytes  = read(stderr_pipe[0], err.buf + err.len, cap);
            err.len   += num_bytes;
            cap       -= num_bytes;
        } while (num_bytes != 0);
        result.err = buf_final(&err, perm);
        close(stderr_pipe[0]);

        // Wait for the child process to finish and get the exit status
        waitpid(pid, (int*)&result.status, 0);
        if (WIFEXITED(result.status)) { result.status = WEXITSTATUS(result.status); }
    }

    return result;
}

int main()
{
    constexpr i32 MB = 1024 * 1024;

    Arena     perm    = arena_new(16 * MB);
    Str       command = {0};
    CmdResult result  = {0};
    Arena     temp    = {0};

    temp    = perm;
    command = Str("ls -lah ./");
    result  = command_execute(command, 14 * MB, 2 * MB, &temp);
    printf("Status: %d\n", result.status);
    printf("stdout:\n%.*s\n", pstr(result.out));
    printf("stderr:\n%.*s\n", pstr(result.err));
    arena_print("After `ls -lah ./` > temp:\n", &temp);
    arena_print("After `ls -lah ./` > perm:\n", &perm);

    temp    = perm;
    command = Str("ls -lah ./abcdef");
    result  = command_execute(command, 14 * MB, 2 * MB, &temp);
    printf("Status: %d\n", result.status);
    printf("stdout:\n%.*s\n", pstr(result.out));
    printf("stderr:\n%.*s\n", pstr(result.err));
    arena_print("After `ls -lah ./abcdef` > temp:\n", &temp);
    arena_print("After `ls -lah ./abcdef` > perm:\n", &perm);

    return 0;
}
