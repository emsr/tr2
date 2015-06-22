CXX = $$HOME/bin/bin/g++ -g -std=c++14 -fdiagnostics-color=always

all: test_filesystem

test_filesystem: test_filesystem.cpp
	$(CXX) -o test_filesystem test_filesystem.cpp path.cc directory_iterator.cc recursive_directory_iterator.cc filesystem.cc

