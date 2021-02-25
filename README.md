
## Man    [![test compilation](https://github.com/husseinnahle/penv/actions/workflows/compile.yml/badge.svg)](https://github.com/husseinnahle/penv/actions/workflows/compile.yml)

`penv` (*process environment variable*) affiche la valeur d'une variable d'environnement d'un processus quelconque.

Usage

* `penv [-p] PID NOM`

Affiche la valeur de la variable d'environnement NOM du processus de pid PID.

```
$ env HELLO=WORLD sleep 10 &
$ penv $! HELLO
WORLD
$ echo $?
0
```

Si la variable d'environnement n'existe pas, `penv` n'affiche rien et termine avec le code de retour 1.

```
$ sleep 10 &
$ penv $! NEXISTEPAS
$ echo $?
1
```

Si la variable d'environnement existe mais est la chaine vide, une ligne vide est affichée et le code de retour est 0.

```
$ env BYE= sleep 10 &
$ penv $! BYE

$ echo $?
0
```

En cas d'erreur (processus inexistant, problème de droits, etc.) `penv` affiche un message d'erreur et retourne 2.


Avec l'option `-p`, `penv` cherche la variable d'environnement dans le processus parent de PID.
Cette option peut servir par exemple à déterminer si un processus et son parent possèdent une même variable d'environnement.

```
$ env HELLO=WORLD sh -c "env HELLO=MONDE sleep 10; true" &
$ psleep=`pgrep -s 0 sleep`
$ # ici on a deux processus:
$ # * `sh` de pid $! avec HELLO=WORLD 
$ # * `sleep` de pid $psleep de avec HELLO=MONDE
$ # le `true` fait qu'il y a deux commandes ce qui force `sh` à creer un sous-processus
$ penv $! HELLO
WORLD
$ penv $psleep HELLO
MONDE
$ penv -p $psleep HELLO
WORLD
```
