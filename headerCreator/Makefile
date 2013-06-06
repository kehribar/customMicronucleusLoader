

PROGRAM_NAME = headerCreator

ifeq ($(shell uname), Linux)
	EXE_SUFFIX =
else ifeq ($(shell uname), Darwin)
	EXE_SUFFIX =
else
	EXE_SUFFIX = .exe
endif

.PHONY:	clean 

all:
	gcc headerCreator.c -o $(PROGRAM_NAME)$(EXE_SUFFIX)

clean:
	rm -rf $(PROGRAM_NAME)$(EXE_SUFFIX) *.o *.exe *.dSYM