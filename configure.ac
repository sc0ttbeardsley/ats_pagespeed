#
# Copyright (c) 2013 LinkedIn Corp. All rights reserved. 
# Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file
# except in compliance with the License. You may obtain a copy of the license at
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software distributed under the
# License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied.
#

AC_INIT([ats_pagespeed], [0.1.0], [bgeffon@linkedin.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

AC_PROG_CXX
AC_PROG_CPP
AC_PROG_CXXCPP
AC_PROG_RANLIB
AC_PROG_LIBTOOL

AC_DEFUN([ATS_PAGESPEED_ADDTO], [
  if test "x$$1" = "x"; then
    test "x$verbose" = "xyes" && echo "  setting $1 to \"$2\""
    $1="$2"
  else
    ats_pagespeed_addto_bugger="$2"
    for i in $ats_pagespeed_addto_bugger; do
      ats_pagespeed_addto_duplicate="0"
      for j in $$1; do
        if test "x$i" = "x$j"; then
          ats_pagespeed_duplicate="1"
          break
        fi
      done
      if test $ats_pagespeed_addto_duplicate = "0"; then
        test "x$verbose" = "xyes" && echo "  adding \"$i\" to $1"
        $1="$$1 $i"
      fi
    done
  fi
])

AC_CONFIG_FILES([
   Makefile
   src/Makefile
  ])
  
AC_ARG_WITH([pagespeed],
  [AS_HELP_STRING([--with-pagespeed],[path to built pagespeed source [default=../lib/src]])],
  [pagespeed=$withval],
  [pagespeed=../lib/src/]
)
AC_SUBST(pagespeed)

AC_OUTPUT
