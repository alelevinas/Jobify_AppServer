# Jobify_AppServer


CI: [![Build Status](https://travis-ci.org/alelevinas/Jobify_AppServer.svg?branch=master)](https://travis-ci.org/alelevinas/Jobify_AppServer) Coverage: [![Coverage Status](https://coveralls.io/repos/github/alelevinas/Jobify_AppServer/badge.svg?branch=master)](https://coveralls.io/github/alelevinas/Jobify_AppServer?branch=master) 



Application Server para el proyecto de Taller de Programación II


Instalación:

Las únicas librería que usamos y no compilamos directamente son leveldb y restclient-cpp, por lo que es necesario ejecutar los comandos

sudo apt-get install libsnappy-dev libleveldb1 libleveldb-dev
sudo apt-get install autoconf libtool git
git clone https://github.com/mrtazz/restclient-cpp.git
cd restclient-cpp
./autogen.sh
./configure
sudo make install
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
cd ..

luego a compilar...

mkdir build
cd build
cmake ..
make

y para ejecutar

cd build
./Jobify_AppServer


Documentación API: [link](http://rebilly.github.io/ReDoc/?url=https://raw.githubusercontent.com/alelevinas/Jobify_AppServer/master/jobify-appserver-serverAPI.yaml)

