#include <slug.h>

const char* logo_art = 
" #####                       #######  #####  \n"
"#     # #      #    #  ####  #     # #     # \n"
"#       #      #    # #    # #     # #       \n"
" #####  #      #    # #      #     #  #####  \n"
"      # #      #    # #  ### #     #       # \n"
"#     # #      #    # #    # #     # #     # \n"
" #####  ######  ####   ####  #######  #####  \n";

void displaylogo() {
    clear_screen();
    printk(logo_art);
}