include config.mk

objs/nxs-fw-ctl-conf-file-json.o: ./src/conf/file-json/file-json.c \
					./src/conf/file-json/file-json.h \
					$(PROJ_CORE_HEADERS) \
					$(PROJ_META_HEADERS) \
					$(PROJ_COLLECTIONS_HEADERS) \
					$(PROJ_DAL_HEADERS) \
					$(NXS_CORE_OBJS) \
					$(NXS_CORE_HEADERS)

					$(CC) -c $(CFLAGS) $(CFLAGS32) $(NXS_CORE_INCLUDES) $(PROJ_INCLUDES) $(NXS_INCLUDES) $(USE_NXS_MODULES) -o objs/nxs-fw-ctl-conf-file-json.o ./src/conf/file-json/file-json.c
