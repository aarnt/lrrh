# LRRH
Minimalist and privacy-oriented WebKitGTK+ browser cloned from [BadWolf](https://hacktivis.me/projects/badwolf), using different keybindings.

LRRH stands for "Little Red Riding Hood", an European fairy tale about a young girl and a Big Bad Wolf.


These are the keybinds used in LRRH

Key         | Action
--          | --
Esc         | Stop loading page
F1          | Show about LRRH
F5          | Reload page
F12         | Show inspector view
Alt+(1 to 9)| Goto first, second, third.. nineth page
Alt+Left    | Goto previous page
Alt+Right   | Goto next page
Ctrl+0      | Use 100% zoom factor
Ctrl+plus   | Increase zoom factor by 10%
Ctrl+minus  | Decrease zoom factor by 10%
Ctrl+Tab    | Switch to the next tab
Ctrl+T      | Open new empty tab
Ctrl+N      | Open link in new tab
Ctrl+D      | Open "https://lite.duckduckgo.com" in new tab
Ctrl+X      | Open "https://searx.info" in new tab
Ctrl+L      | Goto location widget
Ctrl+F      | Goto search widget
Ctrl+G      | Find next
Ctrl+Shift+G| Find previous
Ctrl+I      | Toggle show images on/off
Ctrl+J      | Toggle use javascript on/off
Ctrl+P      | Show print dialog
Ctrl+R      | Reload page
Ctrl+W      | Close current tab
Ctrl+Q      | Quit LRRH

## Manual Installation
Dependencies are:

- C11 Compiler (such as clang or gcc)
- [WebKitGTK](https://webkitgtk.org/), only the latest stable is supported
- [libxml-2.0](http://www.xmlsoft.org/), no known version limitation
- POSIX make with extension for shell in variables (works with GNU, {Net,Free,Open}BSD)
- A pkg-config implementation (pkgconf is recommended)
- (optionnal) gettext implementation (such as GNU Gettext)

Compilation is done with `make`, install with `make install` (`DESTDIR` and `PREFIX` environment variables are supported, amongs other common ones). An example AppArmor profile is provided at `usr.bin.lrrh`, please do some long runtime checks before shipping it or a modified version, help can be provided but with no support.

You'll also need inkscape (command line only) if you want to regenerate the icons, for example after modifying them or adding a new size. These aren't needed for normal installation as it is bundled.

