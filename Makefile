# the compiler: gcc for C program, define as g++ for C++
CPPC = g++

#compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CPPFLAGS = -g -Wall

# Help target from https://gist.github.com/prwhite/8168133

# Add the following 'help' target to your Makefile
# And add help text after each target name starting with '\#\#'
 
help:		## Show this help.
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'

# =================================== 
# Make Targets
# =================================== 
test_main:
	$(CPPC) -Itest/mock -Iinclude $(CPPFLAGS) -o test/$@ test/$@.cpp

test:		## Runs unit tests for the devices
test: test_main
	./test/$^