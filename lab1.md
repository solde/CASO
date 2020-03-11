#  Mach i GNU Hurd
1. Comproveu que el programa proc.c funciona correctament per l'usuari root, però dóna l'error indicatanteriorment si l'executa un usuari no privilegiat (podeu usar l’usuari “demo”).
>
        Root:
        privileged ports: host 0x72  devices 0x73
        Getting processors at array 0x0
                processors at array 0x1033000
        processor_listCnt 1
        processor_list[0] 0x74
        SUCCESS
        CPU Type 19
        CPU Subtype 1
        Running 1
        Slot num 0
        Is master 1
>
        Demo:
        Error getting privileged ports (0x40000001), Operation not permitted

2. Quin processador indica que tenim el programa proc.c? Busqueu a <mach/machine.h> els codis de“CPU Type” i “CPU Subtype”.
>
        El CPU Type 19 se corrsponde a PENTIUMPRO, CPU Subtype 1 no existe porque PENTIUMPRO no tiene subtipos.

3. Expliqueu les altres característiques del processador que mostra proc.c. Obtingueu-les del fitxer<mach/processor_info.h>; localitzeu-hi l’estructura processor_basic_info.

```
#define PROCESSOR_BASIC_INFO    1               /* basic information */

struct processor_basic_info {
        cpu_type_t      cpu_type;       /* type of cpu */
        cpu_subtype_t   cpu_subtype;    /* subtype of cpu */
/*boolean_t*/integer_t  running;        /* is processor running */
        integer_t       slot_num;       /* slot number */
/*boolean_t*/integer_t  is_master;      /* is this the master processor */
};
```

4. Comproveu que el programa memory-management.c dóna errors al compilar... com els podeu arreglar? (pista: falta una coma (,) a la línia 60).Són clars els missatges d’error que dóna el compilador GCC en aquesta situació?
```
memory-management.c:62:41: error: called object is not a function or function pointer
62 |               MACH_PORT_NULL, 0, FALSE, VM_PROT_DEFAULT VM_PROT_ALL, FALSE);
|     
```

        Falta una coma entre els arguments "VM_PROT_DEFAULT" i "VM_PROT_ALL"

5. (COPIAR)

        1. Cada crida a host_processors assigna 0x1000 bites de memória.
        2. Cada crida a vm_map assigna 0x2000 bites de memória.
        3. Assigna adreces a partir de la 0x12e0000.
        4. Si, son posicions de memória contigues.
        5. Es poden assignar permisos de lectura, escriptura, execució, no_change (bloqueja la pagina) i notificacio (notifica l'us).
        6. Dona un "Segmentation fault", ja que s'intenta escriure en una zona reservada només per lectura.
        7. Ha ha funciona

8. (COPIAR)
        



