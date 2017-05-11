#include <nxs-core/nxs-core.h>

// clang-format off

/* Core module system includes */



/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-conf.h>
#include <nxs-fw-ctl-proc.h>

/* Core module defaults */



/* Core module static functions */

// clang-format on

// clang-format off

/* Core module global variables */

nxs_process_t process;

/*
 * Core module config file settings
 */

nxs_fw_ctl_cfg_t nxs_fw_ctl_cfg;

/*
 *
 * Core module functions
 *
 */

// clang-format on

int main(int argc, char *argv[])
{
	int rc;

	nxs_proc_init(&process, (u_char *)argv[0]);
	nxs_proc_setlog(&process, NXS_LOG_LEVEL_ALL, NXS_LOG_MODE_KEEP_OPEN, NXS_LOG_SYSLOG_DONT_USE, NULL);

	switch(nxs_fw_ctl_conf_runtime(argc, argv)) {

		case NXS_FW_CTL_E_OK:

			break;

		case NXS_FW_CTL_E_EXIT:

			exit(EXIT_SUCCESS);

		case NXS_FW_CTL_E_ERR:
		default:

			exit(EXIT_FAILURE);
	}

	nxs_proc_setlog(&process, nxs_fw_ctl_cfg.log_level, NXS_LOG_MODE_KEEP_OPEN, NXS_LOG_SYSLOG_DONT_USE, &nxs_fw_ctl_cfg.log_path);

	rc = nxs_fw_ctl_p_bootstrap();

	exit(rc);
}
