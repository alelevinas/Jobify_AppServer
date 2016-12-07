# Jobify_AppServer


CI: [![Build Status](https://travis-ci.org/alelevinas/Jobify_AppServer.svg?branch=master)](https://travis-ci.org/alelevinas/Jobify_AppServer) Coverage: [![Coverage Status](https://coveralls.io/repos/github/alelevinas/Jobify_AppServer/badge.svg?branch=master)](https://coveralls.io/github/alelevinas/Jobify_AppServer?branch=master) 



Application Server para el proyecto de Taller de Programación II


##6. Instalación
Las únicas librería que usamos y no compilamos directamente son leveldb y restclient-cpp.


Hay 3 formas:
* Compilando el código fuente
* Instalando el paquete ```.deb``` en sistemas debian
* Utilizando el ```Dockerfile```

#####Chats
Primero hace falta descargar las librerías necesarias y luego el código fuente. Una vez descargados, hace falta ejecutar ```cmake``` para luego compilar con ```make JobifyAppserver```
un script ```yaml``` de ejemplo se ve acontinuación:
```yaml
install:
  - sudo pip install cpp-coveralls
  - sudo apt-get install libsnappy-dev libleveldb-dev
  #para jwtcpp
  - sudo apt-get install libcrypto++9-dbg libcrypto++-dev
  - sudo apt-get install libjansson4 libjansson-dev libjansson-dbg
  - sudo apt-get install autoconf libtool
  - git clone https://github.com/mrtazz/restclient-cpp.git
  - cd restclient-cpp
  - ./autogen.sh
  - ./configure
  - sudo make install
  - export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
  - cd ..

script:
  - mkdir build
  - cd build
  - cmake ..
  - make JobifyAppserver
  - ./Jobify_Appserver
```

#####Paquete ```.deb```
Una vez descargado el paquete debe ejecutar el siguiente comando:
`sudo dpkg -i Jobify_AppServer-0.1.1-Linux.deb`

El ejecutable se instalará en ```/home/usuario/local/bin``` por lo que estará en el ```PATH``` y se podrá ejecutar en cualquier terminal. Las bases de datos se almacenarán en ```/home/usuario/.Jobify_AppServer/bds```



#####Dockerfile
Simplemente tener descargado Docker [install](https://docs.docker.com/engine/installation/linux/ubuntulinux/)
Luego ubicarse en el mismo directorio del ```Dockerfile``` y ejecutar el comando ```docker build .```





Documentación API: [link](http://rebilly.github.io/ReDoc/?url=https://raw.githubusercontent.com/alelevinas/Jobify_AppServer/master/jobify-appserver-serverAPI.yaml)

