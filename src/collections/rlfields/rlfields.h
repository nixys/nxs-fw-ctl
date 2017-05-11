#ifndef _INCLUDE_NXS_FW_CTL_C_RLFIELDS_H
#define _INCLUDE_NXS_FW_CTL_C_RLFIELDS_H

// clang-format off

/* Prototypes */

void						nxs_fw_ctl_c_rlfields_init				(nxs_array_t *rlfields);
void						nxs_fw_ctl_c_rlfields_free				(nxs_array_t *rlfields);
nxs_bool_t					nxs_fw_ctl_c_rlfields_yesno				(nxs_string_t *question);
void						nxs_fw_ctl_c_rlfields_add				(nxs_array_t *rlfields, nxs_string_t *prompt, nxs_string_t *defult_value, nxs_array_t *valid_set, nxs_string_t *empty_value, nxs_string_t *data, nxs_bool_t valid_spaces, nxs_bool_t trim_spaces, nxs_array_t *tvariants);
nxs_fw_ctl_err_t				nxs_fw_ctl_c_rlfields_exec				(nxs_array_t *rlfields);
void						nxs_fw_ctl_c_rlfields_arr_str_init			(nxs_array_t *arr_str);
void						nxs_fw_ctl_c_rlfields_arr_str_free			(nxs_array_t *arr_str);
void						nxs_fw_ctl_c_rlfields_arr_str_add			(nxs_array_t *arr_str, nxs_string_t *str_el);

#endif /* _INCLUDE_NXS_FW_CTL_C_RLFIELDS_H */
