// BadWolf: Minimalist and privacy-oriented WebKitGTK+ browser
// Copyright © 2019-2020 Badwolf Authors <https://hacktivis.me/projects/badwolf>
// SPDX-License-Identifier: BSD-3-Clause

#include "uri.h"

#include <glib.h>
#include <glib/gi18n.h>

static void
badwolf_ensure_uri_scheme_test(void)
{
	const gchar *fallback = "about:blank";

	struct
	{
		const gchar *expect;
		const gchar *text;
		gboolean try_file;
	} cases[] = {
	    //
	    {"http://uri.c", "http://uri.c", FALSE},
	    {"http://uri.c", "http://uri.c", TRUE},
	    {"file:///dev/null", "file:///dev/null", FALSE},
	    {"file:///dev/null", "file:///dev/null", TRUE},
	    {fallback, NULL, FALSE},
	    {fallback, NULL, TRUE},
	    {fallback, "", FALSE},
	    {fallback, "", TRUE},
	    {"http:///dev/null", "/dev/null", FALSE},
	    {"file:///dev/null", "/dev/null", TRUE},
	    {"http:///usr/../dev/null", "/usr/../dev/null", FALSE},
	    {"file:///dev/null", "/usr/../dev/null", TRUE},
	    {"http://example.org", "example.org", FALSE},
	    {"http://example.org", "example.org", TRUE},
	    {"http://", "http://", FALSE},
	    {"http://", "http://", TRUE},
	    {"http://badwolf.c", "badwolf.c", FALSE} //
	};

	for(size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
	{
		g_info("badwolf_ensure_uri_scheme(\"%s\", %s)",
		       cases[i].text,
		       cases[i].try_file ? "TRUE" : "FALSE");

		const gchar *got = badwolf_ensure_uri_scheme(cases[i].text, cases[i].try_file);

		if(g_strcmp0(got, cases[i].expect) != 0)
		{
			g_error("expected: \"%s\", got: \"%s\"", cases[i].expect, got);
		}
	}
}

int
main(int argc, char *argv[])
{
	g_test_init(&argc, &argv, NULL);

	g_test_add_func("/badwolf_ensure_uri_scheme/test", badwolf_ensure_uri_scheme_test);

	return g_test_run();
}
