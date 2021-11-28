# Trabajo práctico TLA


## Nombre del Lenguaje

**Golf-IT**

---

## Autores
 - Digon, Lucia - 59030
 - Lopez Guzman, Pedro Jeremias - 60711
 -  Sartorio, Alan Gabriel - 61379

---

## Prerequisitos

Tener instalado yacc, flex y gcc en Linux

```$sudo apt-get install build-essential bison flex```

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

## Programas de ejemplo

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

La idea para el lenguaje surgió a partir de Code Golfing, una disciplina recreativa entre programadores en las que se realizan competencias con el objetivo de escribir programas que resuelvan determinados problemas en la menor cantidad de caracteres posibles. Si bien comenzó como una idea simple para jugar, generó una comunidad dedicada que llegó a crear lenguajes especializados para esto.

El objetivo es crear un lenguaje que sea útil para Code Golfing mientras que siga siendo más o menos legible. Los tokens tratarían de tener la menor longitud posible para poder escribir el programa de la manera más corta posible.

## Instrucciones de uso

### Ejecución de instrucciones

Golf-IT comienza ejecutandose desde la primera línea y usa los saltos de línea como separadores entre instrucciones.

### Variables

Las variables se definen con un nombre que debe empezar con una letra y que puede contener letras o numeros.
Golf-IT soporta dos tipos de variables, enteras y strings. Se pueden declarar de la siguiente manera:

 - Entera:  
    ```#a=5```
 - String:  
    ```^s="Hola Mundo!"```

También permite declarar variables enteras sin inicializarlas y la asignación de variables. Por ejem
```
#a
#b=3
a=b+1
```

### Operaciones

Golf-IT soporta las operaciones aritméticas de suma, resta, multiplicación, división y módulo.

```
#a=1+1
#b=a-2
#c=a*4
#d=c/2
#e=a%2
```

Además soporta operaciónes relacionales como:

```> >= < <= ==```

Y también operaciones lógicas AND, OR y NOT:

```& | !```

### Imprimir en pantalla

Podemos imprimir en pantalla utilizando el character "_":

```
#a=3
_"Hola Mundo\n"
_a
```

###  While loops

Se encierra la condición entre caracteres '@' y luego se agregan {} con saltos de línea adentro
```
#a=0
@a<5@{
a=a+1
}
```

### If/else

Se escribe la condición seguida de un '?' y luego {}, si se quiere añandir un else se escribe ":" seguido de {}
```
#a=1
a==2?{
_"a es dos\n"
}:{
_"a no es dos\n"
}
```
