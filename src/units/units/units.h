#ifndef _INCLUDE_NXS_FW_CTL_U_UNITS_H
#define _INCLUDE_NXS_FW_CTL_U_UNITS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_units_s;

/* Prototypes */

nxs_fw_ctl_u_units_t				*nxs_fw_ctl_u_units_init				(void);
nxs_fw_ctl_u_units_t				*nxs_fw_ctl_u_units_free				(nxs_fw_ctl_u_units_t *u_ctx);

nxs_fw_ctl_err_t				nxs_fw_ctl_u_units_setup				(nxs_fw_ctl_u_units_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *u_name_str);
nxs_fw_ctl_err_t				nxs_fw_ctl_u_units_add					(nxs_fw_ctl_u_units_t *u_ctx);
nxs_fw_ctl_err_t				nxs_fw_ctl_u_units_del					(nxs_fw_ctl_u_units_t *u_ctx);

#endif /* _INCLUDE_NXS_FW_CTL_U_UNITS_H */
