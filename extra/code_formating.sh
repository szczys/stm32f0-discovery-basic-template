#!/bin/bash

# apply kdelibs coding style to all c, cpp and header files in and below the current directory 
# 
# the coding style is defined in http://techbase.kde.org/Policies/Kdelibs_Coding_Style 
# 
# requirements: installed astyle 
#
# (modified by me)

astyle --indent=spaces=4 --brackets=break \
      --indent-labels --pad-oper --unpad-paren --pad-header \
      --keep-one-line-statements --convert-tabs \
      --indent-preprocessor \
      `find -type f -name '*.c'` `find -type f -name '*.cpp'` `find -type f -name '*.h'`
