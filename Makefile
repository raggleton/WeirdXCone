all:
	g++ weird-xcone.cc -o weird-xcone `fastjet-config --cxxflags --libs --plugins` -lNsubjettiness -std=c++11