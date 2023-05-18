#include "unity.h"
#include "muggle/c/muggle_c.h"
#include "hpkg/cli/utils/path.h"

void setUp()
{
}

void tearDown()
{
}

void test_utils_expand_home()
{
	char buf[1024];
	const char *filepath = hpkg_path_expand("~", buf, sizeof(buf));
	TEST_ASSERT_NOT_NULL(filepath);
}

void test_utils_expand()
{
	const char *filepath = "~/.hpkg/settings.yml";
	char buf[MUGGLE_MAX_PATH];

	filepath = hpkg_path_expand(filepath, buf, sizeof(buf));
	TEST_ASSERT_NOT_NULL(filepath);
}

int main()
{
	UNITY_BEGIN();

	RUN_TEST(test_utils_expand_home);
	RUN_TEST(test_utils_expand);

	return UNITY_END();
}
