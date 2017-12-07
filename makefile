#Alison Stiel and Carl Closs
NUM = 5
SOURCE = CA$(NUM).cpp Course.cpp Student.cpp
HEADERS = Course.h Student.h
OUT = CA$(NUM).o Course.o Student.o
FLAGS = -g -Wall -Wextra -Wno-unused-parameter
NAME = schedule.exe
FILE = Closs_Carl_ccloss1_CA$(NUM).tar.gz
TESTOPTS = lol
DEBUG_OPTS = --silent
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
	cd .. && 	tar -cvzf  $(FILE) CA$(NUM)
ifneq "$(findstring remote, $(HOST))"  "remote"
		firefox submit.htm
else 
		mutt -s "CA$(NUM)_submission" ccloss1@binghamton.edu <submit.htm -a ../$(FILE)
endif
	#hack to determine whether we should use firefox or email to self
