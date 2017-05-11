include config.mk

objs/nxs-fw-ctl-p-main-ctx.o: ./src/proc/main/ctx/main-ctx.c \
	./src/proc/main/ctx/main-ctx.h \
	$(PROJ_CORE_HEADERS) \
	$(PROJ_META_HEADERS) \
	$(NXS_CORE_OBJS) \
	$(NXS_CORE_HEADERS)

	$(CC) -c $(CFLAGS) $(CFLAGS32) $(NXS_CORE_INCLUDES) $(PROJ_INCLUDES) $(NXS_INCLUDES) $(USE_NXS_MODULES) -o objs/nxs-fw-ctl-p-main-ctx.o ./src/proc/main/ctx/main-ctx.c
