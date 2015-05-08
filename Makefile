topDownShooter: main.o groundTileMap.o player.o shadow.o minimap.o light.o
	g++ -o topDownShooter main.o groundTileMap.o player.o shadow.o minimap.o light.o -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
