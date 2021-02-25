CFLAGS = -Wall -Werror=vla -pedantic -std=c11
VAL = --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes

# make: compiler penv.c et creer penv
compile:
	gcc $(CFLAGS) penv.c -o penv

# make val $! HELLO: verifier les fuites de memoire
val: compile
	valgrind $(VAL) ./penv

# make git: telecharger les fichiers modifier sur le depot git
git: clean
	git add -A
	git commit -m "update"
	git push

# make check: Faire les tests
check: penv
	bats check.bats

# make clean: rendre le repertoire a son etat initial
clean:
	rm -rf penv

.PHONY: clean check git

