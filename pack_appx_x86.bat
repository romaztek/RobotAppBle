rem "C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x86\makecert.exe" /n "CN=romashka" /r /h 0 /eku "1.3.6.1.5.5.7.3.3,1.3.6.1.4.1.311.10.3.13" -a sha256 /e 10/10/2040 /sv romashka.pvk romashka.cer

rem "C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x86\Pvk2Pfx" /pvk romashka.pvk /pi Aa123456 /spc romashka.cer /pfx romashka.pfx

rem xcopy uwp_assets\assets ..\RobotAppBle\release\assets /sey
xcopy uwp_assets\AppxManifest.xml ..\RobotAppBle\release\ /sy

"E:\Qt\5.15.2\winrt_x86_msvc2019\bin\windeployqt.exe" ..\RobotAppBle\release\PuppetController.exe

"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x86\makeappx.exe" pack /d ..\RobotAppBle\release /p release_x86.appx /o
"C:\Program Files (x86)\Windows Kits\10\bin\10.0.22621.0\x86\SignTool" sign /f romashka.pfx /p Aa123456 /fd sha256 release_x86.appx

timeout 10