#ifndef _INCLUDE_NXS_FW_CTL_C_DIR_H
#define _INCLUDE_NXS_FW_CTL_C_DIR_H

// clang-format off

/* Prototypes */

nxs_fw_ctl_err_t			nxs_fw_ctl_c_dir_clean							(nxs_string_t *dir_path);
nxs_fw_ctl_err_t			nxs_fw_ctl_c_dir_del							(nxs_string_t *dir_path);

nxs_fw_ctl_err_t			nxs_fw_ctl_c_dir_get_name_dirs						(nxs_string_t *dir_path, nxs_array_t *dir_names, nxs_array_t *skip_names);
nxs_fw_ctl_err_t			nxs_fw_ctl_c_dir_get_name_files						(nxs_string_t *dir_path, nxs_array_t *file_names, nxs_array_t *skip_names);
nxs_fw_ctl_err_t			nxs_fw_ctl_c_dir_get_name_subdirs					(nxs_string_t *dir_path, nxs_array_t *dir_names, nxs_array_t *skip_names);

#endif /* _INCLUDE_NXS_FW_CTL_C_DIR_H */
