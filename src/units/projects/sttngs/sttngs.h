#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_STTNGS_H
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_STTNGS_H

// clang-format off

/* Structs declarations */



/* Prototypes */

nxs_bool_t						nxs_fw_ctl_u_projects_sttngs_check				(nxs_string_t *path);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_sttngs_read				(nxs_string_t *path, nxs_string_t *proj_name, nxs_string_t *nxs_fw_version, nxs_array_t *proj_selected_mods);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_sttngs_write				(nxs_string_t *path, nxs_string_t *proj_name, nxs_string_t *nxs_fw_version, nxs_array_t *proj_selected_mods);

#endif /* _INCLUDE_NXS_FW_CTL_U_PROJECTS_STTNGS_H */
