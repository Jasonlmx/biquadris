CXX = g++
CXXFLAGS = -std=c++20 -L/usr/X11R6/lib -Wall -Werror=vla -MMD -lX11
EXEC = biquadris
OBJECTS = blocks.o main.o gameBoard.o levels.o levelController.o window.o
DEPENDS = ${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}
