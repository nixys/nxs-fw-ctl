#ifndef _INCLUDE_NXS_FW_CTL_CONF_ARGS_H
#define _INCLUDE_NXS_FW_CTL_CONF_ARGS_H

// clang-format off

void					nxs_fw_ctl_conf_args_init			(nxs_args_t *args, void *user_ctx);
nxs_fw_ctl_err_t			nxs_fw_ctl_conf_args_runtime			(nxs_args_t args, int argc, u_char **argv);

#endif /* _INCLUDE_NXS_FW_CTL_CONF_ARGS_H */
