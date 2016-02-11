
OBJECTS = Coder.o Licenses.o FileData.o Server.o Client.o main.o
#CXX=g++
CXXFLAGS = -g -O0 -Wall -D_GLIBCXX_DEBUG
#CXXFLAGS = -O2 -pipe -Wall

licenser:  $(OBJECTS)
	$(CXX) -pthread -lssl -lboost_filesystem -lboost_date_time $(OBJECTS) -o licenser

clean:
	rm -f *.o licenser
