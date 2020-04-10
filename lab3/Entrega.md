# LAB3: Pthreads
###### Eric Rufart i David Soldevila

1. Us  demanem  que  acabeu  el  codi,  l’executeu  i  observeu  quines  crides  al  sistema fa  servir  per  crear  els  threads  i  implementar  el  mutex.   Feu  servir  la  comanda strace(1).  No  ́es trivial, però si furgueu prou, veureu que es fa servir futex perimplementar el mutex.  Consulteu el man de totes les syscalls.

Fent servir la comanda ```strace -o out.txt -f -ff ./a.out 4``` hem extret les següents dades.

La cria pthread_create va servir la syscal clone per crear els threads. La crida que fa es la seguent:

```
clone(child_stack=0x7f9de3b29fb0, flags=CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_SETTLS|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID, parent_tid=[5651], tls=0x7f9de3b2a700, child_tidptr=0x7f9de3b2a9d0) = 5651
```

Per crear el mutex es fa servir una crida futex com la següent:

```
futex(0x7f44c0a50908, FUTEX_WAKE_PRIVATE, 1) = 1
```

[Codi](./facMulti.c)

2. Modifiqueu  la  solució  anterior  perquè  nom ́es  s’executi  un  thread  per  core.   Això implica esbrinar quants cores té la vostra màquina i modificar la funcino f perquè tingui un bucle que faci les multiplicacions en bloc i acumuli una sola vegada.

Per saber el nombre de cores que té la màquina fem servir la crida ```get_nprocs_conf()``` que retorna el nombre de processadors logics que detecta el sistema.

[Codi](./facMulti2.c)