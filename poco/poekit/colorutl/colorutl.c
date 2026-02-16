/*****************************************************************************
 * COLORUTL.C - A POE module containing some color-related utilities.
 *
 *	Major POE items/features demonstrated herein:
 *
 *		- Receiving and returning values to Poco caller via pointers,
 *		  with full required pointer bounds checking.
 *		- Providing a library of several functions to the Poco program.
 *		- Mixed C and ASM code implement functions.
 *
 *	This POE module implements 3 functions for Poco callers:
 *
 *		int  ColorDifference(int *pcolor1, int *pcolor2);
 *
 *			This function returns the difference between a pair of rgb
 *			colors.  The difference is the sum of the squares of the
 *			differences of each of the r,g,b components.  It's useful in
 *			determining how similar two colors are.
 *
 *
 *		int  ClosestColor(int *pcolor, int *ptab, int tabcount);
 *
 *			This function searches a table of rgb color values and returns
 *			the index of the color closest to the rgb value specified. The
 *			values in the table are rgb triplets stored as 32-bit integers.
 *			(IE, like the table returned by GetScreenColorMap().)
 *
 *
 *		void GetMenuColors(int *current, int *preferred);
 *
 *			This function returns the current and preferred menu colors.
 *			The current colors are the ones in use right now (or the ones
 *			the user saw last, if no menus/dialogs are on the Ani Pro
 *			screen right now).	The preferred colors are the set the user
 *			would like to see.	These may differ from the current colors
 *			if exact matches don't exist in the user's color palette.
 *			The current and preferred colors are each a set of 5 rgb
 *			triplets; each triplet composed of 32-bit integers.  This
 *			function is useful primary when you are constructing a new
 *			color palette programmatically.  If you have some leftover
 *			slots in the palette, you can put the user's preferred
 *			colors into them; a nice gesture.  (There is no requirement
 *			to do this when constructing a color palette; Ani Pro will
 *			do its best to use as menu colors whatever exists in any
 *			palette.)
 *
 *
 * NOTES:
 *
 *		The functions herein are defined as static if they are called
 *		only from within this module, and as global if they are exported
 *		to the Poco program.
 *
 * MAINTENANCE:
 *
 *	10/15/91	Ian Lepore
 *				Created.
 *	01/10/92	Ian
 *				Added GetMenuIndexes(), RgbToHls(), and HlsToRgb() routines.
 *				Also added init_patches_10a() to Setup_Pocorex() macro.
 ****************************************************************************/

/*----------------------------------------------------------------------------
 * include the usual header files...
 *--------------------------------------------------------------------------*/

#define PUBLIC_CODE
#include "errcodes.h"   /* host error codes (must precede pocorex.h)     */
#include "rexlib.h"     /* required for the hostlibs                     */
#include "pocorex.h"    /* required header file, also includes pocolib.h */
#include "cmap.h"       /* this one defines Rgb3 and such for us.        */


/*----------------------------------------------------------------------------
 * set up the host libraries we need...
 *--------------------------------------------------------------------------*/

// #define HLIB_TYPE_1 AA_POCOLIB	/* this one is always required in a POE */
// #include <hliblist.h>

/*****************************************************************************
* check a Popot for use as read-only in our code.
*	we ensure the pointer is not NULL, and that it points to memory within
*	the min/max range.	we *don't* check to make sure that any specific
*	amount of space is available between the pointer and the max range,
*	because we're not going to write into the space anyway.
****************************************************************************/
static Errcode popot_rdcheck(Popot *p)
{
	Errcode err;

	if (p->pt == NULL)
		err = Err_null_ref;
	else if (p->pt < p->min)
		err = Err_index_small;
	else if (p->pt > p->max)
		err= Err_index_big;
	else
		err = Success;

	return builtin_err = err;
}

/*****************************************************************************
* check a Popot for use as read/write in our code.
*	we ensure the pointer is not NULL, and that it points to memory within
*	the min/max range.	we also check to make sure that the requested
*	amount of space is available between the pointer and the max range.
****************************************************************************/
static Errcode popot_wrcheck(Popot *p, int size)
{
	Errcode err;

	if (p->pt == NULL)
		err = Err_null_ref;
	else if (p->pt < p->min)
		err = Err_index_small;
	else if (((char *)p->pt + size - 1) > p->max)
		err = Err_buf_too_small;
	else
		err = Success;

	return builtin_err = err;
}

/*****************************************************************************
* convert unsigned byte rgb triplets used internally to integer triplets.
****************************************************************************/
static void rgb_to_irgb(void *irgb, void *rgb, int count)
{
	unsigned int  *pout = irgb;
	unsigned char *pin	= rgb;

	while (--count >= 0) {
		*pout++ = *pin++;
		*pout++ = *pin++;
		*pout++ = *pin++;
	}
}

/*****************************************************************************
* find the difference between two rgb colors.
*
*	this routine just checks parameter validity, then calls the assembler
*	routine that does the real work.
****************************************************************************/
int safe_color_dif(Popot pcolor1, Popot pcolor2)
{
	if (popot_rdcheck(&pcolor1) ||
		popot_rdcheck(&pcolor2))
		return builtin_err;

	return color_dif((Rgb3 *)pcolor1.pt, (Rgb3 *)pcolor2.pt);
}

/*****************************************************************************
* find the rgb color in a table that is closest to the requested color.
*
*	this routine just checks parameter validity, then calls the assembler
*	routine that does the real work.
****************************************************************************/
int safe_closestc(Popot pcolor, Popot ptab, int tabcount)
{
	if (popot_rdcheck(&pcolor) ||
		popot_rdcheck(&ptab))
		return builtin_err;

	if (tabcount < 0)
		return builtin_err = Err_parameter_range;

	return closestc((Rgb3 *)pcolor.pt, (Rgb3 *)ptab.pt, tabcount);

}

/*****************************************************************************
* return current menu rgb colors and the user's preferred menu rgb colors.
*
*	either pointer may be NULL, indicating that the caller doesn't want
*	that set of colors returned.  (both could be NULL, but that would be
*	pretty pointless, huh?)
****************************************************************************/
void menu_colors(Popot current, Popot preferred)
{
	Rgb3	*pl_currents;
	Rgb3	*pl_preferreds;

	GetMenuColors(NULL, &pl_currents, &pl_preferreds);

	if (current.pt != NULL) {
		if (popot_wrcheck(&current, 5*sizeof(Rgb3)))
			return;
		rgb_to_irgb(current.pt, pl_currents, 5);
	}

	if (preferred.pt != NULL) {
		if (popot_wrcheck(&preferred, 5*sizeof(Rgb3)))
			return;
		rgb_to_irgb(preferred.pt, pl_preferreds, 5);
	}

	return;
}

/*****************************************************************************
* return current menu color indexes.
*
*	the indexes (whatever happened to the word indicies anyway?) are the
*	five slots in the color palette which are currently being used for the
*	menu colors.  in other words, if you used each of the five indexes to
*	go look in the color palette, you'd find the rgb values returned by
*	the 'currents' portion of the function above.
****************************************************************************/
void menu_indexes(Popot pindexes)
{
	int 	i;
	Pixel	*indexes;
	int 	*pret;

	if (popot_wrcheck(&pindexes, 5*sizeof(int)))
		return;

	GetMenuColors(&indexes, NULL, NULL);

	pret = pindexes.pt;
	for (i = 0; i < 5; ++i)
		*pret++ = *indexes++;

	return;
}

/*****************************************************************************
* check the validity of the poco pointers, then call the real-work routine.
****************************************************************************/
void safe_rgb2hls(int r, int g, int b, Popot ph, Popot pl, Popot ps)
{

	r &= 0x00FF;	/* force color components to be in 0-255 range */
	g &= 0x00FF;
	b &= 0x00FF;

	if (popot_wrcheck(&ph, sizeof(int)) ||
		popot_wrcheck(&pl, sizeof(int)) ||
		popot_wrcheck(&ps, sizeof(int)))
		return;

	rgb_to_hls(r, g, b, ph.pt, pl.pt, ps.pt);

	return;
}

/*****************************************************************************
* check the validity of the poco pointers, then call the real-work routine.
****************************************************************************/
void safe_hls2rgb(Popot pr, Popot pg, Popot pb, int h, int l, int s)
{

	h &= 0x00FF;	/* force color components to be in 0-255 range */
	l &= 0x00FF;
	s &= 0x00FF;

	if (popot_wrcheck(&pr, sizeof(int)) ||
		popot_wrcheck(&pg, sizeof(int)) ||
		popot_wrcheck(&pb, sizeof(int)))
		return;

	hls_to_rgb(pr.pt, pg.pt, pb.pt, h, l, s);

	return;
}

/*----------------------------------------------------------------------------
 * Setup rexlib/pocorex interface structures...
 *--------------------------------------------------------------------------*/

static Lib_proto poe_calls[] = {
  { safe_color_dif, "int  ColorDifference(int *pcolor1, int *pcolor2);" },
  { safe_closestc,	"int  ClosestColor(int *pcolor, int *ptab, int tabcount);" },
  { menu_colors,	"void GetMenuRGB(int *current, int *preferred);" },
  { menu_indexes,	"void GetMenuIndexes(int *indexes);" },
  { safe_rgb2hls,	"void RgbToHls(int r, int g, int b, int *h, int *l, int *s);"},
  { safe_hls2rgb,	"void HlsToRgb(int *r, int *g, int *b, int h, int l, int s);"},
};

Setup_Pocorex(NOFUNC, NOFUNC, "Color Utilities v1.1", poe_calls);

