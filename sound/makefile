all: .test.o play.o
	@echo "running..."
	./.test 700 0.00004 14 | ./play

.test.o: ./.test.c
	@gcc -c .test.c -o .test.o
	@gcc -o .test .test.o -lm
	@echo "generator is ready"

.play.o: ./play.cc
	@gcc -c play.cc -o .play.o
	@g++ -o play .play.o -lasound
	@echo "player is ready"
