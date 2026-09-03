#ifndef MyAppVersion
  #error MyAppVersion must be provided by build-installer.ps1
#endif

#ifndef PluginSource
  #error PluginSource must be provided by build-installer.ps1
#endif

#ifndef InstallerOutput
  #error InstallerOutput must be provided by build-installer.ps1
#endif

[Setup]
AppId=Parallax.SimoneDeAngelis
AppName=Parallax
AppVersion={#MyAppVersion}
AppPublisher=Simone De Angelis
DefaultDirName={commoncf64}\VST3
DisableDirPage=yes
DisableProgramGroupPage=yes
Uninstallable=no
PrivilegesRequired=admin
ArchitecturesAllowed=x64compatible
ArchitecturesInstallIn64BitMode=x64compatible
OutputDir={#InstallerOutput}
OutputBaseFilename=Parallax-{#MyAppVersion}-Windows-x64-Setup
Compression=lzma2
SolidCompression=yes
WizardStyle=modern

[Files]
Source: "{#PluginSource}\*"; DestDir: "{commoncf64}\VST3\Parallax.vst3"; Flags: ignoreversion recursesubdirs createallsubdirs

