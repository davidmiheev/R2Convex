 
t = mainconv.cpp R2Convex.cpp paint.cpp bmpt_tcx.cpp

conv: $(t) rand.cpp
	g++ -O3 $(t) -o conv
	g++ rand.cpp -o rand
