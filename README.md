# FPGA

### Pasos para ejecutar el proyecto

1. Seleccionar el chip correcto del cyclone V
1. * si es diferente del actual tiene que ir al pin planer y setear toodos los I/O
2. En programmer subir el diseño del SoC
3. Ir al platform design y seleccionar el .qsys
4. Generar el HDL (no es necesario)
5. Abrir Eclipse
6. Ir a Import -> general -> existing Project... -> Ir a Demo_Qsys/software y agregar TestMp3 y el bsp (AQUI SE TRABAJA)
7. Limpiar y crear el proyecto de nuevo en ambas carpetas (verificar que el .elf se creo en testClk2)
8. Regenerar el BSP 
9. Run -> Run config..
10. Verificar que el .elf esté
11. en Target connection verificar los puertos de la FPGA
12. en System ID cheks en ambas casillas (ID y timestamp)
13. si no sirve dele en refresh connections en la parte derecha de esa parte (está escondido)
14. dele run

Debe salir una secuencia de luces con los leds de la FPGA y el JTAG debe estar funcionando.


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