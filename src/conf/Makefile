include config.mk

PROJ_CONF_SUB_HEADERS	= $(shell find ./src/conf/ -mindepth 2 -maxdepth 2 -type f -name "*.h")

default: objs/nxs-fw-ctl-conf.o

	for i in `find ./src/conf/  -mindepth 2 -name "Makefile"`; \
	do \
		make -f $$i || exit 1; \
	done

objs/nxs-fw-ctl-conf.o: ./src/conf/conf.c \
				./src/conf/conf.h \
				$(PROJ_CORE_HEADERS) \
				$(PROJ_META_HEADERS) \
				$(PROJ_CONF_SUB_HEADERS) \
				$(PROJ_COLLECTIONS_HEADERS) \
				$(PROJ_DAL_HEADERS) \
				$(NXS_CORE_OBJS) \
				$(NXS_CORE_HEADERS)

				$(CC) -c $(CFLAGS) $(CFLAGS32) $(NXS_CORE_INCLUDES) $(PROJ_INCLUDES) $(NXS_INCLUDES) $(USE_NXS_MODULES) -o objs/nxs-fw-ctl-conf.o ./src/conf/conf.c
