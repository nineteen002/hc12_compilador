# hc12_compilador
Compilador básicos para comandos del lenguaje hc12

Referencia al manual del lenguaje del hc12: http://support.technologicalarts.ca/docs/HC12-related/ThirdParty/CPU12RM.pdf

El programa solo identifica los primeros 20 mnemonicos del lenguajes y mnemonicos adicionales para comprobar diferentes modos de direccionamiento. Los Mnemonicos disponibles se encuentran en una lista tablaMnemonicos, si se quieren agregar mnemonicos simplemente se debe agregar a la lista y al archvio de TABCOP.

Archivo TABCOP
---------------------------
Contiene una tabla con los mnemonicos, modo de direccionamiento, tamaño de operadores, codigo de operacion, tamaño de codigo de operacion. Siempre se debe de agregar la informacion mencionanda si se desea agregar un mnemonico que el programa pueda detectar, de lo contrario se generar un error.

Archivo TABSIM
---------------------------
Este archivo se genera cada que corra el programa y contiene las direcciones de etiquetas en el codigo.


Tipos de direccionamiento
---------------------------
El programa es capaz de verificar mnemonicos con los siguientes tipos de direccionmiento:
Simples: 
        Inherente
        Immediato
        Directo
        Extendido
Relativos
Indexados: (IDX, IDX1, IDX2, [IDX2] y [D,IDX]
