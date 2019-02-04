################
CS:APP Shell Lab
################

NOTICE:
This is a newer version than shell-lab in 2015 Fall lecture.

Files:

Makefile	# Compiles your shell program and runs the tests
README		# This file
tsh.c		# The shell program that you will write and hand in
tshref		# The reference shell binary.

# The remaining files are used to test your shell
sdriver.pl	# The trace-driven shell driver
trace*.txt	# The 15 trace files that control the shell driver
tshref.out 	# Example output of the reference shell on all 15 traces

# Little C programs that are called by the trace files
myspin.c	# Takes argument <n> and spins for <n> seconds
mysplit.c	# Forks a child that spins for <n> seconds
mystop.c        # Spins for <n> seconds and sends SIGTSTP to itself
myint.c         # Spins for <n> seconds and sends SIGINT to itself
  
This lab elaborates how to correctly synchronize execution flow to 
avoid nasty concurrency bugs caused by race conditioning, by managing
a job list mantained by shell.  
For examples of redirection, refer to redirect() in my CS162 repo:
https://github.com/hyqLeonardo/cs162/blob/master/hw1/shell.c
And for mechanism of `pipe()`, refer to my [oneNote page](https://onedrive.live.com/view.aspx?resid=EA543277021158B0%218920&id=documents&wd=target%28CSAPP.one%7CF603253D-4634-4C54-A6B8-9C79B980B673%2FLecture%2016%3A%20System%20I%5C%2FO%7C5A2B0894-8A0A-45A5-9D95-77104DD5DDBB%2F%29
onenote:https://d.docs.live.net/ea543277021158b0/Docs/UCW/CSAPP.one#Lecture%2016%20System%20I/O&section-id={F603253D-4634-4C54-A6B8-9C79B980B673}&page-id={5A2B0894-8A0A-45A5-9D95-77104DD5DDBB}&end).
