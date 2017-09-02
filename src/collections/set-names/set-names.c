#ifndef _INCLUDE_NXS_FW_CTL_C_SET_NAMES_C
#define _INCLUDE_NXS_FW_CTL_C_SET_NAMES_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

#include <ctype.h>

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

nxs_fw_ctl_err_t nxs_fw_ctl_c_set_names(nxs_string_t *src_name, nxs_string_t *name, nxs_string_t *upcase_name, nxs_string_t *inline_name)
{
	size_t           i;
	u_char           c, c_upcase, c_inline;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	for(i = 0; i < nxs_string_len(src_name); i++) {

		c = nxs_string_get_char(src_name, i);

		if(((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_') == 0) {

			nxs_log_write_error(&process, "name does not valid (char pos: %zu)\n", i);

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}

		if(c == '-') {

			c_upcase = '_';
			c_inline = '_';
		}
		else {

			c_upcase = toupper(c);
			c_inline = c;
		}

		if(name != NULL) {

			nxs_string_char_add_char(name, c);
		}

		if(upcase_name != NULL) {

			nxs_string_char_add_char(upcase_name, c_upcase);
		}

		if(inline_name != NULL) {

			nxs_string_char_add_char(inline_name, c_inline);
		}
	}

error:

	return rc;
}

/* Module internal (static) functions */
