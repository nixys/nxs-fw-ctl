#ifndef _INCLUDE_NXS_FW_CTL_C_DIR_C
#define _INCLUDE_NXS_FW_CTL_C_DIR_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

/* Definitions */



/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */

typedef enum						nxs_fw_ctl_c_dir_get_name_type_e				nxs_fw_ctl_c_dir_get_name_type_t;

/* Module declarations */

enum nxs_fw_ctl_c_dir_get_name_type_e
{
	NXS_FW_CTL_C_DIR_GET_NAME_TYPE_DIRS,
	NXS_FW_CTL_C_DIR_GET_NAME_TYPE_FILES
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_c_dir_get_name_objects(nxs_string_t *                   dir_path,
                                                          nxs_array_t *                    obj_names,
                                                          nxs_fw_ctl_c_dir_get_name_type_t type,
                                                          nxs_bool_t                       recursive,
                                                          nxs_string_t *                   parent_name,
                                                          nxs_array_t *                    skip_names);

// clang-format off

/* Module initializations */



/* Module global functions */

// clang-format on

nxs_fw_ctl_err_t nxs_fw_ctl_c_dir_clean(nxs_string_t *dir_path)
{
	int              rd, dir_path_len;
	nxs_fs_dir_t *   dir;
	nxs_fs_dirent_t  dir_entry;
	nxs_fs_stat_t    file_stat;
	nxs_string_t     dirent;
	nxs_fw_ctl_err_t rc;

	if(dir_path == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_fs_dirent_init(&dir_entry);

	dir_path_len = nxs_string_len(dir_path);

	nxs_string_init3(&dirent, dir_path);

	if((dir = nxs_fs_opendir(dir_path)) != NULL) {

		while((rd = nxs_fs_readdir(dir, &dir_entry, NXS_FS_MODE_SET_SKEEP_DOT)) == 0) {

			nxs_string_cpy(&dirent, dir_path_len, dir_entry.d_name, 0);

			if(nxs_fs_lstat(&dirent, &file_stat) < 0) {

				nxs_log_write_error(
				        &process, "can't execute stat for object \"%s\": %s", nxs_string_str(&dirent), strerror(errno));

				nxs_error(rc, NXS_FW_CTL_E_ERR, error);
			}
			else {

				if(S_ISREG(file_stat.st_mode)) {

					/* skip files begining with '.' */
					if(nxs_string_get_char(dir_entry.d_name, 0) != (u_char)'.') {

						if(nxs_fs_unlink(&dirent) < 0) {

							nxs_log_write_error(&process,
							                    "can't remove core-object file: %s (path: %s)",
							                    strerror(errno),
							                    nxs_string_str(&dirent));

							nxs_error(rc, NXS_FW_CTL_E_ERR, error);
						}
					}
				}
			}
		}

		if(rd > 0) {

			nxs_log_write_error(&process, "can't read dir \"%s\": %s", nxs_string_str(dir_path), strerror(rd));

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}
	else {

		nxs_log_write_error(&process, "can't open dir \"%s\": %s", nxs_string_str(dir_path), strerror(errno));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_fs_dirent_free(&dir_entry);
	nxs_string_free(&dirent);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_c_dir_del(nxs_string_t *dir_path)
{
	int              rd, dir_path_len;
	nxs_fs_dir_t *   dir;
	nxs_fs_dirent_t  dir_entry;
	nxs_fs_stat_t    file_stat;
	nxs_string_t     dirent;
	nxs_fw_ctl_err_t rc;

	if(dir_path == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_fs_dirent_init(&dir_entry);

	dir_path_len = nxs_string_len(dir_path);

	nxs_string_init3(&dirent, dir_path);

	if((dir = nxs_fs_opendir(dir_path)) != NULL) {

		while((rd = nxs_fs_readdir(dir, &dir_entry, NXS_FS_MODE_SET_SKEEP_DOT)) == 0) {

			nxs_string_cpy(&dirent, dir_path_len, dir_entry.d_name, 0);

			if(nxs_fs_lstat(&dirent, &file_stat) < 0) {

				nxs_log_write_error(
				        &process, "can't execute stat for object \"%s\": %s", nxs_string_str(&dirent), strerror(errno));

				nxs_error(rc, NXS_FW_CTL_E_ERR, error);
			}
			else {

				if(S_ISREG(file_stat.st_mode)) {

					if(nxs_fs_unlink(&dirent) < 0) {

						nxs_log_write_error(&process,
						                    "can't remove core-object file: %s (path: %s)",
						                    strerror(errno),
						                    nxs_string_str(&dirent));

						nxs_error(rc, NXS_FW_CTL_E_ERR, error);
					}
				}
				else {

					if(S_ISDIR(file_stat.st_mode)) {

						nxs_string_char_add_char(&dirent, (u_char)'/');

						if(nxs_fw_ctl_c_dir_del(&dirent) != NXS_FW_CTL_E_OK) {

							nxs_error(rc, NXS_FW_CTL_E_ERR, error);
						}
					}
				}
			}
		}

		if(rd > 0) {

			nxs_log_write_error(&process, "can't read dir \"%s\": %s", nxs_string_str(dir_path), strerror(rd));

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}
	else {

		nxs_log_write_error(&process, "can't open dir \"%s\": %s", nxs_string_str(dir_path), strerror(errno));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fs_rmdir(dir_path) < 0) {

		nxs_log_write_error(&process, "can't delete dir \"%s\": %s", nxs_string_str(dir_path), strerror(errno));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_fs_dirent_free(&dir_entry);

	nxs_string_free(&dirent);

	return rc;
}

/*
 * Получение массива имён поддиректорий в директории 'dir_path'.
 * Результат будет записан в массив 'dir_names'
 */
nxs_fw_ctl_err_t nxs_fw_ctl_c_dir_get_name_dirs(nxs_string_t *dir_path, nxs_array_t *dir_names, nxs_array_t *skip_names)
{

	return nxs_fw_ctl_c_dir_get_name_objects(dir_path, dir_names, NXS_FW_CTL_C_DIR_GET_NAME_TYPE_DIRS, NXS_NO, NULL, skip_names);
}

/*
 * Получение массива имён файлов в директории 'dir_path'.
 * Результат будет записан в массив 'file_names'
 */
nxs_fw_ctl_err_t nxs_fw_ctl_c_dir_get_name_files(nxs_string_t *dir_path, nxs_array_t *file_names, nxs_array_t *skip_names)
{

	return nxs_fw_ctl_c_dir_get_name_objects(dir_path, file_names, NXS_FW_CTL_C_DIR_GET_NAME_TYPE_FILES, NXS_NO, NULL, skip_names);
}

/*
 * Рекурсивное получение массива имён поддиректорий в директории 'dir_path'.
 * Результат будет записан в массив 'dir_names'.
 * Имя родительской директории в массив 'dir_names' не попадёт.
 */
nxs_fw_ctl_err_t nxs_fw_ctl_c_dir_get_name_subdirs(nxs_string_t *dir_path, nxs_array_t *dir_names, nxs_array_t *skip_names)
{

	return nxs_fw_ctl_c_dir_get_name_objects(dir_path, dir_names, NXS_FW_CTL_C_DIR_GET_NAME_TYPE_DIRS, NXS_YES, NULL, skip_names);
}

/* Module internal (static) functions */

/*
 * Получение массива объектов заданного типа (если задано - рекурсивно).
 *
 * При рекурсивном обходе объекты разного уровня отделены между собой симолом '.'
 * Если указатель на массив 'skip_names' не нулевой - все объекты ФС с совпадающими именами из данного массива будут пропускаться.
 */
static nxs_fw_ctl_err_t nxs_fw_ctl_c_dir_get_name_objects(nxs_string_t *                   dir_path,
                                                          nxs_array_t *                    obj_names,
                                                          nxs_fw_ctl_c_dir_get_name_type_t type,
                                                          nxs_bool_t                       recursive,
                                                          nxs_string_t *                   parent_name,
                                                          nxs_array_t *                    skip_names)
{
	int              rd, dir_path_len;
	nxs_fs_dir_t *   dir;
	nxs_fs_dirent_t  dir_entry;
	nxs_fs_stat_t    file_stat;
	nxs_string_t     dirent, *s, new_parent_name;
	nxs_fw_ctl_err_t rc;
	nxs_bool_t       skip;
	size_t           i;

	if(dir_path == NULL || obj_names == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&new_parent_name);

	nxs_fs_dirent_init(&dir_entry);

	dir_path_len = nxs_string_len(dir_path);

	nxs_string_init3(&dirent, dir_path);

	if((dir = nxs_fs_opendir(dir_path)) != NULL) {

		while((rd = nxs_fs_readdir(dir, &dir_entry, NXS_FS_MODE_SET_SKEEP_DOT)) == 0) {

			nxs_string_cpy(&dirent, dir_path_len, dir_entry.d_name, 0);

			if(nxs_fs_lstat(&dirent, &file_stat) < 0) {

				nxs_log_write_error(
				        &process, "can't execute stat for object \"%s\": %s", nxs_string_str(&dirent), strerror(errno));

				nxs_error(rc, NXS_FW_CTL_E_ERR, error);
			}
			else {

				if(skip_names != NULL) {

					for(skip = NXS_NO, i = 0; i < nxs_array_count(skip_names); i++) {

						s = nxs_array_get(skip_names, i);

						if(nxs_string_cmp(s, 0, dir_entry.d_name, 0) == NXS_YES) {

							skip = NXS_YES;
							break;
						}
					}

					if(skip == NXS_YES) {

						continue;
					}
				}

				switch(type) {

					case NXS_FW_CTL_C_DIR_GET_NAME_TYPE_DIRS:

						if(S_ISDIR(file_stat.st_mode)) {

							s = nxs_array_add(obj_names);

							if(parent_name == NULL) {

								nxs_string_init3(s, dir_entry.d_name);
							}
							else {

								nxs_string_init(s);

								nxs_string_printf(s, "%r.%r", parent_name, dir_entry.d_name);
							}
						}

						break;

					case NXS_FW_CTL_C_DIR_GET_NAME_TYPE_FILES:

						if(S_ISREG(file_stat.st_mode)) {

							s = nxs_array_add(obj_names);

							if(parent_name == NULL) {

								nxs_string_init3(s, dir_entry.d_name);
							}
							else {

								nxs_string_init(s);

								nxs_string_printf(s, "%r.%r", parent_name, dir_entry.d_name);
							}
						}

						break;

					default:

						break;
				}

				if(recursive == NXS_YES && S_ISDIR(file_stat.st_mode)) {

					if(parent_name == NULL) {

						nxs_string_cpy(&new_parent_name, 0, dir_entry.d_name, 0);
					}
					else {

						nxs_string_printf(&new_parent_name, "%r.%r", parent_name, dir_entry.d_name);
					}

					nxs_string_char_add_char(&dirent, (u_char)'/');

					if(nxs_fw_ctl_c_dir_get_name_objects(
					           &dirent, obj_names, type, recursive, &new_parent_name, skip_names) != NXS_FW_CTL_E_OK) {

						nxs_error(rc, NXS_FW_CTL_E_ERR, error);
					}
				}
			}
		}

		if(rd > 0) {

			nxs_log_write_error(&process, "can't read dir \"%s\": %s", nxs_string_str(dir_path), strerror(rd));

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}
	else {

		nxs_log_write_error(&process, "can't open dir \"%s\": %s", nxs_string_str(dir_path), strerror(errno));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_fs_dirent_free(&dir_entry);

	nxs_string_free(&dirent);
	nxs_string_free(&new_parent_name);

	return rc;
}
