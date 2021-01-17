#!/bin/sh
# Transforms an icon size from the Icon Theme Specification to rsvg-convert(1) options
#sed -r -e 's;^([^x]*)x([^@]*)$;-w \1 -h \2;' -e 's;^([^x]*)x([^@]*)@(.*)$;-w \1 -h \2 -z \3;'
sed -r 's;^([^x]*)x([^@]*)$;-w \1 -h \2;'
