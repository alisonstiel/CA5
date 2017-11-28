NUM = 5
SOURCE = CA$(NUM).cpp course.cpp Student.cpp
HEADERS = course.h student.h
OUT = CA$(NUM).o 
FLAGS = -g -Wall -Wextra -Wno-unused-parameter
NAME = Scheduler
FILE = Closs_Carl_ccloss1_CA$(NUM).tar.gz
TESTOPTS = lol
DEBUG_OPTS = --silent
check: 
	g++ -dumpspec
all: $(NAME)
test: $(NAME)
	./$(NAME) $(TESTOPTS)
debug: $(NAME)
	gdb ./$(NAME) $(DEBUG_OPTS)
valgrind: $(NAME)
	valgrind --leak-check=full ./$(NAME) $(TESTOPTS)
$(NAME): $(SOURCE) $(HEADERS)
	g++ -c $(FLAGS) $(SOURCE)
	g++ $(FLAGS) $(OUT) -o $(NAME) 
clean:
	rm -f *.o *.swp *.gch .go* $(NAME)
submit: $(NAME) clean
	cd .. && 	tar -cvzf  $(FILE) CA$(NUM)ccloss1 
ifneq "$(findstring remote, $(HOST))"  "remote"
		firefox submit.htm
else 
		mutt -s "CA$(NUM)_submission" ccloss1@binghamton.edu <submit.htm -a ../$(FILE)
endif
	#hack to determine whether we should use firefox or email to self
