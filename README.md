# Build Instructions
Building on both windows and linux should be about the same.
You're going to need cmake and conan.
Cmake you can grab from kitware or your favorite package manager.
I recommend installing conan from pip. i.e. 
```pip install conan```
Then do something like the following in the source tre
```
mkdir build
cd build
conan ../
cmake ../
```
Then on windows you get sln files to load into VS and on linux you get MakeFiles for make to consume.