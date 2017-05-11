#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_VERSION_H
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_VERSION_H

// clang-format off

/* Structs declarations */



/* Prototypes */

nxs_fw_ctl_err_t				nxs_fw_ctl_u_projects_version_available					(nxs_string_t *nxs_fw_conf_root, nxs_array_t *versions);
nxs_fw_ctl_err_t				nxs_fw_ctl_u_projects_version_select					(nxs_string_t *version, nxs_array_t *versions);

#endif /* _INCLUDE_NXS_FW_CTL_U_PROJECTS_VERSION_H */
