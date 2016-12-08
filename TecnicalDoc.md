#Documentacion Técnica
***
*Proyecto:* Jobify AppServer
*Author:* Grupo 3  
*Created:* 30/11/2016  
*Version:* 1.0.0  
***

##Table of Contents
1. Servidor HTML
2. Bases de datos
3. JSON
4. LOG
5. Protocolo de acceso
6. Instalación
***

##1. Servidor HTTP
Para montar el servidor se utilizó la libreria mongoose-cpp. [Github](https://github.com/Gregwar/mongoose-cpp "mongoose-cpp").
La misma consiste en una encapsulación de mongoose de forma tal que para manejar los distintos endpoints se utilizan controladores.
Los controladores son los que registran las rutas, el HTTP verb y el handler que actuará cuando un cliente le pega.
Actualmente la aplicación consta de 3 controladores:
#### ProfileController
Se utiliza para todo lo relacionado con alta, baja, modificación y actualización de perfiles de usuario.
También para los servicios de agregar contacto, recomendar y búsquedas con filtros.
#### ChatController
Se utiliza para el manejo de chats. Enviar, pedir una conversación o borrar un mensaje entre otras cosas.
#### SharedServerController
Se utiliza para entregar información antes consultada ante el SharedServer.
Son skills, job_positions y categories.
    
***
##2. Bases de datos
Para el almacenado de la información se utilizó la librería leveldb. La misma es noSQL y almacena cualquier tipo de dato en una relación *clave-valor*
Para manejarse con Json creamos la clase DB con métodos que reciben valores en JSON y parsean antes de devolver.
Hay 4 bases de datos principales:
 
* Users
* Accounts
* Sessions
* Chats
 
#####Users
Esta base de datos almacena todo el perfil del usuario en formato json. Un ejemplo de usuario es:
```json
{
		"chats" : ["id1","id2"],
		"contacts" : ["nuan@a.com", "nico@b.com"],
		"coordenates" : "",
		"dob" : "19/08/1993",
		"email" : "ale.l@hotmail.com",
		"firebase_token" : "ftpNLANtYrg:APA91bG1SMVr9u3DSsKV88XVxDjKnAzix3ZLHr1jiKvOIq1rF2LM9PZffhvAjkEAJxzxdWo_Tlq-oX2xSO0P5iQC30QnsxXjydYh4NwY3l5TRIcS8iiFUn51vhiAgRY6Zt82MKnXOH-V",
		"gender" : "male",
		"name" : "Alejandro Levinas",
		"picture" : "",
		"recommended_by" : ["leo@a.com", "pedro@a.com"],
		"username" : "ale.l@hotmail.com"
}
```
#####Accounts
La base de datos almacena usuarios y contraseñas.
#####Sessions
Almacena información de la sesión actual. Esto es, username y timestamp. La clave es el Token, que es una combinación de username, password y timestamp encryptados. Es el mismo token que se devuelve al cliente para acceder a la API. 
#####Chats
Almacena los chats.
Las claves son *username1_username2* o *username2_username1*. Contiene un array de mensajes en el que cada mensaje contiene el *username* del emisor, el *timestamp* y el cuerpo del mensaje.
***
##3. JSON
Para el manejo de JSON se utilizó la librería *jsoncpp* [Github](https://github.com/open-source-parsers/jsoncpp) 

***
##4. LOG
El log se realizó mediante la librería *easylogging++* [Github](https://github.com/easylogging/easyloggingpp)
***
##5. Protocolo de acceso
Para acceder a las funcionalidades principales del servidor hace falta estar autentificado.
Al hacer */signup*, un cliente da de alta un usuario con un *username* y una *password*. La respuesta a ese endpoint es un *token*.
Este *token* tiene una duración y solo es válido mientras no expire.
Para la utilización de la mayoriá de los endpoint hace falta incluir el *token* en los headres HTTP. Es un header con clave ```Token``` y valor el *token* recibido. Ej ```Token: 55FCF21B40F8A9A1D56D95F302DA9045```

##6. Instalación
Hay 3 formas:
* Compilando el código fuente
* Instalando el paquete ```.deb``` en sistemas debian
* Utilizando el ```Dockerfile```

#####Compilando
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
  - ./JobifyAppserver
```

#####Paquete ```.deb```
Una vez descargado el paquete debe ejecutar el siguiente comando:
`sudo dpkg -i Jobify_AppServer-0.1.1-Linux.deb`

El ejecutable se instalará en ```/home/usuario/local/bin``` por lo que estará en el ```PATH``` y se podrá ejecutar en cualquier terminal. Las bases de datos se almacenarán en ```/home/usuario/.Jobify_AppServer/bds```



#####Dockerfile
Simplemente tener descargado Docker [install](https://docs.docker.com/engine/installation/linux/ubuntulinux/)
Luego ubicarse en el mismo directorio del ```Dockerfile``` y ejecutar el comando ```docker build .```