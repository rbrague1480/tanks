test: tank.o bullet.o vector.o display.o  main.cpp texture.o sound.o
	g++ main.cpp tank.o bullet.o vector.o display.o texture.o sound.o -o test -framework OpenGL -framework GLUT -framework OpenAL -lpng -lvorbis -logg -lvorbisfile

tank.o: vector.cpp vector.h tank.cpp tank.h
	g++ -c tank.cpp

bullet.o: vector.cpp vector.h bullet.cpp bullet.h
	g++ -c bullet.cpp

vector.o: vector.cpp vector.h
	g++ -c vector.cpp

display.o: display.cpp display.h
	g++ -c display.cpp

texture.o: texture.cpp texture.h
	g++ -c texture.cpp

sound.o: sound.cpp sound.h
	g++ -c sound.cpp

clean:
	rm tank.o test vector.o display.o bullet.o texture.o