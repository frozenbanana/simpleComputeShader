LIBS= -lGLEW -lSOIL -lassimp -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
CC= g++
CFLAGS = -g -Wall -std=c++1z

make: Shader.o
	$(CC) $(CFLAGS) main.cpp -o run $^ $(LIBS)

Shader.o: Shader.cpp
	$(CC) $(CFLAGS) Shader.cpp -c

clean:
		-rm *.o
		-rm run
