## Preliminaries
- there is a rendering (in the deposit git)
- valid author file
- the makefile is present and has the required rules
- no fault of norm, the norminette being authentic
- the only functions allowed are the calls systems (section 2 of the man) and ctime

## Functionality
When running ft_script, does a new shell launch?
Is it well used? To verify:
- test with "echo $$" before running ft_script, and after. The 2 pid must of course be different.
- do a ls -la in the new shell, check that the display is correct, and that at the end we are always in the new shell. If we went back to the old shell the ft_script is wallowing.


## Test the output file
We're out of the ft_script
The output file is coming soon?
- we will take care of the timestamp and the name of the file
- we will later make an exact diff with what comes out of the script command of the system
- we are looking for the moment if the file contains the activity that has been done in the subshell: we find the / the commands and the / the results

## TTY tests
display the tty in the original shell, run ft_script and display the tty in the subshell. Check that the two posters are different.

## Messages in the file and in the terminal
Like the system binary, the messages at the beginning and end of the session, in the terminal on the one hand, and in the file, on the other hand, with the timestamp, are present and identical to those of the system (space, upper case , just as it should).

## Options to manage
Count the number of correctly implemented options (between 0 and 4, done):
- we can give another name to the output file
- the option -a append the acitivity to the output file
- the -q option omits the start and end messages
- we can place a specific command instead of the shell

## Near the system script
In your ft_script, type a start of the command and then delete it, and finally type another one. The output file contains these "del"? (check with the output of the system script)

## Near the system script++
In your output file, the commands do not appear 2 times, and on the shell when you just enter, there is no empty line that appears.

## Control-C
Throw in your ft_script a program that does not give up right away. Perform a Control-C and verify that you are still in the sub-shell (and thus not returned to the original shell or you have started the ft_script).

## Diff with the system script
Create a tpescript with the system script and then with exactly the same commands in your ft_script.

Compare the 2 output files, only the timestamp has to differ.

Include only manipulations like arrows or del on your shell for both programs.

## Bonus

## Option -k
Check the option by issuing the passwd command in the ft_script (type anything of course, not your password). This command leaves the echo mode of the tty, but the tapes characters must be present in the output file.

## Option -t
Check that there is a default flush in the file every 30 sec, and that it is modifiable by the -t option.

Management of the resizing of the window.
If you see an ft_script that handles resizing the window, it's cool.
To test it: do a ls -C / etc /
- In the normal shell, this must be done eg. 2 columns. Enlarge the window, start again, you will have 3 columns.
- In the system script. it does not work (boom!)
- Test in your ft_script to have the points.

Take the opportunity to check if there is no cheating. It's not common to think about this case, check that it's not a copy-paste project from the source of an OS.