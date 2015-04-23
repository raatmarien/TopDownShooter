topDownShooter: main.o groundTileMap.o player.o shadow.o
	g++ -o topDownShooter main.o groundTileMap.o player.o shadow.o -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
