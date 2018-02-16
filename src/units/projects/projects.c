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

#define mk_path_settings(dst_path, project_root, file_name) \
		nxs_string_printf(dst_path, "%r%r", project_root, file_name)

#define mk_path_core_desc(dst_path, nxs_fw_desc_path, nxs_fw_version, nxs_fw_mods_desc_file) \
		nxs_string_printf(dst_path, "%r/%r/%s", nxs_fw_desc_path, nxs_fw_version, nxs_fw_mods_desc_file)

#define mk_path_objs_dst(dst_path, project_root) \
		nxs_string_printf(dst_path, "%r/objs/", project_root)

#define mk_path_configmk(dst_path, project_root, config_mk_name) \
		nxs_string_printf(dst_path, "%r%r", project_root, config_mk_name)

#define mk_path_distribmk(dst_path, project_root, distrib_mk_name) \
		nxs_string_printf(dst_path, "%r%r", project_root, distrib_mk_name)

#define mk_path_fs_objects(dst_path, tpls_path, nxs_fw_version, fs_objects_name) \
		nxs_string_printf(dst_path, "%r/%r/%r", tpls_path, nxs_fw_version, fs_objects_name)

/* Project globals */
extern		nxs_process_t					process;
extern		nxs_fw_ctl_cfg_t				nxs_fw_ctl_cfg;

/* Module typedefs */



/* Module declarations */

struct nxs_fw_ctl_u_projects_s
{
	nxs_string_t						name;		/* Имя проекта */
	nxs_string_t						inline_name;	/* Имя проекта, которое будет использовано в коде программы */
	nxs_string_t						upcase_name;	/* Имя проекта, приведённое к заглавным буквам */
	nxs_string_t						root;		/* Корневая директория проекта */
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_fs_struct(nxs_fw_ctl_u_projects_t *           u_ctx,
                                                             nxs_fw_ctl_u_projects_fs_objects_t *fs_objects);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_makefiles(nxs_fw_ctl_u_projects_t *           u_ctx,
                                                             nxs_string_t *                      fw_version,
                                                             nxs_fw_ctl_u_projects_fs_objects_t *fs_objects);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_genfiles(nxs_fw_ctl_u_projects_t *           u_ctx,
                                                            nxs_string_t *                      fw_version,
                                                            nxs_fw_ctl_u_projects_fs_objects_t *fs_objects);

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_up_config_mk(nxs_string_t *                        project_root_path,
                                                           nxs_string_t *                        fw_version,
                                                           nxs_fw_ctl_u_projects_core_modules_t *c_mods);
static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_up_distrib_mk(nxs_string_t *project_root_path, nxs_fw_ctl_u_projects_core_modules_t *c_mods);

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_settingsfile_get(nxs_string_t *proj_path,
                                                               nxs_string_t *name,
                                                               nxs_string_t *version,
                                                               nxs_array_t * proj_selected_mods);

// clang-format off

/* Module initializations */

static nxs_string_t _s_tpl_p_name				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_P_NAME);
static nxs_string_t _s_tpl_p_inline_name			= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_P_INLINE_NAME);
static nxs_string_t _s_tpl_p_upcase_name			= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_P_UPCASE_NAME);
static nxs_string_t _s_tpl_p_fw_version				= nxs_string(NXS_FW_CTL_U_PROJECTS_TPL_FW_VERSION);

static nxs_string_t _s_use_nxs_modules				= nxs_string("USE_NXS_MODULES");
static nxs_string_t _s_nxs_includes				= nxs_string("NXS_INCLUDES");
static nxs_string_t _s_nxs_links				= nxs_string("NXS_LINKS");
static nxs_string_t _s_nxs_core_links				= nxs_string("NXS_CORE_LINKS");
static nxs_string_t _s_nxs_fw_version				= nxs_string("NXS_FW_VERSION");
static nxs_string_t _s_nxs_distrib_links			= nxs_string("NXS_DISTRIB_LINKS");
static nxs_string_t _s_nxs_distrib_includes			= nxs_string("NXS_DISTRIB_INCLUDES");
static nxs_string_t _s_nxs_distrib_links_var			= nxs_string("$(NXS_DISTRIB_LINKS_$(OS_DISTRIB)_$(OS_RELEASE))");
static nxs_string_t _s_nxs_distrib_includes_var			= nxs_string("$(NXS_DISTRIB_INCLUDES_$(OS_DISTRIB)_$(OS_RELEASE))");
static nxs_string_t _s_eol					= nxs_string("\n");

static nxs_string_t _s_nxs_fw_settings_name			= nxs_string(".nxs-fw-settings/settings.json");
static nxs_string_t _s_config_mk_name				= nxs_string("config.mk");
static nxs_string_t _s_distrib_mk_name				= nxs_string("distrib.mk");
static nxs_string_t _s_fs_objects_name				= nxs_string("fs-objects.json");

static nxs_string_t _s_question_project_del			= nxs_string("\tyou realy want to delete this project? [y/n]: ");

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

	nxs_string_cpy(&u_ctx->root, 0, proj_root, 0);

	if(nxs_string_get_char(proj_root, nxs_string_len(proj_root) - 1) != (u_char)'/') {

		nxs_string_char_add_char(proj_root, (u_char)'/');
	}

	return nxs_fw_ctl_c_set_names(proj_name, &u_ctx->name, &u_ctx->upcase_name, &u_ctx->inline_name);
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_add(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_fw_ctl_u_projects_core_modules_t *c_mods;
	nxs_fw_ctl_u_projects_fs_objects_t *  fs_objects;
	nxs_fw_ctl_err_t                      rc;
	nxs_string_t                          setting_path, nxs_core_desc_path, fs_objects_path, nxs_fw_version, *s;
	nxs_array_t *                         selected_mods, versions;
	size_t                                i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&setting_path);
	nxs_string_init(&nxs_core_desc_path);
	nxs_string_init(&fs_objects_path);
	nxs_string_init(&nxs_fw_version);

	mk_path_settings(&setting_path, &u_ctx->root, &_s_nxs_fw_settings_name);

	nxs_array_init2(&versions, nxs_string_t);

	c_mods     = nxs_fw_ctl_u_projects_core_modules_init();
	fs_objects = nxs_fw_ctl_u_projects_fs_objects_init();

	if(nxs_fw_ctl_u_projects_sttngs_check(&setting_path) == NXS_YES) {

		nxs_log_write_warn(&process, "can't create project: project exist (settings file: %r)", &setting_path);

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_version_available(&nxs_fw_ctl_cfg.nxs_fw_desc_path, &versions) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\n  project root: %r", &nxs_fw_ctl_cfg.proj_root);
	nxs_log_write_console(&process, "\n  choose nxs-fw version:");

	nxs_fw_ctl_u_projects_version_select(&nxs_fw_version, &versions);

	mk_path_core_desc(&nxs_core_desc_path, &nxs_fw_ctl_cfg.nxs_fw_desc_path, &nxs_fw_version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);
	mk_path_fs_objects(&fs_objects_path, &nxs_fw_ctl_cfg.tpls_path, &nxs_fw_version, &_s_fs_objects_name);

	if(nxs_fw_ctl_u_projects_core_modules_read(c_mods, &nxs_core_desc_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_fs_objects_read(fs_objects, &fs_objects_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_make_fs_struct(u_ctx, fs_objects) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\n  choose nxs-fw modules:");

	nxs_fw_ctl_u_projects_core_modules_select(c_mods, NULL);

	selected_mods = nxs_fw_ctl_u_projects_core_modules_selected_get_mods(c_mods);

	if(nxs_fw_ctl_u_projects_sttngs_write(&setting_path, &u_ctx->name, &nxs_fw_version, selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_make_makefiles(u_ctx, &nxs_fw_version, fs_objects) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_config_mk(&u_ctx->root, &nxs_fw_version, c_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_distrib_mk(&u_ctx->root, c_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_make_genfiles(u_ctx, &nxs_fw_version, fs_objects) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	c_mods     = nxs_fw_ctl_u_projects_core_modules_free(c_mods);
	fs_objects = nxs_fw_ctl_u_projects_fs_objects_free(fs_objects);

	for(i = 0; i < nxs_array_count(&versions); i++) {

		s = nxs_array_get(&versions, i);

		nxs_string_free(s);
	}

	nxs_array_free(&versions);

	nxs_string_free(&setting_path);
	nxs_string_free(&nxs_fw_version);
	nxs_string_free(&nxs_core_desc_path);
	nxs_string_free(&fs_objects_path);

	return rc;
}

nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_del(nxs_fw_ctl_u_projects_t *u_ctx)
{
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_log_write_console(&process, "\tproject '%r' will be deleted:", &u_ctx->name);

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
	nxs_string_t                          setting_path, name, proj_objs_dst, nxs_fw_version, nxs_core_desc_path, *s;
	nxs_fw_ctl_err_t                      rc;
	size_t                                i;

	if(u_ctx == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&read_selected_mods, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	nxs_string_init(&setting_path);
	nxs_string_init(&proj_objs_dst);
	nxs_string_init(&nxs_fw_version);
	nxs_string_init(&nxs_core_desc_path);
	nxs_string_init(&name);

	mk_path_settings(&setting_path, &u_ctx->root, &_s_nxs_fw_settings_name);
	mk_path_objs_dst(&proj_objs_dst, &u_ctx->root);

	c_mods = nxs_fw_ctl_u_projects_core_modules_init();

	/* Чтение версии nxs-fw и текущего списка core-модулей, используемых данным проектом */
	if(nxs_fw_ctl_u_projects_sttngs_read(&setting_path, &name, &nxs_fw_version, &read_selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	mk_path_core_desc(&nxs_core_desc_path, &nxs_fw_ctl_cfg.nxs_fw_desc_path, &nxs_fw_version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);

	if(nxs_fw_ctl_u_projects_core_modules_read(c_mods, &nxs_core_desc_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_fw_ctl_u_projects_core_modules_select(c_mods, &read_selected_mods);

	selected_mods = nxs_fw_ctl_u_projects_core_modules_selected_get_mods(c_mods);

	if(nxs_fw_ctl_u_projects_sttngs_write(&setting_path, &u_ctx->name, &nxs_fw_version, selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_config_mk(&u_ctx->root, &nxs_fw_version, c_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_distrib_mk(&u_ctx->root, c_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&proj_objs_dst) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&setting_path);
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
	nxs_string_t                          setting_path, name, proj_objs_dst, nxs_fw_version, nxs_fw_version_cur, nxs_core_desc_path, *s;
	nxs_fw_ctl_err_t                      rc;
	size_t                                i;

	if(u_ctx == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_array_init(&read_selected_mods, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	nxs_array_init(&versions, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	nxs_string_init(&setting_path);
	nxs_string_init(&proj_objs_dst);
	nxs_string_init(&nxs_fw_version);
	nxs_string_init(&nxs_fw_version_cur);
	nxs_string_init(&nxs_core_desc_path);
	nxs_string_init(&name);

	mk_path_settings(&setting_path, &u_ctx->root, &_s_nxs_fw_settings_name);
	mk_path_objs_dst(&proj_objs_dst, &u_ctx->root);

	c_mods = nxs_fw_ctl_u_projects_core_modules_init();

	/* Чтение текущего списка core-модулей, используемых данным проектом */
	if(nxs_fw_ctl_u_projects_sttngs_read(&setting_path, &name, &nxs_fw_version_cur, &read_selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_version_available(&nxs_fw_ctl_cfg.nxs_fw_desc_path, &versions) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "\n  choose nxs-fw version (current: %r):", &nxs_fw_version_cur);

	nxs_fw_ctl_u_projects_version_select(&nxs_fw_version, &versions);

	mk_path_core_desc(&nxs_core_desc_path, &nxs_fw_ctl_cfg.nxs_fw_desc_path, &nxs_fw_version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);

	if(nxs_fw_ctl_u_projects_core_modules_read(c_mods, &nxs_core_desc_path) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_fw_ctl_u_projects_core_modules_selected_add(c_mods, &read_selected_mods);

	selected_mods = nxs_fw_ctl_u_projects_core_modules_selected_get_mods(c_mods);

	if(nxs_fw_ctl_u_projects_sttngs_write(&setting_path, &u_ctx->name, &nxs_fw_version, selected_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_config_mk(&u_ctx->root, &nxs_fw_version, c_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_up_distrib_mk(&u_ctx->root, c_mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_c_dir_clean(&proj_objs_dst) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&setting_path);
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

	nxs_array_init(&mods_tmp, 0, sizeof(nxs_string_t), 1, NULL, NULL);

	nxs_string_init(&version);
	nxs_string_init(&nxs_core_desc_path);

	if(nxs_fw_ctl_u_projects_settingsfile_get(proj_path, name, &version, &mods_tmp) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	mk_path_core_desc(&nxs_core_desc_path, &nxs_fw_ctl_cfg.nxs_fw_desc_path, &version, NXS_FW_CTL_NXS_FW_MODS_DESC_PATH);

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

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_fs_struct(nxs_fw_ctl_u_projects_t *u_ctx, nxs_fw_ctl_u_projects_fs_objects_t *fs_objects)
{
	nxs_string_t     path, init_dir;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&path);
	nxs_string_init(&init_dir);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_name, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_upcase_name, &u_ctx->upcase_name);

	for(i = 0; nxs_fw_ctl_u_projects_fs_objects_get_init_dir(fs_objects, i, &init_dir) == NXS_FW_CTL_E_OK; i++) {

		nxs_string_printf(&path, "%r%r", &u_ctx->root, &init_dir);

		nxs_fw_ctl_c_copy_tpl_path(&subs, &path);

		if(nxs_fs_mkdir(&path, NXS_FW_CTL_DIR_MODE_DEF) < 0) {

			if(errno != EEXIST) {

				nxs_log_write_error(
				        &process, "can't create project fs structure: %s (creating dir: %r)", strerror(errno), &path);

				nxs_error(rc, NXS_FW_CTL_E_ERR, error);
			}
		}
	}

/*
for(i = 0; nxs_string_str(&init_dirs[i]) != NULL; i++) {

        nxs_string_printf(&path, "%r%r", &u_ctx->root, &init_dirs[i]);

        nxs_fw_ctl_c_copy_tpl_path(&subs, &path);

        if(nxs_fs_mkdir(&path, NXS_FW_CTL_DIR_MODE_DEF) < 0) {

                if(errno != EEXIST) {

                        nxs_log_write_error(
                                &process, "can't create project fs structure: %s (creating dir: %r)", strerror(errno), &path);

                        nxs_error(rc, NXS_FW_CTL_E_ERR, error);
                }
        }
}
*/

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&path);
	nxs_string_free(&init_dir);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_makefiles(nxs_fw_ctl_u_projects_t *           u_ctx,
                                                             nxs_string_t *                      fw_version,
                                                             nxs_fw_ctl_u_projects_fs_objects_t *fs_objects)
{
	nxs_string_t     tpl_path, dst_path, makefile_src, makefile_dst;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;
	mode_t           makefile_mode;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);
	nxs_string_init(&makefile_src);
	nxs_string_init(&makefile_dst);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_name, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_upcase_name, &u_ctx->upcase_name);

	for(i = 0;
	    nxs_fw_ctl_u_projects_fs_objects_get_makefile(fs_objects, i, &makefile_src, &makefile_dst, &makefile_mode) == NXS_FW_CTL_E_OK;
	    i++) {

		nxs_string_printf(
		        &tpl_path, "%r/%r/" NXS_FW_CTL_DIR_NEW_PROJ_TPL "/%r", &nxs_fw_ctl_cfg.tpls_path, fw_version, &makefile_src);
		nxs_string_printf(&dst_path, "%r%r", &u_ctx->root, &makefile_dst);

		if(nxs_fw_ctl_c_copy_tpl(&subs, &tpl_path, &dst_path, makefile_mode) != NXS_FW_CTL_E_OK) {

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&tpl_path);
	nxs_string_free(&dst_path);
	nxs_string_free(&makefile_src);
	nxs_string_free(&makefile_dst);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_make_genfiles(nxs_fw_ctl_u_projects_t *           u_ctx,
                                                            nxs_string_t *                      fw_version,
                                                            nxs_fw_ctl_u_projects_fs_objects_t *fs_objects)
{
	nxs_string_t     tpl_path, dst_path, genfiles_src, genfiles_dst;
	nxs_array_t      subs;
	nxs_fw_ctl_err_t rc;
	size_t           i;
	mode_t           genfiles_mode;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&tpl_path);
	nxs_string_init(&dst_path);
	nxs_string_init(&genfiles_src);
	nxs_string_init(&genfiles_dst);

	nxs_fw_ctl_c_copy_tpl_init(&subs);

	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_name, &u_ctx->name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_inline_name, &u_ctx->inline_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_upcase_name, &u_ctx->upcase_name);
	nxs_fw_ctl_c_copy_tpl_add(&subs, &_s_tpl_p_fw_version, fw_version);

	for(i = 0;
	    nxs_fw_ctl_u_projects_fs_objects_get_genfiles(fs_objects, i, &genfiles_src, &genfiles_dst, &genfiles_mode) == NXS_FW_CTL_E_OK;
	    i++) {

		nxs_string_printf(
		        &tpl_path, "%r/%r/" NXS_FW_CTL_DIR_NEW_PROJ_TPL "/%r", &nxs_fw_ctl_cfg.tpls_path, fw_version, &genfiles_src);
		nxs_string_printf(&dst_path, "%r%r", &u_ctx->root, &genfiles_dst);

		nxs_fw_ctl_c_copy_tpl_path(&subs, &dst_path);

		if(nxs_fw_ctl_c_copy_tpl(&subs, &tpl_path, &dst_path, genfiles_mode) != NXS_FW_CTL_E_OK) {

			nxs_error(rc, NXS_FW_CTL_E_ERR, error);
		}
	}

error:

	nxs_fw_ctl_c_copy_tpl_free(&subs);

	nxs_string_free(&tpl_path);
	nxs_string_free(&dst_path);
	nxs_string_free(&genfiles_src);
	nxs_string_free(&genfiles_dst);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_up_config_mk(nxs_string_t *                        project_root_path,
                                                           nxs_string_t *                        fw_version,
                                                           nxs_fw_ctl_u_projects_core_modules_t *c_mods)
{
	nxs_fw_ctl_err_t rc;
	nxs_string_t     configmk_path, nxs_use_modules, nxs_includes, nxs_links, nxs_core_links, nxs_fw_version, *use_flags, *link_opts,
	        *includes, *core_links;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&configmk_path);
	nxs_string_init(&nxs_use_modules);
	nxs_string_init(&nxs_includes);
	nxs_string_init(&nxs_links);
	nxs_string_init(&nxs_core_links);
	nxs_string_init(&nxs_fw_version);

	use_flags  = nxs_fw_ctl_u_projects_core_modules_using_get_use_flags(c_mods);
	link_opts  = nxs_fw_ctl_u_projects_core_modules_using_get_link_opts(c_mods);
	includes   = nxs_fw_ctl_u_projects_core_modules_using_get_includes(c_mods);
	core_links = nxs_fw_ctl_u_projects_core_modules_using_get_core_links(c_mods);

	mk_path_configmk(&configmk_path, project_root_path, &_s_config_mk_name);

	nxs_string_printf(&nxs_use_modules, "%r\t\t\t\t\t=\t%r\n", &_s_use_nxs_modules, use_flags);
	nxs_string_printf(&nxs_includes, "%r\t\t\t\t\t=\t%r %r\n", &_s_nxs_includes, includes, &_s_nxs_distrib_includes_var);
	nxs_string_printf(&nxs_links, "%r\t\t\t\t\t=\t%r %r\n", &_s_nxs_links, link_opts, &_s_nxs_distrib_links_var);
	nxs_string_printf(&nxs_core_links, "%r\t\t\t\t\t=\t%r\n", &_s_nxs_core_links, core_links);
	nxs_string_printf(&nxs_fw_version, "%r\t\t\t\t\t=\t%r\n", &_s_nxs_fw_version, fw_version);

	/* NXS_FW_VERSION */
	if(nxs_fw_ctl_c_subs_includes(&configmk_path, NXS_YES, &nxs_fw_version, &_s_nxs_fw_version, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* USE_NXS_MODULES */
	if(nxs_fw_ctl_c_subs_includes(&configmk_path, NXS_YES, &nxs_use_modules, &_s_use_nxs_modules, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* NXS_INCLUDES */
	if(nxs_fw_ctl_c_subs_includes(&configmk_path, NXS_YES, &nxs_includes, &_s_nxs_includes, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* NXS_LINKS */
	if(nxs_fw_ctl_c_subs_includes(&configmk_path, NXS_YES, &nxs_links, &_s_nxs_links, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	/* NXS_CORE_LINKS */
	if(nxs_fw_ctl_c_subs_includes(&configmk_path, NXS_YES, &nxs_core_links, &_s_nxs_core_links, &_s_eol) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&configmk_path);
	nxs_string_free(&nxs_use_modules);
	nxs_string_free(&nxs_includes);
	nxs_string_free(&nxs_links);
	nxs_string_free(&nxs_core_links);
	nxs_string_free(&nxs_fw_version);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_up_distrib_mk(nxs_string_t *project_root_path, nxs_fw_ctl_u_projects_core_modules_t *c_mods)
{
	nxs_fw_ctl_err_t rc;
	nxs_string_t     distribmk_path, distribmk_content, name, link_opts, includes;
	size_t           i;

	if(project_root_path == NULL || c_mods == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&distribmk_path);
	nxs_string_init(&distribmk_content);
	nxs_string_init(&name);
	nxs_string_init(&link_opts);
	nxs_string_init(&includes);

	mk_path_distribmk(&distribmk_path, project_root_path, &_s_distrib_mk_name);

	for(i = 0; nxs_fw_ctl_u_projects_core_modules_using_get_distrib(c_mods, i, &name, &link_opts, &includes) == NXS_FW_CTL_E_OK; i++) {

		nxs_string_printf2_cat(&distribmk_content, "%r_%r\t\t\t\t=\t%r\n", &_s_nxs_distrib_links, &name, &link_opts);
		nxs_string_printf2_cat(&distribmk_content, "%r_%r\t\t\t\t=\t%r\n\n", &_s_nxs_distrib_includes, &name, &includes);
	}

	if(nxs_fs_write_file(&distribmk_path, (nxs_buf_t *)&distribmk_content, 0644) < 0) {

		nxs_log_write_error(
		        &process, "can't create %r file: %s (file path: %r)", &_s_distrib_mk_name, strerror(errno), &distribmk_path);

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_string_free(&distribmk_path);
	nxs_string_free(&distribmk_content);
	nxs_string_free(&name);
	nxs_string_free(&link_opts);
	nxs_string_free(&includes);

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

		nxs_string_char_add_char(&setting_path, (u_char)'/');
	}

	nxs_string_printf2_cat(&setting_path, "%r", &_s_nxs_fw_settings_name);

	rc = nxs_fw_ctl_u_projects_sttngs_read(&setting_path, name, version, proj_selected_mods);

	nxs_string_free(&setting_path);

	return rc;
}
