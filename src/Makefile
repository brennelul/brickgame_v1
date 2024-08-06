FLAGS := -Wall -Werror -Wextra --std=gnu11 -lncurses
GCOV_FLAGS := -fprofile-arcs -ftest-coverage
CLANG_FLAGS := --style=Google --verbose
LIBCHECK := $(shell pkg-config --libs check)
CFILES := tetris/main.c tetris/tetris.c gui/graphics.c tests/tests.c
HFILES := tetris/tetris.h
REPORT_DIR = /$@/html/index.html

ifeq ($(shell uname -r | grep WSL -o), WSL)
	HTML_OPEN = @echo -e '\n'HTML $@ is here --\> $(shell pwd)$(REPORT_DIR)
else ifeq ($(shell uname -s), Linux)
	HTML_OPEN = @xdg-open .$(REPORT_DIR)
else
	HTML_OPEN = @open .$(REPORT_DIR)
endif

all: install

check: 
ifeq ("", "$(shell which pkg-config)")
	$(error "Please install pkg-config")
else ifeq ("", "$(LIBCHECK)")
	$(error "Please install check")
endif
install: uninstall
	@mkdir BrickGame
	gcc tetris/main.c tetris/tetris.c gui/graphics.c  -o BrickGame/tetris.out $(FLAGS)
	@echo -e '\n'Game is here --\> $(shell pwd)/BrickGame/
uninstall:
	@rm -rf BrickGame
dvi:
ifeq ("", "$(shell which doxygen)")
	$(error "Please install doxygen")
endif
	$(MAKE) --directory=doxygen
	$(HTML_OPEN)
test: clean check style_check tetris.a
	gcc tests/tests.c tetris.a -o test.out $(FLAGS) $(LIBCHECK)
	./test.out
gcov_report: clean check
ifeq ("", "$(shell which lcov)")
	$(error "Please install lcov")
endif
	@rm -rf gcov_report
	gcc tests/tests.c tetris/tetris.c -o gcov_report.out $(FLAGS) $(GCOV_FLAGS) $(LIBCHECK)
	./gcov_report.out
	lcov -t "brickgame" -o brickgame.info -c -d . -q
	genhtml -o gcov_report/html brickgame.info -q
	@rm -f brickgame.info *.gcda *gcno gcov_report.out
	@rm -f *.o
	$(HTML_OPEN)
tetris.a:
	gcc -c -o tetris.o tetris/tetris.c $(FLAGS)
	ar rcs tetris.a tetris.o
	@rm -f *.o
clean:
	rm -f *.a *.o *.info *.gcda *.gcno test.out *.tar
	rm -rf gcov_report dvi
rebuild: clean test

style_check:
	clang-format -n $(CLANG_FLAGS) $(CFILES)
style_fix:
	clang-format -i $(CLANG_FLAGS) $(CFILES) $(HFILES)

valgrind: clean
	gcc tests/tests.c tetris/tetris.c -o test.out $(FLAGS) $(LIBCHECK)
	valgrind --tool=memcheck --leak-check=yes ./test.out