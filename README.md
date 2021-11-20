# LRRH, another tiny & fast & functional WebKitGTK browser

![Main window](https://raw.githubusercontent.com/aarnt/lrrh/master/lrrh-mainwindow.png)

LRRH is a [BadWolf](https://hacktivis.me/projects/badwolf) derived browser with *sane keybindings*, dark mode, kiosk mode, middle mouse click to open hyperlinks on new tabs and Gemini/Gopher protocol support (trough external aplications).


It stands for "Little Red Riding Hood", an European fairy tale about a young girl and a Big Bad Wolf.


These are the keybinds used in LRRH:

Key           | Action
--            | --
Esc           | Stop loading page / Clear search field and return focus to page
F1            | Show about LRRH
F4            | Toggle Dark Mode on/off
F5            | Reload page
F11           | Toggle Kiosk Mode on/off
F12           | Show inspector view
Alt+(1 to 9)  | Goto first, second, third.. nineth tab
Alt+Left      | Goto previous page
Alt+Right     | Goto next page
Ctrl+0        | Use 100% zoom factor
Ctrl+plus     | Increase zoom factor by 10%
Ctrl+minus    | Decrease zoom factor by 10%
Ctrl+Tab      | Switch to the next tab
Ctrl+Shift+Tab| Switch to the previous tab
Ctrl+D        | Open https://lite.duckduckgo.com in new tab
Ctrl+F        | Goto search widget
Ctrl+G        | Find next
Ctrl+H        | Return focus to page
Ctrl+I        | Toggle show images on/off
Ctrl+J        | Toggle enable javascript on/off
Ctrl+L        | Goto location widget
Ctrl+P        | Show print dialog
Ctrl+Q        | Quit LRRH
Ctrl+R        | Reload page
Ctrl+Shift+C  | Copy statusbar text (hyperlink) to clipboard
Ctrl+Shift+G  | Find previous
Ctrl+Shift+N  | Open link in new tab
Ctrl+T        | Open new empty tab
Ctrl+W        | Close current tab
Ctrl+Z        | Translate selected text to the user system LANG using Google Translator

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

## Manual Installation
Dependencies are:

- POSIX-compatible Shell (ie. mrsh, dash, lksh)
- C11 Compiler (such as clang or gcc)
- [WebKitGTK](https://webkitgtk.org/), only the latest stable(2.32.0+) is supported
- [libxml-2.0](http://www.xmlsoft.org/), no known version limitation
- POSIX make with extension for shell in variables (works with GNU, {Net,Free,Open}BSD)
- A pkg-config implementation (pkgconf is recommended)
- (optional) gettext implementation (such as GNU Gettext)
- (optional, test) [mandoc](https://mdocml.bsd.lv/) (the command) for linting the manpage

Compilation is done with `./configure && make`, install with `make install` (`DESTDIR` and `PREFIX` environment variables are supported, amongs other common ones). An example AppArmor profile is provided at `usr.bin.badwolf`, please do some long runtime checks before shipping it or a modified version, help can be provided but with no support.

You'll also need inkscape (command line only) if you want to regenerate the icons, for example after modifying them or adding a new size. These aren't needed for normal installation as it is bundled.

## Notes
Most of the privacy/security stuff will be done with patches against WebKit as quite a lot isn’t into [WebKitSettings](https://webkitgtk.org/reference/webkit2gtk/stable/WebKitSettings.html) and with generic WebKit extensions that should be resuseable.
