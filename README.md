## Skriveregler

Curly brackets sidder umiddelbart efter det de starter (Funktioner, løkker osv.)

    int fn (type) {

Løkker med kun 1 linje kode benytter ikke curly brackets.

    if this
        code

Funktioner starter med store bogstaver før og efter underscore (\_)

    Calculate_Prices

I prototyper skrives kun datatype, og ikke variabelnavne.

    Prototype (Int, char)

Indrykning består af intervaller på 4 mellemrum

    int main (void) {
        Code is indented
        Like this
    }

## Dokumentation (Doxygen)

Skrives umiddelbart før hver funktion / struct på følgende måde:

    /** Converts a given date to the corresponding day no. in a year.
     * @param[i] date The date in the current month (1-31).
     * @param[i] month The current month (1-12).
     * @return Returns the day as a number (1-365). */
    int Get_Day_In_Year (int date, int month) {
        ...
    }
    
Den første linje beskriver hvad funktionen gør:

    /** Converts a given date to the corresponding day no. in a year.
    
De næste linjer forklarer hvad de forskellige parametre gør:

    @param[i]   //input parameter
    @param[o]   //output
    @param[i/o] //input/output

Den sidste linje forklarer hvad funktionen returnerer. (Gælder ikke for void-funktioner):

    @return Returns the day as a number (1-365).
