LIBS= -lGLEW -lSOIL -lassimp -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
CC= g++
CFLAGS = -g -Wall -std=c++1z

make:
	$(CC) $(CFLAGS) main.cpp -o run $^ $(LIBS)

clean:
		-rm *.o
		-rm run
