# LRRH, another tiny & functional WebKitGTK browser

![Main window](https://raw.githubusercontent.com/aarnt/lrrh/master/lrrh-mainwindow.png)

LRRH is a [BadWolf](https://hacktivis.me/projects/badwolf) derived browser with different keybindings.
It stands for "Little Red Riding Hood", an European fairy tale about a young girl and a Big Bad Wolf.


These are the keybinds used in LRRH:

Key           | Action
--            | --
Esc           | Stop loading page / Clear search field and return focus to page
F1            | Show about LRRH
F5            | Reload page
F12           | Show inspector view
Alt+(1 to 9)  | Goto first, second, third.. nineth tab
Alt+Left      | Goto previous page
Alt+Right     | Goto next page
Ctrl+0        | Use 100% zoom factor
Ctrl+plus     | Increase zoom factor by 10%
Ctrl+minus    | Decrease zoom factor by 10%
Ctrl+Tab      | Switch to the next tab
Ctrl+Shift+Tab| Switch to the previous tab
Ctrl+C        | Copy statusbar text (hyperlink) to clipboard
Ctrl+D        | Open https://lite.duckduckgo.com in new tab
Ctrl+F        | Goto search widget
Ctrl+G        | Find next
Ctrl+H        | Return focus to page
Ctrl+I        | Toggle show images on/off
Ctrl+J        | Toggle enable javascript on/off
Ctrl+L        | Goto location widget
Ctrl+N        | Open link in new tab
Ctrl+P        | Show print dialog
Ctrl+Q        | Quit LRRH
Ctrl+R        | Reload page
Ctrl+Shift+G  | Find previous
Ctrl+T        | Open new empty tab
Ctrl+W        | Close current tab
Ctrl+X        | Open https://searx.info in new tab
Ctrl+Z        | Translate selected text to pt-br using Google Translator

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

