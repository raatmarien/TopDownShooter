CC = g++
CFLAG = -Wall
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
OBJ = main.o              \
      groundTileMap.o     \
      player.o            \
      shadow.o         	  \
      minimap.o        	  \
      light.o          	  \
      contactListener.o   \
      bulletManager.o     \
      mapGen.o            \
      enemy.o             \
      updatableManager.o  \
      lineIntersections.o \
      item.o

all: topDownShooter

%.o : %.c
	gcc -c $(CFLAGS) $< -o $@

topDownShooter: $(OBJ)
	g++ -o topDownShooter $(OBJ) $(LIBS)

clean:
	rm *.o
