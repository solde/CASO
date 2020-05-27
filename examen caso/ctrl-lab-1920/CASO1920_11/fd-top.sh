#!/bin/bash

do_work=1
delay=1.0
vm=1
fd=1

if [ x$1 == x-h ]; then
   echo Us: $0 '[info-options] [ps-process-selection-options] | -h | --help'
   echo '  info selection:    --vm            show vminfo only'
   echo '                     --fd            show file descriptors only'
   echo '  process selection: -e              all processes'
   echo '                     -u <username>   all processes running as user'
   echo '                     -t <pts/N>      all processes running in tty'
   echo '  while running:  q   quits'
   echo '                  s   reads new delay between updates'
   do_work=0
fi
if [ x$1 == x--help ]; then
   echo Us: $0 '[info-options] [ps-process-selection-options] | -h | --help' 
   echo '  info selection:    --vm            show vminfo only'
   echo '                     --fd            show file descriptors only'
   echo '  process selection: -e              all processes'
   echo '                     -u <username>   all processes running as user'
   echo '                     -t <pts/N>      all processes running in tty'
   echo '  while running:  q   quits'
   echo '                  s   reads new delay between updates'
   do_work=0
fi

while [ x$1 != x ]; do
  case $1 in 
    --vm) vm=1; fd=0;
#    echo found --vm
    ;;
    --fd) vm=0; fd=1;
#    echo found --fd
    ;;
    *) 
#    echo found $1
    break;
    ;;
  esac

  shift
done


if [ x$1 == x ]; then
   psopt=
else
   psopt="$*"
fi

while [ $do_work == 1 ]; do
  processes=`ps $psopt -o pid --no-header`

  #clear
  echo -n -e "\x1b\x5b\x48\x1b\x5b\x32\x4a\x1b\x5b\x33\x4a"
  if [ $vm == 1 ]; then
     echo " PID   COMM   USER   S vmpeak vmsz vmlck vmpin vmrss vmdata vmstk vmexe vmlib"
  else 
     echo " PID   COMM   USER   S "
  fi
  for proc in $processes ; do
    user=`ls -ld /proc/$proc 2>/dev/null | awk '{ print $3; }'`
    if [ -f /proc/$proc/status ]; then
     if [ $vm == 1 ]; then
      state=`grep -i state: /proc/$proc/status 2>/dev/null`
      name=`grep -i name: /proc/$proc/status 2>/dev/null`
      vmpeak=`grep -i vmpeak: /proc/$proc/status 2>/dev/null | cut -b 12-16`
      vmsize=`grep -i vmsize: /proc/$proc/status 2>/dev/null | cut -b 12-16`
      vmlck=`grep -i vmlck: /proc/$proc/status 2>/dev/null   | cut -b 12-16`
      vmpin=`grep -i vmpin: /proc/$proc/status 2>/dev/null   | cut -b 12-16`
      vmhwm=`grep -i vmhwm: /proc/$proc/status 2>/dev/null   | cut -b 12-16`
      vmrss=`grep -i vmrss: /proc/$proc/status 2>/dev/null   | cut -b 12-16`
      vmdata=`grep -i vmdata: /proc/$proc/status 2>/dev/null |cut -b 12-16`
      vmstk=`grep -i vmstk: /proc/$proc/status 2>/dev/null |cut -b 12-16`
      vmexe=`grep -i vmexe: /proc/$proc/status 2>/dev/null |cut -b 12-16`
      vmlib=`grep -i vmlib: /proc/$proc/status 2>/dev/null |cut -b 12-16`
      vmpte=`grep -i vmpte: /proc/$proc/status 2>/dev/null |cut -b 12-16`
      vmswap=`grep -i vmswap: /proc/$proc/status 2>/dev/null |cut -b 12-16`
     fi
     if [[ $fd == 1 && $vm == 1 ]]; then
      
      echo "`(echo -n $proc; echo -n '            ') | cut -b -6`" "`(echo $name | sed 's/$/          /'| cut -b 1-) | cut -b 7-14` $user" "`echo $state | cut -b 8`" "$vmpeak $vmsize $vmlck $vmpin $vmrss $vmdata $vmstk $vmexe $vmlib"
      echo "        fd: " `ls -lU /proc/$proc/fd 2>/dev/null | grep -v "total 0" | awk ' { if (length ($9) < 3) { printf("(%s)%s ", $9, $11); } }'`
     elif [ $fd == 1 ]; then
      echo "`(echo -n $proc; echo -n '            ') | cut -b -6`" "`(echo $name | sed 's/$/          /'| cut -b 1-) | cut -b 7-14` $user" "`echo $state | cut -b 8`" 
      echo "        fd: " `ls -lU /proc/$proc/fd 2>/dev/null | grep -v "total 0" | awk ' { if (length ($9) < 3) { printf("(%s)%s ", $9, $11); } }'`
     elif [ $vm == 1 ]; then
      echo "`(echo -n $proc; echo -n '            ') | cut -b -6`" "`(echo $name | sed 's/$/          /'| cut -b 1-) | cut -b 7-14` $user" "`echo $state | cut -b 8`" "$vmpeak $vmsize $vmlck $vmpin $vmrss $vmdata $vmstk $vmexe $vmlib"
     fi
    fi
  done
  read -t $delay -s -N 1 ANS
  if [ x$ANS == xq ]; then
    do_work=0;
  elif [ x$ANS == xs ]; then
    echo 
    read -p "Change delay from $delay to: " delay
  fi

done
