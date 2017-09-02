#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_VERSION_C
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_VERSION_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/projects/version/version-typedefs.h>
#include <units/projects/version/version.h>
#include <units/projects/version/version-subunits.h>

/* Definitions */



/* Project globals */
extern		nxs_process_t			process;
extern		nxs_fw_ctl_cfg_t		nxs_fw_ctl_cfg;

/* Module typedefs */



/* Module declarations */



/* Module internal (static) functions prototypes */

// clang-format on

// clang-format off

/* Module initializations */

static nxs_string_t _s_prompt_version		= nxs_string("\tversion: ");

/* Module global functions */

// clang-format on

/*
 * Подготовка списка доступных версий nxs-fw
 *
 * Возвращаемый результат - это список директорий в /etc/nxs-fw/ (&nxs_fw_ctl_cfg.nxs_fw_conf_root)
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_version_available(nxs_string_t *nxs_fw_conf_root, nxs_array_t *versions)
{
	nxs_fs_dir_t *   dir;
	nxs_fs_dirent_t  dir_entry;
	nxs_fs_stat_t    file_stat;
	nxs_string_t     file, *s;
	nxs_fw_ctl_err_t rc;
	size_t           root_len;
	int              rd;

	rc = NXS_FW_CTL_E_OK;

	nxs_fs_dirent_init(&dir_entry);

	root_len = nxs_string_len(nxs_fw_conf_root);

	nxs_string_init3(&file, nxs_fw_conf_root);

	if((dir = nxs_fs_opendir(nxs_fw_conf_root)) != NULL) {

		while((rd = nxs_fs_readdir(dir, &dir_entry, NXS_FS_MODE_SET_SKEEP_DOT)) == 0) {

			nxs_string_cpy(&file, root_len, dir_entry.d_name, 0);

			if(nxs_fs_lstat(&file, &file_stat) < 0) {

				nxs_log_write_error(
				        &process, "can't stat for object: %s (object: \"%s\")", strerror(errno), nxs_string_str(&file));

				nxs_error(rc, NXS_FW_CTL_E_ERR, error);
			}
			else {

				if(S_ISDIR(file_stat.st_mode)) {

					s = nxs_array_add(versions);

					nxs_string_init3(s, dir_entry.d_name);
				}
			}
		}

		if(rd > 0) {

			nxs_log_write_error(
			        &process, "can't read dir: %s (dir path: \"%s\")", strerror(rd), nxs_string_str(nxs_fw_conf_root));

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}
	else {

		nxs_log_write_error(&process, "can't open dir: %s, (dir path: \"%s\")", strerror(errno), nxs_string_str(nxs_fw_conf_root));
	}

error:

	nxs_fs_dirent_free(&dir_entry);
	nxs_string_free(&file);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_version_select(nxs_string_t *version, nxs_array_t *versions)
{
	nxs_array_t      rlfields;
	nxs_fw_ctl_err_t rc;

	nxs_fw_ctl_c_rlfields_init(&rlfields);

	nxs_fw_ctl_c_rlfields_add(&rlfields, &_s_prompt_version, NULL, versions, NULL, version, NXS_NO, NXS_YES, versions);

	rc = nxs_fw_ctl_c_rlfields_exec(&rlfields);

	nxs_fw_ctl_c_rlfields_free(&rlfields);

	return rc;
}

/* Module internal (static) functions */
