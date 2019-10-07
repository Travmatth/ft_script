Terminal Line Disciplines exist between terminal and programs that are run, can set terminals special characters (backspace, line erase, etc.). Terminal line disciplines are not aautomatically provided on network logins, instead a pseudo-terminal is provided. Pseudo-terminal master processes are opened by a process which then calls fork(). The child establishes the new session, dups fd's and calls exec(). The pseudo-terminal slave is set as the controlling terminal for the child process. To the process above the pseudo-terminal slave, the std fd's are a normal terminal device, but functionality that doesnt apply to the pseudo-terminal (e.g., modifying the baud rate) is ignored. This creates a bi-directional pipe - all input to the slave comes from the master, but with the slave over the line terminal discipline we have additional control as opposed to a plain pipe.

## USES
- The script program make a copy in a file of everything that is input and output during a terminal session, by placing itself between the terminal and a new invocation of a login shell. Script is normally run from a login shell, which waits for script to terminate. While running, everythion output byt the terminal line discipline aov ethe PTY slave is copied to the script file (usually called typescript). Since keystrokes are echoed by the terminal line discipline, those are captured.
- Pseudo-terminals can also be used to drive interactive programs in non-interactive modes. 
- Running coprocesses: compiled programs that use std i/o cannot be invoked simultaneously, as the buffered i/o of each process would deadlock as we cannot fflush() the buffers. Driving these processes from a pseudo-terminal allows a controlling process to manage the child coprocess, can be called with pty_fork() instead of fork().
- Watching long-running processes: for long-running processes that generate sparse output, redirecting this output to a file is a suboptimal solution, as the output is buffered and written in large chunks at once if not periodically flushed. Running such a process under a pty would allow the line buffering to be changed.

## USAGE

`int     posix_openpt(int oflag);` // opens pty master, w oflags changing opened behaviour
Before a slave pty can be used, its permissions need to be set so that it is accessible to applications
`int     grantpt(int fd);` // fd of master pty. sets user id of the slaves device node to the callers real user id & nodes group id, `usually to some group to that has access to individual terminal devices. Permissions are set to allower r/w acess to individual `owners and write access to group owners. Commonly set group ownerships of the slave pty to group tty. 
`int     unlockpt(int fd);` // fd of master. grants acces to slave, allowing applications to open device. default prevention of `opening allows programs to initialize the slave and master before use.
`int     ptsname(int fd);` // fd of master, returns name of pty slave if OK.

On Solaris (not OSX?), need to `ioctl(fds, I_FIND, modules)` the modules `ptem`, `ldterm`, `ttcompat` to fully emulate a native terminal, where `ptem` is the pseudo-terminal emulation module, ldterm is the line discipline module, and ttcompat provides compatibility with older ioctl calls. Usually check to for presence before pushing modules. On linux, OSX and solaris the call to open sets the pty slave as the controlling terminal.

```
ptym_open
    posix_openpt(O_RDWR) // open a master pty
        return if err
    grantpt // set uid of slave
        return if err
    unockpt // unlock slave
    ptsname // get slave pty name

ptys_open
    open(slave_name, O_RDWR)
        return if err
    // if solaris only?
    // push modules if needed

pty_fork
    pty_open
    fork process
        child
            open slave
            close master_fd
            ioctl(fds, TCSANOW, slave_termios)
            ioctl(fds, TIOCSWINSZ, slave_winsize)
            dup2 for std i/o - set slave as i/o
        master
            save master fd
            save child pid