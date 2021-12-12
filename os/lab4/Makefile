CC			=	cc
SRC			=	\
			main	\
			server	\
			http
SERVER		=	server
CLIENT		=	client

OBJ_DIR		=	obj
OBJ_SERVER	=	${addprefix $(OBJ_DIR)/,$(SRC:=.o)}
OBJ_CLIENT	=	${addprefix $(OBJ_DIR)/,$(CLIENT:=.o)}

all:	$(OBJ_DIR) $(SERVER) $(CLIENT)

$(OBJ_DIR)/%.o:	%.c
	$(CC) $< -c -o $@

$(OBJ_DIR): 
	mkdir -p $(OBJ_DIR)

$(SERVER):	$(OBJ_SERVER)
	$(CC) $(OBJ_SERVER) -lpthread -g -o $@

$(CLIENT):	$(OBJ_CLIENT)
	$(CC) $< -lpthread -g -o $@

clean:
	rm -rf $(OBJ_SERVER) $(OBJ_CLIENT)

fclean:	clean
	rm -rf $(SERVER) $(CLIENT)

re:	fclean all
	rm -rf output_ser input_ser
	touch output_ser
	touch input_ser
