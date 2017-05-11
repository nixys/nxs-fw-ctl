#ifndef _INCLUDE_NXS_FW_CTL_C_SUBS_INCLUDES_C
#define _INCLUDE_NXS_FW_CTL_C_SUBS_INCLUDES_C
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



/* Module declarations */



/* Module internal (static) functions prototypes */

// clang-format on

// clang-format off

/* Module initializations */



/* Module global functions */

// clang-format on

/*
 * Замещение заданного блока 'includes' строкой 'new_headers' в указанном файле 'header_path'.
 * Блок 'includes' начинается со строки 'tpl_headers_b' и заканчивается 'tpl_headers_e'
 */
nxs_fw_ctl_err_t nxs_fw_ctl_c_subs_includes(nxs_string_t *substitution_file,
                                            nxs_bool_t    only_start_line,
                                            nxs_string_t *new_headers,
                                            nxs_string_t *tpl_headers_b,
                                            nxs_string_t *tpl_headers_e)
{
	nxs_string_t     hfile, tpl_str;
	nxs_fw_ctl_err_t rc;
	u_char *         c, *t1, *t2;
	size_t           o, c1, c2;
	nxs_bool_t       cont;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&hfile);
	nxs_string_init(&tpl_str);

	if(nxs_fs_read_file_to_str(substitution_file, &hfile) < 0) {

		nxs_log_write_error(
		        &process, "can't read substitution file: %s (file path: %s)", strerror(errno), nxs_string_str(substitution_file));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	o = 0;

	do {

		c = nxs_string_str(&hfile);

		t1 = nxs_string_find_substr_first(&hfile, o, nxs_string_str(tpl_headers_b), nxs_string_len(tpl_headers_b));
		t2 = NULL;

		if(t1 != NULL) {

			t2 = nxs_string_find_substr_first(&hfile,
			                                  o + (t1 - c) + nxs_string_len(tpl_headers_b),
			                                  nxs_string_str(tpl_headers_e),
			                                  nxs_string_len(tpl_headers_e));
		}

		if(t1 != NULL && t2 != NULL) {

			c1 = t1 - c;
			c2 = t2 - c;

			/* Если задан поиск только сначала строки и при этом найденный сегмент находится не в начале строки - продолжаем
			 * поиск без замены */
			if(only_start_line == NXS_YES && c1 > 0 && *(t1 - 1) != (u_char)'\n') {

				o = c1 + nxs_string_len(tpl_headers_b);

				cont = NXS_YES;

				continue;
			}

			nxs_string_ncpy_dyn(&tpl_str, 0, &hfile, c1, c2 - c1 + nxs_string_len(tpl_headers_e));

			if(nxs_string_subs(&hfile, NULL, &tpl_str, new_headers, 1) > 0) {

				cont = NXS_YES;
			}

			o = c2 + nxs_string_len(tpl_headers_e);
		}
		else {

			cont = NXS_NO;
		}
	} while(cont == NXS_YES);

	if(nxs_fs_write_file(substitution_file, (nxs_buf_t *)&hfile, NXS_FW_CTL_FILE_MODE_DEF) < 0) {

		nxs_log_write_error(
		        &process, "can't write substitution file: %s (file path: %s)", strerror(errno), nxs_string_str(substitution_file));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&hfile);
	nxs_string_free(&tpl_str);

	return rc;
}

/* Module internal (static) functions */
