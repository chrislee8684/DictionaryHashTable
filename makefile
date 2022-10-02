spell.exe: spellcheck.o hash.o
	g++ -std=c++11 -o spell.exe spellcheck.o hash.o

spellcheck.o: spellcheck.cpp hash.h
	g++ -std=c++11 -c spellcheck.cpp

hash.o: hash.cpp hash.h
	g++ -std=c++11 -c hash.cpp

debug:
	g++ -std=c++11 -g -o spellDebug.exe spellcheck.cpp hash.cpp

clean:
	rm -f *.exe *.o *.stackdump *~

backup:
	test -d backups || mkdir backups
	cp *.cpp backups
	cp *.h backups
	cp Makefile backups
