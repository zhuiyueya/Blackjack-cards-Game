# 21点扑克牌

# 【如何使用】


1.客户端文件与服务器文件区分

#cardserver.cpp，sendVCode,sendVCode.cpp文件为服务端，需要在linux下运行

#其余代码文件为客户端，需要在windows环境中运行


2.需提前安装的

#需要安装mysql，并且运行mysql并执行如下命令：

1）通过`CREATE DATABASE card;`命令创建一个名为card的db

2)通过`use card`命令进入db

3）通过`CREATE TABLE user (account VARCHAR(32) PRIMARY KEY,pwd VARCHAR(32) NOT NULL,name VARCHAR(32),email VARCHAR(100) NOT NULL);`创建一个名为user表格,其存储了账号，密码，昵称，邮箱

#需要安装redis并运行

#需安装OpenSSL


3.服务端文件需要修改的地方

1）cardserver.cpp：打开文件后在include后面有标明需要修改的，包括mysql账号，密码，IP，端口，redisIP，端口等

2）sendVCode.cpp:打开文件后在include后面有标明需要修改的，包括邮箱，邮箱授权码等

4.编译

1）g++ cardserver.cpp -o cardserver -I/usr/include/mysql/ -L/usr/local/mysql/lib/ -lmysqlclient -lhiredis

2）g++ g++ sendVCode.cpp -o sendVCode -lssl -lcrypto

3）（ps:若未安装g++/ssl请自行安装）


5.运行

1）注意：运行时sendVCode.cpp编译完的可执行文件需跟cardserver.cpp的可执行文件在同一路径下








