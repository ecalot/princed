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
pr.h: Princed Resources : English language strings
¯¯¯¯
 Copyright 2003 Princed Development Team
  Created: 18 Feb 2004

  Author: Enrique Calot <ecalot.cod@princed.com.ar>
  Version: 1.00 (2004-Feb-18)

  Language: English

 Note:
  DO NOT remove this copyright notice
*/

/***************************************************************\
|                           Text Defines                        |
\***************************************************************/

#define PR_ABOUT                  "Princed resources (PR) "PR_VERSION" "OS"\n"PR_COPY"\n"PR_URL"\n\n"

#define PR_CGI_TEXT1              "Content-Type:text/html\n\nRunning as a cgi\n"
#define PR_CGI_TEXT2              "Wynik: typ %02d\n"

#define PR_TEXT_RESULT            "Wynik: %s (%d)\n"
#define PR_TEXT_RESULT_ERR        "Wynik: %d plikow z bledami\n"

#define PR_TEXT_FILE_NOT_FOUND    "Wynik: Plik lub sciezka '%s' nie istnieje\n"

#define PR_TEXT_SCANNING_CURRENT  "Skanowanie plikow dat w aktualnej sciezce\n"
#define PR_TEXT_SCANNING_GIVEN    "Skanowanie plikow dat w podanej sciezce\n"

#define PR_TEXT_IMPORTING_CURRENT "Importowanie wszystkich poprawnych plikow dat z aktualnej sciezki\n"
#define PR_TEXT_IMPORTING_GIVEN   "Importowanie wszystkich poprawnych plikow dat z podanej sciezki\n"

#define PR_TEXT_TASK_COMPILE      "Importowanie '%s' z '%s'\n"
#define PR_TEXT_TASK_CLASSIFY     "Klasyfikowanie '%s'\n"
#define PR_TEXT_TASK_EXTRACT      "Eksportowanie '%s' do '%s'\n"

#define PR_TEXT_DISK_PROCESSING   "Przetwarzanie '%s'...\n"
#define PR_TEXT_DISK_REPLACE      "Plik %s juz istnieje. Nadpisac? [Yes/no/all]:"
#define PR_DISK_REPLACE_NO        'N'
#define PR_DISK_REPLACE_ALL       'A'

#define PR_TEXT_IMPORT_NOT_OPEN   "'%s' nie mogl byc otworzony, pominieto\n"
#define PR_TEXT_IMPORT_SUCCESS    "'%s' zostal zimportowany\n"
#define PR_TEXT_IMPORT_ERRORS     "'%s' ma bledy, pominieto\n"
#define PR_TEXT_IMPORT_DONE       "Importowanie zakonczone, %d plikow zimportowanych bez bledow, %d plikow z bledami\n"

#define PR_TEXT_EXPORT_WORKING    "'%s' zostal zeksportowany\n"
#define PR_TEXT_EXPORT_ERROR      "'%s' ma bledy, przerwano...\n"
#define PR_TEXT_EXPORT_BMP_WARN   "Ostrzezenie: Ekstraktowany plik moze byc uszkodzony\n"

/***************************************************************\
|                        Text Output Arrays                     |
\***************************************************************/

#define PR_TEXT_EXPORT_ARRAY {\
"Dobrze",\
"Blad w czasie otwierania pliku",\
"Blad w czasie ekstraktowania pamieci",\
"Nieprawidlowy plik DAT",\
"Blad w czasie analizowania pliku XML",\
"Blad w czasie analizowania pamieci",\
"Atrybut XML nie zostal rozpoznany",\
"Plik XMS nie zostal znaleziony"}

#define PR_TEXT_CLASSIFY_ARRAY {\
"Blad pamieciowy",\
"Plik nie istnieje lub nie moze byc otwarty",\
"Nieprawidlowy plik DAT",\
"Plik etapowy",\
"Plik graficzny bez palety",\
"Plik dzwiekowy",\
"Plik midi",\
"Poprawny plik z nieokreslona zawartoscia",\
"Plik graficzny",\
"Plik dzwiekowy PC Speakera",\
"","","",\
"Plik danowy POP2"}

#define PR_TEXT_IMPORT_ARRAY {\
"Plik zostal zimportowany",\
"Plik DAT nie zostal znaleziony lub jest nieprawidlowy",\
"Plik DAT nie mogl byc otwarty",\
"Blad w czasie analizowania pliku XML",\
"Brak pamieci",\
"Atrybut XML nie zostal rozpoznany",\
"Plik XMS nie zostal znaleziony"}

/***************************************************************\
|                 Command Line options information              |
\***************************************************************/

#ifndef PR_IGNORE_RAW_OPTION
#define PARSING_RAW "   -r, --raw                  uzywaj surowego formatu\n"
#else
#define PARSING_RAW ""
#endif
#define PARSING_HELP_BEGIN "Uzycie:\n  pr [-x[EXPORTDIR]|-i[IMPORTDIR]|-d] [DATFILEPATH]\n  pr [OPTIONS] [DATFILEPATH]\n\n  Obowiazkowe argumenty dla dlugich opcji sa obowiazkowe dla krotkich opcji takze.\n\n"
#define PARSING_HELP_PART1 "   -i, -c, --import[=DIRNAME] importuj z DIRNAME do wskazanego pliku dat\n   -d, --classify             zwroc rodzaj pliku dat\n   -x, -e, --export[=DIRNAME] eksportuj dany plik dat do DIRNAME\n\n"
#define PARSING_HELP_PART2 "   -a, --setauthor=NAME       ustaw swoje imie w ekstraktowanych plikach plv\n   -b, --backup[=EXTENSION]   zdubluj moje pliki\n   -f, --force                nadpisuj pliki bez pytania\n   -g, --cgi                  uruchom jako CGI i ustaw naglowki mime\n   -h, -?, --help             wyswietl ta pomoc i wyjdz\n   -s, --resource=RESFILE     uzywaj wlasnych zasobow xml\n"PARSING_RAW

#define PARSING_HELP_PART3 "   -R, --recursive            szukaj wszystkich plikow dat (tylko gdy\n                              DATFILEPATH nie jest plikiem dat)\n   -t, --datfile=DATFILE      ustaw plik dat, z ktorego beda odczytane\n                              zasoby inne niz w pliku orginalnym\n       --unknown              generuj nieznany plik dat bez\n                              potwierdzania zadnej ekstrakcji\n   -v, --verbose              wyjasnij co jest robione\n       --version              podaj informacje o wersji i wyjdz\n\n"

#define PR_TXT_AUTHORS           "Autorzy"
#define PR_TXT_CODER             "Kodowanie i g³ówne algorytmy"
#define PR_TXT_COD_ASSIST        "Dodatkowi programisci"
#define PR_TXT_GFX_COMP          "Algorytmy kompresji grafiki"
#define PR_TXT_GFX_DEV           "Tworca formatu graficznego"
#define PR_TXT_MID               "Tworca formatu muzycznego MID"
#define PR_TXT_XML               "Edycja zasobow XML-a"
#define PR_TXT_TRANSLATION       "Tlumaczenie"
#define PR_TXT_ABOUT_TRANSLATOR  "Piotr Kochanek"

