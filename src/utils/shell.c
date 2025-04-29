#include "str.h"
#include "buf.h"
#include "utils/shell.h"

#include <stdio.h>    // perror
#include <stdlib.h>   // EXIT_FAILURE
#include <sys/wait.h> // pid
#include <unistd.h>   // linux related

ShellResult shell_exec(Str command, isize outlen, isize errlen, Arena *perm)
{
    // Get null terminated string of command
    const char* command_cstr = str_c(command, perm);

    // Default result
    ShellResult result = {{}, {}, CMD_FAIL_UNKNOWN};

    int   stdout_pipe[2], stderr_pipe[2];
    pid_t pid;

    // Create pipes for stdout and stderr
    if (pipe(stdout_pipe) == -1 || pipe(stderr_pipe) == -1)
    {
        perror("pipe");
        return (ShellResult){.err = CMD_FAIL_PIPE};
    }

    // Fork the process
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return (ShellResult){.err = CMD_FAIL_FORK};
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
            if (cap <= 0) return (ShellResult){.err = CMD_FAIL_OOM};
            num_bytes  = read(stdout_pipe[0], out.buf + out.len, cap);
            out.len   += num_bytes;
            cap       -= num_bytes;
        } while (num_bytes != 0);
        result.stdout = buf_final(&out, perm);
        close(stdout_pipe[0]);

        // Read stderr from the pipe NOTE: Pointer arithmatic
        Buf err   = buf_new(errlen, perm);
        num_bytes = 0;
        cap       = err.cap;
        do {
            if (cap <= 0) return (ShellResult){.err = CMD_FAIL_OOM};
            num_bytes  = read(stderr_pipe[0], err.buf + err.len, cap);
            err.len   += num_bytes;
            cap       -= num_bytes;
        } while (num_bytes != 0);
        result.stderr = buf_final(&err, perm);
        close(stderr_pipe[0]);

        // Wait for the child process to finish and get the exit status
        waitpid(pid, (int*)&result.status, 0);
        if (WIFEXITED(result.status)) { result.status = WEXITSTATUS(result.status); }
    }

    result.err = CMD_SUCCESS;
    return result;
}
