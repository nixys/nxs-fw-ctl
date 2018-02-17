#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_FS_OBJECTS_C
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_FS_OBJECTS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/projects/fs-objects/fs-objects-typedefs.h>
#include <units/projects/fs-objects/fs-objects.h>
#include <units/projects/fs-objects/fs-objects-subunits.h>

/* Definitions */



/* Project globals */
extern		nxs_process_t		process;
extern		nxs_fw_ctl_cfg_t	nxs_fw_ctl_cfg;

/* Module typedefs */



/* Module declarations */

typedef struct
{
	nxs_string_t		src;
	nxs_string_t		dst;
	mode_t			mode;
} nxs_fw_ctl_u_projects_fs_objects_path_t;

struct nxs_fw_ctl_u_projects_fs_objects_s
{
	nxs_array_t		init_dirs;	/* type: nxs_string_t */
	nxs_array_t		makefiles;	/* type: nxs_fw_ctl_u_projects_fs_objects_path_t */
	nxs_array_t		genfiles;	/* type: nxs_fw_ctl_u_projects_fs_objects_path_t */
};

/* Module internal (static) functions prototypes */

// clang-format on

static void nxs_fw_ctl_u_projects_fs_objects_path_init(void *element);
static void nxs_fw_ctl_u_projects_fs_objects_path_free(void *element);

static nxs_cfg_json_state_t nxs_fw_ctl_u_projects_fs_objects_read_path(nxs_process_t *     proc,
                                                                       nxs_json_t *        json,
                                                                       nxs_cfg_json_par_t *cfg_json_par_el,
                                                                       nxs_array_t *       cfg_arr);
static nxs_cfg_json_state_t
        nxs_fw_ctl_u_projects_fs_objects_read_path_mode(nxs_process_t *proc, nxs_json_t *json, nxs_cfg_json_par_t *cfg_json_par_el);

// clang-format off

/* Module initializations */

static nxs_string_t _s_par_init_dirs		= nxs_string("init_dirs");
static nxs_string_t _s_par_makefiles		= nxs_string("makefiles");
static nxs_string_t _s_par_genfiles		= nxs_string("genfiles");
static nxs_string_t _s_par_src			= nxs_string("src");
static nxs_string_t _s_par_dst			= nxs_string("dst");
static nxs_string_t _s_par_mode			= nxs_string("mode");

/* Module global functions */

// clang-format on

nxs_fw_ctl_u_projects_fs_objects_t *nxs_fw_ctl_u_projects_fs_objects_init(void)
{
	nxs_fw_ctl_u_projects_fs_objects_t *u_ctx;

	u_ctx = (nxs_fw_ctl_u_projects_fs_objects_t *)nxs_malloc(NULL, sizeof(nxs_fw_ctl_u_projects_fs_objects_t));

	nxs_array_init_string(&u_ctx->init_dirs);

	nxs_array_init3(&u_ctx->makefiles,
	                nxs_fw_ctl_u_projects_fs_objects_path_t,
	                &nxs_fw_ctl_u_projects_fs_objects_path_init,
	                &nxs_fw_ctl_u_projects_fs_objects_path_free);
	nxs_array_init3(&u_ctx->genfiles,
	                nxs_fw_ctl_u_projects_fs_objects_path_t,
	                &nxs_fw_ctl_u_projects_fs_objects_path_init,
	                &nxs_fw_ctl_u_projects_fs_objects_path_free);

	return u_ctx;
}

nxs_fw_ctl_u_projects_fs_objects_t *nxs_fw_ctl_u_projects_fs_objects_free(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	nxs_array_free(&u_ctx->init_dirs);
	nxs_array_free(&u_ctx->makefiles);
	nxs_array_free(&u_ctx->genfiles);

	return (nxs_fw_ctl_u_projects_fs_objects_t *)nxs_free(u_ctx);
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_fs_objects_read(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx, nxs_string_t *fs_objects_path)
{
	nxs_fw_ctl_err_t rc;
	nxs_cfg_json_t   cfg_json;
	nxs_array_t      cfg_arr;

	if(u_ctx == NULL || fs_objects_path == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_cfg_json_conf_array_init(&cfg_arr);

	// clang-format off

	nxs_cfg_json_conf_array_add(&cfg_arr,	&_s_par_init_dirs,	&u_ctx->init_dirs,	NULL,	NULL,						NXS_CFG_JSON_TYPE_ARRAY_STRING,	0,	0,	NXS_YES,	NULL);
	nxs_cfg_json_conf_array_add(&cfg_arr,	&_s_par_makefiles,	&u_ctx->makefiles,	NULL,	&nxs_fw_ctl_u_projects_fs_objects_read_path,	NXS_CFG_JSON_TYPE_ARRAY_OBJECT,	0,	0,	NXS_YES,	NULL);
	nxs_cfg_json_conf_array_add(&cfg_arr,	&_s_par_genfiles,	&u_ctx->genfiles,	NULL,	&nxs_fw_ctl_u_projects_fs_objects_read_path,	NXS_CFG_JSON_TYPE_ARRAY_OBJECT,	0,	0,	NXS_YES,	NULL);

	// clang-format on

	nxs_cfg_json_init(&process, &cfg_json, fs_objects_path, NULL, NULL, &cfg_arr);

	if(nxs_cfg_json_read_file(&process, cfg_json) != NXS_CFG_JSON_CONF_OK) {

		nxs_log_write_error(
		        &process, "[%s]: can't read nxs-fw fs objects (fs objects file: %r)", nxs_proc_get_name(&process), fs_objects_path);

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_cfg_json_free(&cfg_json);

	nxs_cfg_json_conf_array_free(&cfg_arr);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_fs_objects_get_init_dir(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx, size_t i, nxs_string_t *init_dir)
{
	nxs_string_t *s;

	if(u_ctx == NULL || init_dir == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	if((s = nxs_array_get(&u_ctx->init_dirs, i)) == NULL) {

		return NXS_FW_CTL_E_EXIST;
	}

	nxs_string_clone(init_dir, s);

	return NXS_FW_CTL_E_OK;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_fs_objects_get_makefile(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx,
                                                               size_t                              i,
                                                               nxs_string_t *                      src,
                                                               nxs_string_t *                      dst,
                                                               mode_t *                            mode)
{
	nxs_fw_ctl_u_projects_fs_objects_path_t *el;

	if(u_ctx == NULL || src == NULL || dst == NULL || mode == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	if((el = nxs_array_get(&u_ctx->makefiles, i)) == NULL) {

		return NXS_FW_CTL_E_EXIST;
	}

	nxs_string_clone(src, &el->src);
	nxs_string_clone(dst, &el->dst);

	*mode = el->mode;

	return NXS_FW_CTL_E_OK;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_fs_objects_get_genfiles(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx,
                                                               size_t                              i,
                                                               nxs_string_t *                      src,
                                                               nxs_string_t *                      dst,
                                                               mode_t *                            mode)
{
	nxs_fw_ctl_u_projects_fs_objects_path_t *el;

	if(u_ctx == NULL || src == NULL || dst == NULL || mode == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	if((el = nxs_array_get(&u_ctx->genfiles, i)) == NULL) {

		return NXS_FW_CTL_E_EXIST;
	}

	nxs_string_clone(src, &el->src);
	nxs_string_clone(dst, &el->dst);

	*mode = el->mode;

	return NXS_FW_CTL_E_OK;
}

/* Module internal (static) functions */

static void nxs_fw_ctl_u_projects_fs_objects_path_init(void *element)
{
	nxs_fw_ctl_u_projects_fs_objects_path_t *el = element;

	nxs_string_init(&el->src);
	nxs_string_init(&el->dst);

	el->mode = 0;
}

static void nxs_fw_ctl_u_projects_fs_objects_path_free(void *element)
{
	nxs_fw_ctl_u_projects_fs_objects_path_t *el = element;

	nxs_string_free(&el->src);
	nxs_string_free(&el->dst);

	el->mode = 0;
}

static nxs_cfg_json_state_t nxs_fw_ctl_u_projects_fs_objects_read_path(nxs_process_t *     proc,
                                                                       nxs_json_t *        json,
                                                                       nxs_cfg_json_par_t *cfg_json_par_el,
                                                                       nxs_array_t *       cfg_arr)
{
	nxs_array_t *                            paths = nxs_cfg_json_get_val(cfg_json_par_el);
	nxs_fw_ctl_u_projects_fs_objects_path_t *el;

	el = nxs_array_add(paths);

	// clang-format off

	nxs_cfg_json_conf_array_add(cfg_arr,	&_s_par_src,		&el->src,	NULL,							NULL,	NXS_CFG_JSON_TYPE_STRING,	0,	0,	NXS_YES,	NULL);
	nxs_cfg_json_conf_array_add(cfg_arr,	&_s_par_dst,		&el->dst,	NULL,							NULL,	NXS_CFG_JSON_TYPE_STRING,	0,	0,	NXS_YES,	NULL);
	nxs_cfg_json_conf_array_add(cfg_arr,	&_s_par_mode,		&el->mode,	&nxs_fw_ctl_u_projects_fs_objects_read_path_mode,	NULL,	NXS_CFG_JSON_TYPE_VOID,		0,	0,	NXS_YES,	NULL);

	// clang-format on

	return NXS_CFG_JSON_CONF_OK;
}

static nxs_cfg_json_state_t
        nxs_fw_ctl_u_projects_fs_objects_read_path_mode(nxs_process_t *proc, nxs_json_t *json, nxs_cfg_json_par_t *cfg_json_par_el)
{
	nxs_string_t *m;
	size_t        i;
	mode_t *      mode = nxs_cfg_json_get_val(cfg_json_par_el);

	if(nxs_json_type_get(json) != NXS_JSON_TYPE_STRING) {

		nxs_log_write_error(&process,
		                    "[%s]: fs object read error: wrong type for key \"mode\", expected type string",
		                    nxs_proc_get_name(&process));

		return NXS_CFG_JSON_CONF_ERROR;
	}

	m = nxs_json_string_val(json);

	for(i = 0; i < nxs_string_len(m); i++) {

		if(nxs_string_get_char(m, i) < '0' || nxs_string_get_char(m, i) > '7') {

			nxs_log_write_error(&process,
			                    "[%s]: fs object read error: wrong value for key \"mode\" (value: \"%r\")",
			                    nxs_proc_get_name(&process),
			                    m);

			return NXS_CFG_JSON_CONF_ERROR;
		}
	}

	sscanf((char *)nxs_string_str(m), "%o", mode);

	return NXS_CFG_JSON_CONF_OK;
}
