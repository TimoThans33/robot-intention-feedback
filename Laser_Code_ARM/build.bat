mkdir Laser_Code_ARM\build

"C:\Program Files\Git\bin\git.exe" rev-parse HEAD > "Laser_Code_ARM\build\version.inf" & "C:\Program Files\Git\bin\git.exe" tag -l --points-at HEAD >> "Laser_Code_ARM\build\version.inf"

C:\bin\arduino-cli core install arduino:samd
C:\bin\arduino-cli core update-index
C:\bin\arduino-cli compile --fqbn arduino:samd:mzero_pro_bl Laser_Code_ARM --output Laser_Code_ARM\

cp Laser_Code_ARM\Laser_Code_ARM.hex Laser_Code_ARM\build\