#!/bin/sh

echo "Trigger $2"
echo $2|sendmail $1
