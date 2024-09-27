# How to Use

## On client.cpp:

Change the values port and site to whatever port and ip is your target, or if you use serveo, just leave it
as your localhost

## On server.py:
Leave it as it is


## To put it to work
$ ssh -R <yourlink>:80:<whateverisonipvariable>:88 serveo.net

$ sudo python3 server.py # Linux
or
$ python server.py       # Windows

Compile the client program with $ make client

run the client on the target machine



Just to be clear, this is not a virus or anything like that. It's just a project I wanted to make, and can't
actually be used for harm as is. 
