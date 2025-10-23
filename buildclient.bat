@echo off
rmdir /s /q build
mkdir build
cd build
cmake .. -DBUILD_SERVER=OFF -DBUILD_CLIENT=ON
cmake --build . --config Release

pause