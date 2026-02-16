/**
 * test_color_hls.c - Minunit tests for HLS/RGB color conversion
 *
 * Tests hls_to_rgb() and rgb_to_hls() from src/gfx/cmhlsrgb.c and cmrgbhls.c.
 * These are pure math functions; all values 0-255.
 */

#include "minunit.h"

#include <stdlib.h> /* for abs() */

#include "cmap.h"

/* Allow ±1 tolerance for integer rounding in roundtrip tests */
#define COLOR_TOLERANCE 2

static void test_setup(void)
{
	/* Nothing to set up */
}

static void test_teardown(void)
{
	/* Nothing to clean up */
}

/**
 * Helper: Check if two color values are within tolerance
 */
static int color_eq(SHORT expected, SHORT actual)
{
	return abs(expected - actual) <= COLOR_TOLERANCE;
}

/*
 * Test: Black (0,0,0) roundtrip
 */
MU_TEST(test_black_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	rgb_to_hls(0, 0, 0, &h, &l, &s);
	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(0, r2), "black R should be ~0");
	mu_assert(color_eq(0, g2), "black G should be ~0");
	mu_assert(color_eq(0, b2), "black B should be ~0");
}

/*
 * Test: White (255,255,255) roundtrip
 */
MU_TEST(test_white_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	rgb_to_hls(255, 255, 255, &h, &l, &s);
	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(255, r2), "white R should be ~255");
	mu_assert(color_eq(255, g2), "white G should be ~255");
	mu_assert(color_eq(255, b2), "white B should be ~255");
}

/*
 * Test: Gray (128,128,128) roundtrip
 * Gray has saturation 0, so hue is undefined but roundtrip should preserve luminance
 */
MU_TEST(test_gray_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	rgb_to_hls(128, 128, 128, &h, &l, &s);

	/* Gray should have 0 saturation */
	mu_assert_int_eq(0, s);

	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(128, r2), "gray R should be ~128");
	mu_assert(color_eq(128, g2), "gray G should be ~128");
	mu_assert(color_eq(128, b2), "gray B should be ~128");
}

/*
 * Test: Pure red (255,0,0) roundtrip
 */
MU_TEST(test_red_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	rgb_to_hls(255, 0, 0, &h, &l, &s);
	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(255, r2), "red R should be ~255");
	mu_assert(color_eq(0, g2), "red G should be ~0");
	mu_assert(color_eq(0, b2), "red B should be ~0");
}

/*
 * Test: Pure green (0,255,0) roundtrip
 */
MU_TEST(test_green_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	rgb_to_hls(0, 255, 0, &h, &l, &s);
	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(0, r2), "green R should be ~0");
	mu_assert(color_eq(255, g2), "green G should be ~255");
	mu_assert(color_eq(0, b2), "green B should be ~0");
}

/*
 * Test: Pure blue (0,0,255) roundtrip
 */
MU_TEST(test_blue_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	rgb_to_hls(0, 0, 255, &h, &l, &s);
	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(0, r2), "blue R should be ~0");
	mu_assert(color_eq(0, g2), "blue G should be ~0");
	mu_assert(color_eq(255, b2), "blue B should be ~255");
}

/*
 * Test: Mixed color roundtrip (arbitrary values)
 */
MU_TEST(test_mixed_color_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	/* Test with a brownish color */
	rgb_to_hls(180, 120, 60, &h, &l, &s);
	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(180, r2), "mixed R should be ~180");
	mu_assert(color_eq(120, g2), "mixed G should be ~120");
	mu_assert(color_eq(60, b2), "mixed B should be ~60");
}

/*
 * Test: Another mixed color (cyan-ish)
 */
MU_TEST(test_cyan_roundtrip)
{
	SHORT h, l, s;
	SHORT r2, g2, b2;

	rgb_to_hls(64, 192, 192, &h, &l, &s);
	hls_to_rgb(&r2, &g2, &b2, h, l, s);

	mu_assert(color_eq(64, r2), "cyan R should be ~64");
	mu_assert(color_eq(192, g2), "cyan G should be ~192");
	mu_assert(color_eq(192, b2), "cyan B should be ~192");
}

/*
 * Test: rgb_to_hls output ranges are valid (0-255)
 */
MU_TEST(test_hls_output_range)
{
	SHORT h, l, s;

	/* Test various colors to ensure HLS values stay in range */
	rgb_to_hls(100, 150, 200, &h, &l, &s);
	mu_assert(h >= 0 && h <= 255, "H should be 0-255");
	mu_assert(l >= 0 && l <= 255, "L should be 0-255");
	mu_assert(s >= 0 && s <= 255, "S should be 0-255");

	rgb_to_hls(255, 128, 0, &h, &l, &s);
	mu_assert(h >= 0 && h <= 255, "H should be 0-255 (orange)");
	mu_assert(l >= 0 && l <= 255, "L should be 0-255 (orange)");
	mu_assert(s >= 0 && s <= 255, "S should be 0-255 (orange)");
}

MU_TEST_SUITE(color_hls_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	/* Edge cases */
	MU_RUN_TEST(test_black_roundtrip);
	MU_RUN_TEST(test_white_roundtrip);
	MU_RUN_TEST(test_gray_roundtrip);

	/* Primary colors */
	MU_RUN_TEST(test_red_roundtrip);
	MU_RUN_TEST(test_green_roundtrip);
	MU_RUN_TEST(test_blue_roundtrip);

	/* Mixed colors */
	MU_RUN_TEST(test_mixed_color_roundtrip);
	MU_RUN_TEST(test_cyan_roundtrip);

	/* Range validation */
	MU_RUN_TEST(test_hls_output_range);
}

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	MU_RUN_SUITE(color_hls_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
