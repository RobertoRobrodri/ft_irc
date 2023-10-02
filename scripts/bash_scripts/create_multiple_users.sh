#!/bin/bash

for i in {1..99}
do
	./../expect_scripts/create_user_test.exp nick_$i user_$i &
done