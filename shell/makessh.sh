#!/bin/bash
# usage: makessh.sh hostname

to_host=$1

rsa_pub="/home/img/.ssh/id_rsa.pub"

if [ ! -f $rsa_pub ];then
        ssh-keygen -t rsa 
fi

ssh "$to_host" 'test -d .ssh || mkdir -m 0700 .ssh ; cat >>.ssh/authorized_keys && chmod 0600 .ssh/*' < ~/.ssh/id_rsa.pub
