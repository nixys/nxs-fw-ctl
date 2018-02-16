include config.mk

PROJ_UNITS_SUB_HEADERS		= $(shell find ./src/units/projects/fs-objects/ -mindepth 2 -maxdepth 2 -type f -name "*.h")

objs/nxs-fw-ctl-u-projects-fs-objects.o: ./src/units/projects/fs-objects/fs-objects.c \
				./src/units/projects/fs-objects/fs-objects.h \
				./src/units/projects/fs-objects/fs-objects-typedefs.h \
				./src/units/projects/fs-objects/fs-objects-subunits.h \
				$(PROJ_CORE_HEADERS) \
				$(PROJ_META_HEADERS) \
				$(PROJ_UNITS_SUB_HEADERS) \
				$(PROJ_COLLECTIONS_HEADERS) \
				$(PROJ_DAL_HEADERS) \
				$(NXS_CORE_HEADERS)

				$(CC) -c $(CFLAGS) $(CFLAGS32) $(NXS_CORE_INCLUDES) $(PROJ_INCLUDES) $(NXS_INCLUDES) $(USE_NXS_MODULES) -o objs/nxs-fw-ctl-u-projects-fs-objects.o ./src/units/projects/fs-objects/fs-objects.c
