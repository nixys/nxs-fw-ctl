#ifndef _INCLUDE_NXS_FW_CTL_C_COPY_TPL_H
#define _INCLUDE_NXS_FW_CTL_C_COPY_TPL_H

// clang-format off

/* Prototypes */

void						nxs_fw_ctl_c_copy_tpl_init				(nxs_array_t *subs);
void						nxs_fw_ctl_c_copy_tpl_free				(nxs_array_t *subs);
void						nxs_fw_ctl_c_copy_tpl_add				(nxs_array_t *subs, nxs_string_t *tpl, nxs_string_t *dst);
nxs_fw_ctl_err_t				nxs_fw_ctl_c_copy_tpl					(nxs_array_t *subs, nxs_string_t *src, nxs_string_t *dst, mode_t mode);
void						nxs_fw_ctl_c_copy_tpl_path				(nxs_array_t *subs, nxs_string_t *path);

#endif /* _INCLUDE_NXS_FW_CTL_C_COPY_TPL_H */
