#ifndef _INCLUDE_NXS_FW_CTL_C_COPY_TPL_C
#define _INCLUDE_NXS_FW_CTL_C_COPY_TPL_C
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

void nxs_fw_ctl_c_copy_tpl_init(nxs_array_t *subs)
{

	nxs_array_init(subs, 0, sizeof(nxs_fw_ctl_m_subs_t), 1);
}

void nxs_fw_ctl_c_copy_tpl_free(nxs_array_t *subs)
{

	nxs_array_free(subs);
}

void nxs_fw_ctl_c_copy_tpl_add(nxs_array_t *subs, nxs_string_t *tpl, nxs_string_t *dst)
{
	nxs_fw_ctl_m_subs_t *s_sub;

	s_sub = nxs_array_add(subs);

	s_sub->tpl = tpl;
	s_sub->dst = dst;
}

nxs_fw_ctl_err_t nxs_fw_ctl_c_copy_tpl(nxs_array_t *subs, nxs_string_t *src, nxs_string_t *dst, mode_t mode)
{
	nxs_string_t         file;
	nxs_fw_ctl_m_subs_t *s_sub;
	size_t               i;

	nxs_string_init(&file);

	if(nxs_fs_read_file_to_str(src, &file) < 0) {

		nxs_log_write_error(&process, "can't read template file: %s (path: %s)", strerror(errno), nxs_string_str(src));

		return NXS_FW_CTL_E_ERR;
	}

	for(i = 0; i < nxs_array_count(subs); i++) {

		s_sub = nxs_array_get(subs, i);

		nxs_string_subs(&file, NULL, s_sub->tpl, s_sub->dst, 0);
	}

	if(nxs_fs_write_file(dst, (nxs_buf_t *)&file, mode) < 0) {

		nxs_log_write_error(&process, "can't write template file: %s (src: %s, dst: %s)", strerror(errno), nxs_string_str(dst));

		return NXS_FW_CTL_E_ERR;
	}

	return NXS_FW_CTL_E_OK;
}

void nxs_fw_ctl_c_copy_tpl_path(nxs_array_t *subs, nxs_string_t *path)
{
	nxs_fw_ctl_m_subs_t *s_sub;
	size_t               i;

	for(i = 0; i < nxs_array_count(subs); i++) {

		s_sub = nxs_array_get(subs, i);

		nxs_string_subs(path, NULL, s_sub->tpl, s_sub->dst, 0);
	}
}

/* Module internal (static) functions */
