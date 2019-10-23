# FT_SCRIPT

ft_script is a simple recreation of the [script](http://man7.org/linux/man-pages/man1/script.1.html) command, designed to expolore the usage of pseudo-terminals in posix environments. ft_script sets up a pseudoterminal pair, where the slave is executed with the command specified by the invocation, or the value of $SHELL, or `/bin/sh` failing that. Input written to the shell is then forwarded to the pty slave, and the output of the slave is forwarded to the pty master, where it is written to the shell. All output written to the shell is duplicated to the specified file (or `typescript` if none provided), so that said file contains a record of the session started by `./ft_script`.    

## Installation
```
git clone https://github.com/travmatth/ft_script
make
```

## Usage
```
./ft_script [-aq] [file [command ...]]
```

`-q`: omit prologue/epilogue on terminal and typescript
`-a`: open typescript with append rather than truncate

## Resources

- Advanced Programming in the Unix Environment: Chapter 19: Pseudo Terminals
- The Linux Programming Interface: Chapter 64: Pseudoterminals
- Pty implementation: [grantpt.c](https://opensource.apple.com/source/Libc/Libc-825.25/stdlib/grantpt.c)
- Time implementation: [time.c](https://code.woboq.org/userspace/glibc/sysdeps/unix/sysv/linux/tcsetattr.c.html)
- IOCTL syscalls: [IOCTL List](https://www.freebsd.org/cgi/man.cgi?query=tty&sektion=4)