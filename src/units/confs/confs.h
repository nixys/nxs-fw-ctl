#ifndef _INCLUDE_NXS_FW_CTL_U_CONFS_H
#define _INCLUDE_NXS_FW_CTL_U_CONFS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_confs_s;

/* Prototypes */

nxs_fw_ctl_u_confs_t				*nxs_fw_ctl_u_confs_init			(void);
nxs_fw_ctl_u_confs_t				*nxs_fw_ctl_u_confs_free			(nxs_fw_ctl_u_confs_t *u_ctx);

nxs_fw_ctl_err_t				nxs_fw_ctl_u_confs_add				(nxs_fw_ctl_u_confs_t *u_ctx, nxs_string_t *fw_version);
nxs_fw_ctl_err_t				nxs_fw_ctl_u_confs_del				(nxs_fw_ctl_u_confs_t *u_ctx);
void						nxs_fw_ctl_u_confs_setup			(nxs_fw_ctl_u_confs_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *conf_name);

#endif /* _INCLUDE_NXS_FW_CTL_U_CONFS_H */
