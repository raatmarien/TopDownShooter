topDownShooter: main.o groundTileMap.o player.o 
	g++ -o topDownShooter main.o groundTileMap.o player.o -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
