# como usar:
# execute no terminal: source commands.sh
# agora é só executar, da lista abaixo, o comando desejado.
# OBS: comandos para Ubuntu, pode ser que mude de SO para SO

alias pth-compile="gcc -o game_of_life_pthread game_of_life_pthread.c -pthread"
alias pth-run="./game_of_life_pthread"

alias omp-compile="gcc -o game_of_life_openmp -fopenmp game_of_life_openmp.c"
alias omp-run="./game_of_life_openmp"

alias seq-compile="gcc -o game_of_life game_of_life.c"
alias seq-run="./game_of_life"