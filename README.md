customMicronucleusLoader
========================

Custom micronucleus based 'single click' firmware installer.

Use the `headerCreator` application to convert the desired .hex file into a header file.

Then, compile the `customMicronucleus` application with the new created `programMemory.h` file.

Also, give a meaningfull name to your customMicronucleus application by editing `PROGRAM_NAME` variable at the Makefile.

ihsan.
