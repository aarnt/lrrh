# Decisions taken over the project lifetime
## What is this?
See <https://akazlou.com/posts-output/2015-11-09-every-project-should-have-decisions/>
Bascially, the idea is to have a file in your projects repo, where you record 
the decisions you make over the course of the project's lifetime.
Rewriting history will not be done but Post-Scriptum notices will.

## 2020-02-04 & 2020-02-09 : Use BSD extensions in the Makefile
The BSD extensions += and ?= for variable extension is used to avoid having to explicitely pass a list of variables to make, thus making build recipies simpler.
They are also compatible with GNU make so portability shouldn't have significantly dropped.
Which also meant dropping `.POSIX` into the file so `CC` wouldn't be set by default to `c99` (which isn't what I want, the codebase is in c11).

## 2020-01-30 02:59 : Make window declaration static
This allows to avoid a quite useless memory allocation that would only be freed at the end of the program.

## 2020-01-22 15:30 : Update copyright to Badwolf Authors
I don't want copyright assignment to me or any organisation, it should belong to every contributor, meaning that any copyright change will need to be accepted by them or their contribution would need to be replaced if need to be.

## 2020-01-08 08:42:00Z : Start of the decisions.md file
From memory and git log:

- Changelog is done when preparing for a release, it is copied into the git tag description. This means that there isn't a central file and isn't a changelog entry added with commits.
- OpenPGP is deprecated in favor of signify, reasons being security and portability.
- Prefer using lower and more standard/ubiquitous layers for implementations, one example is the about information with printf and Gtk Dialog rather than a custom scheme in WebKitGTK.
	- using GLib instead of POSIX was done at first but the quality/behaviour of GLib (like `g_malloc()` calling `abort()` on errors) reverted this decision.
- WebKitGTK was picked because it is the only WebKit port which is maintained enough without a bus-factor (consider giving QtWebKit a hand too?).
- The `.clang-format` file was copied from the one living in my home directory:
  - function declaration is at the start of the line to allow jumping to it with a simple editor
  - braces on their own lines to allow for comments space
  - "tabs for indentation and space for alignement" style is used for flexibility
- mdoc was picked over other formats for the manpage, as it is implemented in mandoc and GNU groff, much simpler to read/write and transform to other formats (like HTML or Markdown).
- Markdown was picked over other formats for informal documentation, it is a simple format supported by most forges, a format which is easier to write and parse could be considered
- Proprietary systems are not supported for their lack of auditability and transparency.
- C was picked because:
  1. I wanted to avoid being limited by binding completeness or model
  2. I think it is a good language which is fit for this purpose provided you are careful enough (such as using LLVM to it's full potential for checks)
  3. It is portable and doesn't introduces more dependencies than is already needed
- POSIX was picked for it's portability (only known one where it's absent is Windows but it's not going to be supported)
- Writing a portable (almost POSIX-only) Makefile with `pkg-config(1)` was picked over other build systems which tend to be over-complicated and rely on only one implementation.
- Originally the URL entry accepted only valid URLs, this was changed to a bit of heuristics for fixing it when it was entered by the user.
- No search entry is provided, this is to encourage users to avoid search engines as much as possible, support for SmartBookmarks with keywords (similar to DuckDuckGo/Searx !bangs) will be added after support for suggestions via bookmarks
- Bookmarks are first-class citizens as they are data explicitely controlled by the user
- No browsing history is provided, this is to encourage to use and improve bookmarks, it will be added later on with data retention limits, ability to wipe it and non-reachable from WebKit (and so the Wild Web).
- Javascript is turned off by default with a checkbox to enable it temporarly, Permissions Requests (microphone, geolocation, …) are all denied, something similar to uMatrix and NoScript will eventually be done to fix this
