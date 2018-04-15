#ifndef _INCLUDE_NXS_FW_CTL_CORE_H
#define _INCLUDE_NXS_FW_CTL_CORE_H

// clang-format off

/* Project version */
#define NXS_FW_CTL_VERSION			"0.9.0"

/* Project basic global definitions */

#define	NXS_FW_CTL_CONF_PATH_DEFAULT		"/etc/nxs-fw-ctl/nxs-fw-ctl.conf"

#define NXS_FW_CTL_NXS_FW_MODS_DESC_PATH	"nxs-fw-mods-desc.conf"

#define NXS_FW_CTL_NXS_CORE_LINKS_INIT		"-L$(NXS_FW_CORE_LIBS_PATH)/$(NXS_FW_VERSION) "

#define NXS_FW_CTL_DIR_MODE_DEF			0755
#define NXS_FW_CTL_FILE_MODE_DEF		0644

#define NXS_FW_CTL_DIR_NEW_PROJ_TPL		"new-proj-tpl"
#define NXS_FW_CTL_DIR_ADD_TPL			"add-tpl"
#define NXS_FW_CTL_DIR_UNITS			"units"
#define NXS_FW_CTL_DIR_COLLECTIONS		"collections"
#define NXS_FW_CTL_DIR_METAS			"metas"
#define NXS_FW_CTL_DIR_PROCS			"procs"
#define NXS_FW_CTL_DIR_CONFS			"confs"
#define NXS_FW_CTL_DIR_DALS			"dals"

/* Project global typedefs */



/* Project global structures */



/* Project errors */
typedef enum
{
	NXS_FW_CTL_E_OK,
	NXS_FW_CTL_E_WARN,
	NXS_FW_CTL_E_ERR,
	NXS_FW_CTL_E_PTR,
	NXS_FW_CTL_E_INIT,
	NXS_FW_CTL_E_EXIT,
	NXS_FW_CTL_E_EXIST,
	NXS_FW_CTL_E_CANCEL,
} nxs_fw_ctl_err_t;

/* Project config structures */

typedef enum
{
	NXS_FW_CTL_CFG_RUNTIME_NONE,
	NXS_FW_CTL_CFG_RUNTIME_PROJECT_ADD,
	NXS_FW_CTL_CFG_RUNTIME_PROJECT_DEL,
	NXS_FW_CTL_CFG_RUNTIME_UNIT_ADD,
	NXS_FW_CTL_CFG_RUNTIME_UNIT_DEL,
	NXS_FW_CTL_CFG_RUNTIME_COLLECTION_ADD,
	NXS_FW_CTL_CFG_RUNTIME_COLLECTION_DEL,
	NXS_FW_CTL_CFG_RUNTIME_META_ADD,
	NXS_FW_CTL_CFG_RUNTIME_META_DEL,
	NXS_FW_CTL_CFG_RUNTIME_PROC_ADD,
	NXS_FW_CTL_CFG_RUNTIME_PROC_DEL,
	NXS_FW_CTL_CFG_RUNTIME_CONF_ADD,
	NXS_FW_CTL_CFG_RUNTIME_CONF_DEL,
	NXS_FW_CTL_CFG_RUNTIME_DAL_ADD,
	NXS_FW_CTL_CFG_RUNTIME_DAL_DEL,
	NXS_FW_CTL_CFG_RUNTIME_CORE_LIST_CHANGE,
	NXS_FW_CTL_CFG_RUNTIME_CORE_VERSION_CHANGE,
	NXS_FW_CTL_CFG_RUNTIME_CORE_LIST_INFO,
	NXS_FW_CTL_CFG_RUNTIME_CORE_VERSION_INFO,
} nxs_fw_ctl_cfg_runtime_type_t;

typedef struct
{
	nxs_string_t				cfg_path;
} nxs_fw_ctl_cfg_ctx_t;

typedef struct
{
	nxs_string_t				tpls_path;
	nxs_string_t				proj_root;

	nxs_string_t				nxs_fw_desc_path;

	nxs_fw_ctl_cfg_runtime_type_t		runtime_type;

	nxs_array_t				free_args;		/* type: nxs_string_t */

	nxs_string_t				log_path;
	int					log_level;
} nxs_fw_ctl_cfg_t;

/* Project includes */



/* Global functions for project's core module */



#endif /* _INCLUDE_NXS_FW_CTL_CORE_H */
