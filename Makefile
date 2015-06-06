VPATH = src:headers
CC = g++
CFLAG = -I headers
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
HEADERS = $(wildcard *.h)

all : topDownShooter

%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAG)

topDownShooter : $(OBJ) $(HEADERS)
	$(CC) -o topDownShooter $(OBJ) $(LIBS)

.PHONY : clean
clean :
	rm topDownShooter $(OBJ)
