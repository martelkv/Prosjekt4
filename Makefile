
run:
	clear
		#c++ -o main.exe $(wildcard *.cpp) -larmadillo
		#c++ *.cpp -o a
		#./main.exe
	g++ -o main_ising_model.exe main_ising_model.cpp IsingModel.cpp -larmadillo
	./main_ising_model.exe
	python plott.py
