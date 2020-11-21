
run:
	clear
		#c++ -o main.exe $(wildcard *.cpp) -larmadillo
		#c++ *.cpp -o a
		#./main.exe
	g++ -o main.exe main.cpp IsingModel.cpp -larmadillo
	./main.exe
	python plott.py
