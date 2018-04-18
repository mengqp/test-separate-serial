
################################################################################
#
#	<date>			<author>			<version>			<desc>
#	12/08/08		sunhaifeng			v1.0				Create
#
#
#
################################################################################

#appname
APP = test_separate_serial

CC = /opt/arm-2011.03/bin/arm-none-linux-gnueabi-gcc


#include
INCLUDE := -I.

#cflags
CFLAGS := -Os -Wall -pipe -lpthread -lrt

#objs
OBJS = usart.o download.o up.o main.o

#app
$(APP) : $(OBJS)
	# $(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@; mv $(APP) ./
	$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@;

#*.o
$(OBJS) : %.o : %.c
	$(CC) -c $(CFLAGS) $(INCLUDE) $< -o $@

#clean
.PHONY : clean
clean :
	 rm -fr *.o
	rm -fr $(APP)
