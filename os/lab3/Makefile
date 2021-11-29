CC			=	clang

BUTTONS		=	button
LEDS		=	led
TASKS		=	main
SRC_DIR		=	src
OBJ_DIR		=	obj
OBJ_MAIN	=	${addprefix $(OBJ_DIR)/,$(TASKS:=.o)}


all:	$(OBJ_DIR)	\
		$(TASKS)	

$(OBJ_DIR):	
	mkdir -p obj

$(TASKS):	$(OBJ_MAIN)
	$(CC) $(CFLAGS) $< -o $(TASKS)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.c
	$(CC) $< -c -o $@

clean:
	rm -rf $(OBJ_MAIN)

fclean:	clean
	rm -rf $(TASKS)

re:	fclean	all
