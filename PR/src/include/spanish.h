/*  Princed V3 - Prince of Persia Level Editor for PC Version
    Copyright (C) 2003 Princed Development Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

    The authors of this program may be contacted at http://forum.princed.com.ar
*/

/*
pr.h: Princed Resources : Spanish language strings
¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 18 Feb 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2004-Feb-18)

  Language: Spanish

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                           Text Defines                        |
\***************************************************************/

#define PR_ABOUT                  "Princed resources (PR) "PR_VERSION" "OS"\n(c) Copyright 2003 - Princed Development Team\n"PR_URL"\n\n"

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nCorriendo como cgi\n"
#define PR_CGI_TEXT2              "Result: %02d type\n"

#define PR_TEXT_RESULT            "Resultado: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Resultado: %d archivos con errores\n"

#define PR_TEXT_FILE_NOT_FOUND    "Resultado: Archivo o directorio '%s' no encontrado\n"

#define PR_TEXT_SCANNING_CURRENT  "Buscando archivos dat en el directorio actual\n"
#define PR_TEXT_SCANNING_GIVEN    "Buscando archivos dat en el directorio dado\n"

#define PR_TEXT_IMPORTING_CURRENT "Importando todos los archivos dat validos del directorio actual\n"
#define PR_TEXT_IMPORTING_GIVEN   "Importando todos los archivos dat validos del directorio dado\n"

#define PR_TEXT_TASK_COMPILE      "Compilando '%s' desde '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "Clasificando '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Extrayendo '%s' a '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "Procesando '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "El archivo %s ya existia. Reemplazar? [Si/no/todos]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'T'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' no abierto, ignorado\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' importado exitosamente\n"
#define PR_TEXT_IMPORT_ERRORS     "'%s' tiene errores, ignorado\n"
#define PR_TEXT_IMPORT_DONE       "Compilacion terminada, %d archivos correctos, %d archivos con errores\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' exportado exitosamente\n"
#define PR_TEXT_EXPORT_ERROR      "'%s' tiene errores, abortando...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Advertencia: El archivo exportado puede estar corrompido\n"


/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Correcto",\
"Error al acceder un archivo",\
"Error de memoria en la extraccion",\
"Archivo DAT invalido",\
"Error de codificacion en el XML",\
"Error de memoria en la lectura del XML",\
"Atributo XML no reconocido",\
"Archivo XML no encontrado"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Error de memoria",\
"Archivo no encontrado no acceso denegado",\
"No es un archivo DAT del POP1 valido",\
"Archivo de niveles",\
"Archivo de graficos con una imagen en la primera entrada (poco comun)",\
"Archivo de sonido digital",\
"Archivo de sonido midi",\
"Archivo DAT valido con contenido indefinido",\
"Archivo de graficos con una paleta en la primera entrada (comun)",\
"Archivo de sonido del parlante de la PC",\
"","","",\
"Archivo del POP2"}

#define PR_TEXT_IMPORT_ARRAY {\
"Archivo compilado exitosamente",\
"El archivo DAT no pudo ser abierto para su escritura",\
"Error de codificacion XML",\
"Sin memoria",\
"Atributo XML no reconocido",\
"Archivo XML no encontrado"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  utiliza el formato crudo\n"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP "Utilizacion: \n\
  pr [-x[EXTRACTDIR]|-c[COMPILEDIR]|-d] [DATFILEPATH]\n\
  pr [OPTIONS] [DATFILEPATH]\n\
\n\
  Los argumentos obligatorios para las opciones largas lo son tambien\n\
  para las opciones cortas.\n\
\n\
   -i, -c, --import[=DIRNAME] importa desde DIRNAME al directorio dado\n\
   -d, --classify             retorna el tipo del archivo dat\n\
   -x, -e, --export[=DIRNAME] extrae el archivo dat en DIRNAME\n\
\n\
   -a, --setauthor=NAME       setea un nombre de autor en los archivos plv\n\
                              extraidos\n\
   -b, --backup[=EXTENSION]   realiza copias de seguridad\n\
   -f, --force                reescribe los archivos sin preguntar\n\
   -g, --cgi                  corre en modo CGI e imprime encabezados mime\n\
   -h, -?, --help             muestra este texto y sale\n\
   -s, --resource=RESFILE     utiliza el archivo xml de recursos especificado\n\
"PARSING_RAW"\
   -R, --recursive            busca todos los archivos dat en subdirectorios\n\
                              (solo si DATFILEPATH no es un archivo dat)\n\
   -t, --datfile=DATFILE      especifica un archivo para leer recursos\n\
                              distintos al archivo original\n\
       --unknown              genera el archivo de items desconocidos sin\n\
                              realizar extraccion alguna\n\
   -v, --verbose              explica lo que se esta haciendo\n\
       --version              impreme informacion de la version y sale\n\
\n"

#define PARSING_ABOUT "Autores: \n\
   Codinficacion & rutinas principales\n\
    Enrique Calot\n\
    Santiago Zamora\n\
\n\
   Algoritmos de compresion de graficos\n\
    Tammo Jan Dijkema\n\
    Enrique Calot\n\
\n\
   Investigacion del formato grafico\n\
    Tammo Jan Dijkema\n\
    Anke Balderer\n\
\n\
   Investigacion del formato de sonido MID\n\
    Christian Lundheim\n\
\n\
   Edicion de resources.xml\n\
    Steven Fayers\n\
\n"
