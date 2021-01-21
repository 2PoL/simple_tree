#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/_types/_s_ifmt.h>
#include <sys/dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
void tree_printf(char *filename, int depth);
void traverse_dir_tree(char *dirname, int depth);
void list_dir_tree(char *dirname, int depth, void (*fcn)(char *, int));
int main(int argc, char **argv) {
  if (argc == 1) {
    traverse_dir_tree(".", 0);
  } else {
    printf("%s\n", argv[1]);
    traverse_dir_tree(*++argv, 0);
  }
  return 0;
}

void tree_printf(char *filename, int depth) {
  while (depth-- > 0) {
    printf("|  ");
  }
  printf("|--");
  printf("%s\n", filename);
}
void traverse_dir_tree(char *dirname, int depth) {
  struct stat stbuf;
  if (stat(dirname, &stbuf) == -1) {
    fprintf(stderr, "traverse_dir_tree():can't read file %s information\n",
            dirname);
    return;
  }
  if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {
    list_dir_tree(dirname, depth, traverse_dir_tree);
  }
}

void list_dir_tree(char *dirname, int depth, void (*fcn)(char *, int)) {
  char name[MAXNAMLEN];
  struct dirent *file_entry;
  DIR *dir;
  if ((dir = opendir(dirname)) == NULL) {
    fprintf(stderr, "list_dir_tree():can't open %s\n", dirname);
    return;
  }
  while ((file_entry = readdir(dir)) != NULL) {
    if (strcmp(file_entry->d_name, ".") == 0 ||
        strcmp(file_entry->d_name, "..") == 0) {
      continue;
    }
    if (strlen(dirname) + strlen(file_entry->d_name) + 2 > sizeof(name)) {
      fprintf(stderr, "list_dir_tree():%s/%s is too long!\n", dirname,
              file_entry->d_name);
      return;
    } else {
      tree_printf(file_entry->d_name, depth);
      sprintf(name, "%s/%s", dirname, file_entry->d_name);
      (*fcn)(name, depth + 1);
    }
  }
  closedir(dir);
}
