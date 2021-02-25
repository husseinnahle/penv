#!/usr/bin/env bats

load test_helper

@test "hello world" {
	env HELLO=WORLD sleep 10 &

	run ./penv $! HELLO

	check 0 "WORLD"

	kill $!
}

@test "envvar n'existe pas" {
	env BONJOUR=LE=MONDE sleep 10 &

	run ./penv $! "NEXISTEPAS"

	check 1 ""

	kill $!
}

@test "envvar vide" {
	env BYE= sleep 10 &

	run ./penv $! "BYE"

	check 0 ""

	kill $!
}

@test "process n'existe pas" {
	run ./penv 1234567890 "HOME"

	[ "$status" = "2" ]
	[ "$output" != "" ]
}

@test "bonjour le monde" {
	env BONJOUR=LE=MONDE sleep 10 &

	run ./penv $! BONJOUR

	check 0 "LE=MONDE"

	kill $!
}

@test "child hello world" {
	# bats
	# \- sh (HELLO=WORLD)
	#    \- sleep (HELLO=MONDE)
	env HELLO=WORLD sh -c "env HELLO=MONDE sleep 10" &
	pchild=`pgrep -s 0 sleep`

	run ./penv $! HELLO
	check 0 "WORLD"

	run ./penv $pchild HELLO
	check 0 "MONDE"

	run ./penv -p $pchild HELLO
	check 0 "WORLD"

	kill $pchild
}

@test "child envvar n'existe pas" {
	# bats
	# \- sh (!HELLO)
	#    \- sleep (HELLO=MONDE)
	sh -c "env HELLO=MONDE sleep 10" &
	pchild=`pgrep -s 0 sleep`

	run ./penv $! HELLO
	check 1 ""

	run ./penv $pchild HELLO
	check 0 "MONDE"

	run ./penv -p $pchild HELLO
	check 1 ""

	kill $pchild
}

@test "child n'existe pas" {
	run ./penv -p 1234567890 "HOME"

	[ "$status" = "2" ]
	[ "$output" != "" ]
}

@test "no env" {
	env -i sleep 10 &

	run ./penv $! "HOME"

	check 1 ""

	kill $!
}

@test "big env" {
	BIG=`strings /bin/bash | head -c 1k`

	env "BIG=$BIG" sleep 10 &

	run ./penv $! "BIG"

	check 0 "$BIG"

	kill $!
}
