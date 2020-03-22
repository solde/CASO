#include <mach.h>
#include <mach_error.h>
#include <mach/mig_errors.h>
#include <mach/thread_status.h>
#include <mach/processor_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <hurd.h>

void inf(){
  volatile i=0;  
    while(1){
          i++;
    }
}

int main ()
{
    printf("Thread.c: %d\n", mach_task_self());
    volatile mach_port_t child;
    int ret = thread_create(mach_task_self(), &child);

    printf("Ret of thread_create: %d\n", ret);
    printf("Child is: %d\n", child);
     
    unsigned int old[256];

    struct i386_thread_state tsolde;
    thread_state_t told = &tsolde;
    
    int size =  THREAD_STATE_MAX;
      
    volatile int state = thread_get_state(child, i386_THREAD_STATE, told, &size);
    if(state < 0) printf("%d\n", state);
    //int state = thread_get_state(child, i386_THREAD_STATE,(thread_state_t) told, &size);
   
    
    //for(int i = 0; i < size; ++i){
    //  printf("%d: %d\n", i, old[i]);
    //}

    unsigned int stack2[512];
    
    old[12] = inf;
    old[15] = &stack2[511];
 
    tsolde.eip = inf;
    tsolde.uesp = &stack2[511];
    
    state=thread_set_state(child, i386_THREAD_STATE, told, size);
    if(state<0) printf("%d\n", state);
    printf("%d, %d,%d, %d\n", tsolde.eip, inf, tsolde.uesp, &stack2[511]);
    state=thread_get_state(child, i386_THREAD_STATE, told, &size);
    if(state<0) printf("%d\n", state);
    state=thread_resume(child);

    if(state<0) printf("%d\n", state);
    return 0;
}
