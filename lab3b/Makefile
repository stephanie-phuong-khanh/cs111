# NAME: Ashley Zhu, Stephanie Doan
# EMAIL: ashleytz@g.ucla.edu, stephaniekdoan@ucla.edu
# ID: 505308582, 604981556

default:
		rm -rf lab3b
		echo '#!/bin/bash' >> lab3b
		echo 'python lab3b.py "$$@"' >> lab3b
		chmod +x lab3b

dist:
	tar -cvzf lab3b-604981556.tar.gz lab3b.py Makefile README

clean:
	rm -f lab3b  *.tar.gz
