# Aide pour les tests bats

check() {
	res=0
	if [ "$status" != "$1" ]; then
		echo "Test échoué: le code de retour est $status ; $1 était attendu"
		res=1
	fi

	if [ "$output" != "$2" ]; then
		echo "Test échoué: la sortie inattendue est"
		echo "$output"
		res=2
	fi

	return "$res"
}

setup() {
	true
}

teardown() {
	kill %% 2> /dev/null
	pchild=`pgrep -s 0 sleep`
	kill $pchild 2> /dev/null
	true
}

# Redefinition eventuelle de `run`
eval orig_"$(declare -f run)"
run() {
	orig_run "$@" 3>&-
}
