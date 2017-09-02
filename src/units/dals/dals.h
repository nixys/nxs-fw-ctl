#ifndef _INCLUDE_NXS_FW_CTL_U_DALS_H
#define _INCLUDE_NXS_FW_CTL_U_DALS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_dals_s;

/* Prototypes */

nxs_fw_ctl_u_dals_t					*nxs_fw_ctl_u_dals_init				(void);
nxs_fw_ctl_u_dals_t					*nxs_fw_ctl_u_dals_free				(nxs_fw_ctl_u_dals_t *u_ctx);

nxs_fw_ctl_err_t					nxs_fw_ctl_u_dals_setup				(nxs_fw_ctl_u_dals_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *d_name_str);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_dals_add				(nxs_fw_ctl_u_dals_t *u_ctx, nxs_string_t *fw_version);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_dals_del				(nxs_fw_ctl_u_dals_t *u_ctx);

#endif /* _INCLUDE_NXS_FW_CTL_U_DALS_H */
