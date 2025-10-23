set -x
rm -rf `pwd`/build/*
cd `pwd`/build &&
	cmake -DBUILD_SERVER=ON -DBUILD_CLIENT=ON .. &&
	make