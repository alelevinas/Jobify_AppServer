# Jobify_AppServer


CI: [![Build Status](https://travis-ci.org/alelevinas/Jobify_AppServer.svg?branch=master)](https://travis-ci.org/alelevinas/Jobify_AppServer) Coverage: [![Coverage Status](https://coveralls.io/repos/github/alelevinas/Jobify_AppServer/badge.svg?branch=master)](https://coveralls.io/github/alelevinas/Jobify_AppServer?branch=master) 



Application Server para el proyecto de Taller de Programación II


Instalación:

La única librería que usamos y no compilamos directamente es leveldb, por lo que es necesario ejecutar el comando

sudo apt-get install libsnappy-dev libleveldb1 libleveldb-dev

luego a compilar...

mkdir build
cd build
cmake ..
make

y para ejecutar

cd build
./Jobify_AppServer


Documentación API: [link](http://rebilly.github.io/ReDoc/?url=https://raw.githubusercontent.com/alelevinas/Jobify_AppServer/master/jobify-appserver-serverAPI.yaml)

