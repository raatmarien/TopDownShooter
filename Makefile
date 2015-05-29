topDownShooter: main.o groundTileMap.o player.o shadow.o minimap.o light.o contactListener.o bulletManager.o mapGen.o enemy.o enemyManager.o
	g++ -o topDownShooter main.o groundTileMap.o player.o shadow.o minimap.o light.o contactListener.o bulletManager.o mapGen.o enemy.o enemyManager.o -lsfml-graphics -lsfml-window -lsfml-system -l Box2D
