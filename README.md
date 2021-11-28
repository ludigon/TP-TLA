# Trabajo práctico TLA
asdf
## Nombre del Lenguaje
asdf
## Autores
asdf
## Prerequisitos
asdf
## Instrucciones de compilación

### Compilación del compilador:
Estando parado en el directorio del proyecto: `make`.

Esto genera el ejecutable del compilador `golfit`.

`make clean` elimina los archivos de compilación.

### Compilación de programas .gol a .c:
`./golfit < programa.gol`

Genera .c por salida estándar.

### Compilacion de .gol a binario:
`./compile.sh < programa.gol`

El archivo de salida es `output`.

## Progrmas de ejemplo

Los programas de ejemplo se encuentran en la carpeta `examples`.

Se pueden compilar a binario mediante las instrucciones previamente dadas: `./compile.sh < exmples/collatz.gol`.

### Collatz
Pide como entrada un numero e imprime la orbita de la conjetura de Collatz empezando por ese numero.

### Fibonacci
Pide como entrada una cantidad N e imprime los primeros N numeros de la secuencia de Fibonacci.

### Guess
Pide un numero por entrada estándar hasta que sea el mismo que haya sido definido previamente en el codigo.

### Prime
Determina si un numero ingresado es primo o no.

### Primes
Lista los N primeros numeros primos positivos. N se ingresa por entrada estandar.

## Ideas y objetivos
asdf
## Instrucciones de uso
asdf
