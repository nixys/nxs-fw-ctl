#include <nxs-core/nxs-core.h>

// clang-format off

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-units.h>

#include <proc/main/main.h>
#include <proc/main/ctx/main-ctx.h>
#include <proc/main/main-subproc.h>

/* Definitions */



/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */



/* Module declarations */



/* Module internal (static) functions prototypes */

// clang-format on

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_project_add(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_project_del(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_unit_add(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_unit_del(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_collection_add(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_collection_del(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_meta_add(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_meta_del(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_proc_add(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_proc_del(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_conf_add(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_conf_del(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_dal_add(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_dal_del(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_list_change(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_list_info(void);

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_version_change(void);
static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_version_info(void);

// clang-format off

/* Module initializations */



/* Module global functions */

// clang-format on

nxs_fw_ctl_err_t nxs_fw_ctl_p_main_runtime(void)
{
	nxs_fw_ctl_err_t rc;

	switch(nxs_fw_ctl_cfg.runtime_type) {

		case NXS_FW_CTL_CFG_RUNTIME_PROJECT_ADD:

			rc = nxs_fw_ctl_p_main_project_add();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_PROJECT_DEL:

			rc = nxs_fw_ctl_p_main_project_del();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_UNIT_ADD:

			rc = nxs_fw_ctl_p_main_unit_add();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_UNIT_DEL:

			rc = nxs_fw_ctl_p_main_unit_del();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_COLLECTION_ADD:

			rc = nxs_fw_ctl_p_main_collection_add();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_COLLECTION_DEL:

			rc = nxs_fw_ctl_p_main_collection_del();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_META_ADD:

			rc = nxs_fw_ctl_p_main_meta_add();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_META_DEL:

			rc = nxs_fw_ctl_p_main_meta_del();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_PROC_ADD:

			rc = nxs_fw_ctl_p_main_proc_add();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_PROC_DEL:

			rc = nxs_fw_ctl_p_main_proc_del();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_CONF_ADD:

			rc = nxs_fw_ctl_p_main_conf_add();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_CONF_DEL:

			rc = nxs_fw_ctl_p_main_conf_del();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_DAL_ADD:

			rc = nxs_fw_ctl_p_main_dal_add();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_DAL_DEL:

			rc = nxs_fw_ctl_p_main_dal_del();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_CORE_LIST_CHANGE:

			rc = nxs_fw_ctl_p_main_core_list_change();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_CORE_LIST_INFO:

			rc = nxs_fw_ctl_p_main_core_list_info();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_CORE_VERSION_CHANGE:

			rc = nxs_fw_ctl_p_main_core_version_change();

			break;

		case NXS_FW_CTL_CFG_RUNTIME_CORE_VERSION_INFO:

			rc = nxs_fw_ctl_p_main_core_version_info();

			break;

		default:

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

	return rc;
}

/* Module internal (static) functions */

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_project_add(void)
{
	nxs_fw_ctl_u_projects_t *proj_ctx;
	nxs_string_t *           name, pwd;
	nxs_fw_ctl_err_t         rc;

	rc = NXS_FW_CTL_E_OK;

	proj_ctx = nxs_fw_ctl_u_projects_init();

	nxs_string_init(&pwd);

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "project name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_string_printf_dyn(&pwd, "%r%r/", &nxs_fw_ctl_cfg.proj_root, name);

	if(nxs_fw_ctl_u_projects_setup(proj_ctx, &pwd, name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_add(proj_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "project '%s' successfully created", nxs_string_str(name));

error:

	nxs_string_free(&pwd);

	proj_ctx = nxs_fw_ctl_u_projects_free(proj_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_project_del(void)
{
	nxs_fw_ctl_u_projects_t *proj_ctx;
	nxs_string_t             pwd, p_name;
	nxs_fw_ctl_err_t         rc;

	rc = NXS_FW_CTL_E_OK;

	proj_ctx = nxs_fw_ctl_u_projects_init();

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_setup(proj_ctx, &pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	switch(nxs_fw_ctl_u_projects_del(proj_ctx)) {

		case NXS_FW_CTL_E_OK:

			nxs_log_write_console(&process, "project '%s' successfully deleted", nxs_string_str(&p_name));

			break;

		case NXS_FW_CTL_E_CANCEL:

			nxs_log_write_console(&process, "project '%s' delete canceled", nxs_string_str(&p_name));

			break;

		default:

			nxs_log_write_console(&process, "project '%s' delete error", nxs_string_str(&p_name));

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	proj_ctx = nxs_fw_ctl_u_projects_free(proj_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_unit_add(void)
{
	nxs_fw_ctl_u_units_t *unit_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	unit_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "unit name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	unit_ctx = nxs_fw_ctl_u_units_init();

	if(nxs_fw_ctl_u_units_setup(unit_ctx, &p_name, &pwd, name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_units_add(unit_ctx) != NXS_FW_CTL_E_OK) {

		nxs_log_write_console(&process, "unit '%s' successfully error", nxs_string_str(name));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "unit '%s' successfully created", nxs_string_str(name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	unit_ctx = nxs_fw_ctl_u_units_free(unit_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_unit_del(void)
{
	nxs_fw_ctl_u_units_t *unit_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	unit_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "unit name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	unit_ctx = nxs_fw_ctl_u_units_init();

	if(nxs_fw_ctl_u_units_setup(unit_ctx, &p_name, &pwd, name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	switch(nxs_fw_ctl_u_units_del(unit_ctx)) {

		case NXS_FW_CTL_E_OK:

			nxs_log_write_console(&process, "unit '%s' successfully deleted", nxs_string_str(name));

			break;

		case NXS_FW_CTL_E_CANCEL:

			nxs_log_write_console(&process, "unit '%s' delete canceled", nxs_string_str(name));

			break;

		default:

			nxs_log_write_console(&process, "unit '%s' delete error", nxs_string_str(name));

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	unit_ctx = nxs_fw_ctl_u_units_free(unit_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_collection_add(void)
{
	nxs_fw_ctl_u_collections_t *coll_ctx;
	nxs_string_t *              name, p_name, pwd;
	nxs_fw_ctl_err_t            rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	coll_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "collection name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	coll_ctx = nxs_fw_ctl_u_collections_init();

	nxs_fw_ctl_u_collections_setup(coll_ctx, &p_name, &pwd, name);

	if(nxs_fw_ctl_u_collections_add(coll_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "collection '%s' successfully created", nxs_string_str(name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	coll_ctx = nxs_fw_ctl_u_collections_free(coll_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_collection_del(void)
{
	nxs_fw_ctl_u_collections_t *coll_ctx;
	nxs_string_t *              name, p_name, pwd;
	nxs_fw_ctl_err_t            rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	coll_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "collection name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	coll_ctx = nxs_fw_ctl_u_collections_init();

	nxs_fw_ctl_u_collections_setup(coll_ctx, &p_name, &pwd, name);

	switch(nxs_fw_ctl_u_collections_del(coll_ctx)) {

		case NXS_FW_CTL_E_OK:

			nxs_log_write_console(&process, "collection '%s' successfully deleted", nxs_string_str(name));

			break;

		case NXS_FW_CTL_E_CANCEL:

			nxs_log_write_console(&process, "collection '%s' delete canceled", nxs_string_str(name));

			break;

		default:

			nxs_log_write_console(&process, "collection '%s' delete error", nxs_string_str(name));

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	coll_ctx = nxs_fw_ctl_u_collections_free(coll_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_meta_add(void)
{
	nxs_fw_ctl_u_metas_t *meta_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	meta_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "meta name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	meta_ctx = nxs_fw_ctl_u_metas_init();

	nxs_fw_ctl_u_metas_setup(meta_ctx, &p_name, &pwd, name);

	if(nxs_fw_ctl_u_metas_add(meta_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "meta '%s' successfully created", nxs_string_str(name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	meta_ctx = nxs_fw_ctl_u_metas_free(meta_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_meta_del(void)
{
	nxs_fw_ctl_u_metas_t *meta_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	meta_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "meta name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	meta_ctx = nxs_fw_ctl_u_metas_init();

	nxs_fw_ctl_u_metas_setup(meta_ctx, &p_name, &pwd, name);

	switch(nxs_fw_ctl_u_metas_del(meta_ctx)) {

		case NXS_FW_CTL_E_OK:

			nxs_log_write_console(&process, "meta '%s' successfully deleted", nxs_string_str(name));

			break;

		case NXS_FW_CTL_E_CANCEL:

			nxs_log_write_console(&process, "meta '%s' delete canceled", nxs_string_str(name));

			break;

		default:

			nxs_log_write_console(&process, "meta '%s' delete error", nxs_string_str(name));

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	meta_ctx = nxs_fw_ctl_u_metas_free(meta_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_proc_add(void)
{
	nxs_fw_ctl_u_procs_t *proc_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	proc_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "proc name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	proc_ctx = nxs_fw_ctl_u_procs_init();

	if(nxs_fw_ctl_u_procs_setup(proc_ctx, &p_name, &pwd, name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_procs_add(proc_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "process '%s' successfully created", nxs_string_str(name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	proc_ctx = nxs_fw_ctl_u_procs_free(proc_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_proc_del(void)
{
	nxs_fw_ctl_u_procs_t *proc_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	proc_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "proc name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	proc_ctx = nxs_fw_ctl_u_procs_init();

	if(nxs_fw_ctl_u_procs_setup(proc_ctx, &p_name, &pwd, name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	switch(nxs_fw_ctl_u_procs_del(proc_ctx)) {

		case NXS_FW_CTL_E_OK:

			nxs_log_write_console(&process, "process '%s' successfully deleted", nxs_string_str(name));

			break;

		case NXS_FW_CTL_E_CANCEL:

			nxs_log_write_console(&process, "process '%s' delete canceled", nxs_string_str(name));

			break;

		default:

			nxs_log_write_console(&process, "process '%s' delete error", nxs_string_str(name));

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	proc_ctx = nxs_fw_ctl_u_procs_free(proc_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_conf_add(void)
{
	nxs_fw_ctl_u_confs_t *conf_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	conf_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "conf name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	conf_ctx = nxs_fw_ctl_u_confs_init();

	nxs_fw_ctl_u_confs_setup(conf_ctx, &p_name, &pwd, name);

	if(nxs_fw_ctl_u_confs_add(conf_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "conf '%s' successfully created", nxs_string_str(name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	conf_ctx = nxs_fw_ctl_u_confs_free(conf_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_conf_del(void)
{
	nxs_fw_ctl_u_confs_t *conf_ctx;
	nxs_string_t *        name, p_name, pwd;
	nxs_fw_ctl_err_t      rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	conf_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "conf name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	conf_ctx = nxs_fw_ctl_u_confs_init();

	nxs_fw_ctl_u_confs_setup(conf_ctx, &p_name, &pwd, name);

	switch(nxs_fw_ctl_u_confs_del(conf_ctx)) {

		case NXS_FW_CTL_E_OK:

			nxs_log_write_console(&process, "conf '%s' successfully deleted", nxs_string_str(name));

			break;

		case NXS_FW_CTL_E_CANCEL:

			nxs_log_write_console(&process, "conf '%s' delete canceled", nxs_string_str(name));

			break;

		default:

			nxs_log_write_console(&process, "conf '%s' delete error", nxs_string_str(name));

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	conf_ctx = nxs_fw_ctl_u_confs_free(conf_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_dal_add(void)
{
	nxs_fw_ctl_u_dals_t *dal_ctx;
	nxs_string_t *       name, p_name, pwd;
	nxs_fw_ctl_err_t     rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	dal_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "dal name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	dal_ctx = nxs_fw_ctl_u_dals_init();

	if(nxs_fw_ctl_u_dals_setup(dal_ctx, &p_name, &pwd, name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_dals_add(dal_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "dal '%s' successfully created", nxs_string_str(name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	dal_ctx = nxs_fw_ctl_u_dals_free(dal_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_dal_del(void)
{
	nxs_fw_ctl_u_dals_t *dal_ctx;
	nxs_string_t *       name, p_name, pwd;
	nxs_fw_ctl_err_t     rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	dal_ctx = NULL;

	if((name = nxs_array_get(&nxs_fw_ctl_cfg.free_args, 0)) == NULL) {

		nxs_log_write_console(&process, "dal name does not set");

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	dal_ctx = nxs_fw_ctl_u_dals_init();

	if(nxs_fw_ctl_u_dals_setup(dal_ctx, &p_name, &pwd, name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	switch(nxs_fw_ctl_u_dals_del(dal_ctx)) {

		case NXS_FW_CTL_E_OK:

			nxs_log_write_console(&process, "dal '%s' successfully deleted", nxs_string_str(name));

			break;

		case NXS_FW_CTL_E_CANCEL:

			nxs_log_write_console(&process, "dal '%s' delete canceled", nxs_string_str(name));

			break;

		default:

			nxs_log_write_console(&process, "dal '%s' delete error", nxs_string_str(name));

			rc = NXS_FW_CTL_E_ERR;

			break;
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	dal_ctx = nxs_fw_ctl_u_dals_free(dal_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_list_change(void)
{
	nxs_fw_ctl_u_projects_t *proj_ctx;
	nxs_string_t             p_name, pwd;
	nxs_fw_ctl_err_t         rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	proj_ctx = NULL;

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	proj_ctx = nxs_fw_ctl_u_projects_init();

	if(nxs_fw_ctl_u_projects_setup(proj_ctx, &pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_change_core_mods(proj_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "nxs-fw core modules for project '%s' successfully changed", nxs_string_str(&p_name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	proj_ctx = nxs_fw_ctl_u_projects_free(proj_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_list_info(void)
{
	nxs_string_t     p_name, pwd, *s;
	nxs_array_t      mods;
	nxs_fw_ctl_err_t rc;
	size_t           i;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	nxs_array_init(&mods, 0, sizeof(nxs_string_t), 1);

	if(nxs_fw_ctl_u_projects_settingsfile_get_mods(&pwd, &p_name, &mods) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "nxs-fw core modules using for project '%s':", nxs_string_str(&p_name));

	for(i = 0; i < nxs_array_count(&mods); i++) {

		s = nxs_array_get(&mods, i);

		nxs_log_write_console(&process, "* %s", nxs_string_str(s));
	}

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	for(i = 0; i < nxs_array_count(&mods); i++) {

		s = nxs_array_get(&mods, i);

		nxs_string_free(s);
	}

	nxs_array_free(&mods);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_version_change(void)
{
	nxs_fw_ctl_u_projects_t *proj_ctx;
	nxs_string_t             p_name, pwd;
	nxs_fw_ctl_err_t         rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);

	nxs_fs_getcwd(&pwd);

	proj_ctx = NULL;

	if(nxs_fw_ctl_u_projects_settingsfile_get_name(&pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	proj_ctx = nxs_fw_ctl_u_projects_init();

	if(nxs_fw_ctl_u_projects_setup(proj_ctx, &pwd, &p_name) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	if(nxs_fw_ctl_u_projects_change_core_version(proj_ctx) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(&process, "nxs-fw core version for project '%s' successfully changed", nxs_string_str(&p_name));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);

	proj_ctx = nxs_fw_ctl_u_projects_free(proj_ctx);

	return rc;
}

static nxs_fw_ctl_err_t nxs_fw_ctl_p_main_core_version_info(void)
{
	nxs_string_t     p_name, pwd, core_version;
	nxs_fw_ctl_err_t rc;

	rc = NXS_FW_CTL_E_OK;

	nxs_string_init(&pwd);
	nxs_string_init(&p_name);
	nxs_string_init(&core_version);

	nxs_fs_getcwd(&pwd);

	if(nxs_fw_ctl_u_projects_settingsfile_get_version(&pwd, &p_name, &core_version) != NXS_FW_CTL_E_OK) {

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

	nxs_log_write_console(
	        &process, "nxs-fw version using for project '%s': %s", nxs_string_str(&p_name), nxs_string_str(&core_version));

error:

	nxs_string_free(&pwd);
	nxs_string_free(&p_name);
	nxs_string_free(&core_version);

	return rc;
}