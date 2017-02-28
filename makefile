Executable = Game

all: $(Executable)

$(Executable): mainController.cpp windowController.cpp gameController.cpp VP.cpp gameObjects.cpp
	g++ -o $(Executable) mainController.cpp windowController.cpp gameController.cpp VP.cpp gameObjects.cpp -lSOIL -lglfw -lGLEW -lGL -ldl -I./ -L./lib -std=c++11
