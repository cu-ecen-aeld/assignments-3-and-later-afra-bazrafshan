#include "systemcalls.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
/**
 * @param cmd the command to execute with system()
 * @return true if the command in @param cmd was executed
 *   successfully using the system() call, false if an error occurred,
 *   either in invocation of the system() call, or if a non-zero return
 *   value was returned by the command issued in @param cmd.
*/
bool do_system(const char *cmd)
{

/*
 * TODO  add your code here
 *  Call the system() function with the command set in the cmd
 *   and return a boolean true if the system() call completed with success
 *   or false() if it returned a failure
*/
    int status = system(cmd);
    if (status == 0)
    {
        printf("system call succeed\n"); 
        return true;
    }
    else
    {
        printf("system call failed\n"); 
        return false;
    }
}

/**
* @param count -The numbers of variables passed to the function. The variables are command to execute.
*   followed by arguments to pass to the command
*   Since exec() does not perform path expansion, the command to execute needs
*   to be an absolute path.
* @param ... - A list of 1 or more arguments after the @param count argument.
*   The first is always the full path to the command to execute with execv()
*   The remaining arguments are a list of arguments to pass to the command in execv()
* @return true if the command @param ... with arguments @param arguments were executed successfully
*   using the execv() call, false if an error occurred, either in invocation of the
*   fork, waitpid, or execv() command, or if a non-zero return value was returned
*   by the command issued in @param arguments with the specified arguments.
*/
bool is_valid_absolute_path(const char *path) {
    return access(path, X_OK) == 0;
}

bool do_exec(int count, ...)
{
    printf("start\n");
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    int stat = 0 ;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    if (!is_valid_absolute_path(command[0])) {
        fprintf(stderr, "Error: %s is not a valid absolute path.\n", command[0]);
        return false;
    }
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork failed");
        return false;
    }
    else if (pid == 0)
    {
        if(execv(command[0],command) == -1)
        {
            printf("wrong path\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        printf("Parent pid\n");
        if (waitpid(pid, &stat, 0) == -1) {
        // Wait failed
        perror("waitpid");
        return false;
        }
        if (WIFEXITED(stat) && WEXITSTATUS(stat) == 0) {
        // Child exited normally with zero status
        return true;
        }
        else {
        // Child exited abnormally or with non-zero status
        return false;
        }
    }  

        /*
        if(WIFEXITED (stat))
        {
            printf("22\n");
            return true;
        }
        else
        {
            printf("33,%d\n",WEXITSTATUS (stat));
            return false;
        }
        
        if (waitpid (0, &stat, 0) == -1)
        {
            printf("22\n");
            return false;
        }
        if (WIFEXITED (stat))
        {
            printf("33,%d\n",WEXITSTATUS (stat));
            return true;
        }
        else
        {
            
        }
        */
    
    

    //command[count] = command[count];

/*
 * TODO:
 *   Execute a system command by calling fork, execv(),
 *   and wait instead of system (see LSP page 161).
 *   Use the command[0] as the full path to the command to execute
 *   (first argument to execv), and use the remaining arguments
 *   as second argument to the execv() command.
 *
*/

    va_end(args);
    printf("44\n");
    return false;
}

/**
* @param outputfile - The full path to the file to write with command output.
*   This file will be closed at completion of the function call.
* All other parameters, see do_exec above
*/
bool do_exec_redirect(const char *outputfile, int count, ...)
{
    va_list args;
    va_start(args, count);
    char * command[count+1];
    int i;
    for(i=0; i<count; i++)
    {
        command[i] = va_arg(args, char *);
    }
    command[count] = NULL;
    // this line is to avoid a compile warning before your implementation is complete
    // and may be removed
    //command[count] = command[count];


/*
 * TODO
 *   Call execv, but first using https://stackoverflow.com/a/13784315/1446624 as a refernce,
 *   redirect standard out to a file specified by outputfile.
 *   The rest of the behaviour is same as do_exec()
 *
*/
    pid_t pid;
    int stat = 0;
    int fd = open(outputfile, O_WRONLY|O_TRUNC|O_CREAT, 0644);

    if (fd < 0) 
    { 
        perror("open"); 
        abort(); 
    }
    pid = fork();
    if(pid == -1)
    {
        perror("fork failed");
    }
    else if (pid == 0)
    {
        if (dup2(fd, 1) < 0) 
        { 
            perror("dup2"); 
            abort(); 
        }
        close(fd);
        execv(command[0],command);
    }
    else
    {
        printf("Parent pid\n");
        close(fd);
        if (waitpid(pid, &stat, 0) == -1) {
        // Wait failed
        perror("waitpid");
        return false;
        }
        if (WIFEXITED(stat) && WEXITSTATUS(stat) == 0) {
        // Child exited normally with zero status
        return true;
        }
        else {
        // Child exited abnormally or with non-zero status
        return false;
        }
    }  
    

    va_end(args);

    return true;
}
