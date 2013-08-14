/* deinstall the Cron Daemon from the desktop and delete the INI entries */
call RxFuncAdd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
call SysLoadFuncs

  if \SysDestroyObject('Cron') then
    say 'Cannot delete program.'

/* Delete INI entries */
call SysIni "USER","Cron Daemon","DELETE:"
