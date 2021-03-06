#ifndef _INCLUDE_NXS_FW_CTL_U_UNITS_C
#define _INCLUDE_NXS_FW_CTL_U_UNITS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/units/units-typedefs.h>
#include <units/units/units.h>
#include <units/units/units-subunits.h>

/* Definitions */

#define NXS_FW_CTL_U_UNITS_TPL_U_FILENAME		"%%NXS_TPL_U_FILENAME%%"
#define NXS_FW_CTL_U_UNITS_TPL_U_RPATH			"%%NXS_TPL_U_RPATH%%"
#define NXS_FW_CTL_U_UNITS_TPL_U_OBJ			"%%NXS_TPL_U_OBJ%%"
#define NXS_FW_CTL_U_UNITS_TPL_U_UPCASE_NAME		"%%NXS_TPL_U_UPCASE_NAME%%"
#define NXS_FW_CTL_U_UNITS_TPL_U_INLINE_NAME		"%%NXS_TPL_U_INLINE_NAME%%"
#define NXS_FW_CTL_U_UNITS_TPL_P_NAME			"%%NXS_TPL_P_NAME%%"
#define NXS_FW_CTL_U_UNITS_TPL_P_INLINE_NAME		"%%NXS_TPL_P_INLINE_NAME%%"

#define NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_TYPEDEF_B	"/*%%_NXS_TPL_HEADERS_TYPEDEF_BEGIN_%%*/"
#define NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_TYPEDEF_E	"/*%%_NXS_TPL_HEADERS_TYPEDEF_END_%%*/"

#define NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_B		"/*%%_NXS_TPL_HEADERS_BEGIN_%%*/"
#define NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_E		"/*%%_NXS_TPL_HEADERS_END_%%*/"

/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct						nxs_fw_ctl_u_units_path_s				nxs_fw_ctl_u_units_path_t;

/* Module declarations */

struct nxs_fw_ctl_u_units_path_s
{
	nxs_string_t					src;
	nxs_string_t					dst;
	mode_t						mode;
};

struct nxs_fw_ctl_u_units_s
{
	nxs_string_t					name;
	nxs_string_t					inline_name;
	nxs_string_t					upcase_name;

	nxs_string_t					obj_name;
	nxs_string_t					ppath;
	nxs_string_t					path;
	nxs_string_t					prpath;
	nxs_string_t					rpath;

	size_t						level;

	nxs_string_t					proj_name;
	nxs_string_t					proj_inline_name;

	nxs_string_t					proj_root;

	nxs_array_t					units_chain;
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_fs_struct(nxs_fw_ctl_u_units_t *u_ctx);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_makefiles(nxs_fw_ctl_u_units_t *u_ctx, nxs_string_t *fw_version);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_genfiles(nxs_fw_ctl_u_units_t *u_ctx, nxs_string_t *fw_version);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_sub_headers(nxs_fw_ctl_u_units_t *u_ctx);

// clang-format off

/* Module initializations */

static nxs_string_t _s_tpl_u_filename			= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_FILENAME);
static nxs_string_t _s_tpl_u_rpath			= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_RPATH);
static nxs_string_t _s_tpl_u_obj			= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_OBJ);

static nxs_string_t _s_tpl_u_upcase_name		= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_UPCASE_NAME);
static nxs_string_t _s_tpl_u_inline_name		= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_INLINE_NAME);
static nxs_string_t _s_tpl_u_p_name			= nxs_string(NXS_FW_CTL_U_UNITS_TPL_P_NAME);
static nxs_string_t _s_tpl_u_p_inline_name		= nxs_string(NXS_FW_CTL_U_UNITS_TPL_P_INLINE_NAME);

static nxs_string_t _s_tpl_u_headers_typedef_b		= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_TYPEDEF_B);
static nxs_string_t _s_tpl_u_headers_typedef_e		= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_TYPEDEF_E);
static nxs_string_t _s_tpl_u_headers_b			= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_B);
static nxs_string_t _s_tpl_u_headers_e			= nxs_string(NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_E);

static nxs_string_t _s_question_units_del		= nxs_string("\tyou realy want to delete this units? [y/n]: ");

static nxs_fw_ctl_u_units_path_t makefiles[] =
{
	{nxs_string("Makefile_tpl"),		nxs_string("Makefile"),							NXS_FW_CTL_FILE_MODE_DEF},

	{{NULL, 0, 0}, {NULL, 0, 0}, 0}
};

static nxs_fw_ctl_u_units_path_t genfiles[] =
{
	{nxs_string("subunits.h_tpl"),		nxs_string("" NXS_FW_CTL_U_UNITS_TPL_U_FILENAME "-subunits.h"),		NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("typedefs.h_tpl"),		nxs_string("" NXS_FW_CTL_U_UNITS_TPL_U_FILENAME "-typedefs.h"),		NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("unit.c_tpl"),		nxs_string("" NXS_FW_CTL_U_UNITS_TPL_U_FILENAME ".c"),			NXS_FW_CTL_FILE_MODE_DEF},
	{nxs_string("unit.h_tpl"),		nxs_string("" NXS_FW_CTL_U_UNITS_TPL_U_FILENAME ".h"),			NXS_FW_CTL_FILE_MODE_DEF},

	{{NULL, 0, 0}, {NULL, 0, 0}, 0},
};

/* Module global functions */

// clang-format on

nxs_fw_ctl_u_units_t *nxs_fw_ctl_u_units_init(void)
{
	nxs_fw_ctl_u_units_t *u_ctx;

	u_ctx = (nxs_fw_ctl_u_units_t *)nxs_malloc(NULL, sizeof(nxs_fw_ctl_u_units_t));

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

	nxs_string_init(&u_ctx->proj_root);

	nxs_array_init(&u_ctx->units_chain, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	return u_ctx;
}

nxs_fw_ctl_u_units_t *nxs_fw_ctl_u_units_free(nxs_fw_ctl_u_units_t *u_ctx)
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

	nxs_string_free(&u_ctx->proj_root);

	for(i = 0; i < nxs_array_count(&u_ctx->units_chain); i++) {

		s = nxs_array_get(&u_ctx->units_chain, i);

		nxs_string_free(s);
	}

	nxs_array_free(&u_ctx->units_chain);

	return (nxs_fw_ctl_u_units_t *)nxs_free(u_ctx);
}

nxs_fw_ctl_err_t
        nxs_fw_ctl_u_units_setup(nxs_fw_ctl_u_units_t *u_ctx, nxs_string_t *proj_name, nxs_string_t *proj_root, nxs_string_t *u_name_str)
{
	nxs_string_t *   s, u_full_name, tmp_str;
	u_char *         c, *b;
	size_t           l, o, i;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&u_full_name);
	nxs_string_init2(&tmp_str, 0, NXS_STRING_EMPTY_STR);

	/*
	 * Парсинг u_name_str
	 */

	for(o = 0, b = nxs_string_str(u_name_str), l = nxs_string_len(u_name_str);
	    (c = nxs_string_find_char_first(u_name_str, o, '.')) != NULL;
	    o = c - b + 1) {

		if(c - b - o == 0) {

			nxs_log_write_error(&process, "wrong unit name (char post: %d)", c - b);

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}

		s = nxs_array_add(&u_ctx->units_chain);

		nxs_string_init(s);

		nxs_string_ncpy(s, 0, u_name_str, o, c - b - o);
	}

	if(o < l) {

		s = nxs_array_add(&u_ctx->units_chain);

		nxs_string_init(s);

		nxs_string_ncpy(s, 0, u_name_str, o, l - o);
	}

	if((l = nxs_array_count(&u_ctx->units_chain)) == 0) {

		nxs_log_write_error(&process, "empty unit name");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	u_ctx->level = l - 1;

	/*
	 * Подготовка переменных
	 */

	/* name */

	s = nxs_array_get(&u_ctx->units_chain, l - 1);

	nxs_string_cpy(&u_ctx->name, 0, s, 0);

	/* Заполнение временной строки, которая потом будет использована для формирования путей */

	for(i = 0; i < l - 1; i++) {

		s = nxs_array_get(&u_ctx->units_chain, i);

		nxs_string_printf2_cat(&tmp_str, "%r/", s);
	}

	/* ppath, path, prpath, rpath */

	s = nxs_array_get(&u_ctx->units_chain, l - 1);

	nxs_string_printf(&u_ctx->ppath, "%r/src/units/%r", proj_root, &tmp_str);
	nxs_string_printf(&u_ctx->path, "%r%r/", &u_ctx->ppath, s);
	nxs_string_printf(&u_ctx->prpath, "%r", &tmp_str);
	nxs_string_printf(&u_ctx->rpath, "%r%r/", &tmp_str, s);

	/* obj_name, upcase_name, inline_name */

	nxs_string_printf(&u_full_name, "%r-u", proj_name);

	for(i = 0; i < l; i++) {

		s = nxs_array_get(&u_ctx->units_chain, i);

		nxs_string_printf2_cat(&u_full_name, "-%r", s);
	}

	nxs_fw_ctl_c_set_names(&u_full_name, &u_ctx->obj_name, &u_ctx->upcase_name, &u_ctx->inline_name);

	/* proj_root */

	nxs_string_cpy(&u_ctx->proj_root, 0, proj_root, 0);

	/* proj_name */

	nxs_fw_ctl_c_set_names(proj_name, &u_ctx->proj_name, NULL, &u_ctx->proj_inline_name);

error:

	nxs_string_free(&u_full_name);
	nxs_string_free(&tmp_str);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_units_add(nxs_fw_ctl_u_units_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	if(nxs_fw_ctl_u_units_make_fs_struct(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_units_make_makefiles(u_ctx, fw_version) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_units_make_genfiles(u_ctx, fw_version) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_units_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_units_del(nxs_fw_ctl_u_units_t *u_ctx)
{
	nxs_fw_ctl_err_t rc;
	nxs_array_t      sub_objects;
	nxs_string_t *   s, unit_name, objs_path;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&unit_name);
	nxs_string_init(&objs_path);

	nxs_array_init(&sub_objects, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	nxs_string_printf(&objs_path, "%r/objs/", &u_ctx->proj_root);

	for(i = 0; i < nxs_array_count(&u_ctx->units_chain); i++) {

		if(i > 0) {

			nxs_string_char_add_char(&unit_name, (u_char)'.');
		}

		s = nxs_array_get(&u_ctx->units_chain, i);

		nxs_string_printf2_cat(&unit_name, "%r", s);
	}

	/* Получение массива имён дочерних объектов */
	if(nxs_fw_ctl_c_dir_get_name_subdirs(&u_ctx->path, &sub_objects, NULL) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\tnext units will be deleted from project '%r':", &u_ctx->proj_name);
	nxs_log_write_console(&process, "\t* %r", &unit_name);

	for(i = 0; i < nxs_array_count(&sub_objects); i++) {

		s = nxs_array_get(&sub_objects, i);

		nxs_log_write_console(&process, "\t* %r.%r", &unit_name, s);
	}

	if(nxs_fw_ctl_c_rlfields_yesno(&_s_question_units_del) == NXS_NO) {

		nxs_error(rc, NXS_FW_CTL_E_CANCEL, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&objs_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_c_dir_del(&u_ctx->path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_units_make_sub_headers(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&unit_name);
	nxs_string_free(&objs_path);

	for(i = 0; i < nxs_array_count(&sub_objects); i++) {

		s = nxs_array_get(&sub_objects, i);

		nxs_string_free(s);
	}

	nxs_array_free(&sub_objects);

	return rc;
}

/* Module internal (static) functions */

static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_fs_struct(nxs_fw_ctl_u_units_t *u_ctx)
{

	if(nxs_fs_mkdir(&u_ctx->path, NXS_FW_CTL_DIR_MODE_DEF) < 0) {

		if(errno == EEXIST) {

			nxs_log_write_error(&process, "can't create new unit: unit exist (unit path: %r)", &u_ctx->path);

			return NXS_FW_CTL_E_ERR;
		}
		else {

			nxs_log_write_error(
			        &process, "can't create unit fs structure: %s (creating dir: %r)", strerror(errno), &u_ctx->path);

			return NXS_FW_CTL_E_ERR;
		}
	}

	return NXS_FW_CTL_E_OK;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_makefiles(nxs_fw_ctl_u_units_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_filename, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_rpath, &u_ctx->rpath);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_obj, &u_ctx->obj_name);

	for(i = 0; nxs_string_len(&makefiles[i].src) > 0; i++) {

		nxs_string_printf(&tpl_path,
		                  "%r/%r/" NXS_FW_CTL_DIR_ADD_TPL "/" NXS_FW_CTL_DIR_UNITS "/%r",
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

static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_genfiles(nxs_fw_ctl_u_units_t *u_ctx, nxs_string_t *fw_version)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_filename, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_rpath, &u_ctx->rpath);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_obj, &u_ctx->obj_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_upcase_name, &u_ctx->upcase_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_p_name, &u_ctx->proj_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_u_p_inline_name, &u_ctx->proj_inline_name);

	for(i = 0; nxs_string_len(&genfiles[i].src) > 0; i++) {

		nxs_string_printf(&tpl_path,
		                  "%r/%r/" NXS_FW_CTL_DIR_ADD_TPL "/" NXS_FW_CTL_DIR_UNITS "/%r",
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

static nxs_fw_ctl_err_t nxs_fw_ctl_u_units_make_sub_headers(nxs_fw_ctl_u_units_t *u_ctx)
{
	nxs_array_t      sub_els;
	size_t           i;
	nxs_string_t     header_path, unit_headers, unit_typedefs, *s;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&sub_els, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	nxs_string_init(&header_path);

	/*
	 * Подготовка нового значения блока 'includes'
	 */

	nxs_string_init2(&unit_headers, 0, (u_char *)NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_B "\n");
	nxs_string_init2(&unit_typedefs, 0, (u_char *)NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_TYPEDEF_B "\n");

	if(nxs_fw_ctl_c_dir_get_name_dirs(&u_ctx->ppath, &sub_els, NULL) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		nxs_string_printf2_cat(&unit_headers, "#include <units/%r%r/%r.h>\n", &u_ctx->prpath, s, s);
		nxs_string_printf2_cat(&unit_typedefs, "#include <units/%r%r/%r-typedefs.h>\n", &u_ctx->prpath, s, s);
	}

	nxs_string_char_cat(&unit_headers, (u_char *)NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_E);
	nxs_string_char_cat(&unit_typedefs, (u_char *)NXS_FW_CTL_U_UNITS_TPL_U_HEADERS_TYPEDEF_E);

	/*
	 * Процесс замещения блока 'includes'
	 */

	/* Определение пути до header-файла в котором необходимо произвести замены */
	if(u_ctx->level == 0) {

		/* Для юнитов нулевого уровня (которые размещены непосредственно в units/) */

		nxs_string_printf(&header_path, "%rsrc/%r-units.h", &u_ctx->proj_root, &u_ctx->proj_name);
	}
	else {

		/* Для вложенных юнитов */

		s = nxs_array_get(&u_ctx->units_chain, u_ctx->level - 1);

		nxs_string_printf(&header_path, "%r%r-subunits.h", &u_ctx->ppath, s);
	}

	/* typedefs */

	if(nxs_fw_ctl_c_subs_includes(&header_path, NXS_YES, &unit_typedefs, &_s_tpl_u_headers_typedef_b, &_s_tpl_u_headers_typedef_e) !=
	   NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* headers */

	if(nxs_fw_ctl_c_subs_includes(&header_path, NXS_YES, &unit_headers, &_s_tpl_u_headers_b, &_s_tpl_u_headers_e) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&header_path);
	nxs_string_free(&unit_headers);
	nxs_string_free(&unit_typedefs);

	for(i = 0; i < nxs_array_count(&sub_els); i++) {

		s = nxs_array_get(&sub_els, i);

		nxs_string_free(s);
	}

	nxs_array_free(&sub_els);

	return rc;
}
