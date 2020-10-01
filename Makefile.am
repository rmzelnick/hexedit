## Makefile.am -- Process this file with automake to produce Makefile.in
bin_PROGRAMS = hexedit

check_PROGRAMS = tests/test

# Hexedit binary
hexedit_SOURCES = src/hexedit.cpp \
		  src/main.cpp
hexedit_CPPFLAGS = -Wall -Wextra -I include/ --std=c++2a

# Hexedit unit test
tests_test_SOURCES = src/hexedit.cpp \
		     tests/test.cpp
tests_test_CPPFLAGS = -Wall -Wextra -I include/ --std=c++2a
tests_test_LDFLAGS = -lcriterion
