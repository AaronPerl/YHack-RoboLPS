EXECUTABLE = camera
CPP_FLAGS = -Wall -Wextra
CXX = g++
SOURCES := $(wildcard *.cpp)
HEADERS := $(wildcard *.hpp)
LIBRARIES := raspicam
LINKS := $(addprefix -l, $(LIBRARIES))

.PHONY : clean all

all : $(EXECUTABLE)

clean :
	rm $(EXECUTABLE)

$(EXECUTABLE) : $(SOURCES) $(HEADERS)
	$(CXX) $(CPP_FLAGS) $(LINKS) $(SOURCES) -o $@
