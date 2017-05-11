#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_STTNGS_C
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_STTNGS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/projects/sttngs/sttngs-typedefs.h>
#include <units/projects/sttngs/sttngs.h>
#include <units/projects/sttngs/sttngs-subunits.h>

/* Definitions */



/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */



/* Module declarations */



/* Module internal (static) functions prototypes */

// clang-format on

// clang-format off

/* Module initializations */

static nxs_string_t _s_par_project_name			= nxs_string("project_name");
static nxs_string_t _s_par_project_modules		= nxs_string("project_modules");
static nxs_string_t _s_par_nxs_fw_version		= nxs_string("nxs_fw_version");

/* Module global functions */

// clang-format on

nxs_bool_t nxs_fw_ctl_u_projects_sttngs_check(nxs_string_t *path)
{
	nxs_fs_stat_t file_stat;

	if(nxs_fs_lstat(path, &file_stat) < 0) {

		return NXS_NO;
	}

	return NXS_YES;
}

/*
 * Считывание параметров проекта из конфигурационного файла по пути "path".
 * По итогам в строку "proj_name" будет сохранено имя проекта, в массиве "proj_selected_mods" (nxs_string_t) - имена выбранных модулей
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_sttngs_read(nxs_string_t *path,
                                                   nxs_string_t *proj_name,
                                                   nxs_string_t *nxs_fw_version,
                                                   nxs_array_t * proj_selected_mods)
{

	nxs_fw_ctl_err_t rc;
	nxs_cfg_json_t   cfg_json;
	nxs_array_t      cfg_arr;

	if(proj_name == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_cfg_json_conf_array_init(&cfg_arr);

	nxs_cfg_json_conf_array_skip_undef(&cfg_arr);

	nxs_cfg_json_conf_array_add(&cfg_arr, &_s_par_project_name, proj_name, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_YES, NULL);

	if(nxs_fw_version != NULL) {

		nxs_cfg_json_conf_array_add(
		        &cfg_arr, &_s_par_nxs_fw_version, nxs_fw_version, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_YES, NULL);
	}

	if(proj_selected_mods != NULL) {

		/* Если требуется считать выбранные для проекта модули */

		nxs_cfg_json_conf_array_add(&cfg_arr,
		                            &_s_par_project_modules,
		                            proj_selected_mods,
		                            NULL,
		                            NULL,
		                            NXS_CFG_JSON_TYPE_ARRAY_STRING,
		                            0,
		                            0,
		                            NXS_YES,
		                            NULL);
	}

	nxs_cfg_json_init(&process, &cfg_json, path, NULL, NULL, &cfg_arr);

	if(nxs_cfg_json_read_file(&process, cfg_json) != NXS_CFG_JSON_CONF_OK) {

		nxs_log_write_error(&process, "can't read or parse project settings file (settings file: %s)", nxs_string_str(path));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_cfg_json_free(&cfg_json);

	nxs_cfg_json_conf_array_free(&cfg_arr);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_sttngs_write(nxs_string_t *path,
                                                    nxs_string_t *proj_name,
                                                    nxs_string_t *nxs_fw_version,
                                                    nxs_array_t * proj_selected_mods)
{
	nxs_string_t     settings, mods, *s;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	if(path == NULL || proj_name == NULL || proj_selected_mods == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&settings);
	nxs_string_init(&mods);

	for(i = 0; i < nxs_array_count(proj_selected_mods); i++) {

		s = nxs_array_get(proj_selected_mods, i);

		if(i > 0) {

			nxs_string_char_add_char_dyn(&mods, (u_char)',');
		}

		nxs_string_printf2_cat_dyn(&mods, "\n\t\t\"%r\"", s);
	}

	nxs_string_printf_dyn(&settings,
	                      "{\n"
	                      "\t\"project_name\": \"%r\",\n"
	                      "\t\"nxs_fw_version\": \"%r\",\n"
	                      "\t\"project_modules\": [%r\n"
	                      "\t]\n"
	                      "}\n",
	                      proj_name,
	                      nxs_fw_version,
	                      &mods);

	if(nxs_fs_write_file(path, (nxs_buf_t *)&settings, NXS_FW_CTL_FILE_MODE_DEF) < 0) {

		nxs_log_write_error(
		        &process, "can't write project setting file: %s (settings file: %s)", strerror(errno), nxs_string_str(path));

		rc = NXS_FW_CTL_E_ERR;
	}

	nxs_string_free(&settings);
	nxs_string_free(&mods);

	return rc;
}

/* Module internal (static) functions */
