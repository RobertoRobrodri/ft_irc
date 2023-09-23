#!/bin/bash

for i in {1..5}
do
	sleep $i && ./../expect_scripts/create_user_test.exp nick_$i user_$i &
done