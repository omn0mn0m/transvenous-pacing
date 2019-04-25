# the compiler: gcc for C program, define as g++ for C++
CPPC = clang++

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
bin:
	mkdir $@

test_main: bin
	$(CPPC) -std=c++11 -Itest/mock -Iinclude $(CPPFLAGS) -o ./test/bin/$@ ./test/$@.cpp -v

test:		## Runs unit tests for the devices
test: test_main
	./test/bin/$^