bt (between)
============

  - counts the time between occurrences of the given string on stdin
  - stdin is consumed.
  - output will be the times in floating point seconds, one per line

Building
--------

Run make. The output binary bt will be created (assuming that your CC
is set to something sane and that your standard libraries conform to
POSIX.1-2001).

Usage
-----
bt <string>

If the string is not given, "\n" is used.
Note that bt disables buffering of stdin and operates one
character at a time, but buffering may need to be disabled for the
program generating input data.

