all: teste
teste: teste.cpp
	g++ teste.cpp -o teste.run -lpthread -g