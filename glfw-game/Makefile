CXXFLAGS	= -g -Wall -I/usr/local/include -I/usr/local/Cellar/glfw/3.2.1/include
LDLIBS		= -L/usr/local/lib -L/usr/local/Cellar/glfw/3.2.1/lib -lglfw -lGLEW -framework OpenGL \
	-framework CoreVideo -framework IOKit -framework Cocoa
OBJECTS		= $(patsubst %.cpp,%.o,$(wildcard *.cpp))
TARGET		= glfw-game

.PHONY:	clean

$(TARGET): $(OBJECTS)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	-$(RM) $(TARGET) $(OBJECTS) *~ .*~ core
