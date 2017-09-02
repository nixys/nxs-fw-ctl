#include <nxs-core/nxs-core.h>

// clang-format off

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <conf/args/args.h>

/* Definitions */

#define NXS_FW_CTL_CONF_ARGS_HELP_MSG			"nxs-fw-ctl short description\n" \
								"Command format:\n" \
								"\tnxs-fw-ctl [-c CONFIG_PATH] [-r PROJECT_ROOT] -a project PROJECT_NAME : create new project\n" \
								"\tnxs-fw-ctl [-c CONFIG_PATH] -a (unit | collection | meta | proc | conf | dal) OBJECT_NAME[.SUB_OBJECT_NAME[...]]: create new object in current project\n" \
								"\tnxs-fw-ctl [-c CONFIG_PATH] -d project : delete current project\n" \
								"\tnxs-fw-ctl [-c CONFIG_PATH] -d (unit | collection | meta | proc | conf | dal) OBJECT_NAME[.SUB_OBJECT_NAME[...]]: delete object from current project\n" \
								"\tnxs-fw-ctl [-c CONFIG_PATH] -u (core | version) : change nxs-fw core modules list or Nixys Framework version\n" \
								"\tnxs-fw-ctl [-c CONFIG_PATH] -i (core | version) : get current nxs-fw core modules list or current Nixys Framework version\n" \
								"Available options:\n" \
								"\t-v: show program version\n" \
								"\t-V: show bare program version\n" \
								"\t-h: show help (this message)\n" \
								"\t-c CONFIG_PATH: change default config file path (default path: %s )\n" \
								"\t-r PROJECT_ROOT: set new project root directory (default path setting up in config file)\n" \
								"\t-a (project | unit | collection | meta | proc | conf | dal): add new project or object into current project\n" \
								"\t-d (project | unit | collection | meta | proc | conf | dal): delete project or object from current project\n" \
								"\t-u core: re-select and update nxs-fw core modules\n" \
								"\t-u version: change using nxs-fw version for project\n" \
								"\t-i core: get current nxs-fw core modules list\n" \
								"\t-i version: get current nxs-fw version for project\n"

/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct						nxs_fw_ctl_conf_args_types_s				nxs_fw_ctl_conf_args_types_t;

/* Module declarations */

struct nxs_fw_ctl_conf_args_types_s
{
	nxs_string_t					cmd;
	nxs_fw_ctl_cfg_runtime_type_t			runtime_type;
};

/* Module internal (static) functions prototypes */

// clang-format on

static int nxs_fw_ctl_conf_args_prep(nxs_args_t args, int argc, u_char **argv);
static int nxs_fw_ctl_conf_args_post(nxs_args_t args, int argc, u_char **argv);
static int nxs_fw_ctl_conf_args_freeargs(nxs_args_t args, int argc, int argind, u_char **argv);
static int nxs_fw_ctl_conf_args_help(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_version(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_bare_version(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_conf(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_add(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_del(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_update(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_info(nxs_args_t args, u_char arg, u_char *optarg);
static int nxs_fw_ctl_conf_args_project_root(nxs_args_t args, u_char arg, u_char *optarg);

// clang-format off

/* Module initializations */

nxs_args_shortopt_t shortopts[] =
{
	{'h',	NXS_ARGS_HAVE_ARGS_NO,		&nxs_fw_ctl_conf_args_help},
	{'v',	NXS_ARGS_HAVE_ARGS_NO,		&nxs_fw_ctl_conf_args_version},
	{'V',	NXS_ARGS_HAVE_ARGS_NO,		&nxs_fw_ctl_conf_args_bare_version},
	{'c',	NXS_ARGS_HAVE_ARGS_YES,		&nxs_fw_ctl_conf_args_conf},
	{'a',	NXS_ARGS_HAVE_ARGS_YES,		&nxs_fw_ctl_conf_args_add},
	{'d',	NXS_ARGS_HAVE_ARGS_YES,		&nxs_fw_ctl_conf_args_del},
	{'u',	NXS_ARGS_HAVE_ARGS_YES,		&nxs_fw_ctl_conf_args_update},
	{'i',	NXS_ARGS_HAVE_ARGS_YES,		&nxs_fw_ctl_conf_args_info},
	{'r',	NXS_ARGS_HAVE_ARGS_YES,		&nxs_fw_ctl_conf_args_project_root},

	NXS_ARGS_NULL
};

nxs_fw_ctl_conf_args_types_t add_types[] =
{
	{nxs_string("project"),			NXS_FW_CTL_CFG_RUNTIME_PROJECT_ADD},
	{nxs_string("unit"),			NXS_FW_CTL_CFG_RUNTIME_UNIT_ADD},
	{nxs_string("collection"),		NXS_FW_CTL_CFG_RUNTIME_COLLECTION_ADD},
	{nxs_string("meta"),			NXS_FW_CTL_CFG_RUNTIME_META_ADD},
	{nxs_string("proc"),			NXS_FW_CTL_CFG_RUNTIME_PROC_ADD},
	{nxs_string("conf"),			NXS_FW_CTL_CFG_RUNTIME_CONF_ADD},
	{nxs_string("dal"),			NXS_FW_CTL_CFG_RUNTIME_DAL_ADD},

	{{NULL, 0, 0},				NXS_FW_CTL_CFG_RUNTIME_NONE}
};

nxs_fw_ctl_conf_args_types_t del_types[] =
{
	{nxs_string("project"),			NXS_FW_CTL_CFG_RUNTIME_PROJECT_DEL},
	{nxs_string("unit"),			NXS_FW_CTL_CFG_RUNTIME_UNIT_DEL},
	{nxs_string("collection"),		NXS_FW_CTL_CFG_RUNTIME_COLLECTION_DEL},
	{nxs_string("meta"),			NXS_FW_CTL_CFG_RUNTIME_META_DEL},
	{nxs_string("proc"),			NXS_FW_CTL_CFG_RUNTIME_PROC_DEL},
	{nxs_string("conf"),			NXS_FW_CTL_CFG_RUNTIME_CONF_DEL},
	{nxs_string("dal"),			NXS_FW_CTL_CFG_RUNTIME_DAL_DEL},

	{{NULL, 0, 0},				NXS_FW_CTL_CFG_RUNTIME_NONE}
};

nxs_fw_ctl_conf_args_types_t upd_types[] =
{
	{nxs_string("core"),			NXS_FW_CTL_CFG_RUNTIME_CORE_LIST_CHANGE},
	{nxs_string("version"),			NXS_FW_CTL_CFG_RUNTIME_CORE_VERSION_CHANGE},

	{{NULL, 0, 0},				NXS_FW_CTL_CFG_RUNTIME_NONE}
};

nxs_fw_ctl_conf_args_types_t info_types[] =
{
	{nxs_string("core"),			NXS_FW_CTL_CFG_RUNTIME_CORE_LIST_INFO},
	{nxs_string("version"),			NXS_FW_CTL_CFG_RUNTIME_CORE_VERSION_INFO},

	{{NULL, 0, 0},				NXS_FW_CTL_CFG_RUNTIME_NONE}
};

/* Module global functions */

// clang-format on

void nxs_fw_ctl_conf_args_init(nxs_args_t *args, void *user_ctx)
{

	nxs_string_init(&args->help);
	nxs_string_printf(&args->help, NXS_FW_CTL_CONF_ARGS_HELP_MSG, NXS_FW_CTL_CONF_PATH_DEFAULT);

	args->prep_function     = &nxs_fw_ctl_conf_args_prep;
	args->post_function     = &nxs_fw_ctl_conf_args_post;
	args->freeargs_function = &nxs_fw_ctl_conf_args_freeargs;

	args->args_s_els = shortopts;

	args->ctx = user_ctx;
}

nxs_fw_ctl_err_t nxs_fw_ctl_conf_args_runtime(nxs_args_t args, int argc, u_char **argv)
{

	switch(nxs_args_handler(&process, args, argc, argv)) {

		case NXS_ARGS_CONF_ERROR:

			return NXS_FW_CTL_E_ERR;

		case NXS_ARGS_CONF_OK_EXIT:

			return NXS_FW_CTL_E_EXIT;
	}

	return NXS_FW_CTL_E_OK;
}

/* Module internal (static) functions */

/* Project command line prepare function */
static int nxs_fw_ctl_conf_args_prep(nxs_args_t args, int argc, u_char **argv)
{

	/* init all command line values */

	return NXS_ARGS_CONF_OK;
}

/* Project command line post function */
static int nxs_fw_ctl_conf_args_post(nxs_args_t args, int argc, u_char **argv)
{

	/* checking for defines and correct command line values (on error - NXS_ARGS_CONF_ERROR value must be returned)*/

	return NXS_ARGS_CONF_OK;
}

/* Project command line freeargs function */
static int nxs_fw_ctl_conf_args_freeargs(nxs_args_t args, int argc, int argind, u_char **argv)
{
	nxs_string_t *s;
	int           i;

	/* processing all free command line arguments */

	for(i = argind; i < argc; i++) {

		s = nxs_array_add(&nxs_fw_ctl_cfg.free_args);

		nxs_string_init2(s, 0, argv[i]);
	}

	return NXS_ARGS_CONF_OK;
}

static int nxs_fw_ctl_conf_args_help(nxs_args_t args, u_char arg, u_char *optarg)
{

	nxs_log_write_info(&process, "%s", nxs_string_str(&args.help));

	return NXS_ARGS_CONF_OK_EXIT;
}

static int nxs_fw_ctl_conf_args_version(nxs_args_t args, u_char arg, u_char *optarg)
{

	nxs_log_write_info(&process, "version: %s (nxs-fw v%s)", NXS_FW_CTL_VERSION, NXS_VERSION);

	return NXS_ARGS_CONF_OK_EXIT;
}

static int nxs_fw_ctl_conf_args_bare_version(nxs_args_t args, u_char arg, u_char *optarg)
{

	nxs_log_write_console(&process, "%s", NXS_FW_CTL_VERSION);

	return NXS_ARGS_CONF_OK_EXIT;
}

static int nxs_fw_ctl_conf_args_conf(nxs_args_t args, u_char arg, u_char *optarg)
{
	nxs_fw_ctl_cfg_ctx_t *cfg_ctx = nxs_args_get_ctx(args);

	nxs_string_char_cpy(&cfg_ctx->cfg_path, 0, optarg);

	return NXS_ARGS_CONF_OK;
}

static int nxs_fw_ctl_conf_args_add(nxs_args_t args, u_char arg, u_char *optarg)
{
	size_t i;

	if(nxs_fw_ctl_cfg.runtime_type != NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "error: redefine option '-%c' not allow", arg);

		return NXS_ARGS_CONF_ERROR;
	}

	for(i = 0; nxs_string_str(&add_types[i].cmd) != NULL; i++) {

		if(nxs_string_char_cmp(&add_types[i].cmd, 0, optarg) == NXS_YES) {

			nxs_fw_ctl_cfg.runtime_type = add_types[i].runtime_type;
		}
	}

	if(nxs_fw_ctl_cfg.runtime_type == NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "unknown add cmd");

		return NXS_ARGS_CONF_ERROR;
	}

	return NXS_ARGS_CONF_OK;
}

static int nxs_fw_ctl_conf_args_del(nxs_args_t args, u_char arg, u_char *optarg)
{
	size_t i;

	if(nxs_fw_ctl_cfg.runtime_type != NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "error: redefine option '-%c' not allow", arg);

		return NXS_ARGS_CONF_ERROR;
	}

	for(i = 0; nxs_string_str(&del_types[i].cmd) != NULL; i++) {

		if(nxs_string_char_cmp(&del_types[i].cmd, 0, optarg) == NXS_YES) {

			nxs_fw_ctl_cfg.runtime_type = del_types[i].runtime_type;
		}
	}

	if(nxs_fw_ctl_cfg.runtime_type == NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "unknown del cmd");

		return NXS_ARGS_CONF_ERROR;
	}

	return NXS_ARGS_CONF_OK;
}

static int nxs_fw_ctl_conf_args_update(nxs_args_t args, u_char arg, u_char *optarg)
{
	size_t i;

	if(nxs_fw_ctl_cfg.runtime_type != NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "error: redefine option '-%c' not allow", arg);

		return NXS_ARGS_CONF_ERROR;
	}

	for(i = 0; nxs_string_str(&upd_types[i].cmd) != NULL; i++) {

		if(nxs_string_char_cmp(&upd_types[i].cmd, 0, optarg) == NXS_YES) {

			nxs_fw_ctl_cfg.runtime_type = upd_types[i].runtime_type;
		}
	}

	if(nxs_fw_ctl_cfg.runtime_type == NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "unknown update cmd");

		return NXS_ARGS_CONF_ERROR;
	}

	return NXS_ARGS_CONF_OK;
}

static int nxs_fw_ctl_conf_args_info(nxs_args_t args, u_char arg, u_char *optarg)
{
	size_t i;

	if(nxs_fw_ctl_cfg.runtime_type != NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "error: redefine option '-%c' not allow", arg);

		return NXS_ARGS_CONF_ERROR;
	}

	for(i = 0; nxs_string_str(&info_types[i].cmd) != NULL; i++) {

		if(nxs_string_char_cmp(&info_types[i].cmd, 0, optarg) == NXS_YES) {

			nxs_fw_ctl_cfg.runtime_type = info_types[i].runtime_type;
		}
	}

	if(nxs_fw_ctl_cfg.runtime_type == NXS_FW_CTL_CFG_RUNTIME_NONE) {

		nxs_log_write_error(&process, "unknown info cmd");

		return NXS_ARGS_CONF_ERROR;
	}

	return NXS_ARGS_CONF_OK;
}

static int nxs_fw_ctl_conf_args_project_root(nxs_args_t args, u_char arg, u_char *optarg)
{

	if(nxs_string_len(&nxs_fw_ctl_cfg.proj_root) > 0) {

		nxs_log_write_error(&process, "error: redefine option '-%c' not allow", arg);

		return NXS_ARGS_CONF_ERROR;
	}

	nxs_string_char_cpy(&nxs_fw_ctl_cfg.proj_root, 0, optarg);

	return NXS_ARGS_CONF_OK;
}
