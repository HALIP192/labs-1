CC			=	cc
SRC			=	\
			main	\
			server	\
			http
SERVER		=	server
CLIENT		=	client
BUTTON		=	buttons

OBJ_DIR		=	obj
OBJ_SERVER	=	${addprefix $(OBJ_DIR)/,$(SRC:=.o)}
OBJ_CLIENT	=	${addprefix $(OBJ_DIR)/,$(CLIENT:=.o)}
OBJ_BUTTON	=	${addprefix $(OBJ_DIR)/,$(BUTTON:=.o)}

all:	$(OBJ_DIR) $(SERVER) $(CLIENT) $(BUTTON)
	touch output_ser
	touch input_ser

$(OBJ_DIR)/%.o:	%.c
	$(CC) $< -c -o $@

$(OBJ_DIR): 
	mkdir -p $(OBJ_DIR)

$(SERVER):	$(OBJ_SERVER)
	$(CC) $(OBJ_SERVER) -lpthread -g -o $@

$(CLIENT):	$(OBJ_CLIENT)
	$(CC) $< -lpthread -g -o $@

$(BUTTON):	$(OBJ_BUTTON)
	$(CC) $< -lpthread -g -o $@

clean:
	rm -rf $(OBJ_SERVER) $(OBJ_CLIENT) $(OBJ_BUTTON)

fclean:	clean
	rm -rf output_ser input_ser
	rm -rf $(SERVER) $(CLIENT) $(BUTTON)

re:	fclean all

start:	$(SERVER) $(CLIENT) $(BUTTON)
	bash script.sh
