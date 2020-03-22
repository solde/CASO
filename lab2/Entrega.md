# CASO: LAB2

**1. Feu una taula amb els temps d'execució que obteniu. Per què els temps d'execució són tandiferents?**

|SYSCALL|sbrk(0)|sbrk(inc)|sched_yiel()|getpid()|fork/waitpid|
|:-:|:-:|:-:|:-:|:-:|:-:|
|Execution time(us)|5||558|481|57403|

**2. Podeu comprovar d'alguna manera que els programes executen realment la crida a sistema? (I noaprofiten el resultat retornat per la crida anterior)**

Nosaltres hem fet servir la opcio -O0 que garateix que no es es faran optimitzacions el la compilacio del nostre code, pel que estem segurs que el nostre codi compilat no esta fent cap optimització.

En canvia la cira a sistema no es compilada ni implementada per nosaltres, pel que no tenim cap seugiretat que es guardi el resultat de la crida anterior. Aixo es pot comprovar fent us d'un debugger y observer el comportament de la crida.

**3.A la darrera transparència del tema Virtualització-Sincronització-MesuresdeRendiment (transp. 50)us demanem que feu un programa que escrigui al disc 500MBytes, mesurant el temps que triga afer-ho. Des d'un usuari no privilegiat (no root), executeu el vostre programa sobre un fitxer en el disc de la màquina que feu servir per aquest laboratori. Per exemple, podeu fer-ho sobre un fitxer enel directori /tmp. Si teniu dues versions del codi, una desenvolupada per cada membre del grup:**

$ write-to-disk1  /tmp/file1.dat

$ write-to-disk2  /tmp/file2.dat

4.

$ 0.500000GB: 123941070 - 122197832 = 1743238us

$ 1.000000GB: 127381697 - 123941102 = 3440595us

$ 2.000000GB: 134445890 - 127381710 = 7064180us

$ 3.000000GB: 144635254 - 134445902 = 10189352us

$ 4.000000GB: 158870605 - 144635265 = 14235340us