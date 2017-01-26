LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
LIBS += -L ../src -lguisan 
CXXFLAGS = -I ../include

calculator:main.o LLCalculator.o calculator.o
	$(CXX) main.o LLCalculator.o calculator.o -o calculator $(LIBS)
clean:
	-rm -rf *.o
	-rm -rf caculator
