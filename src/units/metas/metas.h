#ifndef _INCLUDE_NXS_FW_CTL_U_METAS_H
#define _INCLUDE_NXS_FW_CTL_U_METAS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_metas_s;

/* Prototypes */

nxs_fw_ctl_u_metas_t				*nxs_fw_ctl_u_metas_init				(void);
nxs_fw_ctl_u_metas_t				*nxs_fw_ctl_u_metas_free				(nxs_fw_ctl_u_metas_t *u_ctx);

void						nxs_fw_ctl_u_metas_setup				(nxs_fw_ctl_u_metas_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *m_name);
nxs_fw_ctl_err_t				nxs_fw_ctl_u_metas_add					(nxs_fw_ctl_u_metas_t *u_ctx);
nxs_fw_ctl_err_t				nxs_fw_ctl_u_metas_del					(nxs_fw_ctl_u_metas_t *u_ctx);

#endif /* _INCLUDE_NXS_FW_CTL_U_METAS_H */
