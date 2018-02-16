#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_FS_OBJECTS_H
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_FS_OBJECTS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_projects_fs_objects_s;

/* Prototypes */

nxs_fw_ctl_u_projects_fs_objects_t	*nxs_fw_ctl_u_projects_fs_objects_init		(void);
nxs_fw_ctl_u_projects_fs_objects_t	*nxs_fw_ctl_u_projects_fs_objects_free		(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx);

nxs_fw_ctl_err_t			nxs_fw_ctl_u_projects_fs_objects_read		(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx, nxs_string_t *fs_objects_path);

nxs_fw_ctl_err_t			nxs_fw_ctl_u_projects_fs_objects_get_init_dir	(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx, size_t i, nxs_string_t *init_dir);
nxs_fw_ctl_err_t			nxs_fw_ctl_u_projects_fs_objects_get_makefile	(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx, size_t i, nxs_string_t *src, nxs_string_t *dst, mode_t *mode);
nxs_fw_ctl_err_t			nxs_fw_ctl_u_projects_fs_objects_get_genfiles	(nxs_fw_ctl_u_projects_fs_objects_t *u_ctx, size_t i, nxs_string_t *src, nxs_string_t *dst, mode_t *mode);

#endif /* _INCLUDE_NXS_FW_CTL_U_PROJECTS_FS_OBJECTS_H */
