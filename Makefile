all:
	g++ weird-xcone.cc -o weird-xcone `fastjet-install/bin/fastjet-config --cxxflags --libs --plugins` -lNsubjettiness -lRecursiveTools -std=c++11