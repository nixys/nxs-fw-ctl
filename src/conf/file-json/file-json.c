#include <nxs-core/nxs-core.h>

// clang-format off

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <conf/file-json/file-json.h>

/* Definitions */

/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */



/* Module declarations */



/* Module internal (static) functions prototypes */

// clang-format on

static nxs_cfg_json_state_t nxs_fw_ctl_conf_file_json_prep(nxs_cfg_json_t cfg);
static nxs_cfg_json_state_t nxs_fw_ctl_conf_file_json_post(nxs_cfg_json_t cfg);

static nxs_cfg_json_state_t
        nxs_fw_ctl_conf_file_json_projs_root(nxs_process_t *proc, nxs_json_t *json, nxs_cfg_json_par_t *cfg_json_par_el);

// clang-format off

/* Module initializations */

static nxs_string_t _s_par_loglevel			= nxs_string("loglevel");
static nxs_string_t _s_par_logpath			= nxs_string("logpath");
static nxs_string_t _s_par_proj_root			= nxs_string("proj_root");
static nxs_string_t _s_par_tpls_path			= nxs_string("tpls_path");
static nxs_string_t _s_par_nxs_fw_desc_path		= nxs_string("nxs_fw_desc_path");

/* Module global functions */

// clang-format on

void nxs_fw_ctl_conf_file_json_init(nxs_fw_ctl_cfg_ctx_t *cfg_ctx)
{
}

nxs_fw_ctl_err_t nxs_fw_ctl_conf_file_json_runtime(nxs_fw_ctl_cfg_ctx_t *cfg_ctx)
{
	nxs_cfg_json_t cfg_json;
	nxs_array_t    cfg_arr;

	nxs_cfg_json_conf_array_init(&cfg_arr);

	nxs_cfg_json_conf_array_add(
	        &cfg_arr, &_s_par_loglevel, &nxs_fw_ctl_cfg.log_level, NULL, NULL, NXS_CFG_JSON_TYPE_INT_32, 0, 63, NXS_NO, NULL);
	nxs_cfg_json_conf_array_add(
	        &cfg_arr, &_s_par_logpath, &nxs_fw_ctl_cfg.log_path, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_NO, NULL);
	nxs_cfg_json_conf_array_add(&cfg_arr,
	                            &_s_par_proj_root,
	                            &nxs_fw_ctl_cfg.proj_root,
	                            &nxs_fw_ctl_conf_file_json_projs_root,
	                            NULL,
	                            NXS_CFG_JSON_TYPE_VOID,
	                            0,
	                            0,
	                            NXS_YES,
	                            NULL);
	nxs_cfg_json_conf_array_add(
	        &cfg_arr, &_s_par_tpls_path, &nxs_fw_ctl_cfg.tpls_path, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_YES, NULL);
	nxs_cfg_json_conf_array_add(&cfg_arr,
	                            &_s_par_nxs_fw_desc_path,
	                            &nxs_fw_ctl_cfg.nxs_fw_desc_path,
	                            NULL,
	                            NULL,
	                            NXS_CFG_JSON_TYPE_STRING,
	                            0,
	                            0,
	                            NXS_YES,
	                            NULL);

	nxs_cfg_json_init(
	        &process, &cfg_json, &cfg_ctx->cfg_path, &nxs_fw_ctl_conf_file_json_prep, &nxs_fw_ctl_conf_file_json_post, &cfg_arr);

	if(nxs_cfg_json_read_file(&process, cfg_json) != NXS_CFG_JSON_CONF_OK) {

		nxs_log_write_raw(&process, "json config read error");

		return NXS_FW_CTL_E_ERR;
	}

	nxs_cfg_json_free(&cfg_json);

	nxs_cfg_json_conf_array_free(&cfg_arr);

	return NXS_FW_CTL_E_OK;
}

/* Module internal (static) functions */

/* Project config file prepare function */
static nxs_cfg_json_state_t nxs_fw_ctl_conf_file_json_prep(nxs_cfg_json_t cfg)
{

	/* init config values */

	return NXS_CFG_JSON_CONF_OK;
}

/* Project config file post function */

static nxs_cfg_json_state_t nxs_fw_ctl_conf_file_json_post(nxs_cfg_json_t cfg)
{

	/* check all defined config values and prepare program runtime (on error - NXS_CFG_CONF_ERROR value must be returned)*/

	if((u_char)nxs_string_get_char(&nxs_fw_ctl_cfg.proj_root, nxs_string_len(&nxs_fw_ctl_cfg.proj_root) - 1) != '/') {

		nxs_string_char_add_char(&nxs_fw_ctl_cfg.proj_root, (u_char)'/');
	}

	return NXS_CFG_JSON_CONF_OK;
}

/* Project config file auxiliaries functions */

/*
 * Для обработки этого параметра создаётся отдельный обработчик, т.к. он может быть определён в аргументах командной строки
 */
static nxs_cfg_json_state_t nxs_fw_ctl_conf_file_json_projs_root(nxs_process_t *proc, nxs_json_t *json, nxs_cfg_json_par_t *cfg_json_par_el)
{
	nxs_string_t *var = nxs_cfg_json_get_val(cfg_json_par_el);

	if(nxs_json_type_get(json) != NXS_JSON_TYPE_STRING) {

		nxs_log_write_error(proc,
		                    "json cfg process error: wrong type for value (option: \"%r\", type: %s, expected type: string)",
		                    nxs_json_get_key(json),
		                    nxs_json_type_string(json));

		return NXS_CFG_JSON_CONF_ERROR;
	}

	if(nxs_string_len(var) == 0) {

		nxs_string_cpy(var, 0, nxs_json_string_val(json), 0);
	}

	return NXS_CFG_JSON_CONF_OK;
}
