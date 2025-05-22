### Pasos para ejecutar el proyecto

1. Seleccionar el chip correcto del cyclone V
1. * si es diferente del actual tiene que ir al pin planer y setear toodos los I/O
2. En programmer subir el diseño del SoC
3. Ir al platform design y seleccionar el .qsys
4. Generar el HDL (no es necesario)
5. Abrir Eclipse
6. Limpiar y crear el proyecto de nuevo en ambas carpetas (verificar que el .elf se creo en testClk2)
7. Regenerar el BSP 
8. Run -> Run config..
9. Verificar que el .elf esté
10. en Target connection verificar los puertos de la FPGA
11. en System ID cheks en ambas casillas (ID y timestamp)
12. si no sirve dele en refresh connections en la parte derecha de esa parte (está escondido)
13. dele run

Debe salir una secuencia de luces con los leds de la FPGA y el JTAG debe estar funcionando.

