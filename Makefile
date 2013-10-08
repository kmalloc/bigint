test_big_int : BigInt.cc testBigInt.cc
	g++ -g -Wall -o $@ $^

clean:
	rm test_big_int
