topDownShooter: main.o groundTileMap.o player.o shadow.o minimap.o
	g++ -o topDownShooter main.o groundTileMap.o player.o shadow.o minimap.o -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
