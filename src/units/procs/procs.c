#ifndef _INCLUDE_NXS_FW_CTL_U_PROCS_C
#define _INCLUDE_NXS_FW_CTL_U_PROCS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/procs/procs-typedefs.h>
#include <units/procs/procs.h>
#include <units/procs/procs-subunits.h>

/* Definitions */

#define NXS_FW_CTL_U_PROCS_TPL_P_FILENAME			"%%NXS_TPL_P_FILENAME%%"
#define NXS_FW_CTL_U_PROCS_TPL_P_RPATH				"%%NXS_TPL_P_RPATH%%"
#define NXS_FW_CTL_U_PROCS_TPL_P_OBJ				"%%NXS_TPL_P_OBJ%%"
#define NXS_FW_CTL_U_PROCS_TPL_P_UPCASE_NAME			"%%NXS_TPL_P_UPCASE_NAME%%"
#define NXS_FW_CTL_U_PROCS_TPL_P_INLINE_NAME			"%%NXS_TPL_P_INLINE_NAME%%"
#define NXS_FW_CTL_U_PROCS_TPL_PROJ_NAME			"%%NXS_TPL_PROJ_NAME%%"
#define NXS_FW_CTL_U_PROCS_TPL_PROJ_INLINE_NAME			"%%NXS_TPL_PROJ_INLINE_NAME%%"
#define NXS_FW_CTL_U_PROCS_TPL_PROJ_UPCASE_NAME			"%%NXS_TPL_PROJ_UPCASE_NAME%%"

#define NXS_FW_CTL_U_PROCS_TPL_P_HEADERS_B			"/*%%_NXS_TPL_HEADERS_BEGIN_%%*/"
#define NXS_FW_CTL_U_PROCS_TPL_P_HEADERS_E			"/*%%_NXS_TPL_HEADERS_END_%%*/"

/* Project globals */
extern		nxs_process_t					process;
extern		nxs_fw_ctl_cfg_t				nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct							nxs_fw_ctl_u_procs_path_s				nxs_fw_ctl_u_procs_path_t;

/* Module declarations */

struct nxs_fw_ctl_u_procs_path_s
{
	nxs_string_t						src;
	nxs_string_t						dst;
	mode_t							mode;
};

struct nxs_fw_ctl_u_procs_s
{
	nxs_string_t						name;
	nxs_string_t						inline_name;
	nxs_string_t						upcase_name;

	nxs_string_t						obj_name;
	nxs_string_t						ppath;
	nxs_string_t						path;
	nxs_string_t						prpath;
	nxs_string_t						rpath;

	size_t							level;

	nxs_string_t						proj_name;
	nxs_string_t						proj_inline_name;
	nxs_string_t						proj_upcase_name;

	nxs_string_t						proj_root;

	nxs_array_t						procs_chain;
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_fs_struct(nxs_fw_ctl_u_procs_t *u_ctx);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_makefiles(nxs_fw_ctl_u_procs_t *u_ctx, nxs_string_t *fw_version);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_genfiles(nxs_fw_ctl_u_procs_t *u_ctx, nxs_string_t *fw_version);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_sub_headers(nxs_fw_ctl_u_procs_t *u_ctx);

// clang-format off

/* Module initializations */

static nxs_string_t _s_tpl_p_filename				= nxs_string(NXS_FW_CTL_U_PROCS_TPL_P_FILENAME);
static nxs_string_t _s_tpl_p_rpath				= nxs_string(NXS_FW_CTL_U_PROCS_TPL_P_RPATH);
static nxs_string_t _s_tpl_p_obj				= nxs_string(NXS_FW_CTL_U_PROCS_TPL_P_OBJ);

static nxs_string_t _s_tpl_p_upcase_name			= nxs_string(NXS_FW_CTL_U_PROCS_TPL_P_UPCASE_NAME);
static nxs_string_t _s_tpl_p_inline_name			= nxs_string(NXS_FW_CTL_U_PROCS_TPL_P_INLINE_NAME);
static nxs_string_t _s_tpl_p_p_name				= nxs_string(NXS_FW_CTL_U_PROCS_TPL_PROJ_NAME);
static nxs_string_t _s_tpl_p_p_inline_name			= nxs_string(NXS_FW_CTL_U_PROCS_TPL_PROJ_INLINE_NAME);
static nxs_string_t _s_tpl_p_p_upcase_name			= nxs_string(NXS_FW_CTL_U_PROCS_TPL_PROJ_UPCASE_NAME);

static nxs_string_t _s_tpl_p_headers_b				= nxs_string(NXS_FW_CTL_U_PROCS_TPL_P_HEADERS_B);
static nxs_string_t _s_tpl_p_headers_e				= nxs_string(NXS_FW_CTL_U_PROCS_TPL_P_HEADERS_E);

static nxs_string_t _s_ctx					= nxs_string("ctx");

static nxs_string_t _s_question_procs_del			= nxs_string("\tyou realy want to delete this processes? [y/n]: ");

static nxs_string_t init_dirs[] =
{
	nxs_string("%r"),
	nxs_string("%rctx/"),

	{NULL, 0, 0}
};

static nxs_fw_ctl_u_procs_path_t makefiles[] =
{
	{nxs_string("Makefile_tpl"),		nxs_string("Makefile"),							NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("ctx/Makefile_tpl"),	nxs_string("ctx/Makefile"),						NXS_FW_CTL_FILE_MODE_DEF},

	{{NULL, 0, 0}, {NULL, 0, 0}, 0}
};

static nxs_fw_ctl_u_procs_path_t genfiles[] =
{
	{nxs_string("subproc.h_tpl"),		nxs_string("" NXS_FW_CTL_U_PROCS_TPL_P_FILENAME "-subproc.h"),		NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("proc.c_tpl"),		nxs_string("" NXS_FW_CTL_U_PROCS_TPL_P_FILENAME ".c"),			NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("proc.h_tpl"),		nxs_string("" NXS_FW_CTL_U_PROCS_TPL_P_FILENAME ".h"),			NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("ctx/ctx.c_tpl"),		nxs_string("ctx/" NXS_FW_CTL_U_PROCS_TPL_P_FILENAME "-ctx.c"),		NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("ctx/ctx.h_tpl"),		nxs_string("ctx/" NXS_FW_CTL_U_PROCS_TPL_P_FILENAME "-ctx.h"),		NXS_FW_CTL_FILE_MODE_DEF},

	{{NULL, 0, 0}, {NULL, 0, 0}, 0},
};

/* Module global functions */

// clang-format on

nxs_fw_ctl_u_procs_t *nxs_fw_ctl_u_procs_init(void)
{
	nxs_fw_ctl_u_procs_t *u_ctx;

	u_ctx = (nxs_fw_ctl_u_procs_t *)nxs_malloc(NULL, sizeof(nxs_fw_ctl_u_procs_t));

	nxs_string_init(&u_ctx->name);
	nxs_string_init(&u_ctx->inline_name);
	nxs_string_init(&u_ctx->upcase_name);

	nxs_string_init(&u_ctx->obj_name);
	nxs_string_init(&u_ctx->ppath);
	nxs_string_init(&u_ctx->path);
	nxs_string_init(&u_ctx->prpath);
	nxs_string_init(&u_ctx->rpath);

	u_ctx->level = 0;

	nxs_string_init(&u_ctx->proj_name);
	nxs_string_init(&u_ctx->proj_inline_name);
	nxs_string_init(&u_ctx->proj_upcase_name);

	nxs_string_init(&u_ctx->proj_root);

	nxs_array_init(&u_ctx->procs_chain, 0, sizeof(nxs_string_t), 1);

	return u_ctx;
}

nxs_fw_ctl_u_procs_t *nxs_fw_ctl_u_procs_free(nxs_fw_ctl_u_procs_t *u_ctx)
{
	nxs_string_t *s;
	size_t        i;

	if(u_ctx == NULL) {

		return NULL;
	}

	nxs_string_free(&u_ctx->name);
	nxs_string_free(&u_ctx->inline_name);
	nxs_string_free(&u_ctx->upcase_name);

	nxs_string_free(&u_ctx->obj_name);
	nxs_string_free(&u_ctx->ppath);
	nxs_string_free(&u_ctx->path);
	nxs_string_free(&u_ctx->prpath);
	nxs_string_free(&u_ctx->rpath);

	u_ctx->level = 0;

	nxs_string_free(&u_ctx->proj_name);
	nxs_string_free(&u_ctx->proj_inline_name);
	nxs_string_free(&u_ctx->proj_upcase_name);

	nxs_string_free(&u_ctx->proj_root);

	for(i = 0; i < nxs_array_count(&u_ctx->procs_chain); i++) {

		s = nxs_array_get(&u_ctx->procs_chain, i);

		nxs_string_free(s);
	}

	nxs_array_free(&u_ctx->procs_chain);

	return (nxs_fw_ctl_u_procs_t *)nxs_free(u_ctx);
}

nxs_fw_ctl_err_t
        nxs_fw_ctl_u_procs_setup(nxs_fw_ctl_u_procs_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *p_name_str)
{
	nxs_string_t *   s, p_full_name, tmp_str;
	u_char *         c, *b;
	size_t           l, o, i;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&p_full_name);
	nxs_string_init2(&tmp_str, 0, NXS_STRING_EMPTY_STR);

	/*
	 * Парсинг p_name_str
	 */

	for(o = 0, b = nxs_string_str(p_name_str), l = nxs_string_len(p_name_str);
	    (c = nxs_string_find_char_first(p_name_str, o, '.')) != NULL;
	    o = c - b + 1) {

		if(c - b - o == 0) {

			nxs_log_write_error(&process, "wrong proc name (char post: %d)", c - b);

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}

		s = nxs_array_add(&u_ctx->procs_chain);

		nxs_string_init(s);

		nxs_string_ncpy(s, 0, p_name_str, o, c - b - o);
	}

	if(o < l) {

		s = nxs_array_add(&u_ctx->procs_chain);

		nxs_string_init(s);

		nxs_string_ncpy(s, 0, p_name_str, o, l - o);
	}

	if((l = nxs_array_count(&u_ctx->procs_chain)) == 0) {

		nxs_log_write_error(&process, "empty proc name");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	u_ctx->level = l - 1;

	/*
	 * Подготовка переменных
	 */

	/* name */

	s = nxs_array_get(&u_ctx->procs_chain, l - 1);

	nxs_string_cpy(&u_ctx->name, 0, s, 0);

	/* Заполнение временной строки, которая потом будет использована для формирования путей */

	for(i = 0; i < l - 1; i++) {

		s = nxs_array_get(&u_ctx->procs_chain, i);

		nxs_string_printf2_cat(&tmp_str, "%r/", s);
	}

	/* ppath, path, prpath, rpath */

	s = nxs_array_get(&u_ctx->procs_chain, l - 1);

	nxs_string_printf(&u_ctx->ppath, "%r/src/proc/%r", proj_root, &tmp_str);
	nxs_string_printf(&u_ctx->path, "%r%r/", &u_ctx->ppath, s);
	nxs_string_printf(&u_ctx->prpath, "%r", &tmp_str);
	nxs_string_printf(&u_ctx->rpath, "%r%r/", &tmp_str, s);

	/* obj_name, upcase_name, inline_name */

	nxs_string_printf(&p_full_name, "%r-p", proj_name);

	for(i = 0; i < l; i++) {

		s = nxs_array_get(&u_ctx->procs_chain, i);

		nxs_string_printf2_cat(&p_full_name, "-%r", s);
	}

	nxs_fw_ctl_c_set_names(&p_full_name, &u_ctx->obj_name, &u_ctx->upcase_name, &u_ctx->inline_name);

	/* proj_root */

	nxs_string_cpy(&u_ctx->proj_root, 0, proj_root, 0);

	/* proj_name */

	nxs_fw_ctl_c_set_names(proj_name, &u_ctx->proj_name, &u_ctx->proj_upcase_name, &u_ctx->proj_inline_name);

error:

	nxs_string_free(&p_full_name);
	nxs_string_free(&tmp_str);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_add(nxs_fw_ctl_u_procs_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	if(nxs_fw_ctl_u_procs_make_fs_struct(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_procs_make_makefiles(u_ctx, fw_version) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_procs_make_genfiles(u_ctx, fw_version) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_procs_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_del(nxs_fw_ctl_u_procs_t *u_ctx)
{
	nxs_fw_ctl_err_t rc;
	nxs_array_t      sub_objects, skips;
	nxs_string_t *   s, proc_name, objs_path;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&proc_name);
	nxs_string_init(&objs_path);

	nxs_array_init(&sub_objects, 0, sizeof(nxs_string_t), 1);
	nxs_array_init(&skips, 0, sizeof(nxs_string_t), 1);

	nxs_string_printf(&objs_path, "%r/objs/", &u_ctx->proj_root);

	s = nxs_array_add(&skips);

	nxs_string_init3(s, &_s_ctx);

	for(i = 0; i < nxs_array_count(&u_ctx->procs_chain); i++) {

		if(i > 0) {

			nxs_string_char_add_char(&proc_name, (u_char)'.');
		}

		s = nxs_array_get(&u_ctx->procs_chain, i);

		nxs_string_printf2_cat(&proc_name, "%r", s);
	}

	/* Получение массива имён дочерних объектов */
	if(nxs_fw_ctl_c_dir_get_name_subdirs(&u_ctx->path, &sub_objects, &skips) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\tnext processes will be deleted from project '%s':", nxs_string_str(&u_ctx->proj_name));
	nxs_log_write_console(&process, "\t* %s", nxs_string_str(&proc_name));

	for(i = 0; i < nxs_array_count(&sub_objects); i++) {

		s = nxs_array_get(&sub_objects, i);

		nxs_log_write_console(&process, "\t* %s.%s", nxs_string_str(&proc_name), nxs_string_str(s));
	}

	if(nxs_fw_ctl_c_rlfields_yesno(&_s_question_procs_del) == NXS_NO) {

		nxs_error(rc, NXS_FW_CTL_E_CANCEL, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&objs_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_c_dir_del(&u_ctx->path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_procs_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&proc_name);
	nxs_string_free(&objs_path);

	for(i = 0; i < nxs_array_count(&sub_objects); i++) {

		s = nxs_array_get(&sub_objects, i);

		nxs_string_free(s);
	}

	for(i = 0; i < nxs_array_count(&skips); i++) {

		s = nxs_array_get(&skips, i);

		nxs_string_free(s);
	}

	nxs_array_free(&sub_objects);
	nxs_array_free(&skips);

	return rc;
}

/* Module internal (static) functions */

static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_fs_struct(nxs_fw_ctl_u_procs_t *u_ctx)
{
	nxs_string_t     path;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&path);

	for(i = 0; nxs_string_str(&init_dirs[i]) != NULL; i++) {

		nxs_string_printf(&path, (char *)nxs_string_str(&init_dirs[i]), &u_ctx->path);

		if(nxs_fs_mkdir(&path, NXS_FW_CTL_DIR_MODE_DEF) < 0) {

			if(errno == EEXIST) {

				nxs_log_write_error(
				        &process, "can't create new proc: proc dir exist (dir path: %s)", nxs_string_str(&path));

				nxs_error(rc, NXS_FW_CTL_E_ERR, error);
			}
			else {

				nxs_log_write_error(&process,
				                    "can't create proc fs structure: %s (creating dir: %s)",
				                    strerror(errno),
				                    nxs_string_str(&path));

				nxs_error(rc, NXS_FW_CTL_E_ERR, error);
			}
		}
	}

error:

	nxs_string_free(&path);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_makefiles(nxs_fw_ctl_u_procs_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_filename, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_rpath, &u_ctx->rpath);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_obj, &u_ctx->obj_name);

	for(i = 0; nxs_string_len(&makefiles[i].src) > 0; i++) {

		nxs_string_printf(&tpl_path,
		                  "%r/%r/" NXS_FW_CTL_DIR_ADD_TPL "/" NXS_FW_CTL_DIR_PROCS "/%r",
		                  &nxs_fw_ctl_cfg.tpls_path,
		                  fw_version,
		                  &makefiles[i].src);
		nxs_string_printf(&dst_path, "%r/%r", &u_ctx->path, &makefiles[i].dst);

		if(nxs_fw_ctl_c_copy_tpl(&subs, &tpl_path, &dst_path, makefiles[i].mode) != NXS_FW_CTL_E_OK) {

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&tpl_path);
	nxs_string_free(&dst_path);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_genfiles(nxs_fw_ctl_u_procs_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_filename, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_rpath, &u_ctx->rpath);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_obj, &u_ctx->obj_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_upcase_name, &u_ctx->upcase_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_p_name, &u_ctx->proj_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_p_inline_name, &u_ctx->proj_inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_p_upcase_name, &u_ctx->proj_upcase_name);

	for(i = 0; nxs_string_len(&genfiles[i].src) > 0; i++) {

		nxs_string_printf(&tpl_path,
		                  "%r/%r/" NXS_FW_CTL_DIR_ADD_TPL "/" NXS_FW_CTL_DIR_PROCS "/%r",
		                  &nxs_fw_ctl_cfg.tpls_path,
		                  fw_version,
		                  &genfiles[i].src);
		nxs_string_printf(&dst_path, "%r/%r", &u_ctx->path, &genfiles[i].dst);

		nxs_fw_ctl_c_copy_tpl_path(&subs, &dst_path);

		if(nxs_fw_ctl_c_copy_tpl(&subs, &tpl_path, &dst_path, genfiles[i].mode) != NXS_FW_CTL_E_OK) {

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&tpl_path);
	nxs_string_free(&dst_path);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_procs_make_sub_headers(nxs_fw_ctl_u_procs_t *u_ctx)
{
	nxs_array_t      sub_els;
	size_t           i;
	nxs_string_t     header_path, proc_headers, *s;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&sub_els, 0, sizeof(nxs_string_t), 1);

	nxs_string_init(&header_path);

	/*
	 * Подготовка нового значения блока 'includes'
	 */

	nxs_string_init2(&proc_headers, 0, (u_char *)NXS_FW_CTL_U_PROCS_TPL_P_HEADERS_B "\n");

	if(nxs_fw_ctl_c_dir_get_name_dirs(&u_ctx->ppath, &sub_els, NULL) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		if(nxs_string_cmp(s, 0, &_s_ctx, 0) == NXS_NO) {

			nxs_string_printf2_cat(&proc_headers, "#include <proc/%r%r/%r.h>\n", &u_ctx->prpath, s, s);
		}
	}

	nxs_string_char_cat(&proc_headers, (u_char *)NXS_FW_CTL_U_PROCS_TPL_P_HEADERS_E);

	/*
	 * Процесс замещения блока 'includes'
	 */

	/* Определение пути до header-файла в котором необходимо произвести замены */
	if(u_ctx->level == 0) {

		/* Для процесса нулевого уровня (которые размещены непосредственно в proc/) */

		nxs_string_printf(&header_path, "%rsrc/proc/bootstrap-subproc.h", &u_ctx->proj_root, &u_ctx->proj_name);
	}
	else {

		/* Для вложенных процессов */

		s = nxs_array_get(&u_ctx->procs_chain, u_ctx->level - 1);

		nxs_string_printf(&header_path, "%r%r-subproc.h", &u_ctx->ppath, s);
	}

	/* headers */

	if(nxs_fw_ctl_c_subs_includes(&header_path, NXS_YES, &proc_headers, &_s_tpl_p_headers_b, &_s_tpl_p_headers_e) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&header_path);
	nxs_string_free(&proc_headers);

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		nxs_string_free(s);
	}

	nxs_array_free(&sub_els);

	return rc;
}
