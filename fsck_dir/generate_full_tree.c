#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    // Open the complete directory tree file
    FILE *complete_file = fopen("../complete_directory_tree.txt", "r");
    if (!complete_file) {
        printf("Error: Cannot open complete_directory_tree.txt\n");
        return 1;
    }
    
    // Open the output file
    FILE *output_file = fopen("complete_fsck_dir_directory_treev2.txt", "w");
    if (!output_file) {
        printf("Error: Cannot create output file\n");
        fclose(complete_file);
        return 1;
    }
    
    // Copy the complete directory tree to the output file
    char buffer[1024];
    while (fgets(buffer, sizeof(buffer), complete_file)) {
        fputs(buffer, output_file);
    }
    
    // Close files
    fclose(complete_file);
    fclose(output_file);
    
    printf("Generated complete_fsck_dir_directory_treev2.txt\n");
    return 0;
}