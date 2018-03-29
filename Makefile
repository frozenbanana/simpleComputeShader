LIBS= -lGLEW -lSOIL -lassimp -lGL -lGLU -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi
CC= g++
CFLAGS = -g -Wall -std=c++1z

make: Shader.o GBuffer.o
	$(CC) $(CFLAGS) main.cpp -o run $^ $(LIBS)
#PingPongBuffer.o
Shader.o: Shader.cpp
	$(CC) $(CFLAGS) Shader.cpp -c

GBuffer.o: GBuffer.cpp
	$(CC) $(CFLAGS) GBuffer.cpp -c

#PingPongBuffer.o: PingPongBuffer.cpp
#	$(CC) $(CFLAGS) PingPongBuffer.cpp -c

clean:
		-rm *.o
		-rm run
