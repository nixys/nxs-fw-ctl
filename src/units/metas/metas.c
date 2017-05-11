#ifndef _INCLUDE_NXS_FW_CTL_U_METAS_C
#define _INCLUDE_NXS_FW_CTL_U_METAS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/metas/metas-typedefs.h>
#include <units/metas/metas.h>
#include <units/metas/metas-subunits.h>

/* Definitions */

#define NXS_FW_CTL_U_METAS_TPL_M_FILENAME			"%%NXS_TPL_M_FILENAME%%"
#define NXS_FW_CTL_U_METAS_TPL_M_UPCASE_NAME			"%%NXS_TPL_M_UPCASE_NAME%%"
#define NXS_FW_CTL_U_METAS_TPL_M_INLINE_NAME			"%%NXS_TPL_M_INLINE_NAME%%"

#define NXS_FW_CTL_U_METAS_TPL_M_HEADERS_B			"/*%%_NXS_TPL_HEADERS_BEGIN_%%*/"
#define NXS_FW_CTL_U_METAS_TPL_M_HEADERS_E			"/*%%_NXS_TPL_HEADERS_END_%%*/"

/* Project globals */
extern		nxs_process_t					process;
extern		nxs_fw_ctl_cfg_t				nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct							nxs_fw_ctl_u_metas_path_s				nxs_fw_ctl_u_metas_path_t;

/* Module declarations */

struct nxs_fw_ctl_u_metas_path_s
{
	nxs_string_t						src;
	nxs_string_t						dst;
};

struct nxs_fw_ctl_u_metas_s
{
	nxs_string_t						name;
	nxs_string_t						inline_name;
	nxs_string_t						upcase_name;

	nxs_string_t						path;

	nxs_string_t						proj_name;

	nxs_string_t						proj_root;
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_u_metas_make_genfiles(nxs_fw_ctl_u_metas_t *u_ctx);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_metas_make_sub_headers(nxs_fw_ctl_u_metas_t *u_ctx);

// clang-format off

/* Module initializations */

static nxs_string_t _s_tpl_m_filename				= nxs_string(NXS_FW_CTL_U_METAS_TPL_M_FILENAME);
static nxs_string_t _s_tpl_m_upcase_name			= nxs_string(NXS_FW_CTL_U_METAS_TPL_M_UPCASE_NAME);
static nxs_string_t _s_tpl_m_inline_name			= nxs_string(NXS_FW_CTL_U_METAS_TPL_M_INLINE_NAME);

static nxs_string_t _s_tpl_m_headers_b				= nxs_string(NXS_FW_CTL_U_METAS_TPL_M_HEADERS_B);
static nxs_string_t _s_tpl_m_headers_e				= nxs_string(NXS_FW_CTL_U_METAS_TPL_M_HEADERS_E);

static nxs_string_t _s_question_meta_del			= nxs_string("\tyou realy want to delete this meta? [y/n]: ");

static nxs_fw_ctl_u_metas_path_t genfiles[] =
{
	{nxs_string("meta.h_tpl"),		nxs_string("" NXS_FW_CTL_U_METAS_TPL_M_FILENAME ".h")},

	{{NULL, 0, 0}, {NULL, 0, 0}},
};

/* Module global functions */

// clang-format on

nxs_fw_ctl_u_metas_t *nxs_fw_ctl_u_metas_init(void)
{
	nxs_fw_ctl_u_metas_t *u_ctx;

	u_ctx = (nxs_fw_ctl_u_metas_t *)nxs_malloc(NULL, sizeof(nxs_fw_ctl_u_metas_t));

	nxs_string_init(&u_ctx->name);
	nxs_string_init(&u_ctx->inline_name);
	nxs_string_init(&u_ctx->upcase_name);

	nxs_string_init(&u_ctx->path);

	nxs_string_init(&u_ctx->proj_name);
	nxs_string_init(&u_ctx->proj_root);

	return u_ctx;
}

nxs_fw_ctl_u_metas_t *nxs_fw_ctl_u_metas_free(nxs_fw_ctl_u_metas_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	nxs_string_free(&u_ctx->name);
	nxs_string_free(&u_ctx->inline_name);
	nxs_string_free(&u_ctx->upcase_name);

	nxs_string_free(&u_ctx->path);

	nxs_string_free(&u_ctx->proj_name);
	nxs_string_free(&u_ctx->proj_root);

	return (nxs_fw_ctl_u_metas_t *)nxs_free(u_ctx);
}

void nxs_fw_ctl_u_metas_setup(nxs_fw_ctl_u_metas_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *m_name)
{
	nxs_string_t m_full_name;

	nxs_string_init(&m_full_name);

	/*
	 * Подготовка переменных
	 */

	/* name */

	nxs_string_cpy_dyn(&u_ctx->name, 0, m_name, 0);

	/* path, rpath, ppath */

	nxs_string_printf_dyn(&u_ctx->path, "%r/src/meta/", proj_root);

	/* unit_obj, unit_upcase_name, unit_inline_name */

	nxs_string_printf_dyn(&m_full_name, "%r-m-%r", proj_name, m_name);

	nxs_fw_ctl_c_set_names(&m_full_name, NULL, &u_ctx->upcase_name, &u_ctx->inline_name);

	/* proj_root */

	nxs_string_cpy_dyn(&u_ctx->proj_root, 0, proj_root, 0);

	/* proj_name */

	nxs_fw_ctl_c_set_names(proj_name, &u_ctx->proj_name, NULL, NULL);

	nxs_string_free(&m_full_name);
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_metas_add(nxs_fw_ctl_u_metas_t *u_ctx)
{
	nxs_fw_ctl_err_t rc;
	nxs_string_t     meta_path;
	nxs_fs_stat_t    file_stat;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&meta_path);

	/* Проверка существования "meta" с таким именем */

	nxs_string_printf_dyn(&meta_path, "%r%r.h", &u_ctx->path, &u_ctx->name);

	if(nxs_fs_lstat(&meta_path, &file_stat) < 0) {

		if(errno != ENOENT) {

			nxs_log_write_error(&process,
			                    "can't create new meta: error check meta existing (error: %s, meta path: %s)",
			                    strerror(errno),
			                    nxs_string_str(&meta_path));

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}
	else {

		nxs_log_write_error(&process, "can't create new meta: meta exist (meta path: %s)", nxs_string_str(&meta_path));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* Создание "meta" */

	if(nxs_fw_ctl_u_metas_make_genfiles(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_metas_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&meta_path);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_metas_del(nxs_fw_ctl_u_metas_t *u_ctx)
{
	nxs_fw_ctl_err_t rc;
	nxs_string_t     objs_path, meta_path;
	nxs_fs_stat_t    file_stat;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&objs_path);
	nxs_string_init(&meta_path);

	nxs_string_printf_dyn(&meta_path, "%r%r.h", &u_ctx->path, &u_ctx->name);

	/* Проверка существования "meta" с таким именем */

	if(nxs_fs_lstat(&meta_path, &file_stat) < 0) {

		nxs_log_write_error(&process, "can't delete meta (error: %s, meta path: %s)", strerror(errno), nxs_string_str(&meta_path));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_string_printf_dyn(&objs_path, "%r/objs/", &u_ctx->proj_root);

	nxs_log_write_console(&process, "\tthis meta will be deleted from project '%s':", nxs_string_str(&u_ctx->proj_name));
	nxs_log_write_console(&process, "\t* %s", nxs_string_str(&u_ctx->name));

	if(nxs_fw_ctl_c_rlfields_yesno(&_s_question_meta_del) == NXS_NO) {

		nxs_error(rc, NXS_FW_CTL_E_CANCEL, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&objs_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fs_unlink(&meta_path)) {

		nxs_log_write_error(&process, "can't delete meta (error: %s, meta path: %s)", strerror(errno), nxs_string_str(&meta_path));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_metas_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&objs_path);
	nxs_string_free(&meta_path);

	return rc;
}

/* Module internal (static) functions */

static nxs_fw_ctl_err_t nxs_fw_ctl_u_metas_make_genfiles(nxs_fw_ctl_u_metas_t *u_ctx)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_m_filename, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_m_upcase_name, &u_ctx->upcase_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_m_inline_name, &u_ctx->inline_name);

	for(i = 0; nxs_string_len(&genfiles[i].src) > 0; i++) {

		nxs_string_printf_dyn(&tpl_path,
		                      "%r/" NXS_FW_CTL_DIR_ADD_TPL "/" NXS_FW_CTL_DIR_METAS "/%r",
		                      &nxs_fw_ctl_cfg.tpls_path,
		                      &genfiles[i].src);
		nxs_string_printf_dyn(&dst_path, "%r%r", &u_ctx->path, &genfiles[i].dst);

		nxs_fw_ctl_c_copy_tpl_path(&subs, &dst_path);

		if(nxs_fw_ctl_c_copy_tpl(&subs, &tpl_path, &dst_path) != NXS_FW_CTL_E_OK) {

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&tpl_path);
	nxs_string_free(&dst_path);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_metas_make_sub_headers(nxs_fw_ctl_u_metas_t *u_ctx)
{
	nxs_array_t      sub_els;
	size_t           i;
	nxs_string_t     header_path, meta_headers, *s;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&sub_els, 0, sizeof(nxs_string_t), 1);

	nxs_string_init(&header_path);

	/*
	 * Подготовка нового значения блока 'includes'
	 */

	nxs_string_init2(&meta_headers, 0, (u_char *)NXS_FW_CTL_U_METAS_TPL_M_HEADERS_B "\n");

	if(nxs_fw_ctl_c_dir_get_name_files(&u_ctx->path, &sub_els, NULL) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		nxs_string_printf2_cat_dyn(&meta_headers, "#include <meta/%r>\n", s);
	}

	nxs_string_char_cat_dyn(&meta_headers, (u_char *)NXS_FW_CTL_U_METAS_TPL_M_HEADERS_E);

	/*
	 * Процесс замещения блока 'includes'
	 */

	nxs_string_printf_dyn(&header_path, "%rsrc/%r-meta.h", &u_ctx->proj_root, &u_ctx->proj_name);

	/* headers */

	if(nxs_fw_ctl_c_subs_includes(&header_path, NXS_YES, &meta_headers, &_s_tpl_m_headers_b, &_s_tpl_m_headers_e) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&header_path);
	nxs_string_free(&meta_headers);

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		nxs_string_free(s);
	}

	nxs_array_free(&sub_els);

	return rc;
}
