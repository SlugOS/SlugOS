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
    printk(logo_art);
}