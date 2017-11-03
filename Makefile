all: bigfloat
bigfloat: bigfloat.cpp
	g++ bigfloat.cpp -o bigfloat.run -lpthread -g