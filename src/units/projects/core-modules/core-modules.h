#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_CORE_MODULES_H
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_CORE_MODULES_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_projects_core_modules_s;

/* Prototypes */

nxs_fw_ctl_u_projects_core_modules_t			*nxs_fw_ctl_u_projects_core_modules_init					(void);
nxs_fw_ctl_u_projects_core_modules_t			*nxs_fw_ctl_u_projects_core_modules_free					(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);

nxs_array_t						*nxs_fw_ctl_u_projects_core_modules_selected_get_mods				(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);

nxs_array_t						*nxs_fw_ctl_u_projects_core_modules_using_get_mods				(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);
nxs_string_t						*nxs_fw_ctl_u_projects_core_modules_using_get_use_flags				(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);
nxs_string_t						*nxs_fw_ctl_u_projects_core_modules_using_get_link_opts				(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);
nxs_string_t						*nxs_fw_ctl_u_projects_core_modules_using_get_includes				(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);
nxs_string_t						*nxs_fw_ctl_u_projects_core_modules_using_get_core_links			(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);

void							nxs_fw_ctl_u_projects_core_modules_select					(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_array_t *selected_mods);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_core_modules_selected_add					(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_array_t *selected);

nxs_fw_ctl_err_t					nxs_fw_ctl_u_projects_core_modules_read						(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_string_t *nxs_core_desc_path);

#endif /* _INCLUDE_NXS_FW_CTL_U_PROJECTS_CORE_MODULES_H */
