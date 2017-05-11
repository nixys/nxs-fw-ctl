#ifdef USE_NXS_JSON

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-conf.h>
#include <nxs-fw-ctl-dal.h>

/* Definitions */

/*
 * Possible values:
 * 0 - no config
 * 1 - ini-config
 * 2 - json-config
 */
#define NXS_NXS_FW_CTL_CFG_TYPE				2

/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */



/* Module declarations */



/* Module internal (static) functions prototypes */

// clang-format on

static void nxs_nxs_fw_ctl_conf_cfg_ctx_init(nxs_fw_ctl_cfg_ctx_t *cfg_ctx);

// clang-format off

/* Module initializations */



/* Module global functions */

// clang-format on

nxs_fw_ctl_err_t nxs_fw_ctl_conf_runtime(int argc, char *argv[])
{
	nxs_fw_ctl_cfg_ctx_t cfg_ctx;
	nxs_fw_ctl_err_t     rc;
	nxs_args_t           args;

#if NXS_NXS_FW_CTL_CFG_TYPE & 1
	nxs_cfg_t cfg;
#endif

	nxs_nxs_fw_ctl_conf_cfg_ctx_init(&cfg_ctx);

	/* Command line args */

	nxs_fw_ctl_conf_args_init(&args, &cfg_ctx);

	if((rc = nxs_fw_ctl_conf_args_runtime(args, argc, (u_char **)argv)) != NXS_FW_CTL_E_OK) {

		return rc;
	}

/* Config in ini-format */

#if NXS_NXS_FW_CTL_CFG_TYPE & 1
	nxs_fw_ctl_conf_file_ini_init(&cfg, &cfg_ctx);

	if(nxs_fw_ctl_conf_file_ini_runtime(cfg, &cfg_ctx) != NXS_FW_CTL_E_OK) {

		return NXS_FW_CTL_E_ERR;
	}
#endif

/* Config in json-format */

#if NXS_NXS_FW_CTL_CFG_TYPE & 2
	nxs_fw_ctl_conf_file_json_init(&cfg_ctx);

	if(nxs_fw_ctl_conf_file_json_runtime(&cfg_ctx) != NXS_FW_CTL_E_OK) {

		return NXS_FW_CTL_E_ERR;
	}
#endif

	return NXS_FW_CTL_E_OK;
}

/* Module internal (static) functions */

static void nxs_nxs_fw_ctl_conf_cfg_ctx_init(nxs_fw_ctl_cfg_ctx_t *cfg_ctx)
{

	/* Init config values */

	nxs_string_init(&nxs_fw_ctl_cfg.log_path);

	nxs_fw_ctl_cfg.log_level = NXS_LOG_LEVEL_MEM | NXS_LOG_LEVEL_INFO | NXS_LOG_LEVEL_WARN | NXS_LOG_LEVEL_ERROR;

	nxs_fw_ctl_cfg.runtime_type = NXS_FW_CTL_CFG_RUNTIME_NONE;

	nxs_string_init(&nxs_fw_ctl_cfg.proj_root);
	nxs_string_init(&nxs_fw_ctl_cfg.tpls_path);

	nxs_string_init(&nxs_fw_ctl_cfg.nxs_fw_desc_path);

	/* Init cfg context values */

	nxs_string_init2(&cfg_ctx->cfg_path, 0, (u_char *)NXS_FW_CTL_CONF_PATH_DEFAULT);
}

#endif /* USE_NXS_JSON */
