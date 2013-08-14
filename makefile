run: tank
	./tank

tank: tank.o bullet.o vector.o display.o  main.cpp texture.o sound.o
	g++ -g main.cpp tank.o bullet.o vector.o display.o texture.o sound.o -o tank -framework OpenGL -framework GLUT -framework OpenAL -lpng -lvorbis -logg -lvorbisfile

tank.o: vector.cpp vector.h tank.cpp tank.h
	g++ -g -c tank.cpp

bullet.o: vector.cpp vector.h bullet.cpp bullet.h
	g++ -g -c bullet.cpp

vector.o: vector.cpp vector.h
	g++ -g -c vector.cpp

display.o: display.cpp display.h
	g++ -g -c display.cpp

texture.o: texture.cpp texture.h
	g++ -g -c texture.cpp

sound.o: sound.cpp sound.h
	g++ -g -c sound.cpp

clean:
	rm test *.o