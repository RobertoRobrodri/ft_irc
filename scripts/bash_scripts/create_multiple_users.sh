#!/bin/bash

for i in {1..100}
do
	sleep $i && ./../expect_scripts/create_user_test.exp nick_$i user_$i &
done