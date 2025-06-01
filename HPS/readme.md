# FPGA

# HPS

## Dependencias 
    Instalar Linux en SD
    Instalar SocEDS desde: https://www.intel.com/content/www/us/en/software-kit/665461/intel-soc-fpga-embedded-development-suite-soc-eds-standard-edition-software-version-18-1-for-windows.html

## Compilación
    Abrir Embedded Command Shell y navegar a carpeta del proyecto HPS
    Ejecutar make

## Conexión a internet en FPGA
    Conectar cable ethernet a FPGA
    Conectarse a FPGA con Putty
    Ejecutar comandos para asignar dirección IP 
 
        ifconfig eth0 up
        udhcpc eth0
        ifconfig
    

## Ejecutar en FPGA
    Transferir archivo con scp song_processor root@ip_asignada:/home/root/
    Crear directorio songs
    Transferir canciones con scp song_processor root@ip_asignada:/home/root/songs/
    Asignar permiso de ejecución al programa: chmod +x song_processor