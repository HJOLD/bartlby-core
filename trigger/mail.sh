#!/bin/sh
######
#
#  $1 == email
#  $2 == icq
#  $3 == name
#  $4 == msg
#####



echo -e $4|sendmail "bartlby-email" $1
echo "Mail sent to $1";