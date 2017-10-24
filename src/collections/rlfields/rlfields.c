#ifndef _INCLUDE_NXS_FW_CTL_C_RLFIELDS_C
#define _INCLUDE_NXS_FW_CTL_C_RLFIELDS_C
#endif

// clang-format off

#include <nxs-core/nxs-core.h>

#include <readline/readline.h>
#include <readline/history.h>

/* Project core include */
#include <nxs-fw-ctl-core.h>
#include <nxs-fw-ctl-meta.h>
#include <nxs-fw-ctl-collections.h>
#include <nxs-fw-ctl-dal.h>

/* Definitions */

static nxs_array_t				*tvars;
static nxs_string_t				*def_value;

/* Project globals */
extern		nxs_process_t			process;
extern		nxs_fw_ctl_cfg_t		nxs_fw_ctl_cfg;

/* Module typedefs */

typedef struct					nxs_fw_ctl_c_rlfields_el_s					nxs_fw_ctl_c_rlfields_el_t;

/* Module declarations */

struct nxs_fw_ctl_c_rlfields_el_s
{
	nxs_string_t				*prompt;		/* Текст приглашения */
	nxs_string_t				*defult_value;		/* Предустановленное значение командной строки */
	nxs_array_t				*valid_set;		/* type: nxs_string_t. Множество допустимых вариантов. Ввод будет запрашиваться до тех пор, пока значение будет не из этого множества. */
	nxs_string_t				*empty_value;		/* Значение, которое будет выбрано при пустом вводе */
	nxs_string_t				*data;			/* Строка, в которую будет записано введённое значение */
	nxs_bool_t				valid_spaces;		/* Допустимо ли использовать пробелы при вводе */
	nxs_bool_t				trim_spaces;		/* Удалять из введённой строки начальные и конечные пробелы */
	nxs_array_t				*tvariants;		/* type: nxs_string_t. Множество вариантов для выбора по табуляции */
};

/* Module internal (static) functions prototypes */

// clang-format on

static char **nxs_fw_ctl_c_rlfields_complete(const char *text, int start, int end);
static char *nxs_fw_ctl_c_rlfields_complete_gen(const char *text, int state);
static int nxs_fw_ctl_c_rlfields_defult_value(void);

// clang-format off

/* Module initializations */

static nxs_string_t _s_y			= nxs_string("y");
static nxs_string_t _s_n			= nxs_string("n");
static nxs_string_t _s_empty			= nxs_string("");

/* Module global functions */

// clang-format on

void nxs_fw_ctl_c_rlfields_init(nxs_array_t *rlfields)
{

	if(rlfields == NULL) {

		return;
	}

	nxs_array_init(rlfields, 0, sizeof(nxs_fw_ctl_c_rlfields_el_t), 1, NULL, NULL);
}

void nxs_fw_ctl_c_rlfields_free(nxs_array_t *rlfields)
{

	if(rlfields == NULL) {

		return;
	}

	nxs_array_free(rlfields);
}

nxs_bool_t nxs_fw_ctl_c_rlfields_yesno(nxs_string_t *question)
{
	nxs_array_t   rlfields, arr_str_bool;
	nxs_string_t *q, answer;
	nxs_bool_t    rc;

	rc = NXS_NO;

	q = question;

	if(q == NULL) {

		q = &_s_empty;
	}

	nxs_string_init(&answer);

	nxs_fw_ctl_c_rlfields_arr_str_init(&arr_str_bool);

	nxs_fw_ctl_c_rlfields_arr_str_add(&arr_str_bool, &_s_y);
	nxs_fw_ctl_c_rlfields_arr_str_add(&arr_str_bool, &_s_n);

	nxs_fw_ctl_c_rlfields_init(&rlfields);
	nxs_fw_ctl_c_rlfields_add(&rlfields, q, NULL, &arr_str_bool, NULL, &answer, NXS_NO, NXS_YES, &arr_str_bool);
	nxs_fw_ctl_c_rlfields_exec(&rlfields);
	nxs_fw_ctl_c_rlfields_free(&rlfields);

	nxs_fw_ctl_c_rlfields_arr_str_free(&arr_str_bool);

	if(nxs_string_cmp(&answer, 0, &_s_y, 0) == NXS_YES) {

		rc = NXS_YES;
	}

	nxs_string_free(&answer);

	return rc;
}

void nxs_fw_ctl_c_rlfields_add(nxs_array_t * rlfields,
                               nxs_string_t *prompt,
                               nxs_string_t *defult_value,
                               nxs_array_t * valid_set,
                               nxs_string_t *empty_value,
                               nxs_string_t *data,
                               nxs_bool_t    valid_spaces,
                               nxs_bool_t    trim_spaces,
                               nxs_array_t * tvariants)
{
	nxs_fw_ctl_c_rlfields_el_t *el;

	if(rlfields == NULL) {

		return;
	}

	el = nxs_array_add(rlfields);

	el->prompt       = prompt;
	el->defult_value = defult_value;
	el->valid_set    = valid_set;
	el->empty_value  = empty_value;
	el->data         = data;
	el->valid_spaces = valid_spaces;
	el->trim_spaces  = trim_spaces;
	el->tvariants    = tvariants;
}

nxs_fw_ctl_err_t nxs_fw_ctl_c_rlfields_exec(nxs_array_t *rlfields)
{
	nxs_fw_ctl_c_rlfields_el_t *el;
	nxs_string_t *              vs;
	nxs_bool_t                  f;
	size_t                      i, j, k, l;
	char *                      d;

	for(i = 0; i < nxs_array_count(rlfields); i++) {

		el = nxs_array_get(rlfields, i);

		if(el->prompt == NULL) {

			nxs_log_write_error(&process, "readline exec error: NULL pointer to prompt");

			return NXS_FW_CTL_E_ERR;
		}

		if(nxs_string_str(el->prompt) == NULL) {

			nxs_log_write_error(&process, "readline exec error: prompt string is NULL");

			return NXS_FW_CTL_E_ERR;
		}

		/* Требуется ли запретить ввод пробелов */
		if(el->valid_spaces == NXS_NO) {

			rl_bind_key(' ', NULL);
		}
		else {

			rl_bind_key(' ', rl_insert);
		}

		/* Требуется ли использовать автодополнение */
		if(el->tvariants != NULL) {

			tvars = el->tvariants;

			rl_attempted_completion_function = nxs_fw_ctl_c_rlfields_complete;

			rl_bind_key('\t', rl_complete);
		}
		else {

			tvars = NULL;

			rl_attempted_completion_function = NULL;

			rl_unbind_key('\t');
		}

		/* Ввод строки (производится до тех пор, пока строка не будет удовлетворять всем требованиям) */
		for(f = NXS_NO; f == NXS_NO;) {

			/* Если задано - отразить предопределённый текст для строки (например, для операций обновления данных) */
			if(el->defult_value != NULL) {

				def_value = el->defult_value;

				rl_startup_hook = nxs_fw_ctl_c_rlfields_defult_value;
			}
			else {

				def_value = NULL;

				rl_startup_hook = nxs_fw_ctl_c_rlfields_defult_value;
			}

			d = readline((char *)nxs_string_str(el->prompt));

			if(d == NULL) {

				nxs_log_write_error(&process, "readline exec error: can't allocate memory");

				return NXS_FW_CTL_E_ERR;
			}

			l = strlen(d);

			if(el->trim_spaces == NXS_YES && l > 0) {

				/* Если необходимо удалить начальные и конечные пробелы из введённой строки */

				for(j = 0; j < l && d[j] == ' '; j++)
					;

				for(k = l - 1; k > j && d[k] == ' '; k--)
					;

				nxs_string_char_ncpy(el->data, 0, (u_char *)(d + j), k - j + 1);

				nxs_string_escape(el->data, NULL, NXS_STRING_ESCAPE_TYPE_JSON);
			}
			else {

				nxs_string_char_cpy(el->data, 0, (u_char *)d);

				nxs_string_escape(el->data, NULL, NXS_STRING_ESCAPE_TYPE_JSON);
			}

			d = nxs_free(d);

			if(nxs_string_len(el->data) == 0 && el->empty_value != NULL) {

				/* Если строка пустая или не задано значение по умолчанию для пустых строк */

				nxs_string_cpy(el->data, 0, el->empty_value, 0);

				nxs_string_escape(el->data, NULL, NXS_STRING_ESCAPE_TYPE_JSON);

				f = NXS_YES;
			}
			else {

				if(el->valid_set != NULL) {

					/* Если задано множество допустимых значений - проверяем введённое значение на соответствие этом
					 * множеству (ввод продолжается до тех пор, пока введённая строка не будет удовлетворять) */

					for(j = 0; j < nxs_array_count(el->valid_set); j++) {

						vs = nxs_array_get(el->valid_set, j);

						if(nxs_string_cmp(vs, 0, el->data, 0) == NXS_YES) {

							f = NXS_YES;

							break;
						}
					}
				}
				else {

					f = NXS_YES;
				}
			}
		}

		rl_bind_key(' ', rl_insert);
	}

	return NXS_FW_CTL_E_OK;
}

void nxs_fw_ctl_c_rlfields_arr_str_init(nxs_array_t *arr_str)
{

	nxs_array_init(arr_str, 0, sizeof(nxs_string_t), 1, NULL, NULL);
}

void nxs_fw_ctl_c_rlfields_arr_str_free(nxs_array_t *arr_str)
{
	nxs_string_t *str;
	size_t        i;

	for(i = 0; i < nxs_array_count(arr_str); i++) {

		str = nxs_array_get(arr_str, i);

		nxs_string_free(str);
	}

	nxs_array_free(arr_str);
}

void nxs_fw_ctl_c_rlfields_arr_str_add(nxs_array_t *arr_str, nxs_string_t *str_el)
{
	nxs_string_t *str;

	str = nxs_array_add(arr_str);

	nxs_string_init3(str, str_el);
}

/* Module internal (static) functions */

static char **nxs_fw_ctl_c_rlfields_complete(const char *text, int start, int end)
{
	char **matches;

	matches = NULL;

	if(start == 0) {

		matches = rl_completion_matches((char *)text, &nxs_fw_ctl_c_rlfields_complete_gen);
	}

	return matches;
}

static char *nxs_fw_ctl_c_rlfields_complete_gen(const char *text, int state)
{
	nxs_string_t *str;
	static size_t list_index, len;
	char *        r;

	if(state == 0) {

		list_index = 0;
		len        = strlen(text);
	}

	while(list_index < nxs_array_count(tvars)) {

		str = nxs_array_get(tvars, list_index);

		list_index++;

		if(nxs_string_char_ncmp(str, 0, (u_char *)text, len) == NXS_YES) {

			r = nxs_malloc(NULL, nxs_string_len(str) + 1);

			strcpy(r, (char *)nxs_string_str(str));

			return r;
		}
	}

	return NULL;
}

static int nxs_fw_ctl_c_rlfields_defult_value(void)
{

	if(def_value != NULL) {

		rl_insert_text((char *)nxs_string_str(def_value));
	}
	else {

		rl_insert_text("");
	}

	def_value = NULL;

	rl_startup_hook = NULL;

	return 0;
}
