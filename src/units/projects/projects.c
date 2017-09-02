#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_C
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/projects/projects-typedefs.h>
#include <units/projects/projects.h>
#include <units/projects/projects-subunits.h>

/* Definitions */

#define NXS_FW_CTL_U_PROJECTS_TPL_P_NAME			"%%NXS_TPL_P_NAME%%"
#define NXS_FW_CTL_U_PROJECTS_TPL_P_INLINE_NAME			"%%NXS_TPL_P_INLINE_NAME%%"
#define NXS_FW_CTL_U_PROJECTS_TPL_P_UPCASE_NAME			"%%NXS_TPL_P_UPCASE_NAME%%"
#define NXS_FW_CTL_U_PROJECTS_TPL_USE_MODULES			"%%NXS_TPL_USE_MODULES%%"
#define NXS_FW_CTL_U_PROJECTS_TPL_INCLUDES			"%%NXS_TPL_INCLUDES%%"
#define NXS_FW_CTL_U_PROJECTS_TPL_LINKS				"%%NXS_TPL_LINKS%%"
#define NXS_FW_CTL_U_PROJECTS_TPL_CORE_LINKS			"%%NXS_TPL_CORE_LINKS%%"
#define NXS_FW_CTL_U_PROJECTS_TPL_FW_VERSION			"%%NXS_TPL_FW_VERSION%%"

/* Project globals */
extern		nxs_process_t					process;
extern		nxs_fw_ctl_cfg_t				nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct							nxs_fw_ctl_u_projects_path_s			nxs_fw_ctl_u_projects_path_t;

/* Module declarations */

struct nxs_fw_ctl_u_projects_s
{
	nxs_string_t						name;		/* Имя проекта */
	nxs_string_t						inline_name;	/* Имя проекта, которое будет использовано в коде программы */
	nxs_string_t						upcase_name;	/* Имя проекта, приведённое к заглавным буквам */
	nxs_string_t						root;		/* Корневая директория проекта */
};

struct nxs_fw_ctl_u_projects_path_s
{
	nxs_string_t						src;
	nxs_string_t						dst;
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_fs_struct(nxs_fw_ctl_u_projects_t *u_ctx);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_makefiles(nxs_fw_ctl_u_projects_t *u_ctx,
                                                             nxs_string_t *           fw_version,
                                                             nxs_string_t *           use_flags,
                                                             nxs_string_t *           link_opts,
                                                             nxs_string_t *           includes,
                                                             nxs_string_t *           core_links);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_genfiles(nxs_fw_ctl_u_projects_t *u_ctx);

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_up_config_mk(nxs_string_t *path,
                                                           nxs_string_t *fw_version,
                                                           nxs_string_t *use_modules,
                                                           nxs_string_t *includes,
                                                           nxs_string_t *link_opts,
                                                           nxs_string_t *core_links);

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_settingsfile_get(nxs_string_t *proj_path,
                                                               nxs_string_t *name,
                                                               nxs_string_t *version,
                                                               nxs_array_t * proj_selected_mods);

// clang-format off

/* Module initializations */

static nxs_string_t _s_tpl_p_name				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_P_NAME);
static nxs_string_t _s_tpl_p_inline_name			= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_P_INLINE_NAME);
static nxs_string_t _s_tpl_p_upcase_name			= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_P_UPCASE_NAME);
static nxs_string_t _s_tpl_use_modules				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_USE_MODULES);
static nxs_string_t _s_tpl_includes				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_INCLUDES);
static nxs_string_t _s_tpl_links				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_LINKS);
static nxs_string_t _s_tpl_core_links				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_CORE_LINKS);
static nxs_string_t _s_tpl_fw_version				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_FW_VERSION);

static nxs_string_t _s_use_nxs_modules				= nxs_string("USE_NXS_MODULES");
static nxs_string_t _s_nxs_includes				= nxs_string("NXS_INCLUDES");
static nxs_string_t _s_nxs_links				= nxs_string("NXS_LINKS");
static nxs_string_t _s_nxs_core_links				= nxs_string("NXS_CORE_LINKS");
static nxs_string_t _s_nxs_fw_version				= nxs_string("NXS_FW_VERSION");
static nxs_string_t _s_eol					= nxs_string("\n");

static nxs_string_t _s_question_project_del			= nxs_string("\tyou realy want to delete this project? [y/n]: ");

static nxs_string_t init_dirs[] =
{
	nxs_string("%r"),
	nxs_string("%r.nxs-fw-settings"),
	nxs_string("%rdocs/"),
	nxs_string("%rlogs/"),
	nxs_string("%robjs/"),
	nxs_string("%rsrc/"),
	nxs_string("%rsrc/collections/"),
	nxs_string("%rsrc/conf/"),
	nxs_string("%rsrc/conf/args/"),
	nxs_string("%rsrc/conf/file-ini/"),
	nxs_string("%rsrc/conf/file-json/"),
	nxs_string("%rsrc/dal/"),
	nxs_string("%rsrc/meta/"),
	nxs_string("%rsrc/proc/"),
	nxs_string("%rsrc/units/"),

	{NULL, 0, 0}
};

static nxs_fw_ctl_u_projects_path_t makefiles[] =
{
	{nxs_string("Makefile_tpl"),					nxs_string("Makefile")},
	{nxs_string("config.mk_tpl"),					nxs_string("config.mk")},
	{nxs_string("src/collections/Makefile_tpl"),			nxs_string("src/collections/Makefile")},
	{nxs_string("src/conf/Makefile_tpl"),				nxs_string("src/conf/Makefile")},
	{nxs_string("src/conf/args/Makefile_tpl"),			nxs_string("src/conf/args/Makefile")},
	{nxs_string("src/conf/file-ini/Makefile_tpl"),			nxs_string("src/conf/file-ini/Makefile")},
	{nxs_string("src/conf/file-json/Makefile_tpl"),			nxs_string("src/conf/file-json/Makefile")},
	{nxs_string("src/dal/Makefile_tpl"),				nxs_string("src/dal/Makefile")},
	{nxs_string("src/proc/Makefile_tpl"),				nxs_string("src/proc/Makefile")},
	{nxs_string("src/units/Makefile_tpl"),				nxs_string("src/units/Makefile")},

	{{NULL, 0, 0}, {NULL, 0, 0}}
};

static nxs_fw_ctl_u_projects_path_t genfiles[] =
{
	{nxs_string(".conf.dist-ini_tpl"),				nxs_string("" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME ".conf.dist-ini")},
	{nxs_string(".conf.dist-json_tpl"),				nxs_string("" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME ".conf.dist-json")},
	{nxs_string("src/collections.h_tpl"),				nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-collections.h")},
	{nxs_string("src/conf.h_tpl"),					nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-conf.h")},
	{nxs_string("src/core.c_tpl"),					nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-core.c")},
	{nxs_string("src/core.h_tpl"),					nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-core.h")},
	{nxs_string("src/dal.h_tpl"),					nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-dal.h")},
	{nxs_string("src/meta.h_tpl"),					nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-meta.h")},
	{nxs_string("src/proc.h_tpl"),					nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-proc.h")},
	{nxs_string("src/units.h_tpl"),					nxs_string("src/" NXS_FW_CTL_U_PROJECTS_TPL_P_NAME "-units.h")},
	{nxs_string("src/conf/conf.c_tpl"),				nxs_string("src/conf/conf.c")},
	{nxs_string("src/conf/conf.h_tpl"),				nxs_string("src/conf/conf.h")},
	{nxs_string("src/conf/args/args.c_tpl"),			nxs_string("src/conf/args/args.c")},
	{nxs_string("src/conf/args/args.h_tpl"),			nxs_string("src/conf/args/args.h")},
	{nxs_string("src/conf/file-ini/file-ini.c_tpl"),		nxs_string("src/conf/file-ini/file-ini.c")},
	{nxs_string("src/conf/file-ini/file-ini.h_tpl"),		nxs_string("src/conf/file-ini/file-ini.h")},
	{nxs_string("src/conf/file-json/file-json.c_tpl"),		nxs_string("src/conf/file-json/file-json.c")},
	{nxs_string("src/conf/file-json/file-json.h_tpl"),		nxs_string("src/conf/file-json/file-json.h")},
	{nxs_string("src/proc/bootstrap.c_tpl"),			nxs_string("src/proc/bootstrap.c")},
	{nxs_string("src/proc/bootstrap.h_tpl"),			nxs_string("src/proc/bootstrap.h")},
	{nxs_string("src/proc/bootstrap-subproc.h_tpl"),		nxs_string("src/proc/bootstrap-subproc.h")},
	{nxs_string(".clang-format_tpl"),				nxs_string(".clang-format")},
	{nxs_string(".gitignore_tpl"),					nxs_string(".gitignore")},
	{nxs_string("docs/.placeholder"),				nxs_string("docs/.placeholder")},
	{nxs_string("logs/.gitignore"),					nxs_string("logs/.gitignore")},
	{nxs_string("objs/.gitignore"),					nxs_string("objs/.gitignore")},

	{{NULL, 0, 0}, {NULL, 0, 0}}
};

/* Module global functions */

// clang-format on

nxs_fw_ctl_u_projects_t *nxs_fw_ctl_u_projects_init(void)
{
	nxs_fw_ctl_u_projects_t *u_ctx;

	u_ctx = (nxs_fw_ctl_u_projects_t *)nxs_malloc(NULL, sizeof(nxs_fw_ctl_u_projects_t));

	nxs_string_init(&u_ctx->name);
	nxs_string_init(&u_ctx->inline_name);
	nxs_string_init(&u_ctx->upcase_name);
	nxs_string_init(&u_ctx->root);

	return u_ctx;
}

nxs_fw_ctl_u_projects_t *nxs_fw_ctl_u_projects_free(nxs_fw_ctl_u_projects_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	nxs_string_free(&u_ctx->name);
	nxs_string_free(&u_ctx->inline_name);
	nxs_string_free(&u_ctx->upcase_name);
	nxs_string_free(&u_ctx->root);

	return (nxs_fw_ctl_u_projects_t *)nxs_free(u_ctx);
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_setup(nxs_fw_ctl_u_projects_t *u_ctx, nxs_string_t *proj_root, nxs_string_t *proj_name)
{

	if(u_ctx == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	if(nxs_string_len(proj_root) == 0) {

		nxs_log_write_error(&process, "project root string len is 0");

		return NXS_FW_CTL_E_ERR;
	}

	nxs_string_cpy_dyn(&u_ctx->root, 0, proj_root, 0);

	if(nxs_string_get_char(proj_root, nxs_string_len(proj_root) - 1) != (u_char)'/') {

		nxs_string_char_add_char_dyn(proj_root, (u_char)'/');
	}

	return nxs_fw_ctl_c_set_names(proj_name, &u_ctx->name, &u_ctx->upcase_name, &u_ctx->inline_name);
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_add(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_fw_ctl_u_projects_core_modules_t *c_mods;
	nxs_fw_ctl_err_t                      rc;
	nxs_string_t setting_path, core_mods_dst, *use_flags, *link_opts, *includes, *core_links, nxs_core_desc_path, nxs_fw_version, *s;
	nxs_array_t *selected_mods, versions;
	size_t       i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&setting_path);
	nxs_string_init(&core_mods_dst);
	nxs_string_init(&nxs_core_desc_path);
	nxs_string_init(&nxs_fw_version);

	nxs_string_printf_dyn(&setting_path, "%r.nxs-fw-settings/settings.json", &u_ctx->root);
	nxs_string_printf_dyn(&core_mods_dst, "%r", &u_ctx->root);

	nxs_array_init(&versions, 0, sizeof(nxs_string_t), 1);

	c_mods = nxs_fw_ctl_u_projects_core_modules_init();

	if(nxs_fw_ctl_u_projects_sttngs_check(&setting_path) == NXS_YES) {

		nxs_log_write_warn(&process, "can't create project: project exist (settings file: %s)", nxs_string_str(&setting_path));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_version_available(&nxs_fw_ctl_cfg.nxs_fw_desc_path, &versions) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\n  project root: %s", nxs_string_str(&nxs_fw_ctl_cfg.proj_root));
	nxs_log_write_console(&process, "\n  choose nxs-fw version:");

	nxs_fw_ctl_u_projects_version_select(&nxs_fw_version, &versions);

	nxs_string_printf_dyn(
	        &nxs_core_desc_path, "%r/%r/%s", &nxs_fw_ctl_cfg.nxs_fw_desc_path, &nxs_fw_version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);

	if(nxs_fw_ctl_u_projects_core_modules_read(c_mods, &nxs_core_desc_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_make_fs_struct(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\n  choose nxs-fw modules:");

	nxs_fw_ctl_u_projects_core_modules_select(c_mods, NULL);

	selected_mods = nxs_fw_ctl_u_projects_core_modules_selected_get_mods(c_mods);
	use_flags     = nxs_fw_ctl_u_projects_core_modules_using_get_use_flags(c_mods);
	link_opts     = nxs_fw_ctl_u_projects_core_modules_using_get_link_opts(c_mods);
	includes      = nxs_fw_ctl_u_projects_core_modules_using_get_includes(c_mods);
	core_links    = nxs_fw_ctl_u_projects_core_modules_using_get_core_links(c_mods);

	if(nxs_fw_ctl_u_projects_sttngs_write(&setting_path, &u_ctx->name, &nxs_fw_version, selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_make_makefiles(u_ctx, &nxs_fw_version, use_flags, link_opts, includes, core_links) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_make_genfiles(u_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	c_mods = nxs_fw_ctl_u_projects_core_modules_free(c_mods);

	for(i = 0; i < nxs_array_count(&versions); i++) {

		s = nxs_array_get(&versions, i);

		nxs_string_free(s);
	}

	nxs_array_free(&versions);

	nxs_string_free(&setting_path);
	nxs_string_free(&core_mods_dst);
	nxs_string_free(&nxs_fw_version);
	nxs_string_free(&nxs_core_desc_path);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_del(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_log_write_console(&process, "\tproject '%s' will be deleted:", nxs_string_str(&u_ctx->name));

	if(nxs_fw_ctl_c_rlfields_yesno(&_s_question_project_del) == NXS_NO) {

		nxs_error(rc, NXS_FW_CTL_E_CANCEL, error);
	}

	if(nxs_fw_ctl_c_dir_del(&u_ctx->root) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	return rc;
}

/*
 * Изменение списка core-модулей, используемых в проекте
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_change_core_mods(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_fw_ctl_u_projects_core_modules_t *c_mods;
	nxs_array_t *                         selected_mods, read_selected_mods;
	nxs_string_t setting_path, configmk_path, name, proj_objs_dst, nxs_fw_version, nxs_core_desc_path, *s, *use_flags, *link_opts,
	        *includes, *core_links;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	if(u_ctx == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&read_selected_mods, 0, sizeof(nxs_string_t), 1);

	nxs_string_init(&setting_path);
	nxs_string_init(&configmk_path);
	nxs_string_init(&proj_objs_dst);
	nxs_string_init(&nxs_fw_version);
	nxs_string_init(&nxs_core_desc_path);
	nxs_string_init(&name);

	nxs_string_printf_dyn(&setting_path, "%r.nxs-fw-settings/settings.json", &u_ctx->root);
	nxs_string_printf_dyn(&configmk_path, "%rconfig.mk", &u_ctx->root);
	nxs_string_printf_dyn(&proj_objs_dst, "%r/objs/", &u_ctx->root);

	c_mods = nxs_fw_ctl_u_projects_core_modules_init();

	/* Чтение версии nxs-fw и текущего списка core-модулей, используемых данным проектом */
	if(nxs_fw_ctl_u_projects_sttngs_read(&setting_path, &name, &nxs_fw_version, &read_selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_string_printf_dyn(
	        &nxs_core_desc_path, "%r/%r/%s", &nxs_fw_ctl_cfg.nxs_fw_desc_path, &nxs_fw_version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);

	if(nxs_fw_ctl_u_projects_core_modules_read(c_mods, &nxs_core_desc_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_fw_ctl_u_projects_core_modules_select(c_mods, &read_selected_mods);

	selected_mods = nxs_fw_ctl_u_projects_core_modules_selected_get_mods(c_mods);
	use_flags     = nxs_fw_ctl_u_projects_core_modules_using_get_use_flags(c_mods);
	link_opts     = nxs_fw_ctl_u_projects_core_modules_using_get_link_opts(c_mods);
	includes      = nxs_fw_ctl_u_projects_core_modules_using_get_includes(c_mods);
	core_links    = nxs_fw_ctl_u_projects_core_modules_using_get_core_links(c_mods);

	if(nxs_fw_ctl_u_projects_sttngs_write(&setting_path, &u_ctx->name, &nxs_fw_version, selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_config_mk(&configmk_path, &nxs_fw_version, use_flags, includes, link_opts, core_links) !=
	   NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&proj_objs_dst) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&setting_path);
	nxs_string_free(&configmk_path);
	nxs_string_free(&proj_objs_dst);
	nxs_string_free(&nxs_fw_version);
	nxs_string_free(&nxs_core_desc_path);
	nxs_string_free(&name);

	for(i = 0; i < nxs_array_count(&read_selected_mods); i++) {

		s = nxs_array_get(&read_selected_mods, i);

		nxs_string_free(s);
	}

	nxs_array_free(&read_selected_mods);

	c_mods = nxs_fw_ctl_u_projects_core_modules_free(c_mods);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_change_core_version(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_fw_ctl_u_projects_core_modules_t *c_mods;
	nxs_array_t *                         selected_mods, read_selected_mods, versions;
	nxs_string_t setting_path, configmk_path, name, proj_objs_dst, nxs_fw_version, nxs_fw_version_cur, nxs_core_desc_path, *s,
	        *use_flags, *link_opts, *includes, *core_links;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	if(u_ctx == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&read_selected_mods, 0, sizeof(nxs_string_t), 1);

	nxs_array_init(&versions, 0, sizeof(nxs_string_t), 1);

	nxs_string_init(&setting_path);
	nxs_string_init(&configmk_path);
	nxs_string_init(&proj_objs_dst);
	nxs_string_init(&nxs_fw_version);
	nxs_string_init(&nxs_fw_version_cur);
	nxs_string_init(&nxs_core_desc_path);
	nxs_string_init(&name);

	nxs_string_printf_dyn(&setting_path, "%r.nxs-fw-settings/settings.json", &u_ctx->root);
	nxs_string_printf_dyn(&configmk_path, "%rconfig.mk", &u_ctx->root);
	nxs_string_printf_dyn(&proj_objs_dst, "%r/objs/", &u_ctx->root);

	c_mods = nxs_fw_ctl_u_projects_core_modules_init();

	/* Чтение текущего списка core-модулей, используемых данным проектом */
	if(nxs_fw_ctl_u_projects_sttngs_read(&setting_path, &name, &nxs_fw_version_cur, &read_selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_version_available(&nxs_fw_ctl_cfg.nxs_fw_desc_path, &versions) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\n  choose nxs-fw version (current: %s):", nxs_string_str(&nxs_fw_version_cur));

	nxs_fw_ctl_u_projects_version_select(&nxs_fw_version, &versions);

	nxs_string_printf_dyn(
	        &nxs_core_desc_path, "%r/%r/%s", &nxs_fw_ctl_cfg.nxs_fw_desc_path, &nxs_fw_version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);

	if(nxs_fw_ctl_u_projects_core_modules_read(c_mods, &nxs_core_desc_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_fw_ctl_u_projects_core_modules_selected_add(c_mods, &read_selected_mods);

	selected_mods = nxs_fw_ctl_u_projects_core_modules_selected_get_mods(c_mods);
	use_flags     = nxs_fw_ctl_u_projects_core_modules_using_get_use_flags(c_mods);
	link_opts     = nxs_fw_ctl_u_projects_core_modules_using_get_link_opts(c_mods);
	includes      = nxs_fw_ctl_u_projects_core_modules_using_get_includes(c_mods);
	core_links    = nxs_fw_ctl_u_projects_core_modules_using_get_core_links(c_mods);

	if(nxs_fw_ctl_u_projects_sttngs_write(&setting_path, &u_ctx->name, &nxs_fw_version, selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_config_mk(&configmk_path, &nxs_fw_version, use_flags, includes, link_opts, core_links) !=
	   NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&proj_objs_dst) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&setting_path);
	nxs_string_free(&configmk_path);
	nxs_string_free(&proj_objs_dst);
	nxs_string_free(&nxs_fw_version);
	nxs_string_free(&nxs_fw_version_cur);
	nxs_string_free(&nxs_core_desc_path);
	nxs_string_free(&name);

	for(i = 0; i < nxs_array_count(&read_selected_mods); i++) {

		s = nxs_array_get(&read_selected_mods, i);

		nxs_string_free(s);
	}

	for(i = 0; i < nxs_array_count(&versions); i++) {

		s = nxs_array_get(&versions, i);

		nxs_string_free(s);
	}

	nxs_array_free(&versions);
	nxs_array_free(&read_selected_mods);

	c_mods = nxs_fw_ctl_u_projects_core_modules_free(c_mods);

	return rc;
}

/*
 * Получение имени проекта, расположенного по заданному пути 'proj_path'.
 * При этом создания контекста проекта не происходит.
 * Функция только считывает имя проекта из файла настроек.
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_settingsfile_get_name(nxs_string_t *proj_path, nxs_string_t *name)
{

	if(proj_path == NULL || name == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	return nxs_fw_ctl_u_projects_settingsfile_get(proj_path, name, NULL, NULL);
}

/*
 * Получение версии nxs-fw, используемой для проекта, расположенного по заданному пути 'proj_path'.
 * При этом создания контекста проекта не происходит.
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_settingsfile_get_version(nxs_string_t *proj_path, nxs_string_t *name, nxs_string_t *version)
{

	if(proj_path == NULL || name == NULL || version == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	return nxs_fw_ctl_u_projects_settingsfile_get(proj_path, name, version, NULL);
}

/*
 * Получение списка модулей nxs-fw, используемых для проекта, расположенного по заданному пути 'proj_path' (в т.ч. модули, которые
 * не выбраны пользователем, но используются по зависимостям).
 * При этом создания контекста проекта не происходит.
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_settingsfile_get_mods(nxs_string_t *proj_path, nxs_string_t *name, nxs_array_t *proj_selected_mods)
{
	nxs_fw_ctl_u_projects_core_modules_t *c_mods;
	nxs_array_t                           mods_tmp, *using_mods;
	nxs_string_t                          version, *s, *s_out, nxs_core_desc_path;
	nxs_fw_ctl_err_t                      rc;
	size_t                                i;

	if(proj_path == NULL || name == NULL || proj_selected_mods == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	c_mods = nxs_fw_ctl_u_projects_core_modules_init();

	nxs_array_init(&mods_tmp, 0, sizeof(nxs_string_t), 1);

	nxs_string_init(&version);
	nxs_string_init(&nxs_core_desc_path);

	if(nxs_fw_ctl_u_projects_settingsfile_get(proj_path, name, &version, &mods_tmp) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_string_printf_dyn(
	        &nxs_core_desc_path, "%r/%r/%s", &nxs_fw_ctl_cfg.nxs_fw_desc_path, &version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);

	if(nxs_fw_ctl_u_projects_core_modules_read(c_mods, &nxs_core_desc_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_fw_ctl_u_projects_core_modules_selected_add(c_mods, &mods_tmp);

	using_mods = nxs_fw_ctl_u_projects_core_modules_using_get_mods(c_mods);

	for(i = 0; i < nxs_array_count(using_mods); i++) {

		s_out = nxs_array_get(using_mods, i);

		s = nxs_array_add(proj_selected_mods);

		nxs_string_init3(s, s_out);
	}

error:

	nxs_string_free(&version);
	nxs_string_free(&nxs_core_desc_path);

	for(i = 0; i < nxs_array_count(&mods_tmp); i++) {

		s = nxs_array_get(&mods_tmp, i);

		nxs_string_free(s);
	}

	nxs_array_free(&mods_tmp);

	c_mods = nxs_fw_ctl_u_projects_core_modules_free(c_mods);

	return rc;
}

/* Module internal (static) functions */

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_fs_struct(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_string_t     path;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&path);

	for(i = 0; nxs_string_str(&init_dirs[i]) != NULL; i++) {

		nxs_string_printf_dyn(&path, (char *)nxs_string_str(&init_dirs[i]), &u_ctx->root);

		if(nxs_fs_mkdir(&path, NXS_FW_CTL_DIR_MODE_DEF) < 0) {

			if(errno != EEXIST) {

				nxs_log_write_error(&process,
				                    "can't create project fs structure: %s (creating dir: %s)",
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

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_makefiles(nxs_fw_ctl_u_projects_t *u_ctx,
                                                             nxs_string_t *           fw_version,
                                                             nxs_string_t *           use_flags,
                                                             nxs_string_t *           link_opts,
                                                             nxs_string_t *           includes,
                                                             nxs_string_t *           core_links)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_name, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_upcase_name, &u_ctx->upcase_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_use_modules, use_flags);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_links, link_opts);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_core_links, core_links);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_includes, includes);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_fw_version, fw_version);

	for(i = 0; nxs_string_len(&makefiles[i].src) > 0; i++) {

		nxs_string_printf_dyn(&tpl_path, "%r/" NXS_FW_CTL_DIR_NEW_PROJ_TPL "/%r", &nxs_fw_ctl_cfg.tpls_path, &makefiles[i].src);
		nxs_string_printf_dyn(&dst_path, "%r%r", &u_ctx->root, &makefiles[i].dst);

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

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_genfiles(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_string_t     tpl_path, dst_path;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_name, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_upcase_name, &u_ctx->upcase_name);

	for(i = 0; nxs_string_len(&genfiles[i].src) > 0; i++) {

		nxs_string_printf_dyn(&tpl_path, "%r/" NXS_FW_CTL_DIR_NEW_PROJ_TPL "/%r", &nxs_fw_ctl_cfg.tpls_path, &genfiles[i].src);
		nxs_string_printf_dyn(&dst_path, "%r%r", &u_ctx->root, &genfiles[i].dst);

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

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_up_config_mk(nxs_string_t *path,
                                                           nxs_string_t *fw_version,
                                                           nxs_string_t *use_modules,
                                                           nxs_string_t *includes,
                                                           nxs_string_t *link_opts,
                                                           nxs_string_t *core_links)
{
	nxs_string_t     nxs_use_modules, nxs_includes, nxs_links, nxs_core_links, nxs_fw_version;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&nxs_use_modules);
	nxs_string_init(&nxs_includes);
	nxs_string_init(&nxs_links);
	nxs_string_init(&nxs_core_links);
	nxs_string_init(&nxs_fw_version);

	nxs_string_printf_dyn(&nxs_use_modules, "%r\t\t\t\t=\t%r\n", &_s_use_nxs_modules, use_modules);
	nxs_string_printf_dyn(&nxs_includes, "%r\t\t\t\t=\t%r\n", &_s_nxs_includes, includes);
	nxs_string_printf_dyn(&nxs_links, "%r\t\t\t\t\t=\t%r\n", &_s_nxs_links, link_opts);
	nxs_string_printf_dyn(&nxs_core_links, "%r\t\t\t\t=\t%r\n", &_s_nxs_core_links, core_links);
	nxs_string_printf_dyn(&nxs_fw_version, "%r\t\t\t\t=\t%r\n", &_s_nxs_fw_version, fw_version);

	/* NXS_FW_VERSION */
	if(nxs_fw_ctl_c_subs_includes(path, NXS_YES, &nxs_fw_version, &_s_nxs_fw_version, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* USE_NXS_MODULES */
	if(nxs_fw_ctl_c_subs_includes(path, NXS_YES, &nxs_use_modules, &_s_use_nxs_modules, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* NXS_INCLUDES */
	if(nxs_fw_ctl_c_subs_includes(path, NXS_YES, &nxs_includes, &_s_nxs_includes, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* NXS_LINKS */
	if(nxs_fw_ctl_c_subs_includes(path, NXS_YES, &nxs_links, &_s_nxs_links, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* NXS_CORE_LINKS */
	if(nxs_fw_ctl_c_subs_includes(path, NXS_YES, &nxs_core_links, &_s_nxs_core_links, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&nxs_use_modules);
	nxs_string_free(&nxs_includes);
	nxs_string_free(&nxs_links);
	nxs_string_free(&nxs_core_links);
	nxs_string_free(&nxs_fw_version);

	return rc;
}

/*
 * Получение данных по настройкам проекта
 */
static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_settingsfile_get(nxs_string_t *proj_path,
                                                               nxs_string_t *name,
                                                               nxs_string_t *version,
                                                               nxs_array_t * proj_selected_mods)
{
	nxs_string_t     setting_path;
	nxs_fw_ctl_err_t rc;

	if(proj_path == NULL || name == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	if(nxs_string_len(proj_path) == 0) {

		nxs_log_write_error(&process, "zero project path string len");

		return NXS_FW_CTL_E_ERR;
	}

	nxs_string_init3(&setting_path, proj_path);

	if(nxs_string_get_char(&setting_path, nxs_string_len(&setting_path) - 1) != (u_char)'/') {

		nxs_string_char_add_char_dyn(&setting_path, (u_char)'/');
	}

	nxs_string_printf2_cat_dyn(&setting_path, ".nxs-fw-settings/settings.json");

	rc = nxs_fw_ctl_u_projects_sttngs_read(&setting_path, name, version, proj_selected_mods);

	nxs_string_free(&setting_path);

	return rc;
}
