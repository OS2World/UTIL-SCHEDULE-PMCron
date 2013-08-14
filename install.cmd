/* install the Cron Daemon on the desktop */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

path = Directory()

pathx = path
if right(path,1) \= '\' then
   pathx = path || '\'
  else
   pathx = path

if \SysCreateObject('WPProgram',                                   ,
                    'Cron',                                        ,
                    '<WP_DESKTOP>',                                ,
                    'EXENAME='pathx'CROND.EXE;PROGTYPE=PM',      ,
                    'update') then
   say 'Cannot create object.'
