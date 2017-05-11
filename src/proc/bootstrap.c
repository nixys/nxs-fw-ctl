#include <nxs-core/nxs-core.h>

// clang-format off

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-proc.h>

#include <proc/bootstrap-subproc.h>

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

nxs_fw_ctl_err_t nxs_fw_ctl_p_bootstrap(void)
{
	nxs_fw_ctl_err_t rc;

	/*
	if(nxs_fw_ctl_cfg.daemonized == NXS_YES){

	        nxs_proc_daemonize(&process, NXS_PROCESS_CHLD_SIG_CLEAR, NULL, NXS_PROCESS_NO_UID, NXS_PROCESS_NO_UMASK, NULL);
	}
	*/

	rc = nxs_fw_ctl_p_main_runtime();

	return rc;
}

/* Module internal (static) functions */
