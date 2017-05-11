include config.mk

PROJ_OBJS	= $(shell find ./objs/ -type f -name "*.o")

default:	build

run:	build
	objs/nxs-fw-ctl -c nxs-fw-ctl.conf

build:	core \
	units \
	collections \
	dal \
	conf \
	proc

	$(LINK) $(NXS_LINKS) $(NXS_USER_LINKS) -o objs/nxs-fw-ctl $(PROJ_OBJS) $(NXS_CORE_LINKS)

clean:

	rm -f ./objs/* || true

core:	./src/nxs-fw-ctl-core.c \
		$(PROJ_CORE_HEADERS) \
		$(PROJ_META_HEADERS) \
		$(PROJ_CONF_HEADERS)

		$(CC) -c $(CFLAGS) $(CFLAGS32) $(NXS_CORE_INCLUDES) $(PROJ_INCLUDES) $(NXS_INCLUDES) $(NXS_USER_INCLUDES) $(USE_NXS_MODULES) -o objs/nxs-fw-ctl-core.o ./src/nxs-fw-ctl-core.c

units:
		make -f ./src/units/Makefile

collections:
		make -f ./src/collections/Makefile

dal:
		make -f ./src/dal/Makefile

conf:
		make -f ./src/conf/Makefile

proc:
		make -f ./src/proc/Makefile
