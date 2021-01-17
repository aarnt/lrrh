# BadWolf
Minimalist and privacy-oriented WebKitGTK+ browser.

Homepage: <https://hacktivis.me/projects/badwolf>

```
Copyright © 2019-2020 Badwolf Authors <https://hacktivis.me/projects/badwolf>
SPDX-License-Identifier: BSD-3-Clause
```

The name is a reference to BBC’s Doctor Who Tv serie, I took it simply because I wanted to have a specie in the name, like some other web browsers do, but doesn’t go into the “gentle” zone.

## Differencies
Comparing from other small WebKit browsers for unixes found in the wild:

- Independent of environment, should just work if GTK and WebKitGTK does
- Storing data should be:
  - explicit and optionnal (ie. Applying preferences doesn't imply Saving to disk)
  - not queryabe by WebKit (so the web can't use it)
  - done in a standard format (like XBEL for bookmarks)
- Static UI, no element should be added at runtime, this is to avoid potential tracking via viewport changes
- Small codebase, should be possible to read and understand it completely over an afternoon.
- Does not use modal editing (from vi) as that was designed for editing, not browsing
- UTF-8 encoding by default

Motivation from other clients <https://hacktivis.me/articles/www-client%20are%20broken>

## Contributing
### Translations
You need to have gettext installed. If you want a GUI, poedit exists and Weblate is a good web platform that I might consider hosting at some point.

- Syncing POT file with the source code: ``make po/messages.pot``
- Syncing PO file with the POT file: ``make po/de.po``
- Initialising a new PO file (example for German, `de_DE`): ``msginit -l de_DE -i po/messages.pot -o po/de.po``

## Contacts / Discussions
- IRC: `#badwolf-browser` on FreeNode
- Matrix (bridge): <https://matrix.to/#/#freenode_#badwolf-browser:matrix.org>

## Repositories
### git
- Main: <https://hacktivis.me/git/badwolf/>, <git://hacktivis.me/git/badwolf.git>
- Mirror: <https://gitlab.com/lanodan/badWolf.git>, this one can also be used if you prefer tickets/PRs over emails

### release assets
- Main: <https://hacktivis.me/releases/>
- Mirror: <https://gitlab.com/lanodan/badWolf/tags>

- `*.tar.*` files are tarballs archives to be extracted with a program like `tar(1)`, GNU tar and LibArchive bsdtar are known to work.
- `*.sig` files are OpenPGP signatures done with my [key](https://hacktivis.me/key.asc)(`DDC9 237C 14CF 6F4D D847  F6B3 90D9 3ACC FEFF 61AE`).
- `*.sign` files are minisign (OpenBSD `signify(1)` compatible) signatures, they key used for it can be found at <https://hacktivis.me/release/signify/> as well as other places (feel free to ping me to get it)

## Manual Installation
Dependencies are:
- C11 Compiler (such as clang or gcc)
- [WebKitGTK](https://webkitgtk.org/), only the latest stable is supported
- POSIX make with extension for shell in variables (works with GNU, {Net,Free,Open}BSD)
- A pkg-config implementation (pkgconf is recommended)
- (optionnal) gettext implementation (such as GNU Gettext)

Compilation is done with `make`, install with `make install` (`DESTDIR` and `PREFIX` environment variables are supported, amongs other common ones). An example AppArmor profile is provided at `usr.bin.badwolf`, please do some long runtime checks before shipping it or a modified version, help can be provided but with no support.

You'll also need inkscape (command line only) if you want to regenerate the icons, for example after modifying them or adding a new size. These aren't needed for normal installation as it is bundled.

## Notes
Most of the privacy/security stuff will be done with patches against WebKit as quite a lot isn’t into [WebKitSettings](https://webkitgtk.org/reference/webkit2gtk/stable/WebKitSettings.html) and with generic WebKit extensions that should be resuseable.
