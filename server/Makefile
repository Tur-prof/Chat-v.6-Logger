SRC = BadLoginMessage.h BadLoginRegistr.h User.h User.cpp Message.h Message.cpp Server.h Server.cpp Database.h Database.cpp Logger.h Logger.cpp Chat.h Chat.cpp main.cpp

server : $(SRC)
	g++ -std=c++17 -o server $(SRC) -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql

install:
	install ./server /usr/local/bin

uninstall:
	rm - rf /usr/local/bin/server
