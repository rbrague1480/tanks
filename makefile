test: tank.o bullet.o vector.o display.o  main.cpp
	g++ main.cpp tank.o bullet.o vector.o display.o -o test -framework OpenGL -framework GLUT

tank.o: vector.cpp vector.h tank.cpp tank.h
	g++ -c tank.cpp

bullet.o: vector.cpp vector.h bullet.cpp bullet.h
	g++ -c bullet.cpp

vector.o: vector.cpp vector.h
	g++ -c vector.cpp

display.o: display.cpp display.h
	g++ -c display.cpp

clean:
	rm tank.o test vector.o display.o bullet.o