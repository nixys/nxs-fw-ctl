#ifndef _INCLUDE_NXS_FW_CTL_U_PROJECTS_CORE_MODULES_C
#define _INCLUDE_NXS_FW_CTL_U_PROJECTS_CORE_MODULES_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

#include <units/projects/core-modules/core-modules-typedefs.h>
#include <units/projects/core-modules/core-modules.h>
#include <units/projects/core-modules/core-modules-subunits.h>

/* Definitions */



/* Project globals */
extern		nxs_process_t				process;
extern		nxs_fw_ctl_cfg_t			nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct						nxs_fw_ctl_u_projects_core_modules_el_s				nxs_fw_ctl_u_projects_core_modules_el_t;
typedef struct						nxs_fw_ctl_u_projects_core_modules_select_mods_s		nxs_fw_ctl_u_projects_core_modules_select_mods_t;
typedef struct						nxs_fw_ctl_u_projects_core_modules_using_s			nxs_fw_ctl_u_projects_core_modules_using_t;

/* Module declarations */

struct nxs_fw_ctl_u_projects_core_modules_using_s
{
	nxs_array_t					mods;			/* type: nxs_string_t */
	nxs_string_t					use_flags;
	nxs_string_t					link_opts;
	nxs_string_t					includes;
	nxs_string_t					core_links;
};

struct nxs_fw_ctl_u_projects_core_modules_select_mods_s
{
	nxs_string_t					name;
	nxs_string_t					prompt;
	nxs_string_t					using;
};

struct nxs_fw_ctl_u_projects_core_modules_el_s
{
	nxs_string_t					name;
	nxs_bool_t					mandatory;
	nxs_string_t					use_flag;
	nxs_string_t					link_opt;
	nxs_string_t					includes;
	nxs_array_t					deps;			/* type: nxs_string_t */
	nxs_array_t					libs;			/* type: nxs_string_t */
};

struct nxs_fw_ctl_u_projects_core_modules_s
{
	nxs_array_t					modules;		/* type: nxs_fw_ctl_u_projects_core_modules_el_t. Информация обо всех core-модулях, считанная из конфигурационного файла nxs-fw. */
	nxs_array_t					selected_modules;	/* type: nxs_string_t. Список выбранных пользователем модулей (может отличаться от списка используемых модулей). */
	nxs_fw_ctl_u_projects_core_modules_using_t	using;			/* Списко используемых модулей, в т.ч. если модуль не выбран пользователем, но используется по зависимостям. */
};

/* Module internal (static) functions prototypes */

// clang-format on

static nxs_cfg_json_state_t nxs_fw_ctl_u_projects_core_modules_read_arr(nxs_process_t *     proc,
                                                                        nxs_json_t *        json,
                                                                        nxs_cfg_json_par_t *cfg_json_par_el,
                                                                        nxs_array_t *       cfg_arr);
static void nxs_fw_ctl_u_projects_core_modules_using_add_mod(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_string_t *mod_name);
static void nxs_fw_ctl_u_projects_core_modules_using_mod_free(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);
static void nxs_fw_ctl_u_projects_core_modules_selected_add_mod(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_string_t *mod_name);
static void nxs_fw_ctl_u_projects_core_modules_using_mod_cleanup(nxs_fw_ctl_u_projects_core_modules_t *u_ctx);

// clang-format off

/* Module initializations */

static nxs_string_t _s_par_nxs_core_mods		= nxs_string("nxs_core_mods");
static nxs_string_t _s_par_name				= nxs_string("name");
static nxs_string_t _s_par_mandatory			= nxs_string("mandatory");
static nxs_string_t _s_par_use_flag			= nxs_string("use_flag");
static nxs_string_t _s_par_link_opt			= nxs_string("link_opt");
static nxs_string_t _s_par_deps				= nxs_string("deps");
static nxs_string_t _s_par_libs				= nxs_string("libs");
static nxs_string_t _s_par_includes			= nxs_string("includes");

static nxs_string_t _s_y				= nxs_string("y");
static nxs_string_t _s_n				= nxs_string("n");

/* Module global functions */

// clang-format on

nxs_fw_ctl_u_projects_core_modules_t *nxs_fw_ctl_u_projects_core_modules_init(void)
{
	nxs_fw_ctl_u_projects_core_modules_t *u_ctx;

	u_ctx = (nxs_fw_ctl_u_projects_core_modules_t *)nxs_malloc(NULL, sizeof(nxs_fw_ctl_u_projects_core_modules_t));

	nxs_array_init(&u_ctx->modules, 0, sizeof(nxs_fw_ctl_u_projects_core_modules_el_t), 1);
	nxs_array_init(&u_ctx->using.mods, 0, sizeof(nxs_string_t), 1);
	nxs_array_init(&u_ctx->selected_modules, 0, sizeof(nxs_string_t), 1);

	nxs_string_init(&u_ctx->using.use_flags);
	nxs_string_init(&u_ctx->using.link_opts);
	nxs_string_init(&u_ctx->using.includes);

	nxs_string_init2(&u_ctx->using.core_links, 0, (u_char *)NXS_FW_CTL_NXS_CORE_LINKS_INIT);

	return u_ctx;
}

nxs_fw_ctl_u_projects_core_modules_t *nxs_fw_ctl_u_projects_core_modules_free(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{
	nxs_fw_ctl_u_projects_core_modules_el_t *el;
	nxs_string_t *                           s;
	size_t                                   i, j;

	if(u_ctx == NULL) {

		return NULL;
	}

	for(i = 0; i < nxs_array_count(&u_ctx->modules); i++) {

		el = nxs_array_get(&u_ctx->modules, i);

		nxs_string_free(&el->name);
		nxs_string_free(&el->use_flag);
		nxs_string_free(&el->link_opt);
		nxs_string_free(&el->includes);

		for(j = 0; j < nxs_array_count(&el->deps); j++) {

			s = nxs_array_get(&el->deps, i);

			nxs_string_free(s);
		}

		for(j = 0; j < nxs_array_count(&el->libs); j++) {

			s = nxs_array_get(&el->libs, i);

			nxs_string_free(s);
		}

		nxs_array_free(&el->deps);
		nxs_array_free(&el->libs);
	}

	nxs_array_free(&u_ctx->modules);

	nxs_fw_ctl_u_projects_core_modules_using_mod_free(u_ctx);

	return (nxs_fw_ctl_u_projects_core_modules_t *)nxs_free(u_ctx);
}

/*
 * Получить указатель на массив (nxs_string_t) выбранных модулей
 */
nxs_array_t *nxs_fw_ctl_u_projects_core_modules_selected_get_mods(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	return &u_ctx->selected_modules;
}

/*
 * Получить указатель на массив (nxs_string_t) используемых модулей
 */
nxs_array_t *nxs_fw_ctl_u_projects_core_modules_using_get_mods(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	return &u_ctx->using.mods;
}

/*
 * Получить указатель на строку используемых флагов
 */
nxs_string_t *nxs_fw_ctl_u_projects_core_modules_using_get_use_flags(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	return &u_ctx->using.use_flags;
}

/*
 * Получить указатель на строку используемых опций для линковки
 */
nxs_string_t *nxs_fw_ctl_u_projects_core_modules_using_get_link_opts(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	return &u_ctx->using.link_opts;
}

/*
 * Получить указатель на строку используемых инклудов
 */
nxs_string_t *nxs_fw_ctl_u_projects_core_modules_using_get_includes(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	return &u_ctx->using.includes;
}

/*
 * Получить указатель на строку ядровых библиотек фрэймворка для линковки
 */
nxs_string_t *nxs_fw_ctl_u_projects_core_modules_using_get_core_links(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{

	if(u_ctx == NULL) {

		return NULL;
	}

	return &u_ctx->using.core_links;
}

/*
 * Интерактивный выбор модулей для использования
 */
void nxs_fw_ctl_u_projects_core_modules_select(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_array_t *selected_mods)
{
	nxs_fw_ctl_u_projects_core_modules_el_t *         mod;
	nxs_fw_ctl_u_projects_core_modules_select_mods_t *sm;
	nxs_string_t                                      spaces, *d, *s;
	nxs_array_t                                       rlfields, select_mods, arr_str_bool;
	size_t                                            i, j, max_len;

	nxs_fw_ctl_c_rlfields_init(&rlfields);

	nxs_string_init(&spaces);

	nxs_array_init(&select_mods, 0, sizeof(nxs_fw_ctl_u_projects_core_modules_select_mods_t), 1);

	/* bool autocomplete */

	nxs_fw_ctl_c_rlfields_arr_str_init(&arr_str_bool);

	nxs_fw_ctl_c_rlfields_arr_str_add(&arr_str_bool, &_s_y);
	nxs_fw_ctl_c_rlfields_arr_str_add(&arr_str_bool, &_s_n);

	/* Поиск максимальной длины имени модуля (используется для форматирования приглашений) */
	for(max_len = 0, i = 0; i < nxs_array_count(&u_ctx->modules); i++) {

		mod = nxs_array_get(&u_ctx->modules, i);

		if(mod->mandatory == NXS_NO && max_len < nxs_string_len(&mod->name)) {

			max_len = nxs_string_len(&mod->name);
		}
	}

	for(i = 0; i < nxs_array_count(&u_ctx->modules); i++) {

		mod = nxs_array_get(&u_ctx->modules, i);

		if(mod->mandatory == NXS_NO) {

			sm = nxs_array_add(&select_mods);

			nxs_string_init3(&sm->name, &mod->name);
			nxs_string_init(&sm->prompt);
			nxs_string_init(&sm->using);

			/* Дополнение строки приглашения необходимым числом пробелов */

			nxs_string_clear(&spaces);

			for(j = 0; j < max_len - nxs_string_len(&mod->name); j++) {

				nxs_string_char_add_char(&spaces, (u_char)' ');
			}

			nxs_string_printf(&sm->prompt, "\t%r%r[y/n]: ", &spaces, &mod->name);
		}
	}

	for(i = 0; i < nxs_array_count(&select_mods); i++) {

		sm = nxs_array_get(&select_mods, i);

		if(selected_mods != NULL) {

			d = &_s_n;

			for(j = 0; j < nxs_array_count(selected_mods); j++) {

				s = nxs_array_get(selected_mods, j);

				if(nxs_string_cmp(s, 0, &sm->name, 0) == NXS_YES) {

					d = &_s_y;

					break;
				}
			}
		}
		else {

			d = NULL;
		}

		nxs_fw_ctl_c_rlfields_add(&rlfields, &sm->prompt, d, &arr_str_bool, NULL, &sm->using, NXS_NO, NXS_YES, &arr_str_bool);
	}

	nxs_fw_ctl_c_rlfields_exec(&rlfields);

	nxs_fw_ctl_c_rlfields_free(&rlfields);

	nxs_fw_ctl_c_rlfields_arr_str_free(&arr_str_bool);

	/* Добавление в выборку всех обязательных модулей */
	for(i = 0; i < nxs_array_count(&u_ctx->modules); i++) {

		mod = nxs_array_get(&u_ctx->modules, i);

		if(mod->mandatory == NXS_YES) {

			nxs_fw_ctl_u_projects_core_modules_using_add_mod(u_ctx, &mod->name);
		}
	}

	/* Добавление в выборку всех выбранных пользователем модулей */
	for(i = 0; i < nxs_array_count(&select_mods); i++) {

		sm = nxs_array_get(&select_mods, i);

		if(nxs_string_cmp(&sm->using, 0, &_s_y, 0) == NXS_YES) {

			nxs_fw_ctl_u_projects_core_modules_using_add_mod(u_ctx, &sm->name);
			nxs_fw_ctl_u_projects_core_modules_selected_add_mod(u_ctx, &sm->name);
		}

		nxs_string_free(&sm->name);
		nxs_string_free(&sm->prompt);
		nxs_string_free(&sm->using);
	}

	nxs_array_free(&select_mods);

	nxs_string_free(&spaces);
}

/*
 * Создать массив выбранных и используемых модулей в контексте "u_ctx".
 * Входным аргументом является массив "selected" (nxs_string_t), содержащий список модулей, которые выбрал пользователь и которые необходимо
 * включить.
 * Так же, будет заполнена структура "using" с учётом зависимостей в модулях.
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_core_modules_selected_add(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_array_t *selected)
{
	nxs_fw_ctl_u_projects_core_modules_el_t *mod;
	nxs_string_t *                           s;
	size_t                                   i;

	if(u_ctx == NULL || selected == NULL) {

		return NXS_FW_CTL_E_PTR;
	}

	nxs_fw_ctl_u_projects_core_modules_using_mod_cleanup(u_ctx);

	/* Добавление в выборку всех обязательных модулей */
	for(i = 0; i < nxs_array_count(&u_ctx->modules); i++) {

		mod = nxs_array_get(&u_ctx->modules, i);

		if(mod->mandatory == NXS_YES) {

			nxs_fw_ctl_u_projects_core_modules_using_add_mod(u_ctx, &mod->name);
		}
	}

	/* Добавление в выборку всех выбранных пользователем модулей */
	for(i = 0; i < nxs_array_count(selected); i++) {

		s = nxs_array_get(selected, i);

		nxs_fw_ctl_u_projects_core_modules_using_add_mod(u_ctx, s);
		nxs_fw_ctl_u_projects_core_modules_selected_add_mod(u_ctx, s);
	}

	return NXS_FW_CTL_E_OK;
}

/*
 * Чтение файла с описанием модулей и взаимосвязей между ними (файл поставляется вместе с nxs-fw)
 */
nxs_fw_ctl_err_t nxs_fw_ctl_u_projects_core_modules_read(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_string_t *nxs_core_desc_path)
{
	nxs_fw_ctl_err_t rc;
	nxs_cfg_json_t   cfg_json;
	nxs_array_t      cfg_arr;

	rc = NXS_FW_CTL_E_OK;

	nxs_cfg_json_conf_array_init(&cfg_arr);

	nxs_cfg_json_conf_array_add(&cfg_arr,
	                            &_s_par_nxs_core_mods,
	                            u_ctx,
	                            NULL,
	                            &nxs_fw_ctl_u_projects_core_modules_read_arr,
	                            NXS_CFG_JSON_TYPE_ARRAY_OBJECT,
	                            0,
	                            0,
	                            NXS_YES,
	                            NULL);

	nxs_cfg_json_init(&process, &cfg_json, nxs_core_desc_path, NULL, NULL, &cfg_arr);

	if(nxs_cfg_json_read_file(&process, cfg_json) != NXS_CFG_JSON_CONF_OK) {

		nxs_log_write_error(&process, "can't read nxs-fw core modules config (cfg file: %s)", nxs_string_str(nxs_core_desc_path));

		nxs_error(rc, NXS_FW_CTL_E_ERR, error);
	}

error:

	nxs_cfg_json_free(&cfg_json);

	nxs_cfg_json_conf_array_free(&cfg_arr);

	return rc;
}

/* Module internal (static) functions */

static nxs_cfg_json_state_t nxs_fw_ctl_u_projects_core_modules_read_arr(nxs_process_t *     proc,
                                                                        nxs_json_t *        json,
                                                                        nxs_cfg_json_par_t *cfg_json_par_el,
                                                                        nxs_array_t *       cfg_arr)
{
	nxs_fw_ctl_u_projects_core_modules_t *   fw_core_mods = nxs_cfg_json_get_val(cfg_json_par_el);
	nxs_fw_ctl_u_projects_core_modules_el_t *mod;

	mod = nxs_array_add(&fw_core_mods->modules);

	nxs_string_init(&mod->name);
	nxs_string_init(&mod->use_flag);
	nxs_string_init(&mod->link_opt);
	nxs_string_init(&mod->includes);

	nxs_array_init(&mod->deps, 0, sizeof(nxs_string_t), 1);
	nxs_array_init(&mod->libs, 0, sizeof(nxs_string_t), 1);

	mod->mandatory = NXS_NO;

	nxs_cfg_json_conf_array_add(cfg_arr, &_s_par_name, &mod->name, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_YES, NULL);
	nxs_cfg_json_conf_array_add(cfg_arr, &_s_par_use_flag, &mod->use_flag, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_YES, NULL);
	nxs_cfg_json_conf_array_add(cfg_arr, &_s_par_link_opt, &mod->link_opt, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_YES, NULL);
	nxs_cfg_json_conf_array_add(cfg_arr, &_s_par_includes, &mod->includes, NULL, NULL, NXS_CFG_JSON_TYPE_STRING, 0, 0, NXS_YES, NULL);
	nxs_cfg_json_conf_array_add(cfg_arr, &_s_par_mandatory, &mod->mandatory, NULL, NULL, NXS_CFG_JSON_TYPE_BOOL, 0, 0, NXS_YES, NULL);
	nxs_cfg_json_conf_array_add(cfg_arr, &_s_par_deps, &mod->deps, NULL, NULL, NXS_CFG_JSON_TYPE_ARRAY_STRING, 0, 0, NXS_YES, NULL);
	nxs_cfg_json_conf_array_add(cfg_arr, &_s_par_libs, &mod->libs, NULL, NULL, NXS_CFG_JSON_TYPE_ARRAY_STRING, 0, 0, NXS_YES, NULL);

	return NXS_CFG_JSON_CONF_OK;
}

/*
 * Добавить конкретный core-модуль в массив используемых.
 * При этом добавлены будут и все зависимые модули.
 * Дублирующиеся модули не добавляются в массив.
 *
 * Так же, будет соответствующим образом заполнены строки 'using_use_flags', 'using_link_opts' и 'using_includes'
 *
 * Если модуля с именем "mod_name" нет, то она будет пропущен.
 */
static void nxs_fw_ctl_u_projects_core_modules_using_add_mod(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_string_t *mod_name)
{
	nxs_fw_ctl_u_projects_core_modules_el_t *mod;
	size_t                                   i, j;
	nxs_string_t *                           s;

	for(i = 0; i < nxs_array_count(&u_ctx->using.mods); i++) {

		s = nxs_array_get(&u_ctx->using.mods, i);

		if(nxs_string_cmp(mod_name, 0, s, 0) == NXS_YES) {

			/* Такой модуль уже добавлен */

			return;
		}
	}

	for(i = 0; i < nxs_array_count(&u_ctx->modules); i++) {

		mod = nxs_array_get(&u_ctx->modules, i);

		if(nxs_string_cmp(mod_name, 0, &mod->name, 0) == NXS_YES) {

			/* Добавление модуля в выборку */

			s = nxs_array_add(&u_ctx->using.mods);

			nxs_string_init3(s, &mod->name);

			if(nxs_string_len(&mod->use_flag) > 0) {

				nxs_string_printf2_cat(&u_ctx->using.use_flags, "-D %r ", &mod->use_flag);
			}

			if(nxs_string_len(&mod->link_opt) > 0) {

				nxs_string_printf2_cat(&u_ctx->using.link_opts, "%r ", &mod->link_opt);
			}

			if(nxs_string_len(&mod->includes) > 0) {

				nxs_string_printf2_cat(&u_ctx->using.includes, "%r ", &mod->includes);
			}

			for(j = 0; j < nxs_array_count(&mod->libs); j++) {

				s = nxs_array_get(&mod->libs, j);

				nxs_string_printf2_cat(&u_ctx->using.core_links, "-l%r ", s);
			}

			/* Проход по всем зависимым модулям и добавление их в выборку */

			for(j = 0; j < nxs_array_count(&mod->deps); j++) {

				s = nxs_array_get(&mod->deps, j);

				nxs_fw_ctl_u_projects_core_modules_using_add_mod(u_ctx, s);
			}

			return;
		}
	}
}

static void nxs_fw_ctl_u_projects_core_modules_selected_add_mod(nxs_fw_ctl_u_projects_core_modules_t *u_ctx, nxs_string_t *mod_name)
{
	nxs_fw_ctl_u_projects_core_modules_el_t *mod;
	nxs_string_t *                           s;
	size_t                                   i;

	for(i = 0; i < nxs_array_count(&u_ctx->selected_modules); i++) {

		s = nxs_array_get(&u_ctx->selected_modules, i);

		if(nxs_string_cmp(mod_name, 0, s, 0) == NXS_YES) {

			/* Такой модуль уже добавлен */

			return;
		}
	}

	for(i = 0; i < nxs_array_count(&u_ctx->modules); i++) {

		mod = nxs_array_get(&u_ctx->modules, i);

		if(nxs_string_cmp(mod_name, 0, &mod->name, 0) == NXS_YES) {

			/* Добавление модуля в выборку */

			s = nxs_array_add(&u_ctx->selected_modules);

			nxs_string_init3(s, mod_name);
		}
	}
}

/*
 * Очистка структуры используемых модулей, а так же освобождение выделенной памяти
 */
static void nxs_fw_ctl_u_projects_core_modules_using_mod_free(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{
	nxs_string_t *s;
	size_t        i;

	for(i = 0; i < nxs_array_count(&u_ctx->using.mods); i++) {

		s = nxs_array_get(&u_ctx->using.mods, i);

		nxs_string_free(s);
	}

	nxs_array_free(&u_ctx->using.mods);

	for(i = 0; i < nxs_array_count(&u_ctx->selected_modules); i++) {

		s = nxs_array_get(&u_ctx->selected_modules, i);

		nxs_string_free(s);
	}

	nxs_array_free(&u_ctx->selected_modules);

	nxs_string_free(&u_ctx->using.use_flags);
	nxs_string_free(&u_ctx->using.link_opts);
	nxs_string_free(&u_ctx->using.includes);
	nxs_string_free(&u_ctx->using.core_links);
}

/*
 * Очистка структуры используемых модулей
 */
static void nxs_fw_ctl_u_projects_core_modules_using_mod_cleanup(nxs_fw_ctl_u_projects_core_modules_t *u_ctx)
{
	nxs_string_t *s;
	size_t        i;

	for(i = 0; i < nxs_array_count(&u_ctx->using.mods); i++) {

		s = nxs_array_get(&u_ctx->using.mods, i);

		nxs_string_free(s);
	}

	for(i = 0; i < nxs_array_count(&u_ctx->selected_modules); i++) {

		s = nxs_array_get(&u_ctx->selected_modules, i);

		nxs_string_free(s);
	}

	nxs_string_clear(&u_ctx->using.use_flags);
	nxs_string_clear(&u_ctx->using.link_opts);
	nxs_string_clear(&u_ctx->using.includes);

	nxs_string_char_cpy(&u_ctx->using.core_links, 0, (u_char *)NXS_FW_CTL_NXS_CORE_LINKS_INIT);
}
