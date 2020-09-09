#!/bin/bash

# ***************************************************************************
# * 
# * @file:merge_libs.sh 
# * @author:ebayboy@163.com 
# * @date:2020-09-09 15:46 
# * @version 1.0  
# * @description: Shell script 
# * @Copyright (c)  all right reserved 
#* 
#**************************************************************************/ 

# merge .libsmartfitler.a and libhs.a

cat <<EOF > libsmartfilter_hs.mri
create libsmartfilter_hs.a
addlib .libsmartfilter.a
addlib ./lib/libhs.a
save
end
EOF

ar -M < libsmartfilter_hs.mri 
cp -af libsmartfilter.a  ./build/lib/

exit 0

