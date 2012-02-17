# LIBXMLINUX_PATH: path to the XtratuM library for Linux
LIBXMLINUX_PATH=/opt/xm-sdk/linux/libxm-linux/

all: operador

include ${LIBXMLINUX_PATH}/common/rules.mk

operador: funciones_entrada.c
 
clean:
	@${RM} *.o operador 
