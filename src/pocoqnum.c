/* pocoqnum.c - a dupe of the qnumber routine with an update function */

#include "jimk.h"
#include "errcodes.h"
#include "pocoface.h"
#include "pocolib.h"

typedef struct upddata {
	void* code;
	Popot* ppdat;
} Upddat;

static Errcode ppupdate(Upddat* udd, SHORT value)
{
	Pt_num ret;
	int ival;

	if (udd->code == NULL) {
		return Success;
	}

	ival = value;
	builtin_err = poco_cont_ops(udd->code, &ret, (sizeof(Popot) + sizeof(ival)), *udd->ppdat, ival);
	if (builtin_err < Success) {
		return builtin_err;
	}
	return ret.i;
}

/*****************************************************************************
 * bool UdQnumber(int *num, int min, int max,
					 Errcode (*update)(void *data, int num),
					 void *data, char *fmt, ...)
	 Will abort requestor if update returns < Success
 ****************************************************************************/
bool po_UdSlider(int* inum, int min, int max, void* update,
				 void* data, char* fmt, ...)
{
	short num;
	bool cancel;
	bool mouse_was_on;
	Upddat udd;
	va_list args;

	va_start(args, fmt);
	if (fmt == NULL) {
		fmt = "";
	}

	if (inum == NULL) {
		return builtin_err = Err_null_ref;
	}
	num = *inum;

	if ((num < SHRT_MIN) || (min < SHRT_MIN) || (max < SHRT_MIN) || (num > SHRT_MAX) ||
		(min > SHRT_MAX) || (max > SHRT_MAX) || (min > max)) {
		return builtin_err = Err_parameter_range;
	}

	mouse_was_on = show_mouse();

	udd.code = update;
	if (udd.code != NULL) {
		udd.code = po_fuf_code(udd.code);
		udd.ppdat = (Popot*)&data;  // TODO: needs rework for proper callback data
	}

	cancel = varg_qreq_number(&num, min, max, ppupdate, &udd, NULL, fmt, args);

	if (cancel != false) {
		*inum = num;
	}

	if (!mouse_was_on) {
		hide_mouse();
	}
	va_end(args);
	return cancel;
}
