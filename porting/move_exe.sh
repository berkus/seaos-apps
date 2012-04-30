#!/bin/bash
cd src
for i in *.o; do 
	cp "$i" ../bin/"${i%.o}"
done
cd ..
