#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_H
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_projects_s;

/* Prototypes */

nxs_fw_ctl_u_projects_t					*nxs_fw_ctl_u_projects_init					(void);
nxs_fw_ctl_u_projects_t					*nxs_fw_ctl_u_projects_free					(nxs_fw_ctl_u_projects_t *u_ctx);

nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_setup					(nxs_fw_ctl_u_projects_t *u_ctx, nxs_string_t *proj_root, nxs_string_t *proj_name);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_add					(nxs_fw_ctl_u_projects_t *u_ctx);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_del					(nxs_fw_ctl_u_projects_t *u_ctx);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_change_core_mods				(nxs_fw_ctl_u_projects_t *u_ctx);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_change_core_version			(nxs_fw_ctl_u_projects_t *u_ctx);

nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_settingsfile_get_name			(nxs_string_t *proj_path, nxs_string_t *name);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_settingsfile_get_version			(nxs_string_t *proj_path, nxs_string_t *name, nxs_string_t *version);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_settingsfile_get_mods			(nxs_string_t *proj_path, nxs_string_t *name, nxs_array_t *proj_selected_mods);
#endif /* _INCLUDE_NXS_FW_CTL_U_PROJECTS_H */
