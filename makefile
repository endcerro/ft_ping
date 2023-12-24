# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: edal--ce <edal--ce@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/01 18:59:29 by edal--ce          #+#    #+#              #
#    Updated: 2023/12/24 10:55:19 by edal--ce         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME    = ft_ping

DIRSRC	=	src
OBJD	=	obj
INCLUDE	=	incl

INCLUDEF	=	$(INCLUDE)/header.h	\
				$(INCLUDE)/define.h

SRC	=	main.c	\
	network.c	\
	packet.c	\
	utils.c    print.c

OBJ     = $(SRC:.c=.o)
OBJS    = $(OBJ:%=$(OBJD)/%)

CFLAGS  = -g -fsanitize=address -Wall -Wextra  -Werror
CFRAME  = #-framework AppKit -framework OpenGL

CC      = gcc
RM      = rm -f

LIBFT   = libft
LIB     = libft.a


$(NAME)	:	$(OBJD) $(OBJS) $(INCLUDEF) $(LIB)
		$(CC) -I ./$(INCLUDE) -I ./$(LIBFT)/libft.h $(CFLAGS) $(OBJS) $(LIB) -o $(NAME) 

# $(NAME)   : $(LIB) $(OBJD) $(OBJS) $(INCLUDEF)
#	$(CC) -I ./$(INCLUDE) -I ./$(MLX_OS) $(LIB) $(CFLAGS) $(CFRAME) $(OBJS) -o $(NAME) 

$(OBJD)   :
		@mkdir $(OBJD)

$(OBJD)/%.o : $(DIRSRC)/%.c 
		$(CC) -I ./$(INCLUDE) $(CFLAGS) -o $@ -c $<

$(LIB)      :
		$(MAKE) -C $(LIBFT)
		cp $(LIBFT)/$(LIB) ./.

all     : $(NAME)

clean   :
		$(RM) $(OBJS)

cleanlib : 
		$(MAKE) clean -C $(LIBFT)

fcleanlib : cleanlib 
		echo "fclean $(LIBFT)"
		@$(MAKE) fclean -C $(LIBFT)


fclean    : clean 
		
		$(RM) $(NAME) $(LIB)


bonus   : all

re      : fclean all

.PHONY    : all clean re fclean