#ifndef _INCLUDE_NXS_FW_CTL_U_COLLECTIONS_H
#define _INCLUDE_NXS_FW_CTL_U_COLLECTIONS_H

// clang-format off

/* Structs declarations */

struct nxs_fw_ctl_u_collections_s;

/* Prototypes */

nxs_fw_ctl_u_collections_t				*nxs_fw_ctl_u_collections_init				(void);
nxs_fw_ctl_u_collections_t				*nxs_fw_ctl_u_collections_free				(nxs_fw_ctl_u_collections_t *u_ctx);

void							nxs_fw_ctl_u_collections_setup				(nxs_fw_ctl_u_collections_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *c_name);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_collections_add				(nxs_fw_ctl_u_collections_t *u_ctx, nxs_string_t *fw_version);
nxs_fw_ctl_err_t					nxs_fw_ctl_u_collections_del				(nxs_fw_ctl_u_collections_t *u_ctx);

#endif /* _INCLUDE_NXS_FW_CTL_U_COLLECTIONS_H */
