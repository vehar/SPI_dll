del "*.bak" /f /s /a: H

::visual studio
del "*.user" /f /s /a: H
::del "*.htm" /f /s /a: H
del "*.obj" /f /s /a: H
del "*.pch" /f /s /a: H
del "*.pdb" /f /s /a: H
del "*.idb" /f /s /a: H

::quartus
del "*.ttf" /f /s /a: H
del "*.svf" /f /s /a: H
del "*.rpt" /f /s /a: H
del "*.rpt" /f /s /a: H
del "*.done" /f /s /a: H
del "*.qip" /f /s /a: H

del "*.cdb" /f /s /a: H
del "*.ddb" /f /s /a: H
del "*.tdb" /f /s /a: H
del "*.rvd" /f /s /a: H
del "*.rcfdb" /f /s /a: H
del "*.hier_info" /f /s /a: H
del "*.qmsg" /f /s /a: H
del "*.hdb" /f /s /a: H
del "*.hif" /f /s /a: H

del "*.kpt" /f /s /a: H
del "*.tdf" /f /s /a: H
del "*.rdb" /f /s /a: H
del "*.bpm" /f /s /a: H
del "*.logdb" /f /s /a: H
del "*.sci" /f /s /a: H

del "*.sdo" /f /s /a: H
del "*.vo" /f /s /a: H

::Altium
del "*.LOG" /f /s /a: H

::Keil
del "*.o" /f /s /a: H
del "*.d" /f /s /a: H
del "*.crf" /f /s /a: H
del "*.dep" /f /s /a: H
del "*.sct" /f /s /a: H
del "*.lnp" /f /s /a: H


::set path="C:\Program Files\WinRAR\";%path%
::rar a  -r %cd% *.*
::del "*.bak" 


::sleep 90
