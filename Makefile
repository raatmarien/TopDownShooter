topDownShooter: main.o groundTileMap.o player.o shadow.o minimap.o light.o contactListener.o
	g++ -o topDownShooter main.o groundTileMap.o player.o shadow.o minimap.o light.o contactListener.o -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
