CC = g++
CPP_FLAGS = -Wall -c -std=c++11 -I. -lX11 -O2

all: PA10

PA10: main.o PoseDisplay.o Utils.o Pose.o
	g++ -O2 -Wall -I. -std=c++11 -lX11 -o PA10 main.o PoseDisplay.o Utils.o Pose.o

main.o: main.cpp PoseDisplay.h Point3D.h Utils.h Pose.h Sorter.h
	$(CC) $(CPP_FLAGS) main.cpp

PoseDisplay.o: PoseDisplay.cpp PoseDisplay.h Point3D.h Pose.h
	$(CC) $(CPP_FLAGS) PoseDisplay.cpp

Utils.o: Utils.cpp Point3D.h Utils.h Pose.h
	$(CC) $(CPP_FLAGS) Utils.cpp

Pose.o: Pose.cpp Pose.h Point3D.h
	$(CC) $(CPP_FLAGS) Pose.cpp

clean:
	rm -f main.o PoseDisplay.o Utils.o Pose.o
