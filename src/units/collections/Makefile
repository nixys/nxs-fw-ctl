include config.mk

PROJ_UNITS_SUB_HEADERS		= $(shell find ./src/units/collections/ -mindepth 2 -maxdepth 2 -type f -name "*.h")

objs/nxs-fw-ctl-u-collections.o: ./src/units/collections/collections.c \
					./src/units/collections/collections.h \
					./src/units/collections/collections-typedefs.h \
					./src/units/collections/collections-subunits.h \
					$(PROJ_CORE_HEADERS) \
					$(PROJ_META_HEADERS) \
					$(PROJ_UNITS_SUB_HEADERS) \
					$(PROJ_COLLECTIONS_HEADERS) \
					$(PROJ_DAL_HEADERS) \
					$(NXS_CORE_OBJS) \
					$(NXS_CORE_HEADERS)

					$(CC) -c $(CFLAGS) $(CFLAGS32) $(NXS_CORE_INCLUDES) $(PROJ_INCLUDES) $(NXS_INCLUDES) $(USE_NXS_MODULES) -o objs/nxs-fw-ctl-u-collections.o ./src/units/collections/collections.c
