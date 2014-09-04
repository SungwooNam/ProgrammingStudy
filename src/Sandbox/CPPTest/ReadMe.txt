

http://stackoverflow.com/questions/9037534/how-can-i-avoid-debugging-into-boost-source-code-in-visual-studio

Launch regedit and navigate to the following key:

Under a 32bit OS:

HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VisualStudio\10.0\NativeDE\StepOver 

Under a 64bit OS:

HKEY_LOCAL_MACHINE\SOFTWARE\Wow6432Node\Microsoft\VisualStudio\10.0\NativeDE\StepOver 

Create a new string value there. Name it as you wish. Enter this as a content:

boost\:\:.*

(You need to restart Visual Studio.)
 

if you are using Visual Studio Express the key is HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\VCExpress\10.0\NativeDE\StepOver 
– Ingemar Jan 31 '12 at 9:17 


Add a new .natstepfilter to C:\Program Files (x86)\Microsoft Visual Studio 11.0\Common7\Packages\Debugger\Visualizers

<?xml version="1.0" encoding="utf-8"?>
<StepFilter xmlns="http://schemas.microsoft.com/vstudio/debugger/natstepfilter/2010">
  <Function><Name>boost::.*</Name><Action>NoStepInto</Action></Function>
  <Function><Name>std::.*</Name><Action>NoStepInto</Action></Function>
</StepFilter>