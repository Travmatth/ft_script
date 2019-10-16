# FT_SCRIPT

ft_script is a simple recreation of the (script(1))[] command, designed to expolore the usage of pseudo-terminals in posix environments. ft_script sets up a pseudoterminal pair, where the slave is executed with the command specified by the invocation, or the value of $SHELL, or `/bin/sh` failing that. Input written to the shell is then forwarded to the pty slave, and the output of the slave is forwarded to the pty master, where it is written to the shell. All output written to the shell is duplicated to the specified file (or `typescript` if none provided), so that said file contains a record of the session started by `./ft_script`.    

## Installation
```
git clone https://github.com/travmatth/ft_script
make
```

## Usage
```
./ft_script [-kt] [file [command ...]]
```