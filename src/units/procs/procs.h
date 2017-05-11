#ifndef _INCLUDE_NXS_FW_CTL_U_PROCS_H
#define _INCLUDE_NXS_FW_CTL_U_PROCS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_procs_s;

/* Prototypes */

nxs_fw_ctl_u_procs_t					*nxs_fw_ctl_u_procs_init			(void);
nxs_fw_ctl_u_procs_t					*nxs_fw_ctl_u_procs_free			(nxs_fw_ctl_u_procs_t *u_ctx);

nxs_fw_ctl_err_t					nxs_fw_ctl_u_procs_setup			(nxs_fw_ctl_u_procs_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *p_name_str);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_procs_add				(nxs_fw_ctl_u_procs_t *u_ctx);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_procs_del				(nxs_fw_ctl_u_procs_t *u_ctx);

#endif /* _INCLUDE_NXS_FW_CTL_U_PROCS_H */
