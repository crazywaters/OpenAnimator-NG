/**
 * test_fnsplit.c - Minunit tests for fnsplit() validation paths
 *
 * Tests the fnsplit() function from src/fileio/fpnsplit.c which splits a path
 * into device, directory, filename, and suffix components.
 */

#include "minunit.h"

#include <limits.h>
#include <string.h>

#include "errcodes.h"
#include "filepath.h"

/* Buffer sizes from filepath.h plus null terminator */
static char device[DEV_NAME_LEN + 1];
static char dir[MAX_DIR_LEN + 1];
static char file[MAX_FILE_NAME_LEN + 1];
static char suffix[MAX_SUFFIX_NAME_LEN + 2]; /* +2 for dot and null */

static void test_setup(void)
{
	/* Clear buffers before each test */
	memset(device, 0, sizeof(device));
	memset(dir, 0, sizeof(dir));
	memset(file, 0, sizeof(file));
	memset(suffix, 0, sizeof(suffix));
}

static void test_teardown(void)
{
	/* Nothing to clean up */
}

/*
 * Test: NULL path returns Err_null_ref
 */
MU_TEST(test_fnsplit_null_path)
{
	Errcode err = fnsplit(NULL, device, dir, file, suffix);
	mu_assert_int_eq(Err_null_ref, err);
}

/*
 * Test: Empty string path returns Err_null_ref
 */
MU_TEST(test_fnsplit_empty_path)
{
	Errcode err = fnsplit("", device, dir, file, suffix);
	mu_assert_int_eq(Err_null_ref, err);
}

/*
 * Test: Path longer than PATH_SIZE returns Err_dir_too_long
 */
MU_TEST(test_fnsplit_path_too_long)
{
	/* Create a path of exactly PATH_SIZE characters (which is >= PATH_SIZE) */
	char long_path[PATH_SIZE + 1];
	memset(long_path, 'a', PATH_SIZE);
	long_path[PATH_SIZE] = '\0';

	Errcode err = fnsplit(long_path, device, dir, file, suffix);
	mu_assert_int_eq(Err_dir_too_long, err);
}

/*
 * Test: Multiple dots - first dot starts suffix, file has no dot
 * "a.b.c" -> file="a", suffix=".b.c" (suffix starts at first dot)
 */
MU_TEST(test_fnsplit_multiple_dots)
{
	Errcode err = fnsplit("a.b.c", device, dir, file, suffix);
	mu_assert_int_eq(Success, err);
	mu_assert_string_eq("a", file);
	mu_assert_string_eq(".b.c", suffix);
}

/*
 * Test: Multiple dots with directory path
 * /foo/bar.baz.txt -> file="bar", suffix=".baz." (truncated to 5 chars)
 */
MU_TEST(test_fnsplit_multiple_dots_with_dir)
{
	Errcode err = fnsplit("/foo/bar.baz.txt", device, dir, file, suffix);
	mu_assert_int_eq(Success, err);
	mu_assert_string_eq("/foo/", dir);
	mu_assert_string_eq("bar", file);
	mu_assert_string_eq(".baz.", suffix); /* suffix truncated to 5 chars total */
}

/*
 * Test: Valid simple filename splits correctly
 */
MU_TEST(test_fnsplit_simple_file)
{
	Errcode err = fnsplit("hello.txt", device, dir, file, suffix);
	mu_assert_int_eq(Success, err);
	mu_assert_string_eq("", device);
	mu_assert_string_eq("", dir);
	mu_assert_string_eq("hello", file);
	mu_assert_string_eq(".txt", suffix);
}

/*
 * Test: Valid path with directory splits correctly
 */
MU_TEST(test_fnsplit_with_directory)
{
	Errcode err = fnsplit("/usr/local/bin/app.exe", device, dir, file, suffix);
	mu_assert_int_eq(Success, err);
	mu_assert_string_eq("", device);
	mu_assert_string_eq("/usr/local/bin/", dir);
	mu_assert_string_eq("app", file);
	mu_assert_string_eq(".exe", suffix);
}

/*
 * Test: File without suffix
 */
MU_TEST(test_fnsplit_no_suffix)
{
	Errcode err = fnsplit("Makefile", device, dir, file, suffix);
	mu_assert_int_eq(Success, err);
	mu_assert_string_eq("", device);
	mu_assert_string_eq("", dir);
	mu_assert_string_eq("Makefile", file);
	mu_assert_string_eq("", suffix);
}

/*
 * Test: Directory path with trailing slash
 */
MU_TEST(test_fnsplit_dir_only)
{
	Errcode err = fnsplit("/tmp/", device, dir, file, suffix);
	mu_assert_int_eq(Success, err);
	mu_assert_string_eq("", device);
	mu_assert_string_eq("/tmp/", dir);
	mu_assert_string_eq("", file);
	mu_assert_string_eq("", suffix);
}

MU_TEST_SUITE(fnsplit_suite)
{
	MU_SUITE_CONFIGURE(&test_setup, &test_teardown);

	/* Error cases */
	MU_RUN_TEST(test_fnsplit_null_path);
	MU_RUN_TEST(test_fnsplit_empty_path);
	MU_RUN_TEST(test_fnsplit_path_too_long);

	/* Multiple dots behavior */
	MU_RUN_TEST(test_fnsplit_multiple_dots);
	MU_RUN_TEST(test_fnsplit_multiple_dots_with_dir);

	/* Happy paths */
	MU_RUN_TEST(test_fnsplit_simple_file);
	MU_RUN_TEST(test_fnsplit_with_directory);
	MU_RUN_TEST(test_fnsplit_no_suffix);
	MU_RUN_TEST(test_fnsplit_dir_only);
}

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;

	MU_RUN_SUITE(fnsplit_suite);
	MU_REPORT();
	return MU_EXIT_CODE;
}
