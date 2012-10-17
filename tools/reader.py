# -*- coding: utf-8 -*-
# Raw byte reader and printer.
#
# by Ilja Everilä <saarni@gmail.com>
#
# ChangeLog:

import sys

while True:
    print "%d" % ord(sys.stdin.read(1)),
